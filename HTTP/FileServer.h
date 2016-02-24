#ifndef THESIS_FILESERVER_H
#define THESIS_FILESERVER_H

#include <map>
#include <vector>

using namespace std;

class FileServer {
private:
	map<string, map<string, string>> files;
public:
	FileServer();
	string getFile(string, string);
	vector<string> getSiteList();
};


#endif //THESIS_FILESERVER_H