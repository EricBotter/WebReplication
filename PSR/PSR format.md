PSR Format
==========

PSR Format (PSR = PeerSite Resolver) is a format for requesting which hosts have copies of a certain PeerSite or to
declare to the PSR that a server hosts a certain PeerSite.

PSR Address Resolution
----------------------

Request:
```
PSR/0.3 RESOLVE\r\n
Host: example.peer\r\n
\r\n
```

Response:
```
PSR/0.3 OK\r\n
Address: 198.51.100.84:4011 198.51.100.173:4011\r\n
\r\n
```

Response in case of no server found:
```
PSR/0.3 NONE\r\n
\r\n
```

PSR Host Notification
---------------------

Notification:
```
PSR/0.3 ANNOUNCE\r\n
Available: example1.peer example2.peer example3.peer example4.peer\r\n
Server: 198.51.100.84:4011\r\n
\r\n
```
Note: `Available` contains a dash (`-`) if the FileServer does not host any website.

Response:
```
PSR/0.3 OK\r\n
Replicate: website.peer company.peer
\r\n
```

The `Replicate` key is optional.

PSR Response if incorrect request
---------------------------------

Response:
```
PSR/0.3 INVALID\r\n
\r\n
```
