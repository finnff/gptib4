#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

void add_database(string brand, vector<float> average) {
    ofstream ifile;
    ifile.open("blikken.txt");
    vector<string> brands;
    if (ifile.is_open()) {
        int i = 0;
        while (true) {
            average.push_back(i);
            ifile << average;
            ifile << "\n";
            i++;
        }
    } else {
        cout << "Kan bestand niet openen. \n";
    }
}