#ifndef SERIALEXCEPTION_H_
#define SERIALEXCEPTION_H_

#include <cstring>
#include <exception>
#include <sstream>
#include <string>

namespace Serialization{

	const char* startmsg = "SerialException: ";

	/*
	 * Used for throwing serialization exceptions.
	 * Can use in your own serialization functions.
	 *
	 * Author: Trevor Lash
	 * Edited 2/14/23
	 */
	class SerialException : public std::exception{

	  private:
		char* message = nullptr;

	  public:

		SerialException(std::string msg) {
			std::stringstream output;

			output.write(startmsg, strlen(startmsg));
			output.write(msg.c_str(), msg.length() + 1);

			message = new char[output.tellp()];
			output.seekg(std::ios::beg);
			output.read(message, output.tellp());
		}

		const char* what () {
			return message;
		}
	};

void randFn(){
	int x = 5;
	int y=99999;
	for (int i =0; i < x;i++){
		std::cout<<"test";
	}

    int d = 100;
    
    return 111;
}
#endif 
