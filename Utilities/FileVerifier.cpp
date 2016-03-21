#include <fstream>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include "FileVerifier.h"

const string keypath = "/var/webr/keys/";

void FileVerifier::sign(string filepath, string signaturepath) {
	mkdir(signaturepath.c_str(), 0755);
	char* tempfile = new char[L_tmpnam];
	tempfile = tmpnam(tempfile);
	string website = basename(filepath.substr(0, filepath.find_last_of('/')).c_str());
	string filename = basename(filepath.c_str());
	string outfile = signaturepath + filename + ".sig";

	string command = "openssl dgst -sha256 -sign " + keypath + website + "/private.pem -out "
					 + outfile + ' ' + filepath;
	system(command.c_str());
}

bool FileVerifier::verify(string filepath, string signaturepath) {
	char* tempfile = new char[L_tmpnam];
	tempfile = tmpnam(tempfile);
	string website = basename(filepath.substr(0, filepath.find_last_of('/')).c_str());
	string filename = basename(filepath.c_str());
	string sigfile = signaturepath + filename + ".sig";

	string command = "openssl dgst -sha256 -verify " + keypath + website + "/public.pem -signature "
					 + sigfile + ' ' + filepath + " > " + tempfile;
	system(command.c_str());

	ifstream temp(tempfile);
	string result = string(istreambuf_iterator<char>(temp), istreambuf_iterator<char>());

	return result.find("OK") != result.npos;
}
