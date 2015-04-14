#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include "spreadsheet.h"

using namespace std;


//Will be used for managing a spreadsheet and is not near complete
//Is going to need check for circular references and will eventually
//have all the commands for managing a spreadsheet  such as updating cells
//keeping track and performing undo's, and saving the file.  
spreadsheet::spreadsheet(string sheet_name){
	this->sheet_name = sheet_name;
}
//empty contructor
spreadsheet::spreadsheet(){

}
spreadsheet::~spreadsheet(){
}

//Process a ommand on this spreadsheet. 
void spreadsheet::process_command(string command){
   if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }
    pthread_mutex_lock(&lock);
    //code for editing a spreadsheet goes here to prevent conncurrent operations
    pthread_mutex_unlock(&lock);
}
//creates an empty document for writing to. 
void spreadsheet::create(string sheet_name){
	   char ch;
	   stringstream name_stream;
	   name_stream << "sheets/" << sheet_name << ".txt"; 
	   string name_string = name_stream.str();
       ofstream file(name_string.c_str());    
       if(!file) 
          cerr <<"Can't create file for spreadsheet\n";
        file.close();     
}

