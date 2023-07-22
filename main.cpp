#include <iostream>
#include <string>
#include "util.cpp"
#include "unistd.h"

const char* PROMPT = "pawsh|";
const char* GREEN =  "\033[1;32m";
const char* END = "\033[0m";

int main(){
    std::string cmd;
    std::vector<std::string> split_cmd;
    
    getPwd();

    //event loop
    while(true){
        std::cout << GREEN << PROMPT << END;
        if(pwd)
            std::cout << "" << pwd;
        std::cout  << GREEN << "|>>"  << END;

        std::getline(std::cin, cmd);
        cmd = stripWhitespace(cmd);
        split_cmd = splitStringByWS(cmd);
        cmd = split_cmd[0];
        // diffirent from invalid command case so we explicitly continue
        if(cmd == ""){
            continue;
        }
        else if(cmd == "exit")
            return 0;
        else if(cmd == "pwd"){
            if (getPwd() ==0)
                std::cout << pwd << std::endl;
            else
                std::cerr << "Couldn't get pwd" << std::endl;
        }
        else if(cmd == "cd"){
            chdir(split_cmd[1].c_str());
            getPwd();
        }
        else{
            //check for existing commands, if no such command print invalid
            if(runCommand(cmd.c_str()) != 0)
                std::cout << "Invalid command \"" << cmd << "\"" <<std::endl;
        }
    }
    return 0;
}
