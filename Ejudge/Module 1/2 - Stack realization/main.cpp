#include <iostream>
#include <string>
#include <regex>
#include <exception>

class ParseExceptions : public std::exception {
private:
    std::string _error;

public:

    ParseExceptions() = default;

    explicit ParseExceptions(std::string error) : _error(std::move(error)) {}

    ~ParseExceptions() override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return _error.c_str();
    }


};

template<typename TElement>
class myStack {
private:
    TElement *_array;
    size_t _size;
    size_t _maxSize;

    myStack(const myStack &copy) = delete;

    myStack(const myStack &&move) noexcept = delete;

    myStack &operator=(const myStack &copy) = delete;

    myStack &operator=(const myStack &&copy) = delete;

public:

    explicit myStack(size_t size) {
        _size = 0;
        _maxSize = size;
        _array = new TElement[size];
    }

    ~myStack() {

        delete[] _array;
    }

    void push(const TElement newElem) {
        if (_size < _maxSize) {
            _array[_size] = newElem;
            _size++;
        } else {
            throw ParseExceptions("overflow");
        }
    }

    TElement pop() {
        if (_size > 0) {
            TElement beforeDelete = _array[_size - 1];
            _size--;
            return _array[_size];
        } else
            throw ParseExceptions("underflow");
    }

    void print() {
        if (_size == 0) {
            throw ParseExceptions("empty");

        } else
            for (size_t index = 0; index < _size; index++) {
                std::cout << _array[index];
                if (index < _size - 1)
                    std::cout << " ";
                else {
                    std::cout << std::endl;
                }
            }
    }
};

class StackParser {
private:
    std::regex R_POP =  std::regex(R"(^pop)");
    std::regex R_PUSH = std::regex(R"(^push\s[^\s\t]+)");
    std::regex R_SET =  std::regex(R"(^set_size\s\d+)");
    std::regex R_PRINT = std::regex(R"(^print)");

    myStack<std::string> *_stack{};
public:

    StackParser() = default;

    ~StackParser() {
        delete _stack;
        _stack = nullptr;
    }

    void runParse() {
        int numOfSetSize = 0; // TO check if setting again many times
        std::string inputStr;
        std::regex exitREg = std::regex(R"([\s\t]exit[\s\t])");
        while (std::getline(std::cin, inputStr)) {
            if(!inputStr.empty()) {
                try {
                    if (std::regex_match(inputStr, R_SET)) {
                        if (numOfSetSize == 0) {
                            std::smatch match;
                            if (std::regex_search(inputStr, match, std::regex(R"(\d+)"))) {
                                size_t settingSize = std::stoull(match.str());
                                _stack = new myStack<std::string>(settingSize);
                                numOfSetSize++;
                                continue;
                            }
                        } else
                            throw ParseExceptions("error"); // Setting more then one time
                    } else if (numOfSetSize == 1) {
                        if (std::regex_match(inputStr, R_POP)) {
                            std::cout << _stack->pop() << std::endl;
                            continue;

                        } else if (std::regex_match(inputStr, R_PUSH)) {
                            std::string pushingElement = inputStr.substr(5, inputStr.length() - 5);
                            _stack->push(pushingElement);
                            continue;
                        } else if (std::regex_match(inputStr, R_PRINT)) {
                            _stack->print();
                            continue;
                        } else
                            throw ParseExceptions("error"); // Setting more then one time
                    } else
                        throw ParseExceptions("error"); // Setting more then one time
                }
                catch (ParseExceptions &exc) {
                    std::cout << exc.what() << std::endl;
                }
                catch (std::exception &exc) {
                    std::cout << exc.what() << std::endl;
                }
                catch (...) {
                    std::cout << "Unexpected error!\n";
                }
            }
        }

    }
};


int main() {
    StackParser parser;
    parser.runParse();
    return 0;
}
