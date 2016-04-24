#include <cstdio>
#include <sys/stat.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/pem.h>

#include "FileVerifier.h"

const string keypath = "/var/webr/keys/";

//deprecated, since we use system we might as well use a bash script
void FileVerifier::sign(string filepath, string signaturepath) {
	mkdir(signaturepath.c_str(), 0755);
	string website = filepath.substr(filepath.substr(0, filepath.find_last_of('/')).find_last_of('/') + 1);
	string filename = filepath.substr(0, filepath.find_last_of('/') + 1);
	string outfile = signaturepath + filename + ".sig";

	string command = "openssl dgst -sha256 -sign " + keypath + website + "/private.pem -out "
					 + outfile + ' ' + filepath;
	system(command.c_str());
}

bool FileVerifier::verify(const string& website, const string& fileContent, const string& signature) {
	if (!canVerify(website))
		return false;

	FILE* keyfile = fopen((keypath + website + "/public.pem").c_str(), "r");
	EVP_PKEY* pkey = NULL;
	pkey = PEM_read_PUBKEY(keyfile, NULL, NULL, NULL);

	bool result = false;

	EVP_MD_CTX* ctx = NULL;

	do
	{
		ctx = EVP_MD_CTX_create();
		if(ctx == NULL) {
			printf("EVP_MD_CTX_create failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		const EVP_MD* md = EVP_get_digestbyname("SHA256");
		if(md == NULL) {
			printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		int rc = EVP_DigestInit_ex(ctx, md, NULL);
		if(rc != 1) {
			printf("EVP_DigestInit_ex failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		rc = EVP_DigestVerifyInit(ctx, NULL, md, NULL, pkey);
		if(rc != 1) {
			printf("EVP_DigestVerifyInit failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		rc = EVP_DigestVerifyUpdate(ctx, fileContent.c_str(), fileContent.length());
		if(rc != 1) {
			printf("EVP_DigestVerifyUpdate failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		/* Clear any errors for the call below */
		ERR_clear_error();

		rc = EVP_DigestVerifyFinal(ctx, (const unsigned char*)signature.c_str(), signature.length());
		if(rc != 1) {
			printf("EVP_DigestVerifyFinal failed, error 0x%lx\n", ERR_get_error());
			break;
		}

		result = true;

	} while(0);

	if(ctx) {
		EVP_MD_CTX_destroy(ctx);
	}
	if (pkey) {
		OPENSSL_free(pkey);
	}

	return result;
}

bool FileVerifier::canVerify(string website) {
	return false;
}
