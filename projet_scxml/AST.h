//
// Created by vincent on 07/03/17.
//

#ifndef PARSER_SCXML_AST_H
#define PARSER_SCXML_AST_H


#include "State.h"
#include "Parallel.h"

class AST {
private:
    std::vector<State*> state_list;
    std::vector<State*> all_state_list;
    std::vector<State*> useful_states;
    std::vector<Parallel*> all_parallel_list;
    std::vector<Transition*> all_transitions_list;
    std::vector<std::string> event_in_names;
    std::vector<std::string> event_out_names;
    State* initial;
public:
    AST();
    std::vector<Transition*> ArrangeTransitions();
    void MiseAPlat();
    void MiseAPlatParallele();
    AST(std::vector<State*> state_list, std::vector<Parallel*> , State * initial);
    std::vector<State*> getStateList();
    std::vector<State*> getAllStateList();
    std::vector<std::string> getEventInNames();
    std::vector<std::string> getEventOutNames();
    State* getInitial();
    void EnleverEtatEnTrop();
    void EnleverEtatEnTropRecursive(State*);
};


#endif //PARSER_SCXML_AST_H
