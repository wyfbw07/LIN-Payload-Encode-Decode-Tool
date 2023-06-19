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
int main()
{
//    int operationChoice = 3;
    // Create a class to store DBC info
    LdfParser ldfFile;
    try {
        // Load file from path. Parse and store the content
        ldfFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/ldf_test.ldf");
        std::cout << ldfFile << std::endl;
    }
    catch (std::invalid_argument& err) {
        std::cout << "[Exception catched] " << err.what() << '\n';
    }
    std::cout << "--------------END--------------" << std::endl;
    return 0;
}
