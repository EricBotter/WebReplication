#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include "Utilities/FileVerifier.h"
#include "Utilities/Log.h"

const char* filepath = "/var/webr/websites/";
const char* sigpath = "/var/webr/signatures/";

int main() {
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

			while ((objectent = readdir(sitedir)) != NULL) {
				if (objectent->d_type == DT_DIR)
					continue;

				string temp = curdir + "/";
				temp.append(objectent->d_name);
				fin.open(temp.c_str(), ios::binary);

				long filesize;
				if (stat(temp.c_str(), &st) == 0) {
					FileVerifier::sign(temp.c_str(), string(sigpath) + siteent->d_name + "/");
				} else {
					Log::f(string("Errore nella firma del file ") + objectent->d_name + ": " + strerror(errno));
				}
				fin.close();
			}
		}
		closedir(dir);
	}

}
