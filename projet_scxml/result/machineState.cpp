#include "machineState.h"

using namespace std;

void MachineState::start(){
	currentState= State_1;

}

void MachineState::log(string s){
	cout<<"Entering state " + s<<endl;
}

void MachineState::yolo_action(){
	cout<<"Sent yolo"<<endl;
}

void MachineState::event_3_action(){
	cout<<"Sent event_3"<<endl;
}

void MachineState::allo_action(){
	cout<<"Sent allo"<<endl;
}

void MachineState::event_2_action(){
	cout<<"Sent event_2"<<endl;
}

void MachineState::salut_action(){
	cout<<"Sent salut"<<endl;
}

void MachineState::event_1_action(){
	cout<<"Sent event_1"<<endl;
}


void MachineState::activate(EventIn event){
	q.push(event);
	while(!q.empty()){
		event = q.front();
		q.pop();
		switch(currentState){
			case State_1:
				switch(event){
					case Transition_1:
						cout<<"Doing Transition_1 to State_3 "<<endl;
						currentState=State_3;
						log("State_3");
						salut_action();
						event_1_action();
						break;
					default: break;
				}
				break;
			case State_3:
				switch(event){
					case Transition_2:
						cout<<"Doing Transition_2 to State_2 "<<endl;
						currentState=State_2;
						log("State_2");
						allo_action();
						event_2_action();
						yolo_action();
						break;
					default: break;
				}
				break;
			case State_2:
				switch(event){
					case Transition_3:
						cout<<"Doing Transition_3 to State_1 "<<endl;
						currentState=State_1;
						log("State_1");
						event_3_action();
						break;
					default: break;
				}
				break;
			default:
			break;
		}
	}

}

