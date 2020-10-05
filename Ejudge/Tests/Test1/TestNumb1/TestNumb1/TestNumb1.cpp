#include <iostream>
#include <string>
#include <vector>
#include <set>


//#define DEBUG

int main() {
	int64_t result = 0;
	std::string input = "";
	std::vector<std::string> strNumbers;
	
	//std::cout << INT64_MAX << std::endl;


	//std::getline(std::cin, input);
	// std::cout << "\n\n";
	//std::cin.setstate(std::istream::app);
	while (std::getline(std::cin, input)) {
		bool assemblingNumber = false;
		bool negative = false;
		std::string asmblNum = "";
#ifdef DEBUG
		std::cout << "> New line: '" << input << "'\n";
#endif // DEBUG
		for (auto inputIt = input.begin(); inputIt != input.end(); inputIt++) {
			char curSymb = *inputIt;
#ifdef DEBUG
			std::cout << "> New symbol: '" << curSymb << "'\n";
#endif // DEBUG

			if (curSymb == '-') {
				//if (!negative/* && !assemblingNumber*/ && lastSymbols.empty()) {
				//	/*asmblNum += '-';*/
				//	//assemblingNumber = true;
				//	negative == true;
				//}
				if (assemblingNumber /*&& lastSymbols.empty()*/) {
					result += std::stoll(asmblNum);	// TODO: Add exceptions

#ifdef DEBUG
					std::cout << "> Computing Sum: " << (result) << std::endl;
#endif // DEBUG

					asmblNum = "";
					assemblingNumber = false;
					negative = false;
				}
				else {
					if (negative) {  // minus exist
						continue;
					}
					else {
						negative = true;
						asmblNum += (curSymb);
					}
					/*if (!lastSymbols.empty()) {
						if (lastSymbols.top() == '-') {
							continue;
						}
					}
					else {
						lastSymbols.push(curSymb);
						negative = true;
					}*/
				}

			}
			else
				if (curSymb > 47 && curSymb < 58) {  // 48 is '0', 57 is '9'

					if (!assemblingNumber)
						assemblingNumber = true;
					asmblNum += (curSymb);
					if (inputIt == input.end() - 1) {
						result += std::stoll(asmblNum);	// TODO: Add exceptions
						asmblNum = "";
						assemblingNumber = false;

#ifdef DEBUG
						std::cout << "> Computing ENDLine Sum: " << (result) << std::endl;
#endif // DEBUG
					}
				}
				else {
					if (assemblingNumber) {
						result += std::stoll(asmblNum);	// TODO: Add exceptions
						asmblNum = "";
						assemblingNumber = false;
						negative = false;

#ifdef DEBUG
						std::cout << "> Computing Sum: " << (result) << std::endl;
#endif // DEBUG

					}
					else {
						//if (!lastSymbols.empty()) {
						//	lastSymbols.pop();
						//	if (curSymb == '-') {
						//		lastSymbols.push(curSymb);
						//		negative = true;
						//	}
						//	/*if (lastSymbols.top() == '-') {

						//	}*/
						//}
						//else {

						//}
					}
				}
		}
		input = "";
	}

	std::cout << std::to_string(result) << std::endl;



}

   /*
1
2
3
4
5

   */