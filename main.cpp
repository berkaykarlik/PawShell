#include <iostream>
#include <string>
#include "util.cpp"

const char* PROMPT = "pawsh>>";


int main(){
    std::string cmd;

    //event loop
    while(true){
        std::cout << PROMPT;
        std::getline(std::cin, cmd);
        cmd = stripWhitespace(cmd);
        // diffirent from invalid command case so we explicitly continue
        if(cmd == ""){ 
            continue;
        }
        else if(cmd == "exit"){
            return 0;
        }
        else{
            //check for existing commands, if no such command print invalid
            if(runCommand(cmd.c_str()) != 0)
                std::cout << "Invalid command \"" << cmd << "\"" <<std::endl;
        }
    }



    return 0;
}
