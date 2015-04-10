#include <iostream>
#include <string>

#include <boost/regex.hpp>
#include <string>
#include <sstream>
using namespace std;

int main(){
	stringstream ss;
	
	
		string s = "Hello\nmy y name is ";
		ss << s;
		
		string new_line = "\n";
		std::size_t found = ss.str().find(new_line);
	  	
	  	if (found!=std::string::npos){
	  		char command[found];
	  		ss.str().copy(command, found, 0);
	  		char new_command[ss.str().size() - found + 1];
	  		ss.str().copy(new_command, ss.str().size() - found + 1, found + 1 );
	  		cout << command << "\n";
	  		ss.str("");
	  		ss << new_command;
	  	}
  

}