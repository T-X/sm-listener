# sm-listener - Simple Multicast Listener

A tool to create a simple IPv6 dummy listener, which isn't actually
receiving anything.

## Compilation

$ gcc ./sm-listener.c -O sm-listener

## Running

$ ./sm-listener ff12::1234 eth0
