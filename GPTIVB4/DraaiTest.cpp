
#include "BrickPi3.h" // for BrickPi3
#include <iostream>
#include <unistd.h>    
#include <signal.h>


BrickPi3 BP; // Create a BrickPi3 instance with the default address of 1

void exit_signal_handler(int signo);

void Draaitest(){
  BP.set_motor_power(PORT_A, 50);
  BP.set_motor_power(PORT_B, 50);
  usleep(3000000);//3 Sec
  BP.set_motor_power(PORT_A, 0);
  BP.set_motor_power(PORT_B, 0);
  usleep(500000);//0.5 Sec
  BP.set_motor_power(PORT_A, -50);
  BP.set_motor_power(PORT_B, 50);
  usleep(1000000);//1 Sec
  BP.set_motor_power(PORT_A, 50);
  BP.set_motor_power(PORT_B, 50);
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

  Draaitest();

}





