#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include "FileServer.h"
#include "../Utilities/Log.h"

//#define ROGUE_SERVER

const char* webpath = "/var/webr/websites/";
const char* sigpath = "/var/webr/signatures/";
const map<string, string> mimetypes = {
		{".html", "text/html"},
		{".htm",  "text/html"},
		{"",      "text/html"},
		{".js",   "application/javascript"},
		{".css",  "text/css"},
		{".png",  "image/png"},
		{".jpg",  "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".ico",  "image/x-icon"},
};

FileServer::FileServer() {
}

#ifdef ROGUE_SERVER
int ai = 0;
string FileServer::getFile(string site, string file) {
	if (++ai > 3) { //non atomic, but we don't really care
		ai = 0;
		Log::e("### Sending arbitrary data ###");
		return string(100, 'a');
	}
#else
string FileServer::getFile(string site, string file) {
#endif
	string filepath = webpath + site + file;
	struct stat s;
	if (stat(filepath.c_str(), &s) == 0 && S_ISREG(s.st_mode)) {
		ifstream filein(filepath, ios::binary);
		return string((istreambuf_iterator<char>(filein)),
					  istreambuf_iterator<char>());
	}
	return "";
}

string FileServer::getSignature(string site, string file) {
	string filepath = sigpath + site + file;
	struct stat s;
	if (stat(filepath.c_str(), &s) == 0 && S_ISREG(s.st_mode)) {
		ifstream filein(filepath, ios::binary);
		return string((istreambuf_iterator<char>(filein)),
					  istreambuf_iterator<char>());
	}
	return "";
}

vector<string> FileServer::getSiteList() {
	vector<string> out;
	DIR* dp;
	struct dirent* ep;
	struct stat s;

	dp = opendir(webpath);
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			string fullpath = string(webpath) + ep->d_name;
			if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
				continue;
			if (stat(fullpath.c_str(), &s) == 0 && s.st_mode & S_IFDIR) {
				out.push_back(ep->d_name);
			}
		}
		closedir(dp);
	}
	return out;
}

string FileServer::mimeFromFilename(string filename) {
	size_t index = filename.find_last_of('.');
	if (index == string::npos) {
		return mimetypes.at("");
	}
	if (mimetypes.find(filename.substr(index)) == mimetypes.end()) {
		return "text/plain";
	}
	return mimetypes.at(filename.substr(index));
}
