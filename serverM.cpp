#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string>
#include <ctype.h>

using namespace std;


#define BACKLOG 10 // how many pending connections queue will hold
#define MAXBUFLEN 1000

//Reference: Beej's guide
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}



// Get sockaddr
// Reference: Beej's guide
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Create TCP Connection
// Reference: Beej's guide
int TCPConnection(){

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP

	// Setting the Host Name and Port Number
	if ((rv = getaddrinfo("127.0.0.1", "25131", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
             sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
	return sockfd;
}


int main(void)
{

    int new_fd, u, p;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
	char buf1[1000];
	char buf2[1000];
	char en_buf1[1000];
	char en_buf2[1000];
	char buf_ccode[1000];
	char buf_ccategory[1000];
	char code_1;
	char code_2;

	// Create a TCP Connection
    int sockfd_TCP = TCPConnection();



	// Reference: GeeksForGeeks UDP Server Client Implementation
	struct sockaddr_in servaddr_M;
	memset(&servaddr_M, 0, sizeof(servaddr_M));
	servaddr_M.sin_family = AF_INET;
	servaddr_M.sin_port = 24131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_M.sin_addr);	

    int sockfd_UDP_C;
	struct sockaddr_in servaddr_C, servaddr_C_recv;

	// Created socket for communication with serverC
	if ((sockfd_UDP_C = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");	
	}

	memset(&servaddr_C, 0, sizeof(servaddr_C));
	servaddr_C.sin_family = AF_INET;
	servaddr_C.sin_port = 21131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_C.sin_addr);

	socklen_t C_recv_len = sizeof(servaddr_C_recv);
	memset(&servaddr_C_recv, 0, sizeof(servaddr_C_recv));

	if ((bind(sockfd_UDP_C, (sockaddr*) &servaddr_M, sizeof(servaddr_M)))<0){
		perror("bind failed");
	}


    int sockfd_UDP_EE;
	struct sockaddr_in servaddr_EE, servaddr_EE_recv;

	// Created socket for communication with serverCS
	// Reference: GeeksForGeeks UDP Server Client Implementation
	if ((sockfd_UDP_EE = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");	
	}

	memset(&servaddr_EE, 0, sizeof(servaddr_EE));

	servaddr_EE.sin_family = AF_INET;
	servaddr_EE.sin_port = 23131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_EE.sin_addr);	

	socklen_t EE_recv_len = sizeof(servaddr_EE_recv);
	memset(&servaddr_EE_recv, 0, sizeof(servaddr_EE_recv));


	
    int sockfd_UDP_CS;
	struct sockaddr_in servaddr_CS, servaddr_CS_recv;

	// Created socket for communication with serverCS
	// Reference: GeeksForGeeks UDP Server Client Implementation
	if ((sockfd_UDP_CS = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");	
	}

	memset(&servaddr_CS, 0, sizeof(servaddr_CS));
	servaddr_CS.sin_family = AF_INET;
	servaddr_CS.sin_port = 22131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_CS.sin_addr);

	socklen_t CS_recv_len = sizeof(servaddr_CS_recv);
	memset(&servaddr_CS_recv, 0, sizeof(servaddr_CS_recv));


	//int b2 = bind(sockfd_UDP_CS, (sockaddr*) &servaddr_M, sizeof(servaddr_M));
	//int b3 = bind(sockfd_UDP_EE, (sockaddr*) &servaddr_M, sizeof(servaddr_M));



	cout<<"The main server is up and running."<<endl;

	sin_size = sizeof their_addr;
	// Accepts request, creates a new socket file descriptor
	new_fd = accept(sockfd_TCP, (struct sockaddr *)&their_addr, &sin_size);

	inet_ntop(their_addr.sin_family,
	get_in_addr((struct sockaddr *)&their_addr),
	s, sizeof s);
	char bufC_recv[MAXBUFLEN];
	char bufCS_recv[MAXBUFLEN];
	char bufEE_recv[MAXBUFLEN];
    while(1) {

		// Initializing the buffers
		memset(buf1, '\0', sizeof(char)*1000);
		memset(buf2, '\0', sizeof(char)*1000);
		memset(en_buf1, '\0', sizeof(char)*1000);
		memset(en_buf2, '\0', sizeof(char)*1000);

		// Receive the UserName and Password
        u = recv(new_fd, buf1, 1000, 0);
        p = recv(new_fd, buf2, 1000, 0);
	
		// If the received is not positive, i.e, the client closes then the serverM will wait for new connection, so as
		// to not close the serverM
		if (u<=0 or p<=0){	
			sin_size = sizeof their_addr;
			new_fd = accept(sockfd_TCP, (struct sockaddr *)&their_addr, &sin_size);

			inet_ntop(their_addr.sin_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);

			continue;					
		}

		cout<<"The main server received the authentication for "<<buf1<< " using TCP over port 25131." <<endl;

		char character;
		int temp_num;
	
		// Encoding the the Username by incrementing the alphabets and numbers by 4

		for (int i = 0; buf1[i] != '\0'; i++){
			character = buf1[i];
			if (character >= 'a' && character <= 'z'){
				character = character + 4;
				if (character > 'z'){
					character = character - 'z' - 1 + 'a';
				}
				en_buf1[i] = character;
			}

			else if (character >= 'A' && character <= 'Z'){
				character = character + 4;
				if (character > 'Z'){
					character = character - 'Z' - 1 + 'A';
				}
				en_buf1[i] = character;
			}
			
			else if (isdigit(character)){

				temp_num = int(character-'0');
				temp_num = (temp_num + 4)%10;
				en_buf1[i] = '0' + temp_num;
			}
			else{
				en_buf1[i] = character;
			}
		} 

		// Encoding the the Password by incrementing the alphabets and numbers by 4

		for (int i = 0; buf2[i] != '\0'; i++){

			character = buf2[i];
			if (character >= 'a' && character <= 'z'){
				character = character + 4;
				if (character > 'z'){
					character = character - 'z' - 1 + 'a';
				}
				en_buf2[i] = character;
			}

			else if (character >= 'A' && character <= 'Z'){
				character = character + 4;
				if (character > 'Z'){
					character = character - 'Z' - 1 + 'A';
				}
				en_buf2[i] = character;
			}
			
			else if (isdigit(character)){
				temp_num = int(character-'0');
				temp_num = (temp_num + 4)%10;
				en_buf2[i] = '0' + temp_num;
			}
			else{
				en_buf2[i] = character;
				}
		}
		

		// Send the encrypted Username and Password to ServerC
		sendto(sockfd_UDP_C, en_buf1, 1000, 0, (sockaddr*) &servaddr_C, sizeof(servaddr_C));
		sendto(sockfd_UDP_C, en_buf2, 1000, 0, (sockaddr*) &servaddr_C, sizeof(servaddr_C));
		cout <<"The main server sent an authentication request to serverC."<<endl;

		// Receive the authentication details from ServerC
		recvfrom(sockfd_UDP_C, bufC_recv, MAXBUFLEN-1, 0,(struct sockaddr *)&servaddr_C_recv, &C_recv_len);
		cout<<"The main server received the result of the authentication request from ServerC using UDP over 24131."<<endl;

		// Send the authentication details to client	
	    send(new_fd, bufC_recv, 1000, 0);
		cout<<"The main server sent the authentication result to the client" <<endl;

		string result_credential = bufC_recv;

		int count_buf_ccode = 0;
		int count_buf_ccategory = 0;

		// If the authentication is valid then serverM can receive the query information from the client
		if (strcmp(result_credential.c_str(),"2")==0){
			
			while(1){

				// Initialize the buffers
				memset(buf_ccode, '\0', sizeof(char)*1000);
				memset(buf_ccategory, '\0', sizeof(char)*1000);

				// Receive the query details from client
				count_buf_ccode = recv(new_fd, buf_ccode, 1000, 0);
				count_buf_ccategory = recv(new_fd, buf_ccategory, 1000, 0);

				cout << "The main server received from "<< buf1 << " to query course "<<buf_ccode <<" about "<<buf_ccategory<< " using TCP over port 25131."<<endl;

				code_1 = buf_ccode[0];
				code_2 = buf_ccode[1];				
			
				// If the course code first to digits are C and S then send/receive to/from ServerCS				
				if(code_1 == 'C' && code_2 =='S'){
					sendto(sockfd_UDP_CS, buf_ccode, 1000, 0, (sockaddr*) &servaddr_CS, sizeof(servaddr_CS));
					sendto(sockfd_UDP_CS, buf_ccategory, 1000, 0, (sockaddr*) &servaddr_CS, sizeof(servaddr_CS));
					cout << "The main server sent a request to serverCS."<<endl;
					recvfrom(sockfd_UDP_CS, bufCS_recv, MAXBUFLEN-1, 0,(struct sockaddr *)&servaddr_CS_recv, &CS_recv_len);
					cout << "The main server received the response from serverCS using UDP over port 24131."<<endl;
					send(new_fd, bufCS_recv, 1000, 0);
					cout<<"The main server sent the query information to the client."<<endl;
				}

				// If the course code first to digits are E and E then send/receive to/fromg ServerCEE
				else if(code_1 == 'E' && code_2 =='E'){
					sendto(sockfd_UDP_EE, buf_ccode, 1000, 0, (sockaddr*) &servaddr_EE, sizeof(servaddr_EE) );
					sendto(sockfd_UDP_EE, buf_ccategory, 1000, 0, (sockaddr*) &servaddr_EE, sizeof(servaddr_EE) );
					cout << "The main server sent a request to serverEE."<<endl;
					recvfrom(sockfd_UDP_EE, bufEE_recv, MAXBUFLEN-1, 0,(struct sockaddr *)&servaddr_EE_recv, &EE_recv_len);
					cout << "The main server received the response from serverEE using UDP over port 24131."<<endl;
					send(new_fd, bufEE_recv, 1000, 0);
					cout<<"The main server sent the query information to the client."<<endl;
				}
				

				else{
					string invalidCode = "NF";
					send(new_fd, invalidCode.c_str(), 1000, 0);
				}


			}
		}
    }

	close(sockfd_UDP_C);
	close(sockfd_UDP_CS);
	close(sockfd_UDP_EE);
    close(new_fd); 
    close(sockfd_TCP);


    return 0;
}

