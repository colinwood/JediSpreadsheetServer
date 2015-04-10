#ifndef SESSION_H
#define SESSION_H


class session{

	public:
		session(); //Constructor 
		~session(); //destructor
		spreadsheet connect(std::string sheet_name); //Abstracts whether or not a spreadsheet is already open and just sends the xml data regardless
	private:
		std::set<spreadsheet> sheet_list; //Container that holds all of the active spreadsheets
		
};
#endif