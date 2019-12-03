#!/bin/bash


cd files
rpcgen del.x
gcc server.c del_svc.c -o server
gcc client.c del_clnt.c -o client


#cmake ./files/CMakeLists.txt
#./files/make

#cmake CMakeLists.txt
#make

