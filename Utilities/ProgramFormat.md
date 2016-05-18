Messages from entities to server (v0.1)
=======================================

- `START <port>`
Announces that the program is listening on `<port>`.

- `STOP`
Announces that the program is no longer listening on any port.

- `CONNECT <host> <port> <socket>`
The program has established a connection with the `<host>` on `<port>` using the file descriptor `<socket>` (integer number).

- `CONNECTED <socket>`
The program has received a connection using the file descriptor `<socket>` (integer number).

- `CLOSED <socket>`
The program has closed the file descriptor `<socket>`.

- `RECEIVED <url> <socket>`
The program has received an HTTP request for the object `<url>` on the connection `<socket>`.

- `REQUESTED <url> <socket>`
The program has sent an HTTP request for the object `<url>` using the connection `<socket>`.

- `COMPLETED <url> <socket>`
The program has received the HTTP object `<url>` on the connection `<socket>`.

- `EVADED <url> <socket> <response>`
The program has completed the HTTP request for `<url>` on the connection `<socket>` with the `<response>` code.
