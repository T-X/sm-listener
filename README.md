# sm-listener - Simple Multicast Listener

A tool to create a simple IPv6 dummy listener, which isn't actually
receiving anything.

## Compilation

$ gcc ./sm-listener.c -O sm-listener

## Running

$ ./sm-listener ff12::1234 eth0

***

### Alternative: socat

Now the IPv6 multicast listener in the awesome swiss army knife tool [socat](http://www.dest-unreach.org/socat/) was fixed in v1.7.3.2! Since socat is probably shipped with your Linux distro already, maybe you just want to use that. You can create an IPv6 multicast listener with socat like:

$ socat -u UDP6-RECV:1234,reuseaddr,ipv6-join-group="[ff12::123]:eth0"

http://www.dest-unreach.org/socat/
