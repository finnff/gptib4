#include "BluetoothSocket.h"
#include <unistd.h>
#include "BrickPi3.h" // for BrickPi3
#include <iostream>
#include <signal.h>
#include <string>
using namespace std;

BrickPi3 BP;

void TurnL90()
{
    BP.set_motor_dps(PORT_A, 275);
    BP.set_motor_dps(PORT_B, -275);
    sleep(1);
    BP.reset_all();
}

void TurnR90()
{
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
                cout << input << endl;
            }
            if (input[0] == 'R')
            {
                BP.set_motor_power(PORT_A, -20);
                BP.set_motor_power(PORT_B, 20);
            }
            else if (input[0] == 'L')
            {
                BP.set_motor_power(PORT_A, 20);
                BP.set_motor_power(PORT_B, -20);
            }
            else if (input[0] == 'U')
            {
                BP.set_motor_power(PORT_A, -20);
                BP.set_motor_power(PORT_B, -20);
            }
            else if (input[0] == 'D')
            {
                BP.set_motor_power(PORT_A, 20);
                BP.set_motor_power(PORT_B, 20);
            }
            else if (input[0] = 'F')
            {
                BP.reset_all();
            }
            else if (input[0] = 'A')
            {
                BP.set_motor_power(PORT_A, -100);
                BP.set_motor_power(PORT_B, -100);
            }
            else if (input[0] = 'B')
            {
                BP.set_motor_power(PORT_A, 100);
                BP.set_motor_power(PORT_B, 100);
            }
            else if (input[0] = 'C')
            {
                BP.set_motor_power(PORT_A, -100);
                BP.set_motor_power(PORT_B, 100);
            }

            //doe andere dingen.
            cout << ".";
            cout.flush();
            usleep(500000); // wacht 500 ms
        }

        clientsock->close();
    }
}
