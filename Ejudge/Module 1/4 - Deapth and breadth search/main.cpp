#include <iostream>
#include <set>
#include <exception>
#include <regex>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>

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

class Graph {

public:

    Graph() = default;

    Graph(std::string gType) {
        _graphData = new std::unordered_map<std::string, std::set<std::string>>;
        if (gType == "d")
            _directedGraph = true;
        else if (gType == "u")
            _directedGraph = false;
        else
            throw ParseExceptions("ERROR: Incorrect type of graph");

    }

    void graphCreation(const std::string &firstVertexId, const std::string secondVertexId) {
        auto finded = _graphData->find(firstVertexId);

        if (finded == _graphData->end()) {
            _graphData->insert(std::make_pair(firstVertexId, std::set({secondVertexId})));
        } else {
            auto *neighbours = &_graphData->at(firstVertexId);
            if (std::find(neighbours->begin(), neighbours->end(), secondVertexId) == neighbours->end())
                neighbours->insert(secondVertexId);
        }
        if (!_directedGraph) {
            auto neighbour = _graphData->find(secondVertexId);
            if (neighbour == _graphData->end()) {
                _graphData->insert(std::make_pair(secondVertexId, std::set({firstVertexId})));
            } else {
                std::set<std::string> *neighbours = &_graphData->at(secondVertexId);
                if (/*std::find(neighbours->begin(), neighbours->end(), firstVertexId) nei ==
                    neighbours->end()*/ neighbours->find(firstVertexId) == neighbours->end()) // TODO: How much time?
                    neighbours->insert(firstVertexId);
            }
        }
//        dispEdges();
//        std::cout << "\n*********************************";
    }


    void startSearch(std::string firstVertex, std::string searchType) {
        if (!_graphData->empty()) {
            if (searchType == "d")
                DFS(firstVertex);
            else {
                if (searchType == "b")
                    BFS(firstVertex);
                else
                    throw ParseExceptions("ERROR: Unknown type of search");
            }
        }
    }

    ~Graph() {
        _graphData->clear();
        delete _graphData;
        _graphData = nullptr;
    }

private:
//    enum class TYPE {DIRECTED, UNDIRECTED};
    Graph(Graph &copied) = delete;

    Graph(Graph &&moved) = delete;

    Graph operator=(Graph &copied) = delete;

    Graph operator=(Graph &&moved) = delete;

    std::set<std::string> _usedVertices;

    std::unordered_map<std::string, std::set<std::string>> *_graphData = nullptr; // {vertex, {{vertex,visited}...}}
    bool _directedGraph = false;
    //    TYPE _graphType = TYPE::UNDIRECTED;

    void dispEdges() {
        std::cout << "GRAPH " << (_directedGraph ? "DIRECTED\n" : "UNDIRECTED\n");
        for (auto[vtx, set] : *_graphData) {
            std::cout << "VTX: <" << vtx << "> ~: ";
            for (auto neighb : set)
                std::cout << neighb << "  ";
            std::cout << "\n";
        }
    }

    void BFS(std::string startVertex) {
//        dispEdges(); //TODO REMOVE

        std::queue<std::string> queue;
        std::unordered_set<std::string> visited;
//        std::unordered_set<std::string> queued;
        queue.push(startVertex);

        while (!queue.empty()) {
            std::string curVertex = queue.front();
            queue.pop();
            if (visited.find(curVertex) != visited.end())
                continue;
            std::cout << curVertex << std::endl;
            visited.insert(curVertex);

            std::set<std::string> *neighbours = nullptr;
            auto thing = _graphData->find(curVertex);
            if (thing != _graphData->end()) {
                neighbours = &thing->second;
            } else
                continue;
            std::set<std::string>::const_iterator It;

            for (It = neighbours->begin(); It != neighbours->end(); It++) {
                if (/*queued.find(*revIt) == queued.end() && */visited.find(*It) ==
                                                               visited.end()) {        // TODO Check
                    queue.push(*It);
//                    visited.insert(*It);
//                    queued.insert(*revIt);
                }
            }
        }
    }

    void DFS(std::string startVertex) {
//        dispEdges(); //TODO REMOVE

        std::stack<std::string> stack;
        std::unordered_set<std::string> visited;
//        std::unordered_set<std::string> queued;
        stack.push(startVertex);
        while (!stack.empty()) {
            std::string curVertex = stack.top();
            stack.pop();
            if (visited.find(curVertex) != visited.end())
                continue;
            std::cout << curVertex << std::endl;
            visited.insert(curVertex);
            std::set<std::string> *neighbours = nullptr;
            auto thing = _graphData->find(curVertex);
            if (thing != _graphData->end()) {
                neighbours = &thing->second;
            } else
                continue;

            std::set<std::string>::reverse_iterator revIt;
            if (!neighbours->empty()) {
                for (revIt = neighbours->rbegin(); revIt != neighbours->rend(); revIt++) {
                    std::string neighb = *revIt;
                    if (/*queued.find(neighb) == queued.end() &&*/
                            visited.find(neighb) == visited.end()) {
                        stack.push(neighb);
//                        queued.insert(neighb);
                    }
                }
            }
        }
    }
};

class GraphParser {
private:
    std::regex R_StartInfo = std::regex(R"(^[ud](\s)\S+(\s)[db])");
    std::regex R_Edge = std::regex(R"(^(\S+)\s(\S+))");

    Graph *_graph = nullptr;
public:
    GraphParser() = default;

    ~GraphParser() {
        delete _graph;
        _graph = nullptr;
    }

    int parseString(const std::string &str, std::vector<std::string> &result) {
        if ((str[0] == ' ' || str[0] == '\t') && str.size() < 5)
            return -1; // parse error

        enum class ParseState {
            WHAT, MB_START, MB_EDGE
        } state = ParseState::WHAT;

        std::string gType, startVertex, sType;
        std::string fVertex, sVertrx;
        int numOfSpaces = 0;
        bool end = false;
        while (!end) {
            switch (state) {
                case ParseState::WHAT:
                    if ((str.substr(0, 2) == "d " || str.substr(0, 2) == "u ") &&
                        (str.substr(str.length() - 2, 2) == " b" || str.substr(str.length() - 2, 2) == " d")) {
                        state = ParseState::MB_START;
                        gType = str.at(0);
                        sType = str.at(str.length() - 1);
                    } else
                        state = ParseState::MB_EDGE;
                    break;
                case ParseState::MB_START:
                    numOfSpaces = std::count(str.begin(), str.end(), ' ');
                    if (numOfSpaces == 2) { // Then it's correct START
                        result.push_back(gType);
                        startVertex = str.substr(2, str.length() - 4);
                        result.push_back(startVertex);
                        result.push_back(sType);
                    }  // else
                    // TODO
                    end = true;
                    break;
                case ParseState::MB_EDGE:
                    numOfSpaces = std::count(str.begin(), str.end(), ' ');
                    if (numOfSpaces == 1) {
                        bool isFirst = true;
                        for (auto symb : str) {
                            if (symb == ' ') {
                                isFirst = false;
                                continue;
                            }
                            if (isFirst)
                                fVertex += symb;
                            if (!isFirst)
                                sVertrx += symb;
                        }
                        result.push_back(fVertex);
                        result.push_back(sVertrx);
                    } else
                        return -1;//
                    end = true;
                    break;
                default:
                    return -2; // undefined
            }
        }
        return 0;
    }

    void runParse() {
        bool setConfig = false;
        std::string inputStr;
        std::string startVertexId, searchType;
//        std::chrono::time_point<std::chrono::high_resolution_clock> beg, endd/*, parseBeg, parseEnd*/;
//        beg = std::chrono::system_clock::now();
        while (std::getline(std::cin, inputStr)) {
            if (!inputStr.empty()) {
                std::vector<std::string> results;
                try {
                    int codeError = parseString(inputStr, results);
                    if (codeError == 0) {
                        if (results.size() == 2 && setConfig) {
                            _graph->graphCreation(results.at(0), results.at(1));
                        } // EDGE
                        if (results.size() == 3 && !setConfig) {
                            startVertexId = results.at(1);
                            searchType = results.at(2);
                            auto tyype = std::string(results.at(0));
                            _graph = new Graph(tyype);
                            setConfig = true;
                        } //START
                    } else {
                        if (codeError == -1)
                            throw ParseExceptions("error: -1");
                        if (codeError == -2)
                            throw ParseExceptions("error: -2");
                    }

//                    parseBeg = std::chrono::system_clock::now();
                    /*if (std::regex_match(inputStr, R_StartInfo)) {
                        if (!setConfig) {
                            std::smatch match;

                            std::regex_search(inputStr, match, std::regex(R"(^[du]\s)"));
                            graphType = std::string(match.str());
                            std::regex_search(inputStr, match, std::regex(R"(\s\S+\s)"));
                            startVertexId = match.str();
                            startVertexId = startVertexId.substr(1, startVertexId.length() - 2);
                            std::regex_search(inputStr, match, std::regex(R"(\s[db])"));
                            searchType = match.str();
//                            parseEnd = std::chrono::system_clock::now();
//                            std::cout <<"\nPARSE TIME: " << std::chrono::duration_cast<std::chrono::nanoseconds>(parseEnd-parseBeg).count() << std::endl;
                            _graph = new Graph(std::string(graphType));
                            setConfig = true;
                        } else
                            throw ParseExceptions("error"); // Setting more then one time
                    } else if (setConfig) {
                        if (std::regex_match(inputStr, R_Edge)) {
                            std::smatch matchEdges;
                            std::string vertex1, vertex2;
                            auto reg = std::regex(R"(\S+)");
                            std::string::const_iterator ib = inputStr.begin(), ie = inputStr.end();

                            for (int ind = -1; std::regex_search(ib, ie, matchEdges, reg); ib = matchEdges[0].second) {
                                ind++;
                                if (ind == 0)
                                    vertex1 = matchEdges[0];
                                if (ind == 1)
                                    vertex2 = matchEdges[0];
                                if (ind > 1)
                                    throw ParseExceptions("ERROR: More then two words");
                            }
//                            parseEnd = std::chrono::system_clock::now();
//                            std::cout <<"\nPARSE TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(parseEnd-parseBeg).count() << std::endl;
                            _graph->graphCreation(vertex1, vertex2);
                        } else
                            throw ParseExceptions("error"); // Unknown command
                    } else
                        throw ParseExceptions("error"); // Setting more then one time*/
                }
                catch (ParseExceptions &exc) {
                    std::cout << exc.what() << std::endl;
                }
                catch (std::exception &exc) {
                    std::cout << "ERROR: " << exc.what() << std::endl;
                }
                catch (...) {
                    std::cout << "ERROR: " << "Unexpected error!\n";
                }
            }
        }
//        endd = std::chrono::system_clock::now();
//        std::cout << "\nCREATE TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(endd - beg).count() << std::endl;

        /*
         * Next we start search
         * */
        try {
            _graph->startSearch(startVertexId, searchType);
        }
        catch (ParseExceptions &exc) {
            std::cout << exc.what() << std::endl;
        }
        catch (std::exception &exc) {
            std::cout << "ERROR in search: " << exc.what() << std::endl;
        }
        catch (...) {
            std::cout << "ERROR in search: " << "Unexpected error in search!\n";
        }
    }
};


int main() {
//    std::chrono::time_point<std::chrono::high_resolution_clock> begin, end;
//    begin = std::chrono::system_clock::now();

    GraphParser parser;
    parser.runParse();

//    end = std::chrono::system_clock::now();
//    std::cout << "\nWORK TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << std::endl;
    return 0;
}
