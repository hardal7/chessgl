// C++ program to illustrate the client application in the 
// socket programming 
#include <cstring>
#include <iostream> 
#include <netinet/in.h> 
#include <unistd.h> 

int main() 
{ 
	// creating socket 
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 

	// specifying address 
	sockaddr_in serverAddress; 
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(3169); 
	serverAddress.sin_addr.s_addr = INADDR_ANY; 

	// sending connection request 
	connect(clientSocket, (struct sockaddr*)&serverAddress, 
			sizeof(serverAddress)); 

	// sending data 
	const char* message = "Hello, server!"; 
	send(clientSocket, message, strlen(message), 0); 

	// recieving data 
	char buffer[1024] = { 0 }; 
	recv(clientSocket, buffer, sizeof(buffer), 0); 
  std::cout << "Message from client: " << buffer; 

	// closing socket 
	close(clientSocket); 

	return 0; 
}
