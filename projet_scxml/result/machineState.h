#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <map>

enum State{State_18,State_11_State_12_State_24,State_19_State_21_State_10_State_25,State_20_State_22_State_14_State_24,State_20_State_22_State_14_State_25};

enum EventIn{pass};

enum EventOut{salut,lol,yolo};

class MachineState
	{
	public:
		void link(EventOut, std::function<void()> );
		void link(EventIn, std::function<void()> );
		void start();
		void log(std::string);
		void activate(EventIn);
	private:
		std::queue<EventIn> q;
		std::map<EventOut,std::function<void()>> mapfunctions;
		std::map<EventIn,std::function<void()>> mapfunctionsin;
		
		State currentState;
	};

#endif // MACHINESTATE_H