#include <iostream>
#include <string>

// #define DEBUG

void AddNewNumber(int64_t& sum, std::string & newStringNum, bool & asmbling, bool & negative) {
    sum += std::stoll(newStringNum);

#ifdef DEBUG
    std::cout << "> Computing Sum: " << (result) << std::endl;
#endif // DEBUG

    newStringNum = "";
    asmbling = false;
    negative = false;
}

int main() {
    int64_t result = 0;
    std::string input;

    try {

        while (std::getline(std::cin, input)) {
            bool assemblingNumber = false;
            bool negative = false;
            std::string asmblNum;

#ifdef DEBUG
            std::cout << "> New line: '" << input << "'\n";
#endif // DEBUG

            for (auto inputIt = input.begin(); inputIt != input.end(); inputIt++) {

                char curSymb = *inputIt;

#ifdef DEBUG
                std::cout << "> New symbol: '" << curSymb << "'\n";
#endif // DEBUG

                if (curSymb == '-') {
                    if (assemblingNumber /*&& lastSymbols.empty()*/)
                        AddNewNumber(result, asmblNum, assemblingNumber, negative);
                    else {
                        if (negative)  // minus exist
                            continue;
                        else {
                            negative = true;
                            asmblNum += (curSymb);
                        }
                    }
                } else {
                    if (curSymb > 47 && curSymb < 58) {  // 48 is '0', 57 is '9'

                        if (!assemblingNumber)
                            assemblingNumber = true;
                        asmblNum += (curSymb);
                        if (inputIt == input.end() - 1)
                            AddNewNumber(result, asmblNum, assemblingNumber, negative);
                    } else {
                        if (assemblingNumber)
                            AddNewNumber(result, asmblNum, assemblingNumber, negative);
                    }
                }
            }
            input = "";
        }
    }
    catch (int64_t &) {
        std::cerr << "\n********\n"
                     "Error! Exception with numeric type."
                     "\n********";
    }
    catch (std::invalid_argument &) {
        std::cerr << "\n********\n"
                     "Error! Conversion to numeric type failed!"
                     "\n********";
    }

    std::cout << std::to_string(result) << std::endl;
}