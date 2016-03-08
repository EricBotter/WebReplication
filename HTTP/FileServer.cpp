#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <sys/errno.h>
#include "FileServer.h"
#include "../Utilities/Log.h"

const char* filepath = "/home/websites/";
const map<string, string> mimetypes = {
		{".html", "text/html"},
		{".htm", "text/html"},
		{"", "text/html"},
		{".js", "application/javascript"},
		{".css", "text/css"},
		{".png", "image/png"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".ico", "image/x-icon"},
};

FileServer::FileServer() {
}

void FileServer::init() {
	DIR* dir;
	DIR* sitedir;
	ifstream fin;
	dirent* siteent;
	dirent* objectent;
	struct stat st;

	//Note: Only checks files in the first level for each site
	if ((dir = opendir(filepath)) != NULL) {
		while ((siteent = readdir(dir)) != NULL) {
			if (siteent->d_type != DT_DIR || strcmp(siteent->d_name, ".") == 0 || strcmp(siteent->d_name, "..") == 0)
				continue;

			string curdir = filepath;
			curdir += siteent->d_name;

			sitedir = opendir(curdir.c_str());
			files.insert({siteent->d_name,{}});
			while ((objectent = readdir(sitedir)) != NULL) {
				if (objectent->d_type == DT_DIR)
					continue;

				string temp = curdir + "/";
				temp.append(objectent->d_name);
				fin.open(temp.c_str(), ios::binary);

				long filesize;
				if (stat(temp.c_str(), &st) == 0) {
					filesize = st.st_size;
					temp.resize((size_t)filesize);
					fin.read(&temp[0], filesize);

					files[siteent->d_name].insert({{(string)"/" + objectent->d_name, temp}});
				} else {
					Log::f(string("Errore nell'inserimento del file ")+objectent->d_name+": "+strerror(errno));
				}
				fin.close();
			}
		}
		closedir(dir);
	}
}

string FileServer::getFile(string site, string file) {
	if (files.find(site) != files.end()) {
		return files[site][file];
	}
	return "";
}

vector<string> FileServer::getSiteList() {
	vector<string> out;
	for (auto it = files.begin(); it != files.end(); it++) {
		out.push_back(it->first);
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
