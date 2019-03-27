#include "BluetoothSocket.h"
#include <unistd.h>
#include "BrickPi3.h" // for BrickPi3
#include <iostream>
#include <signal.h>
#include <string>
using namespace std;

BrickPi3 BP;

void TurnL90(){
    BP.set_motor_dps(PORT_A, 275);
    BP.set_motor_dps(PORT_B, -275);
    sleep(1);
    BP.reset_all();
}

void TurnR90(){
    BP.set_motor_dps(PORT_A, -275);
    BP.set_motor_dps(PORT_B, 275);
    sleep(1);
    BP.reset_all();
}


int main()
{
    BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));

    BluetoothServerSocket serversock(2, 1); //2 is het channel-number
    cout << "listening" << endl;
    while (true)
    {
        BluetoothSocket *clientsock = serversock.accept();
        cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
        MessageBox &mb = clientsock->getMessageBox();

        string input;
        while (mb.isRunning())
        {
            input = mb.readMessage(); //blokkeert niet
            if (input != "")
            {
                cout << input << input.size() << flush;
                for(int  i = 0; i < input.size(); i++)
                {
                    cout << (int) input[i] << endl;
                }
                
            }
            if (input[0] == 'R')
            {
                TurnR90();
                cout << input << endl;
            }
            else if (input[0] == 'L')
            {
                TurnL90();
                cout << input << endl;
            }
            else if (input == "UP")
            {
                BP.set_motor_power(PORT_A, -20);
                BP.set_motor_power(PORT_B, -20);
                cout << input << endl;
            }
            else if (input == "DOWN")
            {
                BP.set_motor_power(PORT_A, 20);
                BP.set_motor_power(PORT_B, 20);
                cout << input << endl;
            }


            //doe andere dingen.
            cout << ".";
            cout.flush();
            usleep(500000); // wacht 500 ms
        }

        clientsock->close();
    }
}

void exit_signal_handler(int signo)
{
    if (signo == SIGINT)
    {
        BP.reset_all(); // Reset everything so there are no run-away motors
        exit(-2);
    }
}