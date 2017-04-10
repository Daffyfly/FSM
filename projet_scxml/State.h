//
// Created by vincent on 01/03/17.
//

#ifndef PARSER_SCXML_STATE_H
#define PARSER_SCXML_STATE_H


#include "Transition.h"
#include <iostream>
#include <string>
#include <vector>

class State {
protected:
    std::string name;
    std::vector<Transition*> transitions;
    std::vector<std::string> onEntryEvents;
    std::vector<std::string> onExitEvents;
    std::vector<State*> childStates;
public:
    State(std::string name):name(name){}
    void addTransition(Transition* t);

    std::string getName();
    std::vector<Transition*> getTransitions();
    std::vector<std::string> getOnEntryEvents();
    std::vector<std::string> getOnExitEvents();
    std::vector<State*> getChildStates();

    void eraseTransitions();
    void addEntryEvents(std::string e);
    void addExitEvents(std::string e);
    void addChildState(State * s);
};


#endif //PARSER_SCXML_STATE_H
