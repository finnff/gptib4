// Prodcution version with sensor combination code.
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
vector <string> namen {"Cola", "Nep Sprite", "Witte Monster","Sinas", "Hertog Jan","MonsterZwart","Bavaria","Pepsi", "Cola(Donker)","Sprite(Donker)"};

vector <int> monoresults = {};
vector <int> monodelta = {};
int IsRescan = 0;

vector<vector<long int>> bestand = {
{174, 75, 66, 189, 58, 43, 0}, //(Cola)
{257, 234, 120, 150, 171, 56, 1}, //(nep sprite)
{1116,1213,1014,1116,1213,1104,2}, //(witte monster)
{1171,1136,1119,1171,1136,1119,3}, //(Sinas)
{315, 180, 89, 161, 92, 39, 4}, //(Hertog)
{111,116,811,815,811,111,5}, // Zwarte monster 
{157, 155, 166, 83, 107, 103, 6},  // Bavaria 
{156, 207, 213, 100, 140, 149,7}  // Pepsi
//{483, 266, 261, 325, 112, 85,8}, //Cola Donker!
//{400, 434, 282, 255, 275, 88,9} //Sprite Donker
};

void exit_signal_handler(int signo);
void kleurscanA();


void monocompare(vector<int> & id, vector<int> & delta2){
    if((id[id.size()-1]) == (id[id.size()-2])){
        cout << "Bi-Sensor Conformation: "<<endl;
        cout << namen[id[id.size()-1]]<<endl;
        if((delta2[delta2.size()-1])> 500 || (delta2[delta2.size()-2])>500){
            cout << "However with High Error! ("<<delta2[delta2.size()-1]<<" / " << delta2[delta2.size()-2]<<")"<<endl;
        }
    }
    else{
        if(IsRescan <4){
            IsRescan++;
            cout<< "Sensor Mismatch! Rescanning!"<<endl;
            cout<<"Rescan #: "<< IsRescan << endl;
            kleurscanA();
            monocompare(monoresults,monodelta);   
        } 
        else{
            cout << "Rescans Failed."<<endl;
        }
    }
}

void blikje(vector<vector< long int>> yeet){
    int teempo1 = 1000000000;
    int teempo2 = 0;
    for(unsigned int i = 0; i < yeet.size(); i++){
        if (yeet[i][0] < teempo1) {
            teempo1 = yeet[i][0];
            teempo2 = yeet[i][1];
        }
    }
    if (teempo1 < 999999){
        cout <<"------------"<< namen[teempo2] << " with error points "<< teempo1 <<"------------"<< endl;
        monoresults.push_back(teempo2);
        monodelta.push_back(teempo1);
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
        }
    }
    
    else{
        for(unsigned int k = 0; k < bestand.size(); k++){
            vector<long int> tmp1 = {(((abs (rgb[0] - bestand[k][0]))*(abs (rgb[0] - bestand[k][0]))) 
                                    + ((abs (rgb[1] - bestand[k][1]))*(abs (rgb[1] - bestand[k][1]))) 
                                    + ((abs (rgb[2] - bestand[k][2]))*(abs (rgb[2] - bestand[k][2])))),bestand[k][6]};
            blikkenA.push_back(tmp1);
            
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

// void startup(){
//     if (ulrasoon < 10 cm )
//     kleurscanA();
//     monocompare(monoresults, monodelta);
// }

int main(){
    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    kleurscanA();
    monocompare(monoresults, monodelta);
}

