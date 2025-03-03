#include <iostream>
#include "crow_all.h"
#include <vector>
#include "json.hpp"
#include <cmath>
#include <list>
#include <curl/curl.h>



std::vector<int> matrix1_demo;
std::vector<int> matrix2_demo;
int got_data = 0;   // some flag

template<typename T>
std::list<T> vectorToList(const std::vector<T>& vec) {
    return std::list<T>(vec.begin(), vec.end());
}

char* listToCharSequence(const std::list<int>& lst, char delimiter) {
    // Use a stringstream to build the string
    std::ostringstream oss;

    for (auto it = lst.begin(); it != lst.end(); ++it) {
        oss << *it;
        if (std::next(it) != lst.end()) { // Add delimiter except for the last element
            oss << delimiter;
        }
    }

    // Convert the stringstream to a string
    std::string result = oss.str();

    // Allocate memory for char* and copy the string content
    char* charSequence = new char[result.size() + 1]; // +1 for the null terminator
    std::strcpy(charSequence, result.c_str());

    return charSequence; // Return the dynamically allocated char*
}

std::string listToString(const std::list<int>& intList) {
    std::ostringstream oss;
    
    for (auto it = intList.begin(); it != intList.end(); ++it) {
        oss << *it; // Add the integer to the stream
        if (std::next(it) != intList.end()) {
            oss << ", "; // Add a separator if it's not the last element
        }
    }
    
    return oss.str();
}

// std::vector<int> matrixMultiply(const std::vector<int>& matrix1, const std::vector<int>& matrix2, int size) {
//     std::list<int> result(size * size, 0);



//     for (int i = 0; i < size; ++i) {
//         for (int j = 0; j < size; ++j) {
//             for (int k = 0; k < size; ++k) {
//                 result[i * size + j] += matrix1[i * size + k] * matrix2[k * size + j];
//             }
//         }
//     }
    
//     return result;
// }

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
*   Pushes nums into vector buf while num != -1
*/
// int handleProducerMessage(int num, std::vector<int> buf){
//     if (num == -1){
//         return 1;
//     }
//     else{
//         buf.push_back(num);
//         return 0;
//     }
// }

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
            // Read the HTTP request
            std::string requestContent = req.body;
            
            // Check the type of producer
            auto tmp = getData(requestContent);
            int prod_type = tmp.first, prod_data = tmp.second;

            //std::cout << prod_type << prod_data;

            // Pushes got nums into vector matrix1(2) while num != -1
            if (prod_type == 1) {
                if (prod_data == -1) {
                    got_data += 1;
                }
                else {
                    matrix1_demo.push_back(prod_data);
                }
            } 
            else if (prod_type == 2) {
                if (prod_data == -1) {
                    got_data += 1;
                }
                else {
                    matrix2_demo.push_back(prod_data);
                }
            } 
            else {
                // Unknown producer type
                // ...
                ;
            }

            // Construct an HTTP response
            std::string responseContent = std::to_string(prod_data); // Placeholder for the response content
            res.code = 200;
            res.set_header("Content-Type", "text/plain");
            res.body = responseContent;

            res.end();
        }
    );

    CROW_ROUTE(app, "/end").methods("POST"_method)(
        [&](const crow::request& req, crow::response& res) {
            
            // Read the HTTP request
            int p_num = stoi(req.get_header_value("p_num"));
            int pid = stoi(req.get_header_value("pid"));
            // std::cout << "p_num = " << p_num << "; pid = " << pid << std::endl;

            got_data += 1;

            if (got_data == 2) {
                std::cout << "Result: " << std::endl;
                
                std::list<int> result;

                auto timer1 = std::chrono::high_resolution_clock::now();

                if (isPerfectSquare(matrix1_demo.size()) && isPerfectSquare(matrix2_demo.size())) {
                    result = pieceMatrixMultiply(vectorToList(matrix1_demo), vectorToList(matrix2_demo), sqrt(matrix1_demo.size()), p_num, pid);
                    std::cout << "consumer " << pid << " of " << p_num - 1 <<
                        " has calculated matrix of size " << result.size() << std::endl;
                    // std::cout << "consumer " << pid << " of " << p_num - 1 << " has calculated matrix of size "
                    //     << result.size() / sqrt(matrix1_demo.size()) << " * " << sqrt(matrix1_demo.size()) << std::endl;
                    // for (int i = 0; i < result.size(); i++) {
                    //     std::cout << result[i] << " ";
                    // }
                    // std::cout << std::endl;
                }

                auto timer2 = std::chrono::high_resolution_clock::now();   

                std::chrono::duration<double, std::milli> duration = timer2 - timer1;

                std::cout << "Consumer time: " << duration.count() << " ms" << std::endl;

/////////// VARIANT 4
                CURL *handle;
                CURLcode res;
                handle = curl_easy_init();

                // Set the POST data
                char* charData = listToCharSequence(result, ',');
                curl_easy_setopt(handle, CURLOPT_POSTFIELDS, charData);

                // Create a list of headers
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, "My-Custom-Field: yes");
                headers = curl_slist_append(headers, "Content-Type: text/plain");
                headers = curl_slist_append(headers, ("pid: "   + std::to_string(pid  )).c_str());
                headers = curl_slist_append(headers, ("p-num: " + std::to_string(p_num)).c_str());
                headers = curl_slist_append(headers, ("time-spent: " + std::to_string(duration.count())).c_str());

                // Set the headers for the request
                curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
                
                curl_easy_setopt(handle, CURLOPT_URL, "http://adder:8080/");            
                
                curl_easy_perform(handle); /* post away! */

                curl_easy_cleanup(handle);

                exit(0);
            }
        }
    );

    // while (true && got_data != 2) {

    //     if ((matrix1_demo.size() + matrix2_demo.size()) % 1000 == 0) {
    //         std::cout << (matrix1_demo.size() + matrix2_demo.size()) << " messages received" << std::endl;
    //     }

    //     tcp::socket socket(io);
    //     acceptor.accept(socket);

    //     // Read the HTTP request
    //     boost::asio::streambuf request;
    //     boost::asio::read_until(socket, request, "\r\n\r\n");
    //     std::string requestContent = boost::asio::buffer_cast<const char*>(request.data());
    //     //std::cout << "Received HTTP request:\n" << requestContent << std::endl << std::endl;

    //     // Check the type of producer
    //     auto tmp = getData(requestContent);
    //     int type = tmp.first, data = tmp.second;

    //     //std::cout << "Type: " << type << " Data: " << data << std::endl;

    //     if (type == 1) {
    //         if (data == -1){
    //            got_data += 1;
    //         }
    //         else{
    //             matrix1_demo.push_back(data);
    //         }
    //     } else if (type == 2) {
    //         if (data == -1){
    //            got_data += 1;
    //         }
    //         else{
    //             matrix2_demo.push_back(data);
    //         }
    //     } else {
    //         // Unknown producer type
    //         //std::cout << "Unknown reuest type: " << type << std::endl << std::endl;
    //         ;
    //     }

    //     // Construct an HTTP response
    //     std::string responseContent = std::to_string(data); // Placeholder for the response content
    //     std::string response = "HTTP/1.1 200 OK\r\n";
    //     response += "Content-Length: " + std::to_string(responseContent.length()) + "\r\n";
    //     response += "Content-Type: text/plain\r\n";
    //     response += "\r\n";
    //     response += responseContent;

    //     // Send the response back to the sender
    //     boost::asio::write(socket, boost::asio::buffer(response));
    // }

    // std::cout << "Matrix 1: " << matrix1_demo.size() << std::endl;
    // for (int i = 0; i < matrix1_demo.size(); i++) {
    //     //std::cout << matrix1_demo[i];
    // }

    // std::cout << std::endl;

    // std::cout << "Matrix 2: " << matrix2_demo.size() << std::endl;
    // for (int i = 0; i < matrix2_demo.size(); i++) {
    //     //std::cout << matrix2_demo[i];
    // }

    //std::cout << std::endl;
    //std::cout << std::endl;

    std::cout << "Starting server..." << std::endl;

    app.port(8080).multithreaded().run();

}
