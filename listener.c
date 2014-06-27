/**
 * sm-listener - Simple Multicast Listener
 *
 * Author: Linus Lüssing <linus.luessing@web.de>
 * License: Creative Commons Zero 1.0
 *
 * A tool to create a simple IPv6 dummy listener, which isn't actually
 * receiving anything.
 */

#include <stdio.h>      // for fprintf()
#include <net/if.h>     // for if_nametoindex()
#include <stdlib.h>     // for atoi() and exit()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <string.h>     // for memset()

int create_client_multirecv_socket(char *multicastIP, char *interface)
{
	int sd;					// Socket descriptor
	struct sockaddr_in6 multicastAddr;	// Multicast addresse structure
	struct ipv6_mreq multicastRequest;	// Multicast address join structure

	// Construct bind structure
	memset(&multicastAddr, 0, sizeof(multicastAddr));	// Zero out structure
	multicastAddr.sin6_family = AF_INET6;		// Internet address family
	multicastAddr.sin6_addr = in6addr_any;		// Any incoming interface
	multicastAddr.sin6_port = htons(0);		// Multicast port - let the OS decide

	// Create a best-effort datagram socket using UDP
	if((sd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket() failed");
		return -1;
	}
	// Bind to the multicast port
	if(bind(sd, (struct sockaddr *) &multicastAddr, sizeof(multicastAddr)) < 0) {
		perror("bind() failed");
		return -1;
	}
	// Specify the multicast group
	if(!inet_pton(AF_INET6, multicastIP, &multicastRequest.ipv6mr_multiaddr)) {
		fprintf(stderr, "Invalid IP-address: %s", multicastIP);
		return -1;
	}
	// Accept multicast from specified interface
	multicastRequest.ipv6mr_interface = if_nametoindex(interface);

	// Join the multicast address
	if(setsockopt(sd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (void *) &multicastRequest,
		sizeof(multicastRequest)) < 0) {
		perror("setsockopt() failed");
		return -1;
	}

	return sd;
}



int main(int argc, char *argv[])
{
	int sd;

	if (argc < 2) {
		perror("Not enough command line arguments (must be 2, multicast group and interface");
		exit(1);
	}

	sd = create_client_multirecv_socket(argv[1], argv[2]);
	if(sd < 0) {
		perror("create_client_multirecv_socket failed");
		exit(1);
	}

	while(1)
		sleep(100);

	close(sd);

	return 0;
}
