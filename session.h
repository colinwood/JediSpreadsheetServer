#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include "spreadsheet.h"

class session{

	public:
		session(); //Constructor 
		~session(); //destructor
		spreadsheet* connect(std::string sheet_name, int user_socket); 		//Abstracts whether or not a spreadsheet is already open and just sends the xml data regardless
		void disconnect(spreadsheet* user_sheet, int user_socket);	
	private:
		std::vector<spreadsheet*> sheet_vector; 			//Container that holds all of the active spreadsheets
		bool sheet_exists(std::string sheet_name); 			//Check if a sheet exists 
		bool sheet_open(std::string sheet_name); 			//Check if the sheet is open
		pthread_mutex_t lock;							    //Used to prevent opending sheets concurrently. 
};
#endif