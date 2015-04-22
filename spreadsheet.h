#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include<string>
#include <map>

class spreadsheet{
	public:
		spreadsheet(std::string sheet_name); //Creates a brand new spreadsheet
		spreadsheet();
		~spreadsheet();
		static void create(std::string sheet_name);//Needs to create an empty sheetwith necessary xml data
		void process_command(std::string command);//Thread safe method that process spreadsheet commands
		std::string sheet_name;
		std::vector<int> connected_sockets; //holds pointers to all the active sockets working on this sheet
		void open_file(std::string sheet_name);
		std::map<std::string, std::string> cells_map;
		std::string get_all_cells();
		void save();
	private:
		pthread_mutex_t lock;
		std::vector<char *> tokenize(std::string delimiter, std::string target);
		

		
};
#endif