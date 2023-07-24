#include <iostream>
#include <string>
#include <stdlib.h>
#include "util.cpp"
#include "unistd.h"
#include <climits>
#include <vector>
#include <cstdlib>
#include <cstring>
const char* PROMPT = "pawsh|";
const char* GREEN =  "\033[1;32m";
const char* END = "\033[0m";

int main(){
    std::string cmd;
    std::vector<std::string> split_cmd;
    char pwd[PATH_MAX];


    getcwd(pwd, sizeof(pwd));

    //event loop
    while(true){
        std::cout << GREEN << PROMPT << END;
        if(pwd)
            std::cout << "" << pwd;
        std::cout  << GREEN << "|>>"  << END;

        std::getline(std::cin, cmd);
        cmd = stripWhitespace(cmd);
        std::vector<std::string> args = splitByDelim(cmd," ");
        std::vector<char*> resolved_args= resolveArgs(args);
        cmd = std::string(resolved_args[0]);

        // diffirent from invalid command case so we explicitly continue
        if(cmd == ""){
            continue;
        }
        else if(cmd == "exit")
            return 0;
        else if(cmd == "pwd"){
            if (getcwd(pwd, sizeof(pwd)) != nullptr)
                std::cout << pwd << std::endl;
            else
                std::cerr << "Couldn't get pwd" << std::endl;
        }
        else if(cmd == "cd"){
            if(chdir(resolved_args[1]) != 0)
                std::cerr << "chdir failed" << std::endl;
            getcwd(pwd, sizeof(pwd));
        }
        else if(cmd == "export"){
            long unsigned int eq_index;
            std::string arg, key, val;
            arg = std::string(resolved_args[1]);
            eq_index = arg.find_first_of("=");
            if(eq_index == std::string::npos){
                std::cerr << "Where is assignment operator cheems?" << std::endl;
                continue;
            }
            key = arg.substr(0,eq_index);
            val =arg.substr(eq_index+1);
            if(key.length() <= 0 || val.length() <=0){
                std::cerr << "Environment variable key or value is missing" << std::endl;
                continue;
            }
            setenv(key.c_str(),val.c_str(),true);
        }
        else if(cmd == "unset"){
            unsetenv(resolved_args[1]);
        }
        else{
            executeCommand(resolved_args);
         }

        for (char* ptr : resolved_args) {
            delete[] ptr;
        }
        resolved_args.clear();
        
    }
    return 0;
}
