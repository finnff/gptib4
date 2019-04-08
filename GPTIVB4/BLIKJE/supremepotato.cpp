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

vector <vector <int>> blikken {};
vector <string> namen {"Cola", "Sprite", "Witte Monster","Sinas", "Hertog Jan","Monster Zwart","Bavaria"};
vector <int> zwart {743, 746, 733};
vector <int> wit {383, 385, 387};

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
vector <int> kleurscan();


void blikje(vector<vector< long int>> yeet){
    int teempo1 = 1000000;
    int teempo2 = 0;
    for(unsigned int i = 0; i < yeet.size(); i++){
        if (yeet[i][0] < teempo1) {
            teempo1 = yeet[i][0];
            teempo2 = yeet[i][1];
        }
    }
    if (teempo1 < 999999){
    cout << namen[teempo2] << " with error points "<< teempo1 << endl;
    }

}
void rgbaf(vector<int> rgb){
    vector <long int> tmp1 {};
    vector <vector <long int>> blikken {};
    for(unsigned int j = 0; j < bestand.size(); j++){
        vector<long int> tmp1 = {(((abs (rgb[0] - bestand[j][0]))*(abs (rgb[0] - bestand[j][0]))) 
                                + ((abs (rgb[1] - bestand[j][1]))*(abs (rgb[1] - bestand[j][1]))) 
                                + ((abs (rgb[2] - bestand[j][2]))*(abs (rgb[2] - bestand[j][2])))
                                + ((abs (rgb[3] - bestand[j][3]))*(abs (rgb[3] - bestand[j][3])))
                                + ((abs (rgb[4] - bestand[j][4]))*(abs (rgb[4] - bestand[j][4])))
                                + ((abs (rgb[5] - bestand[j][5]))*(abs (rgb[5] - bestand[j][5])))),bestand[j][3]};
        
    }
    blikken.push_back(tmp1);
    blikje (blikken);
}

vector <int> kleurscan(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_FULL);
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
            int redB = 0;
            int greenB = 0;
            int blueB = 0;
            int time = 0;
            int aantal =0;
            while(time <= 36){
                aantal++;
                redA += Color1.reflected_red;
                greenA += Color1.reflected_green;
                blueA += Color1.reflected_blue;
                redB += Color2.reflected_red;
                greenB += Color2.reflected_green;
                blueB += Color2.reflected_blue;
                usleep(70000);
                time++;
            }
            redA = (redA / aantal);
            greenA = (greenA / aantal);
            blueA = (blueB / aantal);

            redB = (redB / aantal);
            greenB = (greenB / aantal);
            blueB = (blueB / aantal);

            vector <int> rgb = {redA, greenA, blueA, redB, greenB, blueB};
            BP.reset_all();    // Reset everything so there are no run-away motors
            BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_OFF); // turns off rbg
            BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_COLOR_OFF);
            return (rgb);
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
    char input;
    cout << "zwart-wit scan: z" << endl;
    cout << "blik scan: b" << endl;
    cout << "kies een soort scan: " << flush;
    cin >> input;
    if (input = 'z') {
        zwart = kleurscan();
        for(unsigned int i = 0; i < 6; i++){
            cout << "zwart: " << zwart[i] << endl;
        }
        sleep(5);
        wit = kleurscan();
        for(unsigned int i = 0; i < 6; i++){
            cout << "wit: " << wit[i] << endl;
        }

    }
    if (input = 'b') {
        vector <int> scan = kleurscan();
        vector <int> gemscan = {};
        for(unsigned int i = 0; i < 6; i++){
            gemscan.push_back(((scan[i]-zwart[i])*100)/(wit[i]-zwart[i]));
            cout << ((scan[i]-zwart[i])*100)/(wit[i]-zwart[i]) << endl;
        }
    }
    // rgbaf(gemscan);
    
}