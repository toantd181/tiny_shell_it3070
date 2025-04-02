#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include "Features/features.h"

using namespace std;

DirectoryManager directoryManager;
FileManager fileManager;
ScreenManager screenManager;
AliasManager1 aliasManager;

void excuteCommand(const string &command, const vector<string> &args){
    if (command == "copy"){
        directoryManager.copyDirectory(args);   
    }

    else if (command == "create_dir"){
        directoryManager.createDirectory(args);
    }

    else if (command == "delete_dir"){
        directoryManager.deleteDirectory(args);
    }

    else if (command == "list_dir_tree"){
        directoryManager.listDirectoryTree(args);
    }

    else if (command == "move_dir"){
        directoryManager.moveDirectory(args);
    }
}

vector<string> splitInput(const string &input){
    vector<string> tokens;
    string token;
    bool inQuotes = false;

    for (char ch : input)
    {
        if (ch == '\"')
        {
            inQuotes = !inQuotes;
            if (!inQuotes) // End of quoted string
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (isspace(ch) && !inQuotes)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if ((ch == '(' || ch == ')' || ch == ',' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>') && !inQuotes)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, ch));
        }
        else
        {
            token += ch;
        }
    }

    if (!token.empty())
    {
        tokens.push_back(token);
    }

    if (!tokens.empty())
    {
        tokens[0] = aliasManager.resolveAlias(tokens[0]);
    }

    return tokens;
}

int main(){
    string input;
    while(true){
        cout << "tiny_shell> ";
        getline(cin, input);
        if (cin.fail() || cin.eof()){
            cin.clear();
            cout << endl;
            continue;
        }

        vector<string> tokens = splitInput(input);
        if (tokens.empty()){
            continue;
        }

        string command = tokens[0];
        tokens.erase(tokens.begin());
        excuteCommand(command, tokens);
    }
}

