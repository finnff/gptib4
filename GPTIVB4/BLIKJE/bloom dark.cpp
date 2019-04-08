#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>


using namespace std;
BrickPi3 BP;
// sqrd version  

//vector <vector <long int>> blikken {};
vector <string> namen {"Cola", "Sprite", "Witte Monster","Sinas", "Hertog Jan","Monster Zwart","Bavaria"};

vector<vector<long int>> bestand = {
{140,47,37,0}, //(Cola)
{1154,1173,1126,1}, //(sprite)
{116,123,104,2}, //(witte monster)
{1171,1136,1119,3}, //(Sinas)
{157,110,62,4}, //(Hertog)
{85,86,81,5}, // Zwarte monster 
{90,120,130,6}  // Bavaria 
};

 /// RRRRRRR,BBBBB,GGGGG
void exit_signal_handler(int signo);

void blikje(vector<vector< long int>> blikken){
    int teempo1 = 1000000;
    int teempo2 = 0;
    for(unsigned int i = 0; i < blikken.size(); i++){
        if (blikken[i][0] < teempo1) {
            teempo1 = blikken[i][0];
            teempo2 = blikken[i][1];
        }
    }
    cout << namen[teempo2] << " with error points "<< teempo1 << endl;
}
void rgbaf(vector<int> rgb){
    vector <vector <long int>> blikken {};
    for(unsigned int j = 0; j < bestand.size(); j++){
        vector<long int> tmp1 = {(((abs (rgb[0] - bestand[j][0]))*(abs (rgb[0] - bestand[j][0]))) 
                                + ((abs (rgb[1] - bestand[j][1]))*(abs (rgb[1] - bestand[j][1]))) 
                                + ((abs (rgb[2] - bestand[j][2]))*(abs (rgb[2] - bestand[j][2])))),bestand[j][3]};
        
        blikken.push_back(tmp1);
    }
    return blikje(blikken);
}


void kleurscanA(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    sensor_color_t      Color1;
    sensor_color_t      Color2;
    while(true){
        if((BP.get_sensor(PORT_1, Color1) == 0) && (BP.get_sensor(PORT_2, Color2) == 0)){
            BP.set_motor_power(PORT_B, -20); //rotation = ~2.5 sec
            BP.set_motor_dps(PORT_C, 180);
            int redA = 0;
            int greenA = 0;
            int blueA = 0;
            int ambientA = 0;
            int redB = 0;
            int greenB = 0;
            int blueB = 0;
            int ambientB = 0;
            int time = 0;
            int aantal =0;
            while(time <= 36){
                aantal++;
                redA += Color1.reflected_red;
                greenA += Color1.reflected_green;
                blueA += Color1.reflected_blue;
                ambientA += Color1.ambient;
                redB += Color2.reflected_red;
                greenB += Color2.reflected_green;
                blueB += Color2.reflected_blue;
                ambientB += Color2.ambient;
                usleep(70000);
                time++;
            }
            ambientA = ambientA / aantal;
            redA = (redA / aantal) - ambientA;
            greenA = (greenA / aantal) - ambientA;
            blueA = (blueB / aantal) - ambientA;
            if (redA < 0) {
                redA = 0;
            }
            if (greenA < 0) {
                greenA = 0;
            }
            if (blueA < 0) {
                blueA = 0;
            }

            ambientB = ambientB / aantal;
            redB = (redB / aantal) - ambientB;
            greenB = (greenB / aantal) - ambientB;
            blueB = (blueB / aantal) - ambientB;
            if (redB < 0) {
                redB = 0;
            }
            if (greenB < 0) {
                greenB = 0;
            }
            if (blueB < 0) {
                blueB = 0;
            }
            
            vector <int> rgbA = {redA, greenA, blueA};
            vector <int> rgbB = {redB, greenB, blueB};
            BP.reset_all();    // Reset everything so there are no run-away motors
            BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_OFF); // turns off rbg
            BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_OFF);
            return rgbaf(rgbA);
            return rgbaf(rgbB);
        }
    }
}


void kleurscanB(){
    BP.detect();
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_FULL);
    sensor_color_t      Color2;
    while(true){
        if(BP.get_sensor(PORT_2, Color2) == 0){
            int red = 0;
            int green = 0;
            int blue = 0;
            int ambient = 0;
            int time = 0;
            int aantal =0;
            while(time <= 36){
                aantal++;
                red += Color2.reflected_red;
                green += Color2.reflected_green;
                blue += Color2.reflected_blue;
                ambient += Color2.ambient;
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
                blue = 0;
            }
            
            vector <int> rgbB = {red, green, blue};
            BP.reset_all();    // Reset everything so there are no run-away motors
            BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_OFF); // turns off rbg
            return rgbaf(rgbB);
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
    kleurscanA();
    //kleurscanB();//
}

