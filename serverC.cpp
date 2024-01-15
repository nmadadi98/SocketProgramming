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
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define MAXBUFLEN 1000

int main(){

	// Read and Store Username and Passwords on a vector
	ifstream MyFile("cred.txt");
	vector<string> vec_pass;
	string str;
	if (MyFile.is_open())
	{
		int i = 0;
		while(!MyFile.eof())
		{
			getline(MyFile, str);
			vec_pass.push_back(str);
		}
	}


	// Created socket for communication with serverM
	// Reference: GeeksForGeeks UDP Server Client Implementation
    int sockfd_UDP_C;
	struct sockaddr_in servaddr_C, servaddr_C_recv, my_addr;

	if ((sockfd_UDP_C = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");	
	}

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = 21131;
	inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);

	memset(&servaddr_C, 0, sizeof(servaddr_C));
	servaddr_C.sin_family = AF_INET;
	servaddr_C.sin_port = 24131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_C.sin_addr);

	socklen_t C_recv_len = sizeof(servaddr_C_recv);
	memset(&servaddr_C_recv, 0, sizeof(servaddr_C_recv));


	if ((bind(sockfd_UDP_C, (sockaddr*) &my_addr, sizeof(my_addr)))<0){
		perror("bind failed");
	}


	cout<<"The ServerC is up and running using UDP on port 21131."<<endl;

	char buf[MAXBUFLEN];
	char buf2[MAXBUFLEN];

	while(1){

		// Receive Username and Password from serverM 
		recvfrom(sockfd_UDP_C, buf,  MAXBUFLEN-1, 0, (struct sockaddr *)&servaddr_C_recv, &C_recv_len);
		recvfrom(sockfd_UDP_C, buf2, MAXBUFLEN-1, 0, (struct sockaddr *)&servaddr_C_recv, &C_recv_len);
		cout<<"The ServerC received an authentication request from the Main Server."<<endl;
		
		
		string user_name = buf;
		string password_1 = buf2;


		string user;
		string passw;

		int result_user=0;
		int result_pass=0;
		int vec_size = vec_pass.size();
		int itr_vec = vec_size;
		
		// Checking whether the Username and Password exist and are valid
		for (string x: vec_pass){
			itr_vec = itr_vec - 1;
			string delimeter = ",";
			user = x.substr(0, x.find(delimeter));
			if (itr_vec != 0){
				passw = x.substr(x.find(delimeter)+1, x.length()-2-x.find(delimeter));
			}
			else{
				passw = x.substr(x.find(delimeter)+1, x.length()-1-x.find(delimeter));

			}
			if (strcmp(user_name.c_str(),user.c_str())==0) {
				if (strcmp(password_1.c_str(),passw.c_str())==0){
					result_pass = 1;
					break;
				}
				result_user = result_user + 1;
			}
		}
		
		
		char result_final[2];
		if (result_pass == 1){
			result_final[0] = '2';
		}
		else if(result_user>0){
			result_final[0] = '1';
		}
		else{
			result_final[0] = '0';
		}
		result_final[1] = '\0';

		// Send the value to serverM, whether the credentials are valid or not
		sendto(sockfd_UDP_C, result_final, 1000, 0, (sockaddr*) &servaddr_C, sizeof(servaddr_C));
		cout<<"The ServerC finished sending the response to the Main Server."<<endl;
	}
	close(sockfd_UDP_C);
}



















