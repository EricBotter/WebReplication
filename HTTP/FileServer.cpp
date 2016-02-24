#include "FileServer.h"

FileServer::FileServer() {
	files.insert({"example.peer", {
			{"/pagina1", "<html><head><title>Pagina1</title></head><body><h1>Pagina1</h1>Pagina1 di prova.</body></html>"},
			{"/", "<html><head><title>Prova</title></head><body><h1>Prova</h1>Prova prova.</body></html>"}
	}});
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
