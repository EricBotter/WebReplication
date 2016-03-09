#include <fstream>
#include <cassert>
#include "../TCP/Connection.h"
#include "../HTTP/HttpResponse.h"

using namespace std;

const string websitePath = "/home/websites/";

int main() {
	ifstream fin;
	Connection* connection;
	HttpResponse* response;
	string* pageContent;

	fin.open(websitePath + "example.peer/pagina1", ios::binary);
	pageContent = new string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
	fin.close();
	connection = new Connection("127.0.0.1", 8000);
	connection->sendStr("GET http://example.peer/pagina1 HTTP/1.0\r\nHost: example.peer\r\nConnection: keep-alive\r\n\r\n");
	response = new HttpResponse(*connection);
	assert(pageContent->compare(0, pageContent->length(), response->content, response->contentLength) == 0);
	delete response;
	delete pageContent;

	fin.open(websitePath + "example.peer/index", ios::binary);
	pageContent = new string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
	fin.close();
	connection->sendStr("GET http://example.peer/ HTTP/1.0\r\nHost: example.peer\r\nConnection: keep-alive\r\n\r\n");
	response = new HttpResponse(*connection);
	assert(pageContent->compare(0, pageContent->length(), response->content, response->contentLength) == 0);
	delete response;
	delete pageContent;

	fin.open(websitePath + "mywebsite.peer/d.jpg", ios::binary);
	pageContent = new string(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
	fin.close();
	connection->sendStr("GET http://mywebsite.peer/d.jpg HTTP/1.0\r\nHost: mywebsite.peer\r\nConnection: close\r\n\r\n");
	response = new HttpResponse(*connection);
	assert(pageContent->compare(0, pageContent->length(), response->content, response->contentLength) == 0);
	delete response;
	delete pageContent;

	delete connection;

	return 0;
}