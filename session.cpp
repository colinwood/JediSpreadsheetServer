/***********************
Author: Colin Wood
Date:   4/10/2014

Summary:

Class that uses a vector to store pointers to all the active spreadsheets. 
Returns a pointer the spreadsheet the user requests. The server needs to 
notify the session each time a user disconnects othersize the sheet will remain 
open. Needs to have a way to manage users added. 

***********************/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "spreadsheet.h"
#include "session.h"

using namespace std;

//Empty constructor
session::session(){
}
//Destructor
session::~session(){
}
//returns a pointer to the spreadsheet that is capable of modifying the spreadsheet
spreadsheet * session::connect(string sheet_name){
	//Creaet the sheet if it does not exist
	if(!sheet_exists(sheet_name)){
		spreadsheet::create(sheet_name);
	}
	//Check if the sheet is open
	if(!sheet_open(sheet_name)){
		spreadsheet *s = new spreadsheet(sheet_name);
		sheet_vector.push_back(s);
	}
	
	int location;
	for(std::vector<spreadsheet*>::iterator it = this->sheet_vector.begin(); it != sheet_vector.end(); ++it) {
	   
		if((*it)->sheet_name == sheet_name)
		{
			location = distance(sheet_vector.begin(),it);
		}
	}	
	sheet_vector.at(location)->users = sheet_vector.at(location)->users + 1;//increment the user count on this spreadsheet
	return sheet_vector.at(location);
}
//Closes the active spreadsheet if there is no one using it
//it is crucial that the server notifies the session when
//a user disconnects. 
void session::disconnect(string sheet_name){
	
	int location;
	for(std::vector<spreadsheet*>::iterator it = this->sheet_vector.begin(); it != sheet_vector.end(); ++it) {
		if((*it)->sheet_name == sheet_name)
		{
			location = distance(sheet_vector.begin(),it);
			spreadsheet *target = sheet_vector.at(location);
			target->users = target->users - 1;
			if(target->users == 0){
				sheet_vector.erase(it);
				return;
			}
		}
	}	
}
//Check if a sheet exists on the server
//returns true when the sheet is located in the sheet folder
bool session::sheet_exists(string sheet_name){
	stringstream path;
	path << "sheets/" << sheet_name << ".xml"; //define path to file
	ifstream file(path.str()); //Check if it is there by trying to open it
	if(file.is_open() || file.good()){
		file.close(); //Close the file
		return true;
	}
	else
		return false;
}	
//Check if the sheet is open by looking at all the spreadsheet 
//pointers in the vector and seeing if any of their names 
//are equal to the requested one. 
bool session::sheet_open(string sheet_name){
	for(std::vector<spreadsheet *>::iterator it = this->sheet_vector.begin(); it != sheet_vector.end(); ++it) {
		if((*it)->sheet_name == sheet_name)
		{
			return true;
		}
	}
	return false;
}		



		   