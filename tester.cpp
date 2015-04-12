#include <iostream>
#include <string>
#include <vector>

using namespace std;
//returns a vector of tokens
vector<char*> tokenize(string delimiter, string target){
	char * input = new char[target.size() + 1];
	std::copy(target.begin(), target.end(), input);
	input[target.size()] = '\0';
	char *token = std::strtok(input, delimiter.c_str());
	vector<char*> tokens;
	while(token != NULL){
		tokens.push_back(token);
		token = strtok (NULL, delimiter.c_str());
	}
	return tokens;
}
int main()
{
	char* s = "bannanas";
	vector<char*> tokens = tokenize("\n", s);

}

