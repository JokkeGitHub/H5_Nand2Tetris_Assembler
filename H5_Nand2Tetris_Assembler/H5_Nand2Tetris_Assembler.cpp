#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <bitset>

using namespace std;

int main()
{
	vector<string> asmScriptCleaned;

	string line;
	ifstream asmFile("C:\\Users\\joac3146\\source\\repos\\H5_Nand2Tetris_Assembler\\H5_Nand2Tetris_Assembler\\input.asm");

	int variableBit = 16;

	map<string, int> labels;

	map<string, int> variables
	({
		{"SP", 0},
		{"LCL", 1},
		{"ARG", 2},
		{"THIS", 3},
		{"THAT", 4},
		{"R0", 0},
		{"R1", 1},
		{"R2", 2},
		{"R3", 3},
		{"R4", 4},
		{"R5", 5},
		{"R6", 6},
		{"R7", 7},
		{"R8", 8},
		{"R9", 9},
		{"R10", 10},
		{"R11", 11},
		{"R12", 12},
		{"R13", 13},
		{"R14", 14},
		{"R15", 15},
		{"SCREEN", 16384},
		{"KBD", 24576}
		});

	map<string, string> comp
	({
		{"0", "0101010"},
		{"1", "0111111"},
		{"-1", "0111010"},
		{"D", "0001100"},
		{"A", "0110000"},
		{"!D", "0001101"},
		{"!A", "0110001"},
		{"-D", "0001111"},
		{"-A", "0110011"},
		{"D+1", "0011111"},
		{"A+1", "0110111"},
		{"D-1", "0001110"},
		{"A-1", "0110010"},
		{"D+A", "0000010"},
		{"D-A", "0010011"},
		{"A-D", "0000111"},
		{"D&A", "0000000"},
		{"D|A", "0010101"},

		{"M", "1110000"},
		{"!M", "1110001"},
		{"-M", "1110011"},
		{"M+1", "1110111"},
		{"M-1", "1110010"},
		{"D+M", "1000010"},
		{"D-M", "1010011"},
		{"M-D", "1000111"},
		{"D&M", "1000000"},
		{"D|M", "1010101"}
		});

	map<string, string> dest
	({
		{"0", "000"},
		{"M", "001"},
		{"D", "010"},
		{"MD", "011"},
		{"A", "100"},
		{"AM", "101"},
		{"AD", "110"},
		{"AMD", "111"}
		});

	map<string, string> jump
	({
		{"0", "000"},
		{"JGT", "001"},
		{"JEQ", "010"},
		{"JGE", "011"},
		{"JLT", "100"},
		{"JNE", "101"},
		{"JLE", "110"},
		{"JMP", "111"}
		});

	// Removing comments and spaces
	if (asmFile.is_open())
	{
		while (asmFile)
		{
			getline(asmFile, line);
			asmFile >> line;

			for (size_t i = 0; i < line.length(); i++)
			{
				int length = line.length() - i;
				if (line[i] == '/')
				{
					line.erase(i, length);
					i = 99;
				}
			}
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

			if (line.empty() != true)
			{
				asmScriptCleaned.push_back(line);
			}
		}
	}
	asmFile.close();

	// Finding labels
	for (size_t i = 0; i < asmScriptCleaned.size(); i++)
	{
		string tempVariable = asmScriptCleaned[i];

		if (tempVariable[0] == '(')
		{
			tempVariable.erase(0, 1);
			tempVariable.erase(tempVariable.size() - 1);

			labels.insert({ tempVariable, (i - labels.size()) });

			asmScriptCleaned[i].erase();
		}
	}

	string filePath = "C:\\Users\\Public\\Nand2TetrisFil\\output.txt";
	ofstream textFile{ filePath, ios_base::app };

	for (size_t i = 0; i < asmScriptCleaned.size(); i++)
	{
		string tempVariable = asmScriptCleaned[i];

		if (tempVariable[0] == ' ')
		{
			continue;
		}
		else if (tempVariable[0] == '@')
		{
			bool existingVariable = false;
			tempVariable.erase(0, 1);

			if (isdigit(tempVariable[0]))
			{
				int tempInt = stoi(tempVariable);
				cout << std::bitset<16>(tempInt).to_string() << endl;
				textFile << std::bitset<16>(tempInt).to_string() << "\n";
				continue;
			}

			for (auto& x : labels)
			{
				if (x.first == tempVariable)
				{
					cout << std::bitset<16>(x.second).to_string() << endl;
					textFile << std::bitset<16>(x.second).to_string() << "\n";
					existingVariable = true;
					break;
				}
			}

			for (auto& x : variables)
			{
				if (x.first == tempVariable)
				{
					cout << std::bitset<16>(x.second).to_string() << endl;
					textFile << std::bitset<16>(x.second).to_string() << "\n";
					existingVariable = true;
					break;
				}
			}

			if (existingVariable == true)
			{
				continue;
			}
			else if (existingVariable == false)
			{
				variables.insert({ tempVariable, variableBit });
				cout << std::bitset<16>(variableBit).to_string() << endl;
				textFile << std::bitset<16>(variableBit).to_string() << "\n";
				variableBit++;
			}
		}
		else
		{
			string binaryOutput = "111";
			string tempSubstring;
			size_t position;

			if (tempVariable.find('=') != string::npos)
			{
				position = tempVariable.find('=');

				tempSubstring = tempVariable.substr(position + 1, tempVariable.size());

				for (auto& x : comp)
				{
					if (x.first == tempSubstring)
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				tempSubstring = tempVariable.substr(0, position);

				for (auto& x : dest)
				{
					if (x.first == tempSubstring)
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				for (auto& x : jump)
				{
					if (x.first == "0")
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				cout << binaryOutput << endl;
				textFile << binaryOutput << "\n";
			}
			else if (tempVariable.find(';') != string::npos)
			{
				position = tempVariable.find(';');

				tempSubstring = tempVariable.substr(0, position);

				for (auto& x : comp)
				{
					if (x.first == tempSubstring)
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				for (auto& x : dest)
				{
					if (x.first == "0")
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				tempSubstring = tempVariable.substr(position + 1, tempVariable.size());

				for (auto& x : jump)
				{
					if (x.first == tempSubstring)
					{
						binaryOutput.append(x.second);
						break;
					}
				}

				cout << binaryOutput << endl;
				textFile << binaryOutput << "\n";
			}
		}
	}

	textFile.close();
}