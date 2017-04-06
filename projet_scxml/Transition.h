//
// Created by vincent on 01/03/17.
//

#ifndef PARSER_SCXML_TRANSITION_H
#define PARSER_SCXML_TRANSITION_H

#include <iostream>
#include <vector>

class State;

class Transition {
private:
    std::string event;
    std::string target;
    State* to;
    State* from;
    std::vector<std::string> eventsSent;
public:
    Transition(std::string name, std::string destination):event(name),target(destination){}

    void addEventSent(std::string event);
    void setFrom(State* s);
    void setTo(State*);

    State* To();
    State* From();

    Transition* clone();

    std::string getEvent();
    std::string getTarget();
    std::vector<std::string> getEventsSent();
};


#endif //PARSER_SCXML_TRANSITION_H
