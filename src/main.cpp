#include "vex.h"
using namespace vex;

brain Brain;
motor leftMotor = motor(PORT1, false);
motor rightMotor = motor(PORT2, false);
optical opticalSensor = optical(PORT3);

void searchForLine() {
    // First attempt: turn right
    leftMotor.spin(forward, 15, percent);
    rightMotor.spin(reverse, 15, percent);
    
    int startTime = Brain.Timer.time(msec);
    while(Brain.Timer.time(msec) - startTime < 1500) { // ~45-degree turn
        if(opticalSensor.hue() < 10) {  // Found the line again
            leftMotor.stop();
            rightMotor.stop();
            return;
        }
        this_thread::sleep_for(10);
    }
    
    // If no line found, try turning left
    leftMotor.spin(reverse, 15, percent);
    rightMotor.spin(forward, 15, percent);
    
    startTime = Brain.Timer.time(msec);
    while(Brain.Timer.time(msec) - startTime < 3000) { // ~90-degree turn
        if(opticalSensor.hue() < 10) {  
            // Found the line again
            leftMotor.stop();
            rightMotor.stop();
            return;
        }
        this_thread::sleep_for(10);
    }

    // Stop motors if no line found (fail-safe)
    leftMotor.stop();
    rightMotor.stop();
}


int main() {
    Brain.Screen.printAt(2, 30, "Optical Line Following");
    opticalSensor.setLight(ledState::on);
    
    while(1) {
        int hueValue = opticalSensor.hue();
        
        if (hueValue < 10) {
            leftMotor.spin(forward, 50, percent);
            rightMotor.spin(forward, 50, percent);
        } else {
            leftMotor.stop();
            rightMotor.stop();
            searchForLine();
        }
        
        Brain.Screen.clearLine(2);
        Brain.Screen.printAt(2, 60, "Hue: %d", hueValue);
        
        this_thread::sleep_for(10);
    }
}
