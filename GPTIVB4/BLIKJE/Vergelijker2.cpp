#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <vector>

using namespace std;
BrickPi3 BP;


vector <vector <int>> blikken {};


vector<vector<int>> bestand = {
    {//r,//g//b//ID}
    {180,90,70,879872}, // COLA
    {114,157,60,117237} // Sprite?
};

 /// RRRRRRR,BBBBB,GGGGG
void exit_signal_handler(int signo);

int vectorcheck(vector<vector<int>> blik){
    for(unsigned int i = 0; i < blik[0].size(); i++){ // amount of  R matches
        for(unsigned int j = 0; j < blik[1].size(); j++){ // amount of G  matches
            for(unsigned int k = 0; k < blik[2].size(); k++){ // amount of B matches
            if(blik[0].size()>0){
                cout << "MATCHER"<<endl;
                if ((blik[0][i] == blik[1][j]) && (blik[0][i] == blik[2][k])) { //is id R == B and ID R ==G
                    return blik[0][i]; 
                } 
                if ((blik[1][i] == blik[0][j]) && (blik[0][i] == blik[2][k])) { //is id R == B and ID R ==G
                    return blik[0][i]; 
                } 
                if ((blik[2][i] == blik[0][j]) && (blik[0][i] == blik[1][k])) { //is id R == B and ID R ==G
                    return blik[0][i]; 
                } 
                else{
                    cout<<"No match found"<<endl;
                }
            }
            else{
                cout<<"No match found"<<endl;
            }
            }
        }
    }
}
int bestandcheck(vector<int> rgb){
    vector <int> tmpblikken {};
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_OFF);
        for(unsigned int k = 0; k < bestand.size(); k++){
    for(unsigned int i = 0; i < 3; i++){
                cout << rgb[i]<< " > " <<(bestand[k][i]*0.7);
                cout << endl;
                cout << rgb[i] <<" < " << (bestand[k][i]*1.3);
                cout << endl;
            if (rgb[i] > (bestand[k][i]*0.7) && rgb[i] < (bestand[k][i]*1.3)){
                    tmpblikken.push_back(bestand[k][3]);
            }
            else{
                 tmpblikken.push_back(404); // if no match error code
            }
        blikken.push_back(tmpblikken);
        }
    }
    return vectorcheck(blikken);
}
int kleurscan(){
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    sensor_color_t      Color1;
    while(true){
        if(BP.get_sensor(PORT_1, Color1) == 0){
            BP.set_motor_power(PORT_B, -20); //rotation = ~2.5 sec
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
                blue = 0;
            }
            
            vector <int> rgb = {red, green, blue};
            BP.reset_all();    // Reset everything so there are no run-away motors
            BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_OFF); // turns off rbg
            return bestandcheck(rgb);
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
    cout << kleurscan()<<endl;
}

