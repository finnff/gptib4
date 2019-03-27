
#include "BrickPi3.h" // for BrickPi3
#include <iostream>
#include <unistd.h>    
#include <signal.h>


BrickPi3 BP; // Create a BrickPi3 instance with the default address of 1

void exit_signal_handler(int signo);

void TurnL90(){
 BP.set_motor_dps(PORT_A, 275);
 BP.set_motor_dps(PORT_B, -275);
 sleep(1);
 BP.reset_all();
}

void TurnR90(){
 BP.set_motor_dps(PORT_A, -275);
 BP.set_motor_dps(PORT_B, 275);
 sleep(1);
 BP.reset_all();
}

void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

int main(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  
  BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
  BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));

  TurnL90();
  TurnL90();
  TurnR90();
  TurnR90();

}





