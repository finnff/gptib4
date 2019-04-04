#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <vector>

using namespace std;
BrickPi3 BP;

void exit_signal_handler(int signo);

vector <int> kleurscan(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    sensor_color_t      Color1;
    while(true){
        if(BP.get_sensor(PORT_1, Color1) == 0){
            BP.set_motor_power(PORT_B, -60); //rotation = ~2.5 sec
            BP.set_motor_dps(PORT_C, 180);
            int red = 0;
            int green = 0;
            int blue = 0;
            int ambient = 0;
            int time = 0;
            int aantal =0;
            while(time <= 36){
                aantal++;
                red += Color1.reflected_red;
                green += Color1.reflected_green;
                blue += Color1.reflected_blue;
                ambient += Color1.ambient;
                usleep(70000);
                time++;
            }
            ambient = ambient / aantal;
            red = (red / aantal) - ambient;
            green = (green / aantal) - ambient;
            blue = (blue / aantal) - ambient;
            if (red < 0) {
                red = 0;
            }
            if (green < 0) {
                green = 0;
            }
            if (blue < 0) {
                blue = 0
            }
            
            vector <int> rgb = {red, green, blue};
            BP.reset_all();    // Reset everything so there are no run-away motors
            return rgb;
        }
    }
}

void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();
        exit(-2);
    }
}

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    vector<int> printvec = kleurscan();
    for(int i=0; i<printvec.size(); i++){
    cout << printvec[i]<< " " << flush;
    }
    // cout << kcheck(kleurscan())
}

// int kcheck(rgb){
//     for(unsigned int i = 1; i < bestand.size(); i++){
//         if (rgb[0] > (bestand[i][0]*0.9) && rgb[0] < (bestand[i][0]*1.1) ) {
//             if (rgb[1] > (bestand[i][1]*0.9) && rgb[1] < (bestand[i][1]*1.1) ) {
//                 if (rgb[2] > (bestand[i][2]*0.9) && rgb[2] < (bestand[i][2]*1.1) ) {
//                     return bestand[i][3];
//                 }
//             } 
//         } 
//     }
// }