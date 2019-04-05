#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

void read_database(){
    ofstream ifile;
    ifile.open("blikken.txt");
    int i = 0;
    if (ifile.is_open()) {
        while (i < ifile.size()) {
            getline(ifile, line)
            cout << line;
            i++;
        } else {
            cout << "Kan bestand niet openen. \n";
        }
    }
}