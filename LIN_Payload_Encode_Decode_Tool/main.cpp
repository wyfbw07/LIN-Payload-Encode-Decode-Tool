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
	clock_t before_operation, after_operation;

	int operationChoice = 0;
	// Create a class to store LDF info
	LdfParser ldfFile;

	// mark the time before we start
	before_operation = clock();
	try {
		 ldfFile.parse("/Users/wyfbw07/Downloads/Volvo/Test_Database_Files/exampleLIN2.0.ldf");
		// ldfFile.parse("/Users/wyfbw07/Downloads/Volvo/Test_Database_Files/exampleLIN2.1.ldf");
		// ldfFile.parse("/Users/wyfbw07/Downloads/Volvo/Test_Database_Files/exampleLIN2.2.ldf");
		std::cout << ldfFile;
		// mark the time once we are done
		after_operation = clock();
		// MARK: - Function call choices
		switch (operationChoice) {
		case 1:
		{
			// Decode
			int dlc = 4;
			int frameId = 0x20;
			unsigned char rawPayload[8] = { 0xa0, 0x0, 0x0, 0x08, 0xff, 0xff, 0xff, 0xff };
			std::map<std::string, std::tuple<double, std::string, LinSigEncodingValueType> > result = ldfFile.decode(frameId, rawPayload, dlc);
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
			int encodedDlc = 4;
			unsigned char encodedPayload[8];
			std::vector<std::pair<std::string, double> > signalsToEncode;
			signalsToEncode.push_back(std::make_pair("Reg_Set_Voltage", 14.6));
			signalsToEncode.push_back(std::make_pair("Ramp_Time", 0));
			signalsToEncode.push_back(std::make_pair("Cut_Off_Speed", 0));
			signalsToEncode.push_back(std::make_pair("Exc_Limitation", 0));
			signalsToEncode.push_back(std::make_pair("Derat_Shift", 0));
			signalsToEncode.push_back(std::make_pair("MM_Request", 1));
			signalsToEncode.push_back(std::make_pair("Reg_Blind", 0));
			encodedDlc = ldfFile.encode(frameId, signalsToEncode, encodedPayload);
			// Print results
			if (encodedDlc != -1) {
				std::cout << "-----------------------------------------------" << std::endl;
				std::cout << "Encoded frame size: " << encodedDlc << '\n';
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

	// print statistics
	double operation_time = double(after_operation - before_operation) / CLOCKS_PER_SEC;
	std::cerr << "Operation time:     " << operation_time << std::endl;
	std::cout << "----------------------END----------------------" << std::endl;
	return 0;
}
