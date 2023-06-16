//
//  main.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <iostream>
#include <stdexcept>
#include "ldf_parser.hpp"

 // defining main with arguments
int main(int argc, char* argv[])
{
//    int operationChoice = 3;
    // Create a class to store DBC info
    LdfParser dbcFile;
    try {
        // Load file from path. Parse and store the content
        dbcFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/ldf_test.ldf");
        std::cout << dbcFile << std::endl;
    }
    catch (std::invalid_argument& err) {
        std::cout << "[Exception catched] " << err.what() << '\n';
    }
    std::cout << "--------------END--------------" << std::endl;
    return 0;
}
