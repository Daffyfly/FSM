//
// Created by vincent on 01/03/17.
//

#include <unordered_set>
#include <algorithm>
#include "codeGenerator.h"

using namespace std;


void codeGenerator::app(std::string filename, int tab, std::string s){
    myfile.open(path+filename,std::ios_base::app);
    for(;tab>0;tab--)
        myfile<<"\t";
    myfile<<s;
    myfile.close();
}

codeGenerator::codeGenerator(std::string path, std::string name_of_file, AST & ast){
    this->path = path;
    this->name_of_machine_state = name_of_file;
    name_of_machine_state[0]=toupper(name_of_machine_state[0]);
    this->dotcpp = name_of_file;
    dotcpp.append(".cpp");
    this->doth = name_of_file;
    doth.append(".h");
    myfile.open(path+dotcpp);
    myfile<<"";
    myfile.close();
    myfile.open(path+doth);
    myfile<<"";
    myfile.close();
    this->ast = ast;
}

void codeGenerator::eventType(std::string event){
    for(string tmp : ast.getEventOutNames()){
        if(!tmp.compare(event)) {
            app(dotcpp, 6, "mapfunctions["+tmp+"]();\n");
            return;
        }
    }
    for(string tmp : ast.getEventInNames()){
        if(!tmp.compare(event)) {
            app(dotcpp, 6, "q.push("+ tmp + ");\n");
            return;
        }
    }
}



static string toUpper(std::string s){
    string upper = s;
    transform(upper.begin(), upper.end(),upper.begin(), ::toupper);
    return upper;
}

void codeGenerator::generateFile() {
    vector<State *> state_list = ast.getStateList();
    vector<string> event_names_in = ast.getEventInNames();
    vector<string> event_names_out = ast.getEventOutNames();
    app(dotcpp, 0, "#include \"" + doth + "\"\n\n");
    app(dotcpp, 0, "using namespace std;\n\n");
    app(doth, 0, "#ifndef " + toUpper(name_of_machine_state) + "_H\n");
    app(doth, 0, "#define " + toUpper(name_of_machine_state) + "_H\n\n");
    app(doth, 0, "#include <iostream>\n");
    app(doth, 0, "#include <string>\n");
    app(doth, 0, "#include <vector>\n");
    app(doth, 0, "#include <queue>\n");
    app(doth,0,"#include <functional>\n");
    app(doth, 0, "#include <map>\n\n");
    app(doth, 0, "enum State{");
    bool first = true;
    for (State *s : ast.getAllStateList()) {
        if (s->getChildStates().empty()) {
            string tmp_name = s->getName();
            if (!first) {
                app(doth, 0, ",");
            }
            app(doth, 0, tmp_name);
            first = false;
        }
    }
    app(doth, 0, "};\n\n");
    app(doth, 0, "enum EventIn{");
    if (!event_names_in.empty()){
        string first_event_name = event_names_in.at(0);
        for (string s : event_names_in) {
            if (first_event_name != s) {
                app(doth, 0, ",");
            }
            app(doth, 0, s);
        }
    }
    app(doth,0,"};\n\n");

    app(doth,0,"enum EventOut{");
    if (!event_names_out.empty()){
        string first_event_name = event_names_out.at(0);
        for(string s : event_names_out) {
            if (first_event_name != s) {
                app(doth, 0, ",");
            }
            app(doth,0,s);
        }
    }
    app(doth,0,"};\n\n");
    for(string s : event_names_out){
        app(dotcpp,0,"static void "+s+"_action(){\n\tcout<<\"Sent "+ s + "\"<<endl;\n}\n\n");
    }
    for(string s : event_names_in){
        app(dotcpp,0,"static void "+s+"_action(){\n\tcout<<\"Done "+ s + "\"<<endl;\n}\n\n");
    }
    app(dotcpp,0, "void MachineState::link(EventOut e, function<void()> f){\n"
            "    mapfunctions[e]= f;\n"
            "}\n\n");
    app(dotcpp,0, "void MachineState::link(EventIn e, function<void()> f){\n"
            "    mapfunctionsin[e]= f;\n"
            "}\n\n");
    app(doth,0,"class "+name_of_machine_state+"\n\t{\n");
    app(doth,1,"public:\n");
    app(doth,2,"void link(EventOut, std::function<void()> );\n");
    app(doth,2,"void link(EventIn, std::function<void()> );\n");
    app(doth,2,"void start();\n");
    app(dotcpp,0,"void "+name_of_machine_state+"::start(){\n");
    app(dotcpp,1,"currentState= " + ast.getInitial()->getName() +";\n\n");
    for(string e : event_names_out){
        app(dotcpp,1,"mapfunctions.emplace("+e+",&"+e+"_action);\n");
    }
    for(string e : event_names_in){
        app(dotcpp,1,"mapfunctionsin.emplace("+e+",&"+e+"_action);\n");
    }
    app(dotcpp,0,"}\n\n");
    app(doth,2,"void log(std::string);\n");
    app(dotcpp,0,"void "+name_of_machine_state+"::log(string s){\n\tcout<<\"Entering state \" + s<<endl;\n}\n\n");
    app(doth,2,"void activate(EventIn);\n");
    app(doth,1,"private:\n");
    app(doth,2,"std::queue<EventIn> q;\n");
    app(doth,2,"std::map<EventOut,std::function<void()>> mapfunctions;\n");
    app(doth,2,"std::map<EventIn,std::function<void()>> mapfunctionsin;\n");
    app(doth,2,"\n");
    app(doth,2,"State currentState;\n");
    app(doth,1,"};\n\n");
    app(doth,0,"#endif // "+toUpper(name_of_machine_state)+"_H");

    app(dotcpp,0,"\nvoid "+name_of_machine_state+"::activate(EventIn event){\n");
    app(dotcpp,1,"q.push(event);\n");
    app(dotcpp,1,"while(!q.empty()){\n");
    app(dotcpp,2,"event = q.front();\n");
    app(dotcpp,2, "mapfunctionsin[event]();\n");
    app(dotcpp,2,"q.pop();\n");
    app(dotcpp,2,"switch(currentState){\n");
    for(State * s : ast.getAllStateList()){
        generateCodeOfState(*s,0);
    }
    app(dotcpp,3,"default:\n\t\t\tbreak;\n");
    app(dotcpp,2,"}\n");
    app(dotcpp,1,"}\n\n");
    app(dotcpp,0,"}\n\n");
}

void codeGenerator::generateCodeOfState(State& s, int tab){
    if(!s.getTransitions().empty()){
        app(dotcpp,3,"case "+s.getName()+":\n");
        app(dotcpp,tab+4,"switch(event){\n");
        for(Transition * t : s.getTransitions()) {
            app(dotcpp,tab+5,"case "+t->getEvent()+":\n");
            app(dotcpp,tab+6,"currentState="+t->To()->getName()+";\n");
            app(dotcpp,tab+6,"log(\""+t->To()->getName()+"\");\n");
            for(string s1 : s.getOnExitEvents()) {
                eventType(s1);
            }
            for(string s2 : t->getEventsSent()){
                cout<<s2<<endl;
                eventType(s2);
            }
            for(string s3 : t->To()->getOnEntryEvents()){
                eventType(s3);
            }
            app(dotcpp,tab+6,"break;\n");
        }
        app(dotcpp,tab+5,"default: break;\n");
        app(dotcpp,tab+4,"}\n");
        app(dotcpp,4,"break;\n");
    }
}