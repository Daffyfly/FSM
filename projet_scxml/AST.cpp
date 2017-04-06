//
// Created by vincent on 07/03/17.
//

#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include "AST.h"
#include "StateParallele.h"
#include "TransitionParallele.h"

using namespace std;

static vector<State *> allStateList(vector<State *> s){
    vector<State *> result;
    for(State * tmp : s) {
        if (!string("5State").compare(typeid(*tmp).name())){
            result.push_back(tmp);
            vector<State *> tmp_list = allStateList(tmp->getChildStates());
            for (State *a : tmp_list) {
                result.push_back(a);
            }
        }
    }
    return result;
}

static void inheritParallel(vector<Parallel*> &p, vector<State*> &state_list){
    for(Parallel * tmp : p) {
        for (State *a : tmp->GetInitials()) {
            tmp->addInitial(a->getChildStates().at(0));
            tmp->putInMap(a->getChildStates().at(0),a->getChildStates());
            for(State *s : a->getChildStates()){
                if (!string("5State").compare(typeid(*s).name()))
                    state_list.push_back(s);
            }
            p.erase(std::remove(p.begin(), p.end(), a), p.end());
        }
    }
}

static vector<string> names_parallel_states;


static void recursive_append_name(string name, int where, std::map<State*,vector<State*>> map_states){
    std::map<State*,vector<State*>>::iterator it_tmp = map_states.begin();
    advance(it_tmp,where);
    if(it_tmp != map_states.end()){
        for(State * s: it_tmp->second){
            string tmp = name;
            tmp.append("_" + s->getName());
            recursive_append_name(tmp,where+1,map_states);
        }
    }
    else{
        names_parallel_states.push_back(name);
    }
}

static void getParallelStatesNames(Parallel * p){
    std::map<State*,vector<State*>> map_states =  *(p->GetMapStates());
    std::map<State*,vector<State*>>::iterator it = map_states.begin();
    vector<State*> firsts = it->second;
    for(State * s : firsts){
        string name;
        name.append(s->getName());
        recursive_append_name(name,1,map_states);
    }
}


static void setSousEtats(vector<StateParallele*> & tmplist, Parallel * p){
    for(StateParallele * state : tmplist)
    {
        string name = state->getName();
        std::map<State*,vector<State*>> map_states =  *(p->GetMapStates());
        std::map<State*,vector<State*>>::iterator it = map_states.begin();
        while(it!=map_states.end()) {
            for (State *tmp : it->second) {
                if (name.find(tmp->getName()) != std::string::npos) {
                    state->addSousEtat(tmp);
                }
            }
            it++;
        }
    }

}

static State* destinationTransition(TransitionParallele * t, StateParallele * current, Parallel * p, vector<StateParallele*> tmplist) {
    vector<State*> result;

    std::map<State*,vector<State*>> map_states =  *(p->GetMapStates());
    std::map<State*,vector<State*>>::iterator it = map_states.begin();

    int j = 0;
    string namedest;
    vector<State*> first_states = it->second;
    auto itfind = find_if(first_states.begin(), first_states.end(),
                      [&t](State* obj) {return !obj->getName().compare(t->getTargets().at(0)->getName());});
    if(itfind != first_states.end()){
        namedest.append((*itfind)->getName());
        j=1;
    }
    else{
        namedest.append(current->getSousEtats().at(0)->getName());
    }
    int i = 1;
    it++;
    while(it != map_states.end()){
        if(j < t->getTargets().size()) {
            auto itfi = find_if(it->second.begin(), it->second.end(),
                                  [&t, &j](State *obj) {
                                      return !obj->getName().compare(t->getTargets().at(j)->getName());
                                  });
            if (itfi != it->second.end()) {
                namedest.append("_").append((*itfi)->getName());
                i++;
                j++;
            } else {
                namedest.append("_").append(current->getSousEtats().at(i++)->getName());
            }
        }
        else{
            namedest.append("_").append(current->getSousEtats().at(i++)->getName());
        }
        it++;
    }
    for(StateParallele * s : tmplist){
        if(s->getName().find(namedest) != string::npos)
            return s;
    }
    return nullptr;
}

void AST::MiseAPlatParallele(){
    for(Parallel * p : all_parallel_list){
        vector<StateParallele*> resultlist = vector<StateParallele*>();
        vector<StateParallele*> tmplist = vector<StateParallele*>();
        getParallelStatesNames(p);
        for(string s : names_parallel_states) {
            tmplist.push_back(new StateParallele (s));
        }
        names_parallel_states.clear();
        setSousEtats(tmplist,p);
        for(StateParallele * s : tmplist){
            vector<TransitionParallele*> transitionslist = vector<TransitionParallele*>();
            for(State * tmp : s->getSousEtats())
            {
                for(Transition * t : tmp->getTransitions())
                {
                    auto it = find_if(transitionslist.begin(), transitionslist.end(),
                                      [&t](TransitionParallele* obj) {return !obj->getName().compare(t->getEvent());});
                    if(it != transitionslist.end()){
                        for(string e : t->getEventsSent())
                            (*it)->addEventSent(e);
                        (*it)->addTarget(t->To());
                    }
                    else{
                        TransitionParallele* trans = new TransitionParallele(t->getEvent());
                        for(string e : t->getEventsSent()){
                            trans->addEventSent(e);
                        }
                        trans->addTarget(t->To());
                        trans->addFrom(s);
                        transitionslist.push_back(trans);
                    }
                }
                for(string e : tmp->getOnEntryEvents()){
                    s->addEntryEvents(e);
                }
                for(string e : tmp->getOnExitEvents()){
                    s->addExitEvents(e);
                }
            }
            for(TransitionParallele *t : transitionslist){
                State* destinations = destinationTransition(t,s,p,tmplist);
                Transition* tmptrans = new Transition(t->getName(), destinations->getName());
                tmptrans->setTo(destinations);
                tmptrans->setFrom(s);
                for(string e : t->getEventsSent()){
                    tmptrans->addEventSent(e);
                }
                all_transitions_list.push_back(tmptrans);
                s->addTransition(tmptrans);
            }
            for(Transition * tt : p->getTransitions()){
                vector<Transition*> translist = s->getTransitions();
                auto it = find_if(translist.begin(), translist.end(),
                                  [&tt](Transition* obj) {return !obj->getEvent().compare(tt->getEvent());});
                if(it == translist.end()) {
                    Transition *tmptrans = tt->clone();
                    tt->setFrom(s);
                    all_transitions_list.push_back(tmptrans);
                    s->addTransition(tmptrans);
                    for(string e : p->getOnExitEvents()){
                        s->addExitEvents(e);
                    }
                }
            }
            resultlist.push_back(s);
        }
        for(StateParallele * s : resultlist){
            for(State * ss : s->getSousEtats()){
                for(Transition * tt : ss->getTransitions()){
                    all_transitions_list.erase(std::remove(all_transitions_list.begin(),
                                                           all_transitions_list.end(),
                                                           tt),
                                               all_transitions_list.end());
                }
                all_state_list.erase(std::remove(all_state_list.begin(),
                                                 all_state_list.end(),
                                                       ss),
                                     all_state_list.end());
            }
            all_state_list.push_back(s);
        }
        for(Transition * tt : all_transitions_list){
            if(!tt->To()->getName().compare(p->getName()))
                tt->setTo(resultlist.at(0));
        }
        if(initial == p){
            initial = resultlist.at(0);
        }
        for(string e : p->getOnEntryEvents()){
            resultlist.at(0)->addEntryEvents(e);
        }
        for(Parallel * pp : all_parallel_list){
            std::map<State*,vector<State*>> * map_states =  pp->GetMapStates();
            std::map<State*,vector<State*>>::iterator it = (*map_states).begin();
            while(it!=(*map_states).end()){
                vector<State*> statelist = it->second;
                auto itfind = find_if(statelist.begin(), statelist.end(),
                                      [&p](State * s) {return !s->getName().compare(p->getName());});
                if(itfind != statelist.end()){
                    it->second.erase(std::remove(it->second.begin(),it->second.end(),*itfind),it->second.end());
                    for(State * ss : resultlist){
                        it->second.push_back(ss);
                    }
                    pp->GetMapStates()->insert(std::make_pair(it->first, it->second));
                }
                it++;
            }
        }
        all_parallel_list.erase(std::remove( all_parallel_list.begin(),
                                             all_parallel_list.end(),
                                               p),
                                all_parallel_list.end());

    }
}

void AST::EnleverEtatEnTropRecursive(State * s) {
    auto it = find_if(useful_states.begin(), useful_states.end(),
                      [&s](State * ss) {return !s->getName().compare(ss->getName());});
    if(it == useful_states.end()){
        useful_states.push_back(s);
        for(Transition * t : s->getTransitions()){
            EnleverEtatEnTropRecursive(t->To());
        }
    }
}

void AST::EnleverEtatEnTrop(){
    EnleverEtatEnTropRecursive(initial);
}


AST::AST(vector<State*> state_list, vector<Parallel *> parallel_list, State * initial){
    this->initial=initial;
    this->state_list=state_list;
    inheritParallel(parallel_list,state_list);
    this->all_parallel_list=parallel_list;
    this->all_state_list = allStateList(state_list);
    unordered_set<string> event_set_out;
    unordered_set<string> event_set_in;

    all_transitions_list = ArrangeTransitions();
    MiseAPlat();
    MiseAPlatParallele();
    EnleverEtatEnTrop();

    for(State * s : useful_states) {
        for (Transition *t : s->getTransitions()) {
            event_set_in.insert(t->getEvent());
        }
    }
    event_in_names = vector<string>(event_set_in.begin(), event_set_in.end());
    for(State * s : useful_states) {
        for (Transition *t : s->getTransitions()) {
            for (string tmp : t->getEventsSent()) {
                if(std::find(event_in_names.begin(), event_in_names.end(), tmp) == event_in_names.end()) {
                    event_set_out.insert(tmp);
                }
            }
        }
        for (string tmp : s->getOnEntryEvents()) {
            if(std::find(event_in_names.begin(), event_in_names.end(), tmp) == event_in_names.end()) {
                event_set_out.insert(tmp);
            }
        }
        for (string tmp : s->getOnExitEvents()) {
            if(std::find(event_in_names.begin(), event_in_names.end(), tmp) == event_in_names.end()) {
                event_set_out.insert(tmp);
            }
        }
    }
    event_out_names = vector<string>(event_set_out.begin(), event_set_out.end());
}

AST::AST(){}

vector<Transition*> AST::ArrangeTransitions(){
    vector<Transition*> t_list;
    for(State * s : all_state_list){
        for(Transition * t : s->getTransitions()){
            t->setFrom(s);
            for(State * tmp : all_state_list){
                if(!tmp->getName().compare(t->getTarget())) {
                    t->setTo(tmp);
                    break;
                }
            }
            for(Parallel * tmp : all_parallel_list){
                if(!tmp->getName().compare(t->getTarget())) {
                    t->setTo(tmp);
                    break;
                }
            }
            t_list.push_back(t);
        }
    }
    for(Parallel * s : all_parallel_list){
        for(Transition * t : s->getTransitions()){
            t->setFrom(s);
            for(State * tmp : all_state_list){
                if(!tmp->getName().compare(t->getTarget())) {
                    t->setTo(tmp);
                    break;
                }
            }
            for(Parallel * tmp : all_parallel_list){
                if(!tmp->getName().compare(t->getTarget())) {
                    t->setTo(tmp);
                    break;
                }
            }
            t_list.push_back(t);
        }
    }
    return t_list;
}

void AST::MiseAPlat(){
    for(int i = 0; i<all_transitions_list.size(); i++){
        Transition * t = all_transitions_list.at(i);
        State * tmp = t->To();
        while(!tmp->getChildStates().empty())
        {
            t->setTo(tmp->getChildStates().at(0));
            tmp = t->To();
        }
        tmp = t->From();
        if(!tmp->getChildStates().empty())
        {
            for(State * tmp2 : tmp->getChildStates()){
                vector<Transition*> transitions = tmp2->getTransitions();
                auto it = find_if(transitions.begin(), transitions.end(),
                                  [&t](Transition* obj) {return !obj->getEvent().compare(t->getEvent());});
                if (it == transitions.end()) {
                    Transition * newtransition = t->clone();
                    newtransition->setFrom(tmp2);
                    tmp2->addTransition(newtransition);
                    all_transitions_list.push_back(newtransition);
                }
            }

            all_transitions_list.erase(std::remove(all_transitions_list.begin(),
                                                   all_transitions_list.end(),
                                                   t),
                                       all_transitions_list.end());
            delete t;
            i--;
        }
    }
    for(int i = 0; i<all_state_list.size(); i++){
        State * s = all_state_list.at(i);
        if(!s->getChildStates().empty()){
            all_state_list.erase(std::remove(all_state_list.begin(), all_state_list.end(), s), all_state_list.end());
            i--;
        }
    }
}

std::vector<State*> AST::getStateList() {
    return state_list;
}

std::vector<State*> AST::getAllStateList() {
    return useful_states;
}


std::vector<string> AST::getEventInNames() {
    return event_in_names;
}

std::vector<string> AST::getEventOutNames() {
    return event_out_names;
}

State* AST::getInitial() {
    return initial;
}