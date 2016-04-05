#!/bin/sh

cd ..
make

#Startup of servers
./Resolver > test/resolver.log &
resolver_pid=$!

sleep 1

./FileServer > test/fileserver.log &
fs_pid=$!

./Proxy > test/proxy.log &
proxy_pid=$!

sleep 2

curl -s -x "127.0.0.1:8000" http://example.peer > test/example.txt
diff test/example.txt /var/webr/websites/example.peer/index

kill $proxy_pid
kill $fs_pid
kill $resolver_pid

sleep 2

