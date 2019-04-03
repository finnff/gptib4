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


vector <int> kleurscan()
{
    signal(SIGINT, exit_signal_handler)
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLLOR_FULL);
    int red = 0;
    int green = 0;
    int blue = 0;
    int ambient = 0;
    while(time <= 36){
        int aantal++;
        red += Color1.reflected_red;
        green += Color1.reflected_green;
        blue += Color1.reflected_blue;
        ambient += Color1.ambient;
        usleep(70000);
        int time++;
    }
    int red = red / aantal;
    int green = green / aantal;
    int blue = blue / aantal;
    int ambient = ambient / aantal;
    vector <int> rgb = {red, green, blue, ambient}
    return rgb;
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

int main()
{
    cout << kleurscan() << endl;
}


void exit_signal_handler(int signo){
    if(signo == SIGINT){
        BP.reset_all();
        exit(-2);
    }
}
