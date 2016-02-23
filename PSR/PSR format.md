PSR Format
==========

PSR Format (PSR = PeerSite Resolver) is a format for requesting which hosts have copies of a certain PeerSite or to
declare to the PSR that a server hosts a certain PeerSite.

PSR Address Resolution
----------------------

Request:
```
PSR/0.1\r\n
Host: example.peer\r\n
\r\n
```

Response:
```
PSR/0.1\r\n
Address: 198.51.100.84:31826 198.51.100.173:43298\r\n
\r\n
```

PSR Host Notification
---------------------

Notification:
```
PSR/0.1\r\n
Available: example1.peer example2.peer example3.peer example4.peer\r\n
\r\n
```

Response:
```
PSR/0.1\r\n
Message: OK\r\n
\r\n
```
