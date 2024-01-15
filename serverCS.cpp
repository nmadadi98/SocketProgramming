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

	// Read and Store the details on a vector
	ifstream MyFile("cs.txt");
	vector<string> vec_courses;
	string str;
	if (MyFile.is_open())
	{
		int i = 0;
		while(!MyFile.eof())
		{
			getline(MyFile, str);
			vec_courses.push_back(str);
		}
	}

	int vec_size = vec_courses.size();


	// Created socket for communication with serverM
	// Reference: GeeksForGeeks UDP Server Client Implementation
    int sockfd_UDP_CS;
	struct sockaddr_in servaddr_M, my_addr;

	if ((sockfd_UDP_CS = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");	
	}

	memset(&my_addr, 0, sizeof(my_addr));
	memset(&servaddr_M, 0, sizeof(servaddr_M));

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = 22131;
	inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);

	socklen_t servaddr_M_len = sizeof(servaddr_M);
	servaddr_M.sin_family = AF_INET;
	servaddr_M.sin_port = 24131;
	inet_pton(AF_INET, "127.0.0.1", &servaddr_M.sin_addr);

	if ((bind(sockfd_UDP_CS, (sockaddr*) &my_addr, sizeof(my_addr)))<0){
		perror("bind failed");
	}

	cout<<"The ServerCS is up and running using UDP on port 22131."<<endl;

	char buf[MAXBUFLEN];
	char buf2[MAXBUFLEN];

	while(1){

		// Receive the coursecode and category from serverM
		recvfrom(sockfd_UDP_CS, buf,  MAXBUFLEN-1, 0, (struct sockaddr *)&servaddr_M, &servaddr_M_len);
		recvfrom(sockfd_UDP_CS, buf2, MAXBUFLEN-1, 0, (struct sockaddr *)&servaddr_M, &servaddr_M_len);

		string coursecode = buf;
		string category = buf2;
		string Credits = "Credit";
		string Professor = "Professor";
		string Days = "Days";
		string CourseName = "CourseName";
		string NotFound = "NF";  // Not Found 
		string information;

		string c_code;
		string c_credits_extra;
		string c_credits;
		string c_professor_extra;
		string c_professor;
		string c_days_extra;
		string c_days;
		string c_name;

		cout <<"The ServerCS received a request from the Main Server about the "<< category << " of "<< coursecode <<"." <<endl;

		// Loop through all the entries to find the desired information
		// And then send the response to serverM
		int count_vec_size = vec_size;
		for (string x: vec_courses){
			string delimeter = ",";

			c_code = x.substr(0, x.find(delimeter));
			c_credits_extra = x.substr(x.find(delimeter)+1);

			c_credits = c_credits_extra.substr(0, c_credits_extra.find(delimeter));
			c_professor_extra = c_credits_extra.substr(c_credits_extra.find(delimeter)+1);

			c_professor = c_professor_extra.substr(0, c_professor_extra.find(delimeter));
			c_days_extra = c_professor_extra.substr(c_professor_extra.find(delimeter)+1);

			c_days = c_days_extra.substr(0, c_days_extra.find(delimeter));
			c_name = c_days_extra.substr(c_days_extra.find(delimeter)+1, c_days_extra.length()-1-c_days_extra.find(delimeter));
			
			// Checking which information needs to be sent
			if (strcmp(c_code.c_str(),coursecode.c_str())==0) {

				if (strcmp(category.c_str(),Credits.c_str())==0){
					information = c_credits;
				}
				else if (strcmp(category.c_str(),Professor.c_str())==0){
					information = c_professor;
				}
				else if (strcmp(category.c_str(),Days.c_str())==0){
					information = c_days;
				}
				else if (strcmp(category.c_str(),CourseName.c_str())==0){
					information = c_name;
				}

				cout<< "The course information has been found: The "<< category << " of "<<coursecode<< " is "<< information<<"." <<endl;
				sendto(sockfd_UDP_CS, information.c_str(), 1000, 0, (sockaddr*) &servaddr_M, sizeof(servaddr_M));
				cout << "The ServerCS finished sending the response to the Main Server." <<endl;
				break;
			}

			count_vec_size = count_vec_size - 1;

			// If the course is not found
			if (count_vec_size == 0){
				information = NotFound;
				cout << "Didn't find the course: "<< coursecode<<"." <<endl;
				sendto(sockfd_UDP_CS, information.c_str(), 1000, 0, (sockaddr*) &servaddr_M, sizeof(servaddr_M));
				cout << "The ServerCS finished sending the response to the Main Server." <<endl;
			}			

		}
	}

	close(sockfd_UDP_CS);
}

