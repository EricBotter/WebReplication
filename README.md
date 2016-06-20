# Web.R
A network to fight against censorship.
Bachelor Thesis project of Eric Botter.

## Abstract
In the current Web, websites are vulnerable to censorship, enacted by attacks such as denial of service (DoS) or IP/DNS filtering, which prevent communications to the server hosting the website. Web.R provides a solution to server-based censorship by introducing replication of websites among multiple hosts, while maintaining standard Web functionalities.  
The replicas of the websites are handled by custom Web servers, which communicate their IP address to a dedicated server that keeps track of where websites are hosted. The network client is a HTTP proxy which is aware of the replication mechanism and manages file transfers within the network. The network also implements file verification based on a public-key cryptographic system, to prevent document modification by third parties.  
The users can access this system by using modern browsers which support standard HTTP proxies.

# Requirements
## Generic requirements
- GCC 5.3.0 or newer
- CMake 3.2 or newer
- libcrypto 1.0.0 or newer (which requires zlib)

## Specific OS requirements
### Windows
Cygwin is required.

### OS X
As of Mac OS 10.11, the default lib location in OS X (`/usr/lib/`) contains a version of libcrypto (0.9.9) which does not offer some of the functionalities needed by Web.R.  
You can install a newer version of openssl (for example using package managers such as Homebrew) and link against that version.

### Linux
Generic requirements should be enough.

# Instruction to setup and use
Once cloned this repository in a folder of choice, unzip the file `setup.zip` inside the folder `/var/` (or its Cygwin equivalent on Windows), such that the folder `/var/webr/` is created.  
Then, move into another folder of choice and compile Web.R using the following commands:  

```
$ cmake /path/to/Web.R/location/  
$ make  
```

Three executables are generated: `Proxy`, `Resolver` and `FileServer`.  
To run a "local network", run the executables in the following order, making sure that eventually all three executables are running at the same time:

1. `Resolver`
2. `FileServer`
3. `Proxy`

Then, instruct your browser of choice (Firefox is suggested) to use the HTTP Proxy located at the address `127.0.0.1:8000`.
The Proxy does not support HTTPS connections: make sure that the Proxy is used only for HTTP connections.  
The file `setup.zip` contains enough information to access three websites:

- `example.peer`: Basic website, only two pages
- `mywebsite.peer`: Another basic website, to test images and multiple requests
- `game.peer`: A little game that I made in high school, to show client-side dynamic content

Kill the three executables to shutdown the network.
