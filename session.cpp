#include <iostream>
#include "spreadsheet.cpp"
#include <boost/regex.hpp>
#include <set>
#include <string>
#include "session.h"
using namespace std;

session::session(){

}
session::~session(){

}

spreadsheet session::connect(string sheet_name){
	spreadsheet s;
	s = spreadsheet();
	return s;
}