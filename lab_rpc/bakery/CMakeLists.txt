cmake_minimum_required(VERSION 3.29)
project(bakery C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/include/tirpc)

link_libraries(tirpc nsl)

add_executable(client bakery_client.c bakery_clnt.c bakery_xdr.c)

add_executable(server bakery_server.c bakery_xdr.c bakery_svc.c)
