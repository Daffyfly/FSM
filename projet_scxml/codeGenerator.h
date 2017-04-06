//
// Created by vincent on 01/03/17.
//

#ifndef PARSER_SCXML_CODEGENERATOR_H
#define PARSER_SCXML_CODEGENERATOR_H


#include <fstream>
#include "State.h"
#include "AST.h"

class codeGenerator {
private:
    std::string path;
    std::string dotcpp;
    std::string doth;
    std::string name_of_machine_state;
    std::ofstream myfile;
    AST ast;

    void eventType(std::string event);
    void generateCodeOfState(State&,int);

public:
    codeGenerator(std::string path, std::string name_of_file, AST & ast);
    void generateFile();
    void app(std::string filename, int tab, std::string s);
};


#endif //PARSER_SCXML_CODEGENERATOR_H
