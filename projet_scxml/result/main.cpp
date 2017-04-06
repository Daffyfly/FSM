//
// Created by vincent on 15/03/17.
//

#include <iostream>
#include "machineState.h"

int main(void){
    MachineState m;
    m.start();
    m.activate(Transition_2);
    m.activate(Transition_1);
    m.activate(Transition_2);
    return 0;
}
