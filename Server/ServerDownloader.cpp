#include <sys/stat.h>
#include <climits>
#include <cstring>
#include "ServerDownloader.h"
#include "../Utilities/WebrdReader.h"

const string webpath = "/var/webr/websites/";
const string sigpath = "/var/webr/signatures/";

//source: http://nion.modprobe.de/blog/archives/357-Recursive-directory-creation.html
static void _mkdir(const char *dir) {
	char tmp[PATH_MAX];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);
	if(tmp[len - 1] == '/')
		tmp[len - 1] = 0;
	for(p = tmp + 1; *p; p++)
		if(*p == '/') {
			*p = 0;
			mkdir(tmp, S_IRWXU);
			*p = '/';
		}
	mkdir(tmp, S_IRWXU);
}

ServerDownloader::ServerDownloader() {
	downloaderThread = new thread(&ServerDownloader::threadFunction, this);
}

ServerDownloader::~ServerDownloader() {
	wd.enqueueRequest(nullptr);
	requestQueue.push(nullptr);
	if (downloaderThread->joinable()) {
		downloaderThread->join();
	}
	delete downloaderThread;
}

bool ServerDownloader::enqueueWebsite(const string& website) {
	HttpRequest hr;
	hr.version = "HTTP/1.0";
	hr.method = "GET";
	hr.url = "/.webrd";
	hr.headers = {{"Connection", "keep-alive"}, {"Host", website}};

	shared_ptr<VerifiedObjectRequest> temp = make_shared<VerifiedObjectRequest>(hr);
	wd.enqueueRequest(temp);
	requestQueue.push(temp);
	return true;
}

void ServerDownloader::threadFunction() {
	shared_ptr<VerifiedObjectRequest> request;
	while ((request = requestQueue.pop())) {
		request->waitForVerification();
		if (!request->canBeVerified() || (request->canBeVerified() && request->isVerified())) {
			if (request->getObjectUrl() == "/.webrd") {
				WebrdReader wr;
				wr.readFromString(string(request->getObject()->getHttpResponse().content, request->getObject()->getHttpResponse().contentLength));
				// queueing files request
				if (wr.website == request->getWebsite()) {
					for (const string& file : wr.files) {
						HttpRequest hr;
						hr.version = "HTTP/1.0";
						hr.method = "GET";
						hr.url = file;
						hr.headers = {{"Connection", "keep-alive"}, {"Host", wr.website}};
						shared_ptr<VerifiedObjectRequest> temp = make_shared<VerifiedObjectRequest>(hr);
						wd.enqueueRequest(temp);
						requestQueue.push(temp);
					}
				}
			}
			string folder = webpath + request->getWebsite() + request->getObjectUrl().substr(0, request->getObjectUrl().find_last_of("/"));
			_mkdir(folder.c_str());

			FILE* object = fopen((webpath + request->getWebsite() + request->getObjectUrl()).c_str(), "wb");
			fwrite(request->getObject()->getHttpResponse().content, request->getObject()->getHttpResponse().contentLength, 1, object);
			fclose(object);

			FILE* signature = fopen((sigpath + request->getWebsite() + request->getObjectUrl()).c_str(), "wb");
			fwrite(request->getSignature()->getHttpResponse().content, request->getSignature()->getHttpResponse().contentLength, 1, signature);
			fclose(signature);
		}
	}
}

