//
// Created by vincent on 05/04/17.
//

#ifndef PARSER_SCXML_TRANSITIONPARALLELE_H
#define PARSER_SCXML_TRANSITIONPARALLELE_H


#include <vector>
#include <string>
#include "State.h"

class TransitionParallele {
private:
    std::vector<State*> targets;
    std::vector<State*> froms;
    std::string name;
    std::vector<std::string> eventsSent;
public:
    TransitionParallele(std::string);
    void addTarget(State*);
    void addFrom(State*);
    void addEventSent(std::string);
    std::vector<State*> getTargets();
    std::vector<State*> getFroms();
    std::vector<std::string> getEventsSent();
    std::string getName();
};


#endif //PARSER_SCXML_TRANSITIONPARALLELE_H
