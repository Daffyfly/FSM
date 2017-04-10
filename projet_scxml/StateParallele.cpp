//
// Created by vincent on 05/04/17.
//

#include "StateParallele.h"
#include "State.h"


void StateParallele::addSousEtat(State* s){
    sousEtats.push_back(s);
}

std::vector<State*> StateParallele::getSousEtats(){
    return sousEtats;
}

void StateParallele::eraseSousEtats(){
    sousEtats.clear();
}