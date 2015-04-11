#include <iostream>
#include <string>
#include "spreadsheet.h"
#include <fstream>
#include <sstream>

using namespace std;


//Will be used for managing a spreadsheet and is not near complete
//Is going to need check for circular references and will eventually
//have all the commands for managing a spreadsheet  such as updating cells
//keeping track and performing undo's, and saving the file.  
spreadsheet::spreadsheet(string sheet_name){
	this->sheet_name = sheet_name;
	this->users = 0;
}
spreadsheet::~spreadsheet(){
}

string spreadsheet::process_command(string command){
	string s = "hello";
	return s;
}
//Looks in the sheets folder and copies

void spreadsheet::create(string sheet_name){
	   char ch;
	   stringstream name_stream;
	   name_stream << "sheets/" << sheet_name << ".xml"; 
	   string name_string = name_stream.str();
       ifstream f1("sheets/base_spreadsheet.xml");
       ofstream f2(name_string.c_str());
       
       if(!f1) 
          cerr <<"Can't open IN file\n";
       if(!f2) 
          cerr << "Can't open OUT file\n";
       
       while(f1 && f1.get(ch) ) 
          f2.put(ch);

	spreadsheet s(sheet_name);
}

