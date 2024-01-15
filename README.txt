Full Name: Nithin Kodand Reddy Madadi
Student ID: 7692947131


I have implemented all the required portion of the assignment, which will be explained below. I did not do the optional part.


My Code Files:

client
serverM
serverC
serverEE
serverCS


client:


Description:

	Communicates only with serverM to get authentication from serverC, and details from serverEE and serverCS.
	Communicates with serverM using TCP connection.
	We can try the username and password 3 times, if it fails the client will close.
	Username and Password cannot be empty, it will keep on asking if the user just presses ENTER.
	Once Authentication is done and is correct we can type the CourseCode and Cateory, then it sends Course Code and Category to serverM.
	Course Code and Category cannot be empty, it will keep on asking if the user just presses ENTER.
	Later we get the final response for our query.
	Once we get the result it will again ask for CourseCode and Category and the loop keeps on running.

Format of all the messages exchanged:

	client and serverM:

	For sending:
	UserName: pointer to array, used c_str() on string username  to get pointer
	Password: pointer to array, used c_str() on string password to get pointer
	CourseCode: pointer to array, used c_str() on string coursecode to get pointer
	Category: pointer to array, used c_str() on string category to get pointer

	Authentication Response: charArray
	2 - Credentials are correct
	1 - When Username is correct and Password does not match
	0 - When Username is wrong

	Response for Course: charArray
	Will check the first 2 elements of this charArray, if NF, it means Didn't find the course. 



serverM:

Description:
	
	Communicates with client with TCP, and server C/EE/CS with UDP.
	Creates all the above Sockets.
	Receives the Username and Password from client. If the client closes then the serverM will wait for other connections to be made.
	The Username and Password are encoded and then sent to serverC.
	Once we get the reply from serverC, we send the results to client, and then we check if the reply is 2.
	If it is not 2 then the loop goes back to the beginning and the process is repeated again.
	If it is 2 then serverM now expects the client to send CourseCode and Category.
	We type the details, and it will check the first 2 elements to see where the request needs to be sent.
	If CS it will send to serverCS, If EE it will send to serverEE.
	Later we send the details to serverCS or serverEE and then receive the information which will send to the client.

Format of all the messages exchanged:

	serverM and client:
	charArray for receiving username, Password, CourseCode, Category.
	charArray for sending authentication response from serverC and final response from server EE/CS to client.

	serverM and serverC:
	charArray for sending encoded Username, Password.
	charArray for the authentication response. If it is 2 it means that the credentials are valid.

	serverM and serverEE/CS:
	charArray for sending CourseCode, Category.
	charArray for the receiving the final response.



serverC:

Description:

	Commnuicates with serverM using UDP.
	Reads the cred text file and stores all the information in a vector.
	It receives the encoded credentials. It separates the Username and Password and validates them.
	It sends 2 when both Username and Password are correct.
	It sends 1 when only the Username is correct.
	It sends 0 when the Username is wrong.		

Format of all the messages exchanged:

	serverM and serverC:
	charArray for receiving encoded Username, Password.
	charArray for sending the authentication response.



serverEE:

Description:

	Communicates with serverM using UDP.
	Reads the ee text file and stores all the information in a vector.
	It receives the CourseCode and Categories.
	It then seperates the text line in vector based on categories using substr().
	It compares with the input CourseCode with all the coursecodes, if there is a match, and the Category is also match, it stores
	the result in a string
	Then we send the result to serverM.
	If the result was not found it sends "NF" to serverM. 
	In both the casess string will be converted to pointer using c_str() to send to serverM
	

Format of all the messages exchanged:

	serverM and serverEE:
	charArray for receiving CourseCode and Category.
	For sending the final response: pointer to array, used c_str() on string final response to get pointer



serverCS:

Description:

	Communicates with serverM using UDP.
	Reads the ee text file and stores all the information in a vector.
	It receives the CourseCode and Categories.
	It then seperates the text line in vector based on categories using substr().
	It compares with the input CourseCode with all the coursecodes, if there is a match, and the Category is also match, it stores
	the result in a string
	Then we send the result to serverM.
	If the result was not found it sends "NF" to serverM. 
	In both the casess string will be converted to pointer using c_str() to send to serverM
	

Format of all the messages exchanged:

	serverM and serverCS:
	charArray for receiving CourseCode and Category.
	For sending the final response: pointer to array, used c_str() on string final response to get pointer



The project works fine according to requirement.
Assumption 1: Categories entered will be correct.
Assumption 2: Once the password is valid and we go to type the course information we do not close client abruptly.
			  I did not handle this case, according to piazza post @376 I hope that is fine.



Reused code: I used code from Beej's  guide for creating TCP Connection, sigchld_handler function and also *get_in_addr function.
			 I used code from GeeksForGeeks for creating UDP Connections.
			 I have commented in the code where I used from either of the above cases.	






