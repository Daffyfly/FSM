//
// Created by vincent on 01/03/17.
//

#ifndef PARSER_SCXML_XMLPARSER_H
#define PARSER_SCXML_XMLPARSER_H

#include <iostream>
#include "State.h"
#include "AST.h"

class Xmlparser {
private:
    std::string file_location;
public:
    Xmlparser(std::string document):file_location(document){}
    AST getAST();
};


#endif //PARSER_SCXML_XMLPARSER_H
