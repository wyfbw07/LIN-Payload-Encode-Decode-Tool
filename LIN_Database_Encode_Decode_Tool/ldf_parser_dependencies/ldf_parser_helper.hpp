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

namespace Utils {

/*  A special ver version of trim function that removes any leading and trailling white spaces
    and also removes all occurrences of new line and tab characters
 */
inline std::string& trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);   //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));   //prefixing spaces
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

inline std::string trimCopy(std::string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);   //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));   //prefixing spaces
    return str;
}

inline std::string& getWordBeforeDelimitor(std::string& word, char delimiter) {
    std::stringstream lineStream(word);
    getline(lineStream, word, delimiter);
    word.erase(remove(word.begin(), word.end(), '\r'), word.end());
    trim(word);
    return word;
}

inline std::string getLine(std::istream& lineStream, char delimiter) {
    std::string word;
    getline(lineStream, word, delimiter);
    trim(word); // Remove leading and/or trailling white spaces
    return word;
}

}

#endif /* ldf_parser_helper_hpp */
