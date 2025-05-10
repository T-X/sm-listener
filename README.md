# sm-listener - Simple Multicast Listener

A tool written in C to create a simple IPv6 dummy listener, which
isn't actually receiving anything.

## Compilation

``$ gcc ./sm-listener.c -O sm-listener``

## Running

``$ ./sm-listener ff12::1234 eth0``

Then check ip-maddr to verify if the listener was added:

``$ ip -6 maddr show dev eth0``

***

### Alternative 1: socat

Now the IPv6 multicast listener in the awesome swiss army knife tool [socat](http://www.dest-unreach.org/socat/) was fixed in v1.7.3.2! Since socat is probably shipped with your Linux distro already, maybe you just want to use that. You can create an IPv6 multicast listener with socat like:

``$ socat -u UDP6-RECV:1234,reuseaddr,ipv6-join-group="[ff12::123]:eth0" -``

http://www.dest-unreach.org/socat/

(Note: 1.8.0.0 broke it again and 1.8.0.1 fixed it)

### Alternative 2: mcjoin

https://github.com/troglobit/mcjoin

### Alternative 3: iproute2 "autojoin"

``$ ip -6 addr add ff12::123 dev eth0 autojoin``
