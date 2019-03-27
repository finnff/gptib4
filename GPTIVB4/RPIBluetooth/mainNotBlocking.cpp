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
        while (true)
        {
            input = mb.readMessage(); //blokkeert niet
            if (input != "")
            {
                cout << input << endl;
            }
            if (input[0] == 'R')
            {
                BP.set_motor_power(PORT_A, -30);
                BP.set_motor_power(PORT_B, 30);
            }
            else if (input[0] == 'L')
            {
                BP.set_motor_power(PORT_A, 30);
                BP.set_motor_power(PORT_B, -30);
            }
            else if (input[0] == 'U')
            {
                BP.set_motor_power(PORT_A, -30);
                BP.set_motor_power(PORT_B, -30);
            }
            else if (input[0] == 'D')
            {
                BP.set_motor_power(PORT_A, 30);
                BP.set_motor_power(PORT_B, 30);
            }
            else if (input[0] = 'F')
            {
                BP.reset_all();
            }

            usleep(200000); // wacht 500 ms
        }

        clientsock->close();
    }
}
