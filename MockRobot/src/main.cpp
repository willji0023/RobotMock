// Copyright (c) William Jin 2016. All Rights Reserved.

#include <iostream>
#include "RobotMock.hpp"
#include "Timer.hpp"

enum class State {
	ready, // raising tongs height state
	armOpen, // tongs opening state
	armOpenTimer, // tongs opening timer state
	heightDecrease, // lowering tongs height state
	armGrab, // tongs closing state
	armGrabTimer, // tongs closing state timer
	finished // tongs closed and done state
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
        		robot.SetSetpoint(KHeight);
        		state = State::armOpen;
        	}
        	break;
        case State::armOpen:
        	if (robot.AtSetpoint()){
        		std::cout << "Robot is at starting point\n";
        	    state = State::armOpenTimer;
        	    robot.SetClaw(false);
        	    timer.Start();
        	}
        	break;
        case State::armOpenTimer:
        	if (timer.HasPeriodPassed(3.0)){
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
        	if (timer.HasPeriodPassed(3.0)){
        		state = State::finished;
        		timer.Reset();
        	}
        	break;
        case State::finished:
        	if (robot.GetClaw()){
        		std::cout << "Claw has been closed\n";
        		state = State::ready;
        	}
        	break;
    }
}

}
