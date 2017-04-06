//
// Created by vincent on 03/04/17.
//

#include "Parallel.h"

void Parallel::addInitial(State* s){
    this->initials.push_back(s);
}
std::vector<State*> Parallel::GetInitials(){
    return initials;
}

std::map<State*, std::vector<State*>> * Parallel::GetMapStates(){
    return &map_states;
};

void Parallel::putInMap(State * s, std::vector<State*> vect){
    map_states.insert ( std::pair<State*,std::vector<State*>>(s,vect));
}