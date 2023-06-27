//
//  main.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <iostream>
#include <stdexcept>
#include "ldf_parser.hpp"

int main()
{
    int operationChoice = 0;
    // Create a class to store DBC info
    LdfParser ldfFile;
    try {
         ldfFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/exampleLIN.ldf");
//        ldfFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/ldf_test.ldf");
        std::cout << ldfFile;
        // MARK: - Function call choices
        switch (operationChoice) {
        case 1:
        {
            // Decode
            int dlc = 2;
            int msgId = 2;
            unsigned char rawPayload[8] = { 0x77, 0xc3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
            std::map<std::string, double> result = ldfFile.decode(msgId, rawPayload, dlc);
            // Print decoded message info
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Decoded signal values: \n";
            for (auto& decodedSig : result) {
                std::cout << "\t[Signal] " << decodedSig.first << ": " << decodedSig.second << std::endl;
            }
            /* Sample output should look like:
             Decoded signal values:
                 [Signal] Head_Position: 3
                 [Signal] Horizontal_Position: 7
                 [Signal] SeatBack_Position: 8
                 [Signal] Vertical_Position: 7
            */
        }
        default:
            break;
        }
    }
    catch (std::invalid_argument& err) {
        std::cout << "[Exception catched] " << err.what() << '\n';
    }
    
    
    // Memory test
//    std::vector<LdfParser> parsers;
//    for (int i = 0; i < 10000; i++) {
//        try {
//            LdfParser ldfFile_temp;
//            ldfFile_temp.parse("/Users/wyfbw07/Downloads/Test_Database_Files/ldf_test.ldf");
//            parsers.push_back(ldfFile_temp);
//        }catch (std::invalid_argument& err) {
//            std::cout << "[Exception catched] " << err.what() << '\n';
//        }
//    }

    std::cout << "--------------END--------------" << std::endl;
    return 0;
}
