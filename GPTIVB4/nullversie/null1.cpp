
#include "BrickPi3.h" // for BrickPi3
#include <stdio.h>      // for printf

BrickPi3 BP; // Create a BrickPi3 instance with the default address of 1
//BrickPi3 BP_7(7); // Create a BrickPi3 instance with address 7

int main(){
  //BrickPi3_set_address(1, ""); // set BrickPi3 with any id to the default address of 1
  //BrickPi3_set_address(7, "192A0F96514D4D5438202020FF080C23"); // set BrickPi3 with id 192A0F96514D4D5438202020FF080C23 to address 7

	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
	
}
