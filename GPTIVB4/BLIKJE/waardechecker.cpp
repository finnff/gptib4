#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <vector>

using namespace std;
BrickPi3 BP;

void exit_signal_handler(int signo);

// waarde bestand importeren


vector <int> kleurscan(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    sensor_color_t      Color1;
    while(true){
        if(BP.get_sensor(PORT_1, Color1) == 0){
            BP.set_motor_power(PORT_B, -30); //rotation = ~2.5 sec
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
            red = red / aantal;
            green = green / aantal;
            blue = blue / aantal;
            ambient = ambient / aantal;
            vector <int> rgb = {red, green, blue, ambient};
            BP.reset_all();    // Reset everything so there are no run-away motors
            return rgb;
        }
    }
}

// int kleurcheck(k)
// {
//     /*import kleurenbestand */
//     for(unsigned int i = 0; i < /*bestand regels*/; i++)
//     {
//         if (k >/*bestand*/[i][1] && k < /*bestand*/[i][2]) {
//             return /*bestand*/[i][0];
//             /* later geluidsfunctie hier misschien aanroepen*/
//             break;
//         }
//     }
// }


void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();
        exit(-2);
    }
}

int main()
{
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    vector<int> printvec = {};
    vector<vector<int>> giga ={};
    for (size_t i=0; i<21;i++){
        printvec=kleurscan();
        giga.push_back(printvec);
    }
    for(size_t l = 0; l<giga.size();l++){
        for(int k=0; k<giga[l].size(); k++){
            cout << giga[l][k]<<", "<<flush;
    }
	cout<<endl;
}
}
