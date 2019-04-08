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

int run = 0;
vector <vector <int>> blikken {};
vector <string> namen {"Cola", "Sprite", "Witte Monster","Sinas", "Hertog Jan","Monster Zwart","Bavaria"};
vector <int> zwartwit {};

vector<vector<int>> bestand = {
{69,20,15,0}, //(Cola)
{54,73,26,1}, //(sprite)
{55,56,46,2}, //(witte monster)
{71,36,19,3}, //(Sinas)
{70,40,22,4}, //(Hertog)
{34,37,26,5}, // Zwarte monster 
{34,46,46,6}  // Bavaria 
};

 /// RRRRRRR,BBBBB,GGGGG
void exit_signal_handler(int signo);
void kleurscan();

void blikje(vector<vector<int>> blikken){
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
}
void rgbaf(vector<int> rgb){
    for(unsigned int j = 0; j < bestand.size(); j++){
        vector<int> tmp1 = {(abs (rgb[0] - bestand[j][0])) + (abs (rgb[1] - bestand[j][1])) + (abs (rgb[2] - bestand[j][2])),bestand[j][3]};
        blikken.push_back(tmp1);
    }
    return blikje(blikken);
}

void kleurscan(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    sensor_color_t      Color1;
    while(true){
        if(BP.get_sensor(PORT_1, Color1) == 0){
            BP.set_motor_power(PORT_B, -20); //rotation = ~2.5 sec
            BP.set_motor_dps(PORT_C, 180);
            while(time <= 36){
                aantal++;
                int red += Color1.reflected_red;
                int green += Color1.reflected_green;
                int blue += Color1.reflected_blue;
                usleep(70000);
                time++;
            }
            red = (red / aantal);
            green = (green / aantal);
            blue = (blue / aantal);}
            vector <int> rgb = {red, green, blue};
            BP.reset_all();    // Reset everything so there are no run-away motors
            BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_OFF); // turns off rbg
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
    vector <int> zwart = kleurscan();
    sleep(5);
    vector <int> wit = kleurscan();
    sleep(10);
    vector <int> scan = kleurscan();
    vector <int> gemscan = {};
    for(unsigned int i = 0; i < 2; i++)
    {
        gemscan.push_back(((scan[i]-zwart[i])*100)/(wit[i]-zwart[i]));
        cout << ((scan[i]-zwart[i])*100)/(wit[i]-zwart[i]) << endl;
    }
    // rgbaf(gemscan);
}

