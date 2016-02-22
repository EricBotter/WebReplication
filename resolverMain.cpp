#include <iostream>
#include <sys/unistd.h>
#include "TCP/Connection.h"

using namespace std;

int main() {
	Connection c("127.0.0.1", 8000);

	cout << c.receive("\n") << endl;

	c.sendStr("ciao bello\n");

	cout << c.receive("\n") << endl;

	c.sendStr("1234567890");

	sleep(1000);

	return 0;
}