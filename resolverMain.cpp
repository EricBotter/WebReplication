#include <iostream>
#include <sys/unistd.h>
#include "TCP/Connection.h"
#include "HTTP/HttpResponse.h"

using namespace std;

int main() {
	Connection c("127.0.0.1", 8000);

	//cout << c.receive("\n") << endl;

	c.sendStr("GET http://connelplaza.altervista.org/ HTTP/1.1\r\nHost: connelplaza.altervista.org\r\nConnection: close\r\n\r\n");

	HttpResponse response(c);
	cout << response.compile() << endl;
	sleep(60);

	return 0;
}