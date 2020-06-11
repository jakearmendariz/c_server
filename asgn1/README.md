# HTTP server
Jake Armendariz
May 5, 2020

## What is it?
http server built in C that handles get, put and head requests.

### Run with
- compile:    ! make
- run:        ! ./httpserver [port number]

### Files
- server.c:   sets up the server on correct port, calls functions to handle request and respose
- request.c:  contains functions to handle get, put and head requests and send the response to client
- library.c:  helper functions for parsing the request and debugging