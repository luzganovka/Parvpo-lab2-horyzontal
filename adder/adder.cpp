#include <iostream>
#include "crow_all.h"
#include <vector>
#include "json.hpp"
#include <cmath>
#include <list>



std::vector<int> matrix1_demo;
std::vector<int> matrix2_demo;
int got_data = 0;   // some flag

template<typename T>
std::list<T> vectorToList(const std::vector<T>& vec) {
    return std::list<T>(vec.begin(), vec.end());
}

/*
*   Simply multiplies two matrixes, given as two one-dimension lists.
*   it, it1, it2 -- are iterators for result, first and second matrixes respectively.
*/
std::list<int> matrixMultiply(const std::list<int>& matrix1, const std::list<int>& matrix2, int size) {
    std::list<int> result(size * size, 0);

    auto it = result.begin();

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto it1 = matrix1.begin();
            std::advance(it1, i * size);
            
            auto it2 = matrix2.begin();
            std::advance(it2, j);
            
            for (int k = 0; k < size; ++k) {
                *it += *it1 * *it2;
                std::advance(it1, 1);
                std::advance(it2, size);
            }
            
            ++it;
        }
    }
    
    return result;
}

/*
*   Calculates a certain piece of the result matrix.
*   Gets two matrixes, given as two one-dimension lists.
*   it, it1, it2 -- are iterators for result, first and second matrixes respectively.
*/
std::list<int> pieceMatrixMultiply(const std::list<int>& matrix1, const std::list<int>& matrix2, const int size, const int p, const int pid) {
    std::list<int> result(size * size / p, 0);

    auto it = result.begin();

    for (int i = pid * (size / p); i < (pid + 1) * (size / p); ++i) {    // for several lines in matrix1
        for (int j = 0; j < size; ++j) {    // for all columns in matrix2
            auto it1 = matrix1.begin();
            std::advance(it1, i * size);
            
            auto it2 = matrix2.begin();
            std::advance(it2, j);
            
            for (int k = 0; k < size; ++k) {
                *it += *it1 * *it2;
                std::advance(it1, 1);
                std::advance(it2, size);
            }
            
            ++it;
        }
    }
    
    return result;
}

/*
*   Returns true if given num is a square of some numeric.
*/
bool isPerfectSquare(int num) {
    int squareRoot = std::sqrt(num);
    return squareRoot * squareRoot == num;
}

/*
*   Simply converts a vector to an array. No surprise.
*/
int* vectorToArray(const std::vector<int>& vec) {
    int* arr = new int[vec.size()]; // Create a new array with the same size as the vector
    
    // Copy the elements from the vector to the array
    for (size_t i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    
    return arr; // Return the pointer to the array
}

/*
*   Parses pseudo-json string into <int, int> <mes.type, mes.content>
*/
std::pair<int, int> getData(const std::string& requestContent) {
    std::string buffer;

    //std::size_t closingCurlyBracePos = requestContent.find("{");

    //std::cout << "closingCurlyBracePos: " << closingCurlyBracePos  << std::endl;

    if (1) {

        for (int i = 0; i < requestContent.length(); i++) {
            buffer += requestContent[i];
        }

        //buffer = requestContent.substr(0, closingCurlyBracePos + 1);
        //std::cout << buffer << " " << buffer.length() <<  std::endl;
    }
    else {
        return std::pair<int, int>(-1, -520);
    }

    nlohmann::json json = nlohmann::json::parse(buffer);

    int messageType = json["message_type"];
    int messageContent = json["message_content"];

    //std::cout << messageType << " " << messageContent << std::endl;

    return std::pair<int, int>(messageType, messageContent);
}

int main() {
    crow::SimpleApp app;
    crow::logger::setLogLevel(crow::LogLevel::Warning);

    CROW_ROUTE(app, "/")
    .methods("POST"_method)(
        [&](const crow::request& req, crow::response& res) {

            std::cout << ">>> ADDER GOT SOME POST!" << std::endl;

            // // Read the HTTP request
            std::string requestContent = req.body;
            std::cout << requestContent << std::endl;
            
            // // Check the type of producer
            // auto tmp = getData(requestContent);
            // int prod_type = tmp.first, prod_data = tmp.second;

        }
    );

    CROW_ROUTE(app, "/end").methods("POST"_method)(
        [&](const crow::request& req, crow::response& res) {
            
            // Read the HTTP request
            int p_num = stoi(req.get_header_value("p_num"));
            int pid = stoi(req.get_header_value("pid"));
            
        }
    );

    std::cout << "Starting server..." << std::endl;

    app.port(8080).multithreaded().run();

}
