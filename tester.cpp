#include <iostream>
#include <boost/regex.hpp>
#include <string>


using namespace std;

int main(){

  string client_message = "register Johnny\n";

  string valid_pattern = ".*\n";
  static const boost::regex valid_regex(valid_pattern);
  boost::smatch what;
  regex_search(client_message, what, valid_regex, boost::match_not_dot_newline);
  string command = std::string(what[1].first,what[1].second);
  client_message.erase(0, command.size());

  cout << client_message;

}