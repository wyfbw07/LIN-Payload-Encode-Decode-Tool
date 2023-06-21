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

inline std::string& trim(std::string& str)
{
    str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    return str;
}

inline std::string trimCopy(std::string& str)
{
    str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    return str;
}

inline std::string& getWordBeforeDelimitor(std::string& word, char delimiter) {
    std::stringstream lineStream(word);
    getline(lineStream, word, delimiter);
    word.erase(remove(word.begin(), word.end(), '\r'), word.end());
    trim(word);
    return word;
}

}

#endif /* ldf_parser_helper_hpp */
