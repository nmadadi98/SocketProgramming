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

using namespace std;

// Code Reference from Beej's guide: Creation of sockets



#define MAXDATASIZE 1000 // max number of bytes we can get at once

// Get sockaddr
// Reference: Beej's guide
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
}
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main()
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
	char buf_M_recv[MAXDATASIZE];
	char buf_M_recv_2[MAXDATASIZE];
	char code_1;
	char code_2;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

	// Reference: Beej's guide
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP
	// Setting the Host Name and Port Number
    if ((rv = getaddrinfo("127.0.0.1", "25131", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }



    for(p = servinfo; p != NULL; p = p->ai_next) {
		// Created a socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
		// Connected to socket
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
	s, sizeof s);	

	cout << "The client is up and running."<<endl;
	int count=0;
	while(1){

		// Initial empty string loop till atleast username and password is length 1
		string username="";
		string password="";
		while(username.length()==0){
		cout << "Please enter the username: ";
		getline(cin, username);
		}
		while(password.length()==0){
		cout << "Please enter the password: ";
		getline(cin, password);
		}
		
		// Send the username and password to serverM
		send(sockfd, username.c_str(), MAXDATASIZE, 0);
		send(sockfd, password.c_str(), MAXDATASIZE, 0);

		cout << username << " sent an authentication request to the main server."<<endl;
		
		// Recieve the authentication details
		recv(sockfd, buf_M_recv, 1000, 0);

		string result = buf_M_recv;

		// To get the dynamically assigned Port Number
		// Reference: Piazza post @188
		unsigned int clientPort;
		struct sockaddr_in clientAddress;
		bzero(&clientAddress, sizeof(clientAddress));
		socklen_t len = sizeof(clientAddress);
		getsockname(sockfd, (struct sockaddr *)&clientAddress, &len);
		clientPort = ntohs(clientAddress.sin_port);


		// If the credentials are correct
		if (strcmp(result.c_str(),"2")==0){
				count = 0;
				cout<<username<< " received the result of authentication using TCP over port "<< clientPort << ". Authentication is successful" <<endl;
		}
		
		// If the Username is wrong
		else if (strcmp(result.c_str(),"0")==0){
				count = count + 1;
				cout<<username<< " received the result of authentication using TCP over port "<< clientPort <<". Authentication failed: Username Does not exist"<<endl;
				cout<<endl;
				cout<<"Attempts remaining:" <<3 - count<<endl;
		}	
		
		// If the Password is wrong
		else if (strcmp(result.c_str(),"1")==0){
				count = count + 1;
				cout<<username<< " received the result of authentication using TCP over port "<< clientPort <<". Authentication failed: Password does not match"<<endl;
				cout<<endl;
				cout<<"Attempts remaining:" <<3 - count<<endl;
		}
		
		// Loop till 3 tries, if fails break the while loop
		if (count == 3) {
			cout<<"Authentication Failed for 3 attempts. Client will shut down."<<endl;
			exit(0);
			break;
		}
	
		// Variables to store Course Code and Category
		string ccode;
		string ccategory;
		
		// Authentication Passed
		if (count == 0){

			while(1){

				// Initial empty strings loop till atleast Course code and category is length 1
				ccode="";
				ccategory="";
				while(ccode.length()==0){
					cout << "Please enter the course code to query: ";
					getline(cin, ccode);
				}
				while(ccategory.length()==0){
					cout << "Please enter the category (Credit/Professor/Days/CourseName): ";
					getline(cin, ccategory);
				}
				
				// Send details to serverM
				send(sockfd, ccode.c_str(), MAXDATASIZE, 0);
				send(sockfd, ccategory.c_str(), MAXDATASIZE, 0);

				cout << username << " sent a request to the main server." <<endl;
				
				// Receive the information from the serverM
				recv(sockfd, buf_M_recv_2, 1000, 0);

				string information = buf_M_recv_2;
				
				code_1 = buf_M_recv_2[0];
				code_2 = buf_M_recv_2[1];

				cout << "The client received the response from the Main server using TCP over port "<< clientPort <<"."<<endl;
				if(code_1 == 'N' && code_2 =='F'){
					cout << "Didn't find the course:" << ccode << endl;
					cout << endl;
					cout << endl;
					cout << "-----Start a new request-----"	<<endl;
				}
				else{
					cout << "The "<< ccategory <<" of "<< ccode<<" is "<< information << endl;
					cout << endl;
					cout << endl;
					cout << "-----Start a new request-----"	<<endl;				
				}

			}
		}
	}

	freeaddrinfo(servinfo);

    close(sockfd);

    return 0;

}
