// Copyright (c) William Jin 2016. All Rights Reserved.

#include <iostream>
#include "RobotMock.hpp"
#include "Timer.hpp"

enum class State {
	ready, // waiting for button to pressed state
	armOpen, // tongs opening state
	armOpenTimer, // tongs opening timer state
	heightDecrease, // lowering tongs height state
	armGrab, // tongs closing state
	armGrabTimer, // tongs closing state timer
	heightIncrease, // raising tongs height state
	finished // waiting for robot to reach start height state
};

int main() {
    RobotMock robot;
    State state = State::ready;  // Assign the initial state here
    Timer timer;
    constexpr double kHeightHalf = 7.2;
    constexpr double KHeight = 14.4;

    while (1) {
        switch (state) {
        case State::ready:
        	if (robot.GetPressed()){
        		std::cout << "Button was push and robot is ready to go\n";
        		state = State::armOpen;
        	}
        	break;
        case State::armOpen:
        	state = State::armOpenTimer;
        	robot.SetClaw(false);
        	timer.Start();
        	break;
        case State::armOpenTimer:
        	if (timer.HasPeriodPassed(0.4)){
        		state = State::heightDecrease;
        		timer.Reset();
        	}
        	break;
        case State::heightDecrease:
        	if (!robot.GetClaw()){
        		std::cout << "Claw is now open\n";
        		state = State::armGrab;
            	robot.SetSetpoint(kHeightHalf);
        	}
        	break;
        case State::armGrab:
        	if (robot.AtSetpoint()){
        		std::cout << "Robot has reached set point\n";
        	    state = State::armGrabTimer;
        	    robot.SetClaw(true);
        	    timer.Start();
        	}
        	break;
        case State::armGrabTimer:
        	if (timer.HasPeriodPassed(0.4)){
        		state = State::heightIncrease;
        		timer.Reset();
        	}
        	break;
        case State::heightIncrease:
        	if (robot.GetClaw()){
        		std::cout << "Claw has been closed\n";
        		state = State::finished;
        		robot.SetSetpoint(KHeight);
        	}
        	break;
        case State::finished:
        	if (robot.AtSetpoint()){
        		std::cout << "Robot is ready and waiting for button to be pushed\n";
        		state = State::ready;
        	}
    }
}

}
