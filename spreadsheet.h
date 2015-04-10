#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include<string>

class spreadsheet{
	public:
		spreadsheet(std::string sheet_name); //Creates a brand new spreadsheet
		~spreadsheet();
		static void create(std::string sheet_name);//Needs to create an empty sheetwith necessary xml data
		std::string process_command(std::string command);
		std::string sheet_name;
		int users;
	private:
};
#endif