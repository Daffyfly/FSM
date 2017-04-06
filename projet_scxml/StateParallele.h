//
// Created by vincent on 05/04/17.
//

#ifndef PARSER_SCXML_STATEPARALLELE_H
#define PARSER_SCXML_STATEPARALLELE_H


#include "State.h"

class StateParallele : public State{
private:
    std::vector<State*> sousEtats;
public:
    StateParallele(std::string name):State(name){}
    void addSousEtat(State*);
    std::vector<State*> getSousEtats();
};


#endif //PARSER_SCXML_STATEPARALLELE_H
