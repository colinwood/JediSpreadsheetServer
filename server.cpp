/*
* A simple asych server that as of today just echose back what a client sends to it. 
*/

#include <stdio.h>
#include <string.h>    
#include <stdlib.h>   
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>    
#include <pthread.h> 
#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <sstream>

using namespace std;

void *accepted_callback(void *); //Forward decleration

int main(int argc , char *argv[])
{

    int socket_desc; //Descriptor of the tcp client connected
    int new_socket; //Socket for client
    int client_address_size; //How long the address of the client is
    int *new_sock; //Pointer to the new socket
    struct sockaddr_in server , client;
    char *message; //Incoming message

    int port_number; //Port the client can connect on

     if (argc < 2) {
        cout << "Enter a port or i will beat you with my lightsaber\n";
        exit(1);
     }
    port_number = atoi(argv[1]); // convert the port to an int

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        cout << "Could not create socket\n";
    }

    //PFill out the socketaddr structire
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port_number );

    //Bind the socket so we can use it like a jedi
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << ("Binding failed\n");
        return 1;
    }
    cout << "Binding socket using the force!\n";

    //Listen for connections
    listen(socket_desc , 3);

    //Accept and incoming connection
    cout << "Waiting for incoming connections...\n";

    client_address_size = sizeof(struct sockaddr_in); //Get the size of the client address. 
    
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&client_address_size)) )
    {
        cout << "Connection accepted\n";

        //Reply to the client letting them know they are connected
        message = (char*) "Client you are connected...\n";
        write(new_socket , message , strlen(message));

        pthread_t sniffer_thread;
        new_sock = (int*) malloc(1);
        *new_sock = new_socket;

        if( pthread_create( &sniffer_thread , NULL ,  accepted_callback , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        cout << "Client thread created for transmitting message\n";
    }

    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}


/*
 * After a connection is made calls this to transit messages
 * */
void *accepted_callback(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

    //Send some messages to the client
    message = (char*) "Enter something and i will echo it back. BTW nice shirt\n";
    write(sock , message , strlen(message));

    
    string message_string;

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {

        //Send the message back to client
        write(sock , client_message , strlen(client_message));

        cout << client_message;
        

        //check if it contains a new line
        //if new line process message
        //else append to current message
        //clear out buffer

        memset(client_message, 0, sizeof client_message); //Clear out the message buffer. 
        //This is where we need to wait for the message terminator

    }

    if(read_size == 0)
    {
        cout << "Client disconnected\n";
        fflush(stdout); //Flush std out
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);
    return 0;
}