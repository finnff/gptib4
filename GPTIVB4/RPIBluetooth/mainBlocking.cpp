#include "BluetoothSocket.h"
using namespace std;

int main() {
	BluetoothServerSocket serversock(2, 1);  //2 is het channel-number
	cout << "listening" << endl;
	while(true) {
		BluetoothSocket* clientsock = serversock.accept();
		cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
		iostream& sockstream = clientsock->getStream();
		try {
			
			
			string input;
			while(true) {
				sockstream >> input;  //blokkeert tot er iets te lezen valt
				cout << "recieved: " << input << endl;
			}
			
		}
		catch(BluetoothException& be) {
			cout << "connection lost" << endl;
		}
		clientsock->close();
			
	}
}
		
		
			