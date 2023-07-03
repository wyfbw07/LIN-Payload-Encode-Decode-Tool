//
//  ldf_parser_helper.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/20/23.
//

#ifndef ldf_parser_helper_hpp
#define ldf_parser_helper_hpp

#include <string>
#include <sstream>
#include <algorithm>

namespace utils {

	// A special version of trim function that removes any leading and trailling white spaces
	// and also removes all occurrences of new line and tab characters
	inline std::string& trim(std::string& str)
	{
		str.erase(str.find_last_not_of(' ') + 1);   //suffixing spaces
		str.erase(0, str.find_first_not_of(' '));   //prefixing spaces
		str.erase(remove(str.begin(), str.end(), '\t'), str.end());
		str.erase(remove(str.begin(), str.end(), '\r'), str.end());
		str.erase(remove(str.begin(), str.end(), '\n'), str.end());
		return str;
	}

	// A custom getline function that trims the word before returning
	inline std::string getline(std::istream& lineStream, char delimiter) {
		std::string word;
		getline(lineStream, word, delimiter);
		trim(word);
		return word;
	}

	// Gives the condition name in a string
	inline std::string lastTokenOf(std::string& str) {
		// Read the first token from an input string stream constructed with the string in reverse
		// And read until the first white space
		std::string reversedToken, conditionName;
		std::istringstream({ str.rbegin(), str.rend() }) >> reversedToken;
		// Read until either delimeters
		for (size_t i = 0; i < reversedToken.size(); i++) {
			if (reversedToken[i] == ';' || reversedToken[i] == '}') {
				break;
			}
			conditionName += reversedToken[i];
		}
		// Reverse back the token
		return { conditionName.rbegin(), conditionName.rend() };
	}

	// A custom stoi function that detects the input number base
	inline int stoi(std::string number) {
		if (number[0] == '0' && number.size() > 1) {
			if (number[1] == 'x') {
				// Input is HEX
				return std::stoi(number, 0, 16);
			}
			// Input is OCTAL
			return std::stoi(number, 0, 8);
		}
		// Input is DEC
		return std::stoi(number, 0, 10);
	}

}

#endif /* ldf_parser_helper_hpp */
