//
// Created by vincent on 05/04/17.
//

#include "TransitionParallele.h"

TransitionParallele::TransitionParallele(std::string n) : name(n) {}

void TransitionParallele::addTarget(State* s){
    targets.push_back(s);
}
void TransitionParallele::addFrom(State* s){
    froms.push_back(s);
}
void TransitionParallele::addEventSent(std::string e){
    eventsSent.push_back(e);
}
std::vector<State*> TransitionParallele::getTargets(){
    return targets;
}
std::vector<State*> TransitionParallele::getFroms(){
    return froms;
}
std::vector<std::string> TransitionParallele::getEventsSent(){
    return eventsSent;
}
std::string TransitionParallele::getName(){
    return name;
}