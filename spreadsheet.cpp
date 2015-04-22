#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <unistd.h>
#include "spreadsheet.h"
#include <cstring>

using namespace std;


//Will be used for managing a spreadsheet and is not near complete
//Is going to need check for circular references and will eventually
//have all the commands for managing a spreadsheet  such as updating cells
//keeping track and performing undo's, and saving the file.
spreadsheet::spreadsheet(string sheet_name) {
  this->sheet_name = sheet_name;
  this->open_file(sheet_name);
}
//empty contructor
spreadsheet::spreadsheet() {

}
spreadsheet::~spreadsheet() {
}

//Process a ommand on this spreadsheet.
void spreadsheet::process_command(string command) {
  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init failed\n");
  }
  pthread_mutex_lock(&lock);
  //code for editing a spreadsheet goes here to prevent conncurrent operations
  pthread_mutex_unlock(&lock);
}
//reads file and returns the cell commands
void spreadsheet::open_file(string sheet_name){
  string path = "sheets/";
  path += sheet_name;
  path += ".txt";

  fstream readFile(path.c_str());
    string line;
    while (getline(readFile, line)) {
      vector<char *> tokens = tokenize(" ", line);
      cells_map[tokens.at(1)] = tokens.at(2);
    }
    readFile.close(); 
}

std::string spreadsheet::get_all_cells(){
  string response= "";

  map<string, string>::iterator iterator;
  for(iterator = cells_map.begin(); iterator != cells_map.end(); iterator++) {
    response += "cell ";
    response += iterator->first;
    response += " ";
    response += iterator->second;
    response += "\n";
  }
  return response;
}

void save(){

}

//creates an empty document for writing to.
void spreadsheet::create(string sheet_name) {
  string path = "sheets/";
  path += sheet_name;
  path += ".txt";
  cout << path << endl;

  ofstream file(path.c_str());
  if (!file)
    cerr << "Can't create file for spreadsheet\n";
  file.close();
}

vector<char *> spreadsheet::tokenize(string delimiter, string target) {
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

