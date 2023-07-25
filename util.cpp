#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>   // For fork(), execvp()
#include <sys/wait.h> // For waitpid()

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

std::vector<std::string> splitByDelim(const std::string s, const char* delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, (char)*delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> resolveArgs(const std::vector<std::string>& args) {
    std::vector<std::string> resolved_args;

    for (const auto& arg : args) {
        if (arg[0] == '$') {
            char* env_var = getenv(arg.substr(1).c_str());
            if (env_var) {
                resolved_args.push_back(std::string(env_var));
            } else {
                resolved_args.push_back(std::string(""));
            }
        } else {
            resolved_args.push_back(arg);
        }
    }
    return resolved_args;
}

void executeCommand(const std::vector<std::string>& args){
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Fork failed!" << std::endl;
        return;
    } else if (pid == 0) {
        //execvp requires c-string
        std::vector<char*> c_args;
        for (const auto& arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        // c_args.push_back(nullptr);

        // Child process
        execvp(c_args[0], c_args.data());
        std::cerr << "Command not found: " << args[0] << std::endl;
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}
