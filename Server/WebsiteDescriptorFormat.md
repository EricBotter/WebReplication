Website Descriptor Format (v0.1)
================================

Every website in Web.R must have a file which describes the file available for the website.
This file is needed by the servers to download the single files within websites.

Example descriptor
------------------

```
WEBRD/0.1
website=example.peer
version=1
FILES
/favicon.ico
/index
/pagina1
```

Description
-----------

The file must be named `.webrd`, must be located in the root folder of the website it describes and has to be served by
FileServers as any other object.
The first line of the file must be `WEBRD/0.1`.
The next two lines specify the website and the version of the website.
The next line must be `FILES`.
Then the file has one row for each file contained in the website: each line starts with a forward slash (`/`) and
specifies the object path relative to the root of the website (as it can be found in HTTP GET requests).