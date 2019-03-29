 
#include "BrickPi3.h" // for BrickPi3
#include <iostream>      // for cout
#include <unistd.h>     // for sleep
#include <signal.h>     // for catching exit signals
#include <iomanip>		// for setw and setprecision

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

void DriveForward(){
    BP.set_motor_power(PORT_A, -30);
    BP.set_motor_power(PORT_B, -30);
}

void BrakeBoth(){
    BP.set_motor_power(PORT_A, 0);
    BP.set_motor_power(PORT_B, 0);
    BP.reset_all();
}


int main(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

    BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);

    sensor_ultrasonic_t Ultrasonic2;
    float curr_distance =15.0;

    while(true){
        while(BP.get_sensor(PORT_2, Ultrasonic2) == 0){
		    cout << "Ultrasonic sensor (S2): "   << Ultrasonic2.cm << "cm" << endl;
            curr_distance = Ultrasonic2.cm;
            if(curr_distance>10){
                BP.set_motor_power(PORT_B, -30);
                BP.set_motor_power(PORT_A, -30);
            }
            if(curr_distance<=10){
                BP.reset_all();
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
