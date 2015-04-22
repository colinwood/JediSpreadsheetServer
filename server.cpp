#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <set>
#include <cstring>
#include "session.h"
#include "spreadsheet.h"
#include <stack>

using namespace std;


void *accepted_callback(void *); //Forward decleration
vector<char*> tokenize(string delimiter, string target);
static session sesh;


void *accepted_callback(void *); //Forward decleration
vector<char*> tokenize(string delimiter, string target);
stack<pair<string, string> > * undoStack;
map<string, string> * cells;

int main(int argc , char *argv[])
{
    cells = new map<string, string>;
    undoStack = new stack<pair<string, string> >;
    int socket_desc; //Descriptor of the tcp client connected
    static int new_socket; //Socket for client
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
    pthread_t sniffer_thread[5];
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
    if ( bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << ("Binding failed\n");
        return 1;
    }
    cout << "Binding socket using the force!\n";

    //Listen for connections
    listen(socket_desc , 5);

    //Accept and incoming connection
    cout << "Waiting for incoming connections...\n";

    //Get the size of the client address.
    int i = 0;
    while (true)
    {
        client_address_size = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&client_address_size);
        cout << "Connection accepted\n";
        //Reply to the client letting them know they are connected
        new_sock = (int*) malloc(100);
        *new_sock = new_socket;

        pthread_create(&sniffer_thread[i] , NULL ,  accepted_callback , (void*) new_sock);
        i++;
        cout << "Client thread created for transmitting message\n";
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(sniffer_thread[i], NULL);
    }
    if (new_socket < 0)
    {
        perror("accept failed");
        return 1;
    }

}
//Ensurtes the user is in the user db also used to check if the username is unique
bool validate_user(string client_name)
{
    fstream readFile("users.txt");
    string line;
    while (getline(readFile, line)) {

        if (line == client_name) {
            readFile.close();
            return true;
        }
    }
    readFile.close();
    return false;
}



/*
 * After a connection is made calls this to transit messages
 * */
void* accepted_callback(void *socket_desc)
{
    cout << "Accepted callback..." << endl;
    //std::vector<string> cellnames;
    //std::vector<string> contents;
    int sock;
    //Get the socket descriptor
    sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[100];
    vector<char *> tokens;
    spreadsheet* user_sheet = NULL;
    string client_name;
    string sheet_name;

    const string connect = "connect";
    const string celcom = "cell";
    const string circular = "circular";
    const string undo = "undo";
    const string register_command = "register";

    stringstream messagestream;
    //Receive a message from client
    while ( (read_size = recv(sock , client_message , 100 , 0)) > 0 )
    {
        messagestream << client_message; //streaM used for holding the message in case we don't get full message
        tokens = tokenize("\n", messagestream.str());
        string response;
        bool cell_update = false;
        if (!tokens.empty())
        {

            vector<char *> command = tokenize(" ", tokens.at(0));
            if (!command.empty()) {
                //CONNECT COMMAND
                if (connect.compare(command.at(0)) == 0  && command.size() == 3) {
                    //cout << command.at(0) << "connect command"<< endl;
                    client_name = command.at(1);
                    sheet_name = command.at(2);

                    //validate the user if you cant then send error message
                    if (!validate_user(client_name)) {

                        response = "error 4 ";
                        response.append(client_name);
                        response.append("\n");
                        cout << "unregistered user connect request" << endl;
                    }
                    else {
                        user_sheet = sesh.connect(sheet_name, sock); //connect the user to the spreadsheet and pass along the socket
                        cout << "Client: " << client_name << " Connecting to : " << user_sheet << endl;
                        cout << sheet_name << " Users: " << user_sheet->connected_sockets.size() << endl;//output how many active users
                        response = "connected 2\n";
                        //Need to fetch sheet data here
                    }
                }
                //REGISTER COMMAND
                else if (register_command.compare(command.at(0)) == 0 && command.size() == 2) {

                    string client_name = command.at(1);

                    if (!validate_user(client_name)) {
                        cout << "Registering new user: " << client_name << endl;
                        std::fstream fs;
                        fs.open ("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                        fs << client_name << "\n";
                        fs.close();
                        response = "registered \n";
                    }
                    else {
                        response = "error 4 ";
                        response.append(client_name);
                        response.append("\n");
                    }


                }
                //CELL COMMAND
                else if (celcom.compare(command.at(0)) == 0 && command.size() > 2 ) {

                    string cellname = command.at(1);
                    string content = command.at(2);

                    command.erase(command.begin(), command.begin() + 1);
                    int i = 2;
                    while( i != command.size()){
                        content += command.at(i);
                        i++;
                    }
                    string oldcontent = "";
                    if (cells->count(cellname) > 0) {
                        oldcontent = (*cells)[cellname];
                    }
                    undoStack->push(pair<string, string>(cellname, oldcontent));
                    (*cells)[cellname] = content;
                    cout << cellname << " " << oldcontent << " inserted into the stack" << endl;
                    response = "cell ";
                    response += cellname;
                    response += " ";
                    response += content;
                    response += "\n";
                    cout << response << endl;
                    cell_update = true;
                }
                //UNDO COMMAND
                else if (undo.compare(command.at(0)) == 0 && command.size() == 1) {

                    string cellname;
                    string content;
                    //please add iterators here
                    //cellname = cellnames.at(cellnames.size()-i+1);
                    //content = contents.at(contents.size()-i+1);

                    cout << "Received undo request" << endl;
                    if (undoStack->size() > 0) {
                        cout << "The stack is being popped: see response in next message" << endl;
                        pair<string, string> blah = undoStack->top();
                        undoStack->pop();
                        response = "cell ";
                        response += blah.first;
                        response += " ";
                        response += blah.second;
                        response += "\n";
                        cout << response << endl;
                        //Possible fix:
                        (*cells)[blah.first] = blah.second;
                        cell_update = true; // flag to send out to everyone
                    }
                    else
                    {
                        cout << "but the stack is empty!" << endl;
                    }
                }
                //DID NOT RECEIVE A COMMAND
                else {
                    cout << "invalid command" << endl;
                    response = "error 2 invalid_command\n";
                }
            }
            //Make sure we actually have a sheet to iterate over
            if (cell_update && user_sheet != NULL) {
                for (std::vector<int>::iterator it = user_sheet->connected_sockets.begin(); it != user_sheet->connected_sockets.end(); ++it) {
                    int target_socket = *it;
                    write(target_socket, response.c_str(), response.size()); //Send Response to clients
                }
            }
            else {
                write(sock, response.c_str(), response.size()); //Send Response to clients
            }
            messagestream.str("");
        }
        memset(client_message, 0, sizeof client_message); //Clear out the message buffer.
    }

    if (read_size == 0 )
    {
        if(user_sheet != NULL){
        for (std::vector<int>::iterator it = user_sheet->connected_sockets.begin(); it != user_sheet->connected_sockets.end(); ++it) {
            int target_socket = *it;
            if (sock == target_socket) {
                user_sheet->connected_sockets.erase(it);
                break;
            }
        }
    }
        cout << "Client disconnected\n";
        fflush(stdout); //Flush std out
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);
    return 0;
}

//Helper method for splitting a string
vector<char*> tokenize(string delimiter, string target) {
    char * input = new char[target.size() + 1];
    std::copy(target.begin(), target.end(), input);
    input[target.size()] = '\0';
    char *token = std::strtok(input, delimiter.c_str());
    vector<char*> tokens;
    while (token != NULL) {
        tokens.push_back(token);
        token = std::strtok(NULL, delimiter.c_str());
    }
    return tokens;
}