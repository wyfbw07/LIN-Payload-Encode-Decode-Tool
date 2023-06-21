//
//  ldf_parser_helper.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/20/23.
//

#include <iostream>
#include "ldf_parser_helper.hpp"

bool Helper::findMatch(std::string targetMatch, std::string candidateString, std::istream& in, char delimiter){
    if (candidateString == targetMatch + delimiter) {
        candidateString.pop_back();
        return true;
    }
    else if (candidateString == targetMatch) {
        // Save istream positon before peeking next word
        std::streampos posBeforePeek = in.tellg();
        char delimiterRead;
        in >> delimiterRead;
        if (delimiterRead == delimiter) {
            return true;
        }
        else {
            // No match found, restore istream position
            in.seekg(posBeforePeek);
            return false;
        }
    }
    return false;
}
