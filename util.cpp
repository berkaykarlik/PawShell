#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <array>
#include <unistd.h>
#include <climits>

char pwd[PATH_MAX];

std::string stripWhitespace(const std::string& input) {
    // Find the first non-whitespace character
    size_t firstNonWhitespace = input.find_first_not_of(" \t\n\r");

    // If the input string contains only whitespace characters, return an empty string
    if (firstNonWhitespace == std::string::npos)
        return "";

    // Find the last non-whitespace character
    size_t lastNonWhitespace = input.find_last_not_of(" \t\n\r");

    // Extract the substring containing the non-whitespace characters
    return input.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1);
}

std::vector<std::string> splitStringByWS(const std::string& input){
    std::vector<std::string> words;
    std::istringstream iss(input);
    std::string word;

    while(iss >> word){
        words.push_back(word);
    }
    return words;
}

int runCommand(const char* cmd) {
    constexpr int bufferSize = 128;
    std::array<char, bufferSize> buffer;

    FILE* pipe = popen(cmd, "r");
     if (!pipe) {
        std::cerr << "Error opening pipe." << std::endl;
        return 1;
    }

    while(!feof(pipe)){
        if(fgets(buffer.data(), bufferSize, pipe) != nullptr){
            std::cout << buffer.data() ;
        }
    }
    return pclose(pipe);
}

int getPwd(){
    if (getcwd(pwd, sizeof(pwd)) != nullptr)
        return 0;
    return 1;
}

