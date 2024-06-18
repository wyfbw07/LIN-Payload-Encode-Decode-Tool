//
//  main.cpp
//  LIN_Database_Encode_Decode_Tool
//
//  Created by Yifan Wang on 6/14/23.
//

#include <bitset>
#include <iomanip>
#include <iostream>
#include "ldf_parser.hpp"

int main()
{
	// timing mechanism
    clock_t parserBeforeOperation, parserAfterOperation;
	clock_t totalBeforeOperation, totalAfterOperation;
    // mark the time before we start
    parserBeforeOperation = clock(); totalBeforeOperation = clock();

	int operationChoice = 2;
	// Create a class to store LDF info
	LdfParser ldfFile;
    
    try {
        // Access the sample ldf
        std::string projectDir = "/Users/Local/LIN_Payload_Encode_Decode_Tool"; // REPLACE THIS to your project directory
        std::string sampleLdfName = "exampleLIN_2.0_Large.ldf";
        ldfFile.parse(projectDir + "/sample_ldf/" + sampleLdfName);
        
        // Show parser results
		std::cout << ldfFile;
        
        // mark the time once we are done
        parserAfterOperation = clock();
        
		// MARK: - Function call choices
		switch (operationChoice) {
		case 1:
		{
			// Decode
			int msgSize = 4;
			int frameId = 0x20;
			unsigned char rawPayload[8] = { 0xa0, 0x0, 0x0, 0x08, 0xff, 0xff, 0xff, 0xff };
			std::map<std::string, std::tuple<double, std::string, LinSigEncodingValueType> > result;
			result = ldfFile.decode(frameId, msgSize, rawPayload);
			// Print decoded message info
			std::cout << "-----------------------------------------------" << std::endl;
			std::cout << "Decoded signal values: \n";
			for (auto& decodedSig : result) {
				std::cout << "\t[Signal] " << decodedSig.first << ": "
					<< std::fixed << std::get<0>(decodedSig.second) << std::setprecision(3);
				if (std::get<1>(decodedSig.second) != "") {
					std::cout << " " << std::get<1>(decodedSig.second) << std::endl;
				}
				else {
					std::cout << std::endl;
				}
			}
		}
		break;
		case 2:
		{
			// Encode test case
			int frameId = 0x20;
			unsigned char encodedPayload[8];
			std::vector<std::pair<std::string, double> > signalsToEncode;
			signalsToEncode.push_back(std::make_pair("Reg_Set_Voltage", 14.6));
			signalsToEncode.push_back(std::make_pair("Ramp_Time", 0));
			signalsToEncode.push_back(std::make_pair("Cut_Off_Speed", 0));
			signalsToEncode.push_back(std::make_pair("Exc_Limitation", 0));
			signalsToEncode.push_back(std::make_pair("Derat_Shift", 0));
			signalsToEncode.push_back(std::make_pair("MM_Request", 1));
			signalsToEncode.push_back(std::make_pair("Reg_Blind", 0));
            int encodedfrmSize = ldfFile.encode(frameId, signalsToEncode, encodedPayload);
			// Print results
			if (encodedfrmSize != -1) {
				std::cout << "-----------------------------------------------" << std::endl;
				std::cout << "Encoded frame size: " << encodedfrmSize << '\n';
				std::cout << "Display encoded payload as array (leftmost is [0]): ";
				for (short i = 0; i < 8; i++) {
					printf("%x ", encodedPayload[i]);
				}
				std::cout << std::endl;
				for (short i = 0; i < 8; i++) {
					std::cout << std::bitset<8>(encodedPayload[i]) << " ";
				}
				std::cout << std::endl;
			}
		}
		break;
		default:
			break;
		}
	}
	catch (std::invalid_argument& err) {
		std::cout << "[Exception catched] " << err.what() << '\n';
		return -1;
	}
    
    // mark the time once we are done
    totalAfterOperation = clock();
    
	// print statistics
	double parserOperationTime = double(parserAfterOperation - parserBeforeOperation) / CLOCKS_PER_SEC;
    double totalOperationTime = double(totalAfterOperation - totalBeforeOperation) / CLOCKS_PER_SEC;
	std::cout << "\nParser operation time:\t" << parserOperationTime << std::endl;
    std::cout << "Total operation time:\t" << totalOperationTime << std::endl;
	std::cout << "----------------------END----------------------" << std::endl;
	return 0;
}
