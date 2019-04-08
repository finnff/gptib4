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
vector<vector<int>> superA;
vector<vector<int>> superB;


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
            superA.push_back(rgbA);
            superB.push_back(rgbB);
            break;
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
    int totera=0;
    int totega=0;
    int toteba=0;
    int toterb=0;
    int totegb=0;
    int totebb=0;
    

    signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
    for(int i = 0; i< 10;i++){
        cout << i << endl;
        kleurscanA();
    }

    cout<< "Rgb A waardes: "<< endl;
    for(size_t j=0;j<superA.size();j++){
        for(size_t k =0; k<superA[j].size();k++){
            cout << superA[j][k]<<","<<flush;
        }

        totera=totera + superA[j][0];
        totega=totega + superA[j][1];
        toteba=toteba + superA[j][2];
        cout<<endl;
    }

    cout<< "Rgb B waardes: "<< endl;

    for(size_t j=0;j<superB.size();j++){
        for(size_t k =0;k<superB[j].size();k++){
            cout << superB[j][k]<<","<<flush;
        }
        toterb=toterb + superB[j][0];
        totegb=totegb + superB[j][1];
        totebb=totebb + superB[j][2];
        cout<<endl;
    }
    cout << (totera/10)<<", "<<(totega/10)<<", "<<(toteba/10)<<", "<< (toterb/10)<<", "<<(totegb/10)<<", "<<(totebb/10)<<", "<<endl;
}
