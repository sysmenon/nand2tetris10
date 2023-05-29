#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "JackTokenizer.h"

using namespace std;

int main(int argc, char **argv) {
    vector<string> files;
    string argUnknown = argv[1];
    if(argUnknown.find(".jack") != string::npos) {
        files.insert(files.begin(), argUnknown.substr(0, argUnknown.find(".")));
    } else {
        string path = argv[1];
        for(const auto & entry : filesystem::directory_iterator(path)) {
            string s = entry.path().string();
            string base_filename = s.substr(s.find_last_of("/\\") + 1);
            if(base_filename.find(".jack") != string::npos) {
                files.insert(files.begin(), s);
            }
            
        }
    }
    for(string s : files) {

    
        ofstream outputFile;
        string base_filename = s.substr(s.find_last_of("/\\") + 1);
        base_filename = base_filename.substr(0, base_filename.find("."));

        outputFile.open(base_filename + "T.xml");
        if(!outputFile.is_open()) {
            cout << "can't open output file" << endl;
            return 0;
        }
        JackTokenizer tokens = JackTokenizer(s);
        outputFile << "<tokens>" << endl;
        while(tokens.hasMoreTokens()) { //if more tokens exist in the stream
            tokens.advance();
            if(tokens.tokenType() == my_enums::tokenType::STRING_CONST) {
                outputFile << "<stringConstant> " << tokens.stringVal() << " </stringConstant>" << endl;
            } else if(tokens.tokenType() == my_enums::tokenType::INT_CONST) {
                outputFile << "<integerConstant> " << tokens.intVal() << " </integerConstant>" << endl;
            } else if(tokens.tokenType() == my_enums::tokenType::KEYWORD) {
                outputFile << "<keyword> " << tokens.stringKeyWord() << " </keyword>" << endl;
            } else if(tokens.tokenType() == my_enums::tokenType::IDENTIFIER) {
                outputFile << "<identifier> " << tokens.identifier() << " </identifier>" << endl;
            } else {
                if(tokens.symbol() == '<') {
                    outputFile << "<symbol> " << "&lt;" << " </symbol>" << endl;
                } else if(tokens.symbol() == '>') {
                    outputFile << "<symbol> " << "&gt;" << " </symbol>" << endl;
                } else if(tokens.symbol() == '\"') {
                    outputFile << "<symbol> " << "&quot;" << " </symbol>" << endl;
                } else if(tokens.symbol() == '&') {
                    outputFile << "<symbol> " << "&amp;" << " </symbol>" << endl;
                } else {
                    outputFile << "<symbol> " << tokens.symbol() << " </symbol>" << endl;
                }
                
            }
            

        }
        outputFile << "</tokens>" << endl;
    }
        return 0;
}