#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include<string>


class spreadsheet{
	public:
		spreadsheet(std::string sheet_name); //Creates a brand new spreadsheet
		spreadsheet();
		~spreadsheet();
		static void create(std::string sheet_name);//Needs to create an empty sheetwith necessary xml data
		void process_command(std::string command);//Thread safe method that process spreadsheet commands
		std::string sheet_name;
		std::vector<int> connected_sockets; //holds pointers to all the active sockets working on this sheet
	private:
		pthread_mutex_t lock;
		
};
#endif