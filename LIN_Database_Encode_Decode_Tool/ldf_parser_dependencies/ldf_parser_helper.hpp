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

// A special ver version of trim function that removes any leading and trailling white spaces
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

// Gives the last word in a string
inline std::string lastTokenOf(std::string& str) {
    std::string token;
    // read the first token from an input string stream constructed with the string in reverse
    std::istringstream( { str.rbegin(), str.rend() } ) >> token;
    
    // return the reverse of the token that was read
    return { token.rbegin(), token.rend() } ;
}

// A custom stoi function that detects the input number base
inline int stoi(std::string number) {
    if (number[0] == '0' && number.size() > 1)
    {
        if (number[1] == 'x')
        {
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
