#ifndef THESIS_FILESERVER_H
#define THESIS_FILESERVER_H

#include <map>
#include <vector>

using namespace std;

class FileServer {
public:
	FileServer();
	string getFile(string, string);
	string getSignature(string, string);
	vector<string> getSiteList();
	static string mimeFromFilename(string extension);
};


#endif //THESIS_FILESERVER_H
