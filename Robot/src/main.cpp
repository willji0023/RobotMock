// Copyright (c) William Jin 2016. All Rights Reserved.

#include "RobotMock.hpp"
#include "Timer.hpp"

enum class State {
	ready, // raising tongs height state
	armOpen, // tongs opening state
	heightDecrease, // lowering tongs height state
	armGrab // tongs closing state
};

int main() {
    RobotMock robot;
    State state = State::ready;  // Assign the initial state here
    Timer timer;

    while (1) {
        switch (state) {
        case State::ready:
        	if (robot.GetPressed()){
        		robot.SetSetpoint(24);
        		if (robot.AtSetpoint()){
        			std::cout << "Robot is at starting point\n";
        			state = State::armOpen;
        		}
        	}
        	break;
        case State::armOpen:
        	robot.SetClaw(false);
			if (robot.GetClaw){
				std::cout << "Claw is now open\n";
				state = State::heightDecrease;
			}
			break;
        case State::heightDecrease:
        	robot.SetSetpoint(12);
        	if (robot.AtSetpoint()){
        		std::cout << "Robot has reached set point\n";
        		state = State::armGrab;
        	}
    		break;
        case State::armGrab:
        	robot.SetClaw(true);
        	if (robot.GetClaw()){
        		std::cout << "Claw has been closed\n";
        		state = State::ready;
        	}
        	break;
    }
}

}
