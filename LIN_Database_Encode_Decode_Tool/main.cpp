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
    int operationChoice = 1;
    // Create a class to store LDF info
    LdfParser ldfFile;
    try {
        ldfFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/Seatdb.ldf");
        //        ldfFile.parse("/Users/wyfbw07/Downloads/Test_Database_Files/exampleLIN_medium.ldf");
        std::cout << ldfFile;
        // MARK: - Function call choices
        switch (operationChoice) {
            case 1:
            {
                // Decode
                int dlc = 2;
                int frameId = 2;
                unsigned char rawPayload[8] = { 0x25, 0xa8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
                std::map<std::string, double> result = ldfFile.decode(frameId, rawPayload, dlc);
                // Print decoded message info
                std::cout << "-----------------------------------------------" << std::endl;
                std::cout << "Decoded signal values: \n";
                for (auto& decodedSig : result) {
                    std::cout << "\t[Signal] " << decodedSig.first << ": " << decodedSig.second << std::endl;
                }
                /* Sample output should look like:
                 Decoded signal values:
                 [Signal] Head_Position: 0
                 [Signal] Horizontal_Position: 2
                 [Signal] SeatBack_Position: 5
                 [Signal] Vertical_Position: 5
                 */
            }
            case 2:
            {
                // Encode test case
                int frameId = 2;
                int encodedDlc = 0;
                unsigned char encodedPayload[8];
                std::vector<std::pair<std::string, double> > signalsToEncode;
                signalsToEncode.push_back(std::make_pair("Head_Position", 0));
                signalsToEncode.push_back(std::make_pair("Horizontal_Position", 2));
                signalsToEncode.push_back(std::make_pair("SeatBack_Position", 5));
                signalsToEncode.push_back(std::make_pair("Vertical_Position", 5));
                encodedDlc = ldfFile.encode(frameId, signalsToEncode, encodedPayload);
                // Print results
                std::cout << "-----------------------------------------------" << std::endl;
                std::cout << "Encoded frame size: " << encodedDlc << '\n';
                std::cout << "Display encoded payload as array (leftmost is [0]): ";
                for (short i = 0; i < 8; i++) {
                    printf("%x ", encodedPayload[i]);
                }
                std::cout << std::endl;
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
