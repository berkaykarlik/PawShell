#include <iostream>
#include <string>
#include <stdlib.h>
#include "util.cpp"
#include "unistd.h"
#include <climits>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
        std::vector<char*> args;
        cmd = stripWhitespace(cmd);
        char* token = strtok(const_cast<char*>(cmd.c_str()), " ");
        while (token != nullptr) {
            args.push_back(token);
            token = strtok(nullptr, " ");
        }
        args.push_back(nullptr);
        cmd = std::string(args[0]);

        std::vector<char*> resolved_args;

        for(size_t i = 0 ; i < args.size(); i++){
            if (args[i] == nullptr) {
                continue;
            }
            if(args[i][0] == '$'){
                char * env_var = getenv(args[i] +1);
                if(env_var){
                    resolved_args.push_back(env_var);
                }
                else{
                    resolved_args.push_back(args[i]);
                }
            }
            else{
                resolved_args.push_back(args[i]);
            }
        }
        
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
            pid_t pid = fork();
            if (pid == -1) {
                std::cerr << "Fork failed!" << std::endl;
                continue;
            } else if (pid == 0) {
                // Child process
                execvp(resolved_args[0], resolved_args.data());
                std::cerr << "Command not found: " << resolved_args[0] << std::endl;
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
         }
    }
    return 0;
}
