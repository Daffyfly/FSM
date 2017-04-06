//
// Created by vincent on 01/03/17.
//

#include "Transition.h"
#include "State.h"

using namespace std;

string Transition::getEvent(){
    return event;
}

string Transition::getTarget(){
    return target;
}

Transition * Transition::clone(){
    Transition * tmp = new Transition(this->getEvent(),this->getTarget());
    tmp->setTo(this->To());
    tmp->setFrom(this->From());
    for(string s : this->getEventsSent()){
        tmp->addEventSent(s);
    }
    return tmp;
}

State * Transition::To(){
    return to;
}

State * Transition::From(){
    return from;
}

void Transition::setFrom(State * s){
    this->from = s;
}

void Transition::setTo(State * s){
    this->to = s;
}

vector<string> Transition::getEventsSent(){
    return eventsSent;
}

void Transition::addEventSent(std::string event){
    this->eventsSent.push_back(event);
}