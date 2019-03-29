#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision

//zwrt/ wat = 1800 - 2000
// zwart> 2000
//wit < 1800

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);

    sensor_light_t      Light1;

    while(true){
        float waarde;
        if(BP.get_sensor(PORT_1, Light1) == 0){
            waarde = Light1.reflected;
            if(waarde >1800 && waarde < 2000){
                BP.set_motor_dps(PORT_A, -200);
                BP.set_motor_dps(PORT_B, -200);
            }else if(waarde < 1800){//links
                BP.set_motor_dps(PORT_A, 50);
                BP.set_motor_dps(PORT_B, -50);
            }else if(waarde > 2000){//rechts
                BP.set_motor_dps(PORT_A, -50);
                BP.set_motor_dps(PORT_B, 50);
            }
        }

    }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();    // Reset everything so there are no run-away motors
        exit(-2);
    }
}