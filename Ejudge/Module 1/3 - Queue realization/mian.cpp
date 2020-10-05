#include <iostream>
#include <string>
#include <regex>
#include <exception>
#include <fstream>

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
class myQueue {
private:
    TElement *_array = nullptr;
    int64_t _size;
    int64_t _maxSize;
    int64_t _rareInd;
    int64_t _frontInd;
    myQueue(const myQueue &copy) = delete;

    myQueue(const myQueue &&move) noexcept = delete;

    myQueue &operator=(const myQueue &copy) = delete;

    myQueue &operator=(const myQueue &&copy) = delete;

public:

    explicit myQueue(size_t size) {
        _size = 0;
        _maxSize = size;
        _array = new TElement[size];
        _rareInd = -1;
        _frontInd = -1;
    }

    ~myQueue() { // TODO ADd
        delete[] _array;
        _array = nullptr;
    }

    void push(const TElement newElem) {
        if (_size < _maxSize) {
            if (_rareInd == -1)
                _rareInd = 0;
            _frontInd = (_frontInd + 1) % _maxSize;
            _array[_frontInd] = newElem;
            _size ++;
        } else {
            throw ParseExceptions("overflow");
        }
    }

    TElement pop() {
        if (_size > 0) {
            TElement beforeDelete = _array[_rareInd];
            if (_size == 1) { // TODO Is it equvalent to the if(front == rare)?
                _rareInd = -1;
                _frontInd = -1;
            } else
                _rareInd++;
            _size--;
            return beforeDelete;
        } else
            throw ParseExceptions("underflow");
    }

    void print(std::ostream &fout) {
        if (_size == 0) {
            throw ParseExceptions("empty");
        } else {
            int64_t index;
            for (index = _rareInd; index != _frontInd; index = (index+1) % _maxSize) { // TODO >????????????
                fout << _array[index] << " ";
            }
            fout << _array[index] << "\n";
        }
    }
};

class QueueFileParser {
private:
    std::regex R_POP = std::regex(R"(^pop)");
    std::regex R_PUSH = std::regex(R"(^push\s[^\s\t]+)");
    std::regex R_SET = std::regex(R"(^set_size\s\d+)");
    std::regex R_PRINT = std::regex(R"(^print)");

    myQueue<std::string> *_queue{};
    std::ifstream _fin;
    std::ofstream _fout;
public:

    QueueFileParser(const std::string &inPath, const std::string &outPath) {
        _fin.open(inPath);
        _fout.open(outPath, std::ios::trunc);
    }

    ~QueueFileParser() {
        _queue = nullptr;
    }

    void runParse() {
        try {
            if (_fin.is_open() && _fout.is_open()) {
                int numOfSetSize = 0; // TO check if setting again many times
                while (_fin.peek() != EOF) {
                    try {

                        std::string inputStr;
                        char smb;
                        while (_fin.get(smb) && smb != '\n') {
                            inputStr += smb;
                        }
                        if (!inputStr.empty()) {
                            if (std::regex_match(inputStr, R_SET)) {
                                if (numOfSetSize == 0) {
                                    std::smatch match;
                                    if (std::regex_search(inputStr, match, std::regex(R"(\d+)"))) {
                                        size_t settingSize = std::stoull(match.str());
                                        _queue = new myQueue<std::string>(settingSize);
                                        numOfSetSize++;
                                        continue;
                                    }
                                } else
                                    throw ParseExceptions("error");
                            } else if (numOfSetSize == 1) {
                                if (std::regex_match(inputStr, R_POP)) {
                                    auto popedElement = _queue->pop();
                                    _fout << popedElement << std::endl;
                                    continue;

                                } else if (std::regex_match(inputStr, R_PUSH)) {
                                    std::string pushingElement = inputStr.substr(5, inputStr.length() - 5);
                                    _queue->push(pushingElement);
                                    continue;
                                } else if (std::regex_match(inputStr, R_PRINT)) {
                                    _queue->print(_fout);
                                    continue;
                                } else
                                    throw ParseExceptions("error");
                            } else
                                throw ParseExceptions("error");
                        }
                    }
                    catch (const ParseExceptions &exc) {
                        _fout << exc.what() << std::endl;
                    }
                }
            } else
                std::cerr << "file(s) not open\n";
        }
        catch (const std::exception &exc) {
            std::cout << "ERROR: " << exc.what() << std::endl;
        }
        catch (...) {
            std::cout << "ERROR: Unexpected error!\n";
        }
        _fin.close();
        _fout.close();
    }
};


int main(int argc, char *argv[]) {
    if (argc == 3) {
        std::string inputPath = argv[1];
        std::string outputPath = argv[2];

        QueueFileParser parser(inputPath, outputPath);
        parser.runParse();
        return 0;
    } else
        std::cout << "Incorrect number of arguments!\n";
}
