#!/bin/bash


cd files
rpcgen del.x
g++ server.cpp del_svc.c del_xdr.c poly.cpp -o server
g++ client.c del_clnt.c del_xdr.c -o client


#cmake ./files/CMakeLists.txt
#./files/make

#cmake CMakeLists.txt
#make

