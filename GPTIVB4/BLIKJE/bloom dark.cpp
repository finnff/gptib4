//For debug purposes testing induvidiual mesured values and deltas.

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

//vector <vector <long int>> blikken {};
vector <string> namen {"Cola", "Nep Sprite", "Witte Monster","Sinas", "Hertog Jan","Monster Zwart","Bavaria"};

vector<vector<long int>> bestand = {
{174, 46, 41, 166, 22, 15, 0}, //(Cola)
{160, 155, 67, 115, 136, 39, 1}, //(nep sprite)
{163, 138, 118, 139, 121, 92, 2}, //(witte monster)
{202, 85, 48, 163, 75, 30,  3}, //(Sinas)
{175, 125, 70, 128, 83, 30,   4}, //(Hertog)
{123, 109, 83, 63, 63, 37, 5}, // Zwarte monster 
{ 73, 89, 87, 51, 68, 70, 6},  // Bavaria 
{ 78, 123, 140, 40, 73, 93,7},  // Pepsi
{128, 177, 91, 94, 128, 62, 8},  // Heniken
{108, 72, 55, 81, 60, 32,  9},  //  Export
{203, 174, 116, 135, 125, 83, 10},  // De Klok
{88, 101, 52, 72, 95, 38, 11}  // grolsch
};

 /// RRRRRRR,BBBBB,GGGGG
void exit_signal_handler(int signo);

void blikje(vector<vector< long int>> yeet){
    int teempo1 = 1000000000;
    int teempo2 = 0;
    for(unsigned int i = 0; i < yeet.size(); i++){
        if (yeet[i][0] < teempo1) {
            //cout << yeet[i][0]<< " --- "<< yeet[i][1]<<endl;
            teempo1 = yeet[i][0];
            teempo2 = yeet[i][1];
        }
    }
    if (teempo1 < 999999){
    cout <<"------------"<< namen[teempo2] << " with error points "<< teempo1 <<"------------"<< endl;
    }
}

void rgbaf(vector<int> rgb, bool check){
    vector <vector <long int>> blikkenA {};
    vector <vector <long int>> blikkenB {};
    vector<long int> tmp2;
    vector<long int> tmp1;
    if(check == true){
        for(unsigned int j = 0; j < bestand.size(); j++){
            vector<long int> tmp2 = {(((abs (rgb[0] - bestand[j][3]))*(abs (rgb[0] - bestand[j][3]))) 
                                    + ((abs (rgb[1] - bestand[j][4]))*(abs (rgb[1] - bestand[j][4]))) 
                                    + ((abs (rgb[2] - bestand[j][5]))*(abs (rgb[2] - bestand[j][5])))),bestand[j][6]};
            blikkenB.push_back(tmp2);
            cout << "ΔRB "<< (abs (rgb[0] - bestand[j][3]))<< " with "<< namen[bestand[j][6]]<<endl;
            cout << "ΔBB "<< (abs (rgb[1] - bestand[j][4]))<< " with "<< namen[bestand[j][6]]<<endl;
            cout << "ΔGB "<< (abs (rgb[2] - bestand[j][5]))<< " with "<< namen[bestand[j][6]]<<endl;
            cout << "sum squared Δ "<< (((abs (rgb[0] - bestand[j][3]))*(abs (rgb[0] - bestand[j][3]))) 
                                    + ((abs (rgb[1] - bestand[j][4]))*(abs (rgb[1] - bestand[j][4]))) 
                                    + ((abs (rgb[2] - bestand[j][5]))*(abs (rgb[2] - bestand[j][5]))))<<endl;
            cout << endl;
        }
    }
    
    else{
        for(unsigned int k = 0; k < bestand.size(); k++){
            vector<long int> tmp1 = {(((abs (rgb[0] - bestand[k][0]))*(abs (rgb[0] - bestand[k][0]))) 
                                    + ((abs (rgb[1] - bestand[k][1]))*(abs (rgb[1] - bestand[k][1]))) 
                                    + ((abs (rgb[2] - bestand[k][2]))*(abs (rgb[2] - bestand[k][2])))),bestand[k][6]};
            blikkenA.push_back(tmp1);
            cout << "ΔRA "<< (abs (rgb[0] - bestand[k][0]))<<" with "<< namen[bestand[k][6]]<<endl;
            cout << "ΔBA "<< (abs (rgb[1] - bestand[k][1]))<<" with "<< namen[bestand[k][6]]<<endl;
            cout << "ΔGA "<< (abs (rgb[2] - bestand[k][2]))<<" with "<< namen[bestand[k][6]]<<endl;
            cout << "sum squared Δ "<<(((abs (rgb[0] - bestand[k][0]))*(abs (rgb[0] - bestand[k][0]))) 
                                    + ((abs (rgb[1] - bestand[k][1]))*(abs (rgb[1] - bestand[k][1]))) 
                                    + ((abs (rgb[2] - bestand[k][2]))*(abs (rgb[2] - bestand[k][2]))))<<endl;
            cout << endl;
        }
    }
    blikje(blikkenA);
    blikje(blikkenB);
}


void kleurscanA(){
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
            int ambientA = 0;
            int redB = 0;
            int greenB = 0;
            int blueB = 0;
            int ambientB = 0;
            int time = 0;
            int aantal =0;
            cout<< "Scanning: " << endl;
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
            blueA = (blueA / aantal) - ambientA;
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
                rgbaf(rgbA, false);
                return rgbaf(rgbB, true);
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
}

