//
// Created by vincent on 01/03/17.
//

#include "Xmlparser.h"
#include <fstream>
#include "resources/rapidxml-1.13/rapidxml.hpp"
#include "State.h"
#include "AST.h"
#include "Parallel.h"


using namespace rapidxml;
using namespace std;

static State * initial_state = nullptr;

vector<Parallel*> parallel_list;

static void fill_parallel_state(xml_node<> * state_node, Parallel * parallel, string initial);

static void fill_transition_attribute(xml_node<> * state_node, State * state_tmp, string initial){
    for(xml_node<> * transition_node = state_node->first_node(); transition_node; transition_node=transition_node->next_sibling()){
        string tmp = transition_node->name();
        if(!tmp.compare("transition")){
            Transition * t =new Transition(transition_node->first_attribute("event")->value(),transition_node->first_attribute("target")->value());
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                t->addEventSent(event_node->first_attribute("event")->value());
            }
            state_tmp->addTransition(t);
        }
        if(!tmp.compare("onentry")){
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                state_tmp->addEntryEvents(event_node->first_attribute("event")->value());
                cout<<tmp<<endl;
            }
        }
        if(!tmp.compare("onexit")){
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                state_tmp->addExitEvents(event_node->first_attribute("event")->value());
            }
        }
        if(!tmp.compare("state")){
            State * tmp_state = new State(transition_node->first_attribute("id")->value());
            fill_transition_attribute(transition_node, tmp_state, initial);
            if(!(tmp_state->getName()).compare(initial)){
                initial_state=tmp_state;
            }
            state_tmp->addChildState(tmp_state);
        }
        if(!tmp.compare("parallel")){
            Parallel * tmp_state = new Parallel(transition_node->first_attribute("id")->value());
            fill_parallel_state(transition_node, tmp_state, initial);
            if(!(tmp_state->getName()).compare(initial)){
                initial_state=tmp_state;
            }
            state_tmp->addChildState(tmp_state);
            parallel_list.push_back(tmp_state);
        }
    }
}

static void fill_parallel_state(xml_node<> * state_node, Parallel * parallel, string initial){
    for(xml_node<> * transition_node = state_node->first_node(); transition_node; transition_node=transition_node->next_sibling()){
        string tmp = transition_node->name();
        if(!tmp.compare("transition")){
            Transition * t =new Transition(transition_node->first_attribute("event")->value(),transition_node->first_attribute("target")->value());
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                t->addEventSent(event_node->first_attribute("event")->value());
            }
            parallel->addTransition(t);
        }
        if(!tmp.compare("onentry")){
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                parallel->addEntryEvents(event_node->first_attribute("event")->value());
            }
        }
        if(!tmp.compare("onexit")){
            for(xml_node<> * event_node = transition_node->first_node("send"); event_node; event_node=event_node->next_sibling()){
                parallel->addExitEvents(event_node->first_attribute("event")->value());
            }
        }
        if(!tmp.compare("state")){
            State * tmp_state = new State(transition_node->first_attribute("id")->value());
            fill_transition_attribute(transition_node, tmp_state, initial);
            if(!(tmp_state->getName()).compare(initial)){
                initial_state=tmp_state;
            }
            parallel->addInitial(tmp_state);
        }
        if(!tmp.compare("parallel")){
            Parallel * tmp_state = new Parallel(transition_node->first_attribute("id")->value());
            fill_transition_attribute(transition_node, tmp_state, initial);
            if(!(tmp_state->getName()).compare(initial)){
                initial_state=tmp_state;
            }
            parallel->addInitial(tmp_state);
            parallel_list.push_back(tmp_state);
        }
    }
}


static void fill_state_list(xml_node<> * root_node, vector<State*> & state_list){
    string initial = root_node->first_attribute("initial")->value();
    for(xml_node<> * state_node = root_node->first_node(); state_node; state_node=state_node->next_sibling()){
        string tmp = state_node->name();
        if(!tmp.compare("final") || !tmp.compare("state")) {
            State *tmp_state = new State(state_node->first_attribute("id")->value());
            if (!(tmp_state->getName()).compare(initial)) {
                initial_state = tmp_state;
            }
            fill_transition_attribute(state_node, tmp_state,initial);
            state_list.push_back(tmp_state);
        }
        if(!tmp.compare("parallel")){
            Parallel * tmp_state = new Parallel(state_node->first_attribute("id")->value());
            fill_parallel_state(state_node, tmp_state, initial);
            if(!(tmp_state->getName()).compare(initial)){
                cout<<initial<<endl;
                initial_state=tmp_state;
                cout<<initial_state->getName()<<endl;
            }
            parallel_list.push_back(tmp_state);
        }
    }
}

AST Xmlparser::getAST(){
    xml_document<> doc;
    xml_node<> * root_node;

    vector<State*> state_list;

    ifstream theFile (file_location);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("scxml");
    fill_state_list(root_node, state_list);
    return AST(state_list,  parallel_list, initial_state);
}