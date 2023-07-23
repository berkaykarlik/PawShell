#include <string>


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


