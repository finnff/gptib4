#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(){
    string tmp1;
    cout << "kies een bestand" << flush;
    cin >> tmp1;
    string call = "omxplayer /home/pi/gptib4/GPTIVB4/BLIKJE/audio/";
    tmp1 = tmp1 + ".ogg";
    string newcall = call + tmp1;
    cout << newcall << endl;
    system(newcall.c_str());
}