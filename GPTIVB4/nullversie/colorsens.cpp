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
	cout<<"test1"<<endl;
    BP.detect();
	cout<<"testc"<<endl;
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_FULL);
	cout<<"testb"<<endl;
    sensor_color_t      Color1;
	cout<<"testa"<<endl;
while(true){
    if(BP.get_sensor(PORT_1, Color1) == 0){
	cout<<"aysdayusdas"<<endl;
        signal(SIGINT, exit_signal_handler);
        int red = 0;
        int green = 0;
        int blue = 0;
        int ambient = 0;
        int time = 0;
        int aantal =0;
	cout<<"test2"<<endl;
        while(time <= 36){
		cout << "while test" <<endl;
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
        cout << red << green << blue << ambient << endl;
        //vector <int> rgb = {red, green, blue, ambient};
        vector <int> rgb = {};
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
kleurscan();
//    vector<int> printvec = kleurscan();
//    for(int i=0; i<printvec.size(); i++){
//        cout << printvec[i]<< endl;
//    }
}
