//
// Created by vincent on 01/03/17.
//

#include "State.h"

using namespace std;

void State::addTransition(Transition * t) {
    transitions.push_back(t);
}

string State::getName(){
    return name;
}

vector<State*> State::getChildStates() {
    return childStates;
}

vector<Transition*> State::getTransitions(){
    return transitions;
}

vector<string> State::getOnEntryEvents(){
    return onEntryEvents;
}

vector<string> State::getOnExitEvents() {
    return onExitEvents;
}

void State::addEntryEvents(std::string e) {
    onEntryEvents.push_back(e);
}

void State::addExitEvents(std::string e) {
    onExitEvents.push_back(e);
}

void State::addChildState(State * s){
    childStates.push_back(s);
}