all: 

	g++ server.cpp session.cpp spreadsheet.cpp -o server -lpthread -g
