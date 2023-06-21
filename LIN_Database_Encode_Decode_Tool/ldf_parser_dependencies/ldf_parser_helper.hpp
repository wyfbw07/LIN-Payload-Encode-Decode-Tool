//
//  ldf_parser_helper.hpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/20/23.
//

#ifndef ldf_parser_helper_hpp
#define ldf_parser_helper_hpp

#include <string>
#include <fstream>


namespace Helper {

bool findMatch(std::string targetMatch, std::string candidateString, std::istream& in, char delimiter);

}

#endif /* ldf_parser_helper_hpp */
