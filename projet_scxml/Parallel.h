//
// Created by vincent on 03/04/17.
//

#ifndef PARSER_SCXML_PARALLEL_H
#define PARSER_SCXML_PARALLEL_H


#include <map>
#include "State.h"

class Parallel : public State{
private:
    std::vector<State*> initials;
    std::map<State*, std::vector<State*>> map_states;
public:
    Parallel(std::string name):State(name){}
    void addInitial(State*);
    std::vector<State*> GetInitials();
    std::map<State*, std::vector<State*>> * GetMapStates();
    void putInMap(State * s, std::vector<State*> vect);
};


#endif //PARSER_SCXML_PARALLEL_H
