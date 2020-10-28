#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "QInt.h"


/*  Select C++17 in project properties to build  */


int main(int argc, char** argv) {
	std::string inp_path, out_path;

	std::ifstream fin(inp_path);
	if (!fin.is_open())
		std::cout << "Can not open " << inp_path << "\n";
	else {
		std::cout << "Open " << inp_path << " successfully\n";

		std::ofstream fout(out_path, std::ios::trunc);
		if (!fout.is_open())
			std::cout << "Can not create " << out_path << "\n";
		else {
			std::cout << "Create " << out_path << " successfully\n\n";


			// ----------------------------------------------------------


			std::string line;
			while (getline(fin, line, '\n')) {
				std::stringstream line_stream(line);
				std::vector<std::string> args;

				std::string arg;
				while (line_stream >> arg)
					args.emplace_back(arg);

				if (args.size() == 3) {
					//	3 operands operation
					if (args[1] == "~" || args[1] == "rol" || args[1] == "ror") {
						// Perform 3 operands operation
						// TYPE is args[0], operator is args[1], value is args[2]
						TYPE type = args[0] == "2" ? TYPE::BINARY : (args[0] == "10" ? TYPE::DECIMAL : TYPE::HEXADECIMAL);

						QInt value(type, args[2]);

						if (args[1] == "~")
							~value;
						else if (args[1] == "rol")
							value.rol();
						else
							value.ror();

						value.printStream(type, STREAM);
					}
					else {
						// Perform convert operation
						// TYPE_FROM is args[0], TYPE_TO is args[1], value is args[2]
						TYPE type_from = args[0] == "2" ? TYPE::BINARY : (args[0] == "10" ? TYPE::DECIMAL : TYPE::HEXADECIMAL);
						TYPE type_to = args[1] == "2" ? TYPE::BINARY : (args[1] == "10" ? TYPE::DECIMAL : TYPE::HEXADECIMAL);

						QInt value(type_from, args[2]);

						value.printStream(type_to, STREAM);
					}
				}
				else {
					//  4 operands operation
					// TYPE is args[0], value1 is args[1], operator is args[2], value2 is args[3]
					TYPE type = args[0] == "2" ? TYPE::BINARY : (args[0] == "10" ? TYPE::DECIMAL : TYPE::HEXADECIMAL);

					QInt value1(type, args[1]);
					QInt value2;
					if (args[2] != "<<" && args[2] != ">>")
						value2.scan(type, args[3]);

					if (args[2] == "+")
						value1 += value2;
					else if (args[2] == "-")
						value1 -= value2;
					else if (args[2] == "*")
						value1 *= value2;
					else if (args[2] == "/")
						value1 /= value2;
					else if (args[2] == "&")
						value1 &= value2;
					else if (args[2] == "|")
						value1 |= value2;
					else if (args[2] == "^")
						value1 ^= value2;
					else if (args[2] == "<<") {
						int num_bit = stoi(args[3]);
						value1 <<= num_bit;
					}
					else {
						int num_bit = stoi(args[3]);
						value1 >>= num_bit;
					}

					value1.printStream(type, STREAM);
				}
			}


			// ----------------------------------------------------------


			fout.close();
		}

		fin.close();
	}
}
