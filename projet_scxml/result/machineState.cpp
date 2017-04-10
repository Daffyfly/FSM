#include "machineState.h"

using namespace std;

static void salut_action(){
	cout<<"Sent salut"<<endl;
}

static void lol_action(){
	cout<<"Sent lol"<<endl;
}

static void yolo_action(){
	cout<<"Sent yolo"<<endl;
}

static void pass_action(){
	cout<<"Done pass"<<endl;
}

void MachineState::link(EventOut e, function<void()> f){
    mapfunctions[e]= f;
}

void MachineState::link(EventIn e, function<void()> f){
    mapfunctionsin[e]= f;
}

void MachineState::start(){
	currentState= State_18;

	mapfunctions.emplace(salut,&salut_action);
	mapfunctions.emplace(lol,&lol_action);
	mapfunctions.emplace(yolo,&yolo_action);
	mapfunctionsin.emplace(pass,&pass_action);
}

void MachineState::log(string s){
	cout<<"Entering state " + s<<endl;
}


void MachineState::activate(EventIn event){
	q.push(event);
	while(!q.empty()){
		event = q.front();
		mapfunctionsin[event]();
		q.pop();
		switch(currentState){
			case State_18:
				switch(event){
					case pass:
						currentState=State_11_State_12_State_24;
						log("State_11_State_12_State_24");
						mapfunctions[yolo]();
						mapfunctions[lol]();
						break;
					default: break;
				}
				break;
			case State_11_State_12_State_24:
				switch(event){
					case pass:
						currentState=State_19_State_21_State_10_State_25;
						log("State_19_State_21_State_10_State_25");
						mapfunctions[salut]();
						break;
					default: break;
				}
				break;
			case State_19_State_21_State_10_State_25:
				switch(event){
					case pass:
						currentState=State_20_State_22_State_14_State_24;
						log("State_20_State_22_State_14_State_24");
						break;
					default: break;
				}
				break;
			case State_20_State_22_State_14_State_24:
				switch(event){
					case pass:
						currentState=State_20_State_22_State_14_State_25;
						log("State_20_State_22_State_14_State_25");
						break;
					default: break;
				}
				break;
			case State_20_State_22_State_14_State_25:
				switch(event){
					case pass:
						currentState=State_20_State_22_State_14_State_24;
						log("State_20_State_22_State_14_State_24");
						break;
					default: break;
				}
				break;
			default:
			break;
		}
	}

}

