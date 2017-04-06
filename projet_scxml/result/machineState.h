#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

enum State{State_1,State_3,State_2};

enum EventIn{Transition_3,Transition_2,Transition_1};

enum EventOut{yolo,event_3,allo,event_2,salut,event_1};

class MachineState
	{
	public:
		void start();
		void log(std::string);
		void activate(EventIn);
	private:
		std::queue<EventIn> q;
		void yolo_action();
		void event_3_action();
		void allo_action();
		void event_2_action();
		void salut_action();
		void event_1_action();
		
		State currentState;
	};

#endif // MACHINESTATE_H