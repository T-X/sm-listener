/* SPDX-FileCopyrightText: 2026 Linus Lüssing <linus.luessing@c0d3.blue> */
/* SPDX-License-Identifier: CC0-1.0 */

/**
 * sm-listener - Simple Multicast Listener
 *
 * A tool to create a simple IPv6 dummy listener, which isn't actually
 * receiving anything.
 */

#include <stdio.h>      // for fprintf()
#include <net/if.h>     // for if_nametoindex()
#include <unistd.h>	// for sleep() and close()
#include <stdlib.h>     // for atoi() and exit()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <string.h>     // for memset()

enum mc_api {
	MC_API_RFC3678,
	MC_API_RFC3493,
};

static int multicast_join_rfc3678(int sd, struct in6_addr *multicastAddr, int ifindex)
{
	struct group_req groupRequest;

	memset(&groupRequest, 0, sizeof(groupRequest));	// Zero out structure

	// Specify the multicast group
	groupRequest.gr_group.ss_family = AF_INET6;
	// Accept multicast from specified interface
	groupRequest.gr_interface = ifindex;
	((struct sockaddr_in6 *)&groupRequest.gr_group)->sin6_addr = *multicastAddr;

	// Join the multicast address
	if(setsockopt(sd, IPPROTO_IPV6, MCAST_JOIN_GROUP, (void *) &groupRequest,
		sizeof(groupRequest)) < 0) {
		perror("setsockopt(MCAST_JOIN_GROUP) failed");
		return -1;
	}

	return 0;
}

static int multicast_join_rfc3493(int sd, struct in6_addr *multicastAddr, int ifindex)
{
	struct ipv6_mreq multicastRequest;	// Multicast address join structure

	memset(&multicastRequest, 0, sizeof(multicastRequest));	// Zero out structure

	multicastRequest.ipv6mr_multiaddr = *multicastAddr;

	// Accept multicast from specified interface
	multicastRequest.ipv6mr_interface = ifindex;

	// Join the multicast address
	if(setsockopt(sd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (void *) &multicastRequest,
		sizeof(multicastRequest)) < 0) {
		perror("setsockopt(IPV6_JOIN_GROUP) failed");
		return -1;
	}

	return 0;
}

static int multicast_join(enum mc_api api, int sd, struct in6_addr *multicastAddr, int ifindex)
{
	switch(api) {
	case MC_API_RFC3678:
		return multicast_join_rfc3678(sd, multicastAddr, ifindex);
	case MC_API_RFC3493:
		return multicast_join_rfc3493(sd, multicastAddr, ifindex);
	}

	return -1;
}

int create_client_multirecv_socket(enum mc_api api, const char *multicastIP, const char *interface)
{
	int sd;					// Socket descriptor
	struct sockaddr_in6 sockAddrAny;	// bind to any address/interface
	struct in6_addr multicastAddr;
	int ifindex;

	// Construct bind structure
	memset(&sockAddrAny, 0, sizeof(sockAddrAny));	// Zero out structure
	sockAddrAny.sin6_family = AF_INET6;		// Internet address family
	sockAddrAny.sin6_addr = in6addr_any;		// Any incoming interface
	sockAddrAny.sin6_port = htons(0);		// Multicast port - let the OS decide

	// Create a best-effort datagram socket using UDP
	if((sd = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket() failed");
		return -1;
	}
	// Bind to the multicast port
	if(bind(sd, (struct sockaddr *) &sockAddrAny, sizeof(sockAddrAny)) < 0) {
		perror("bind() failed");
		return -1;
	}
	if(!inet_pton(AF_INET6, multicastIP, &multicastAddr)) {
		fprintf(stderr, "Invalid IP-address: %s", multicastIP);
		return -1;
	}

	ifindex = if_nametoindex(interface);

	return multicast_join(api, sd, &multicastAddr, ifindex);
}

void help(const char *command) {
	printf("Usage: %s <multicast group> <interface> [rfc3678|rfc3493]\n", command);
}

int main(int argc, const char *argv[])
{
	int sd; enum mc_api api = MC_API_RFC3678;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") ||
		    !strcmp(argv[i], "--help")) {
			help(argv[0]);
			exit(0);
		}
	}

	if (argc < 3) {
		fprintf(stderr, "Not enough command line arguments (must be 2, multicast group and interface");
		help(argv[0]);
		exit(1);
	}

	if (argc > 3) {
		if (!strcmp(argv[3], "rfc3493")) {
			api = MC_API_RFC3493;
		} else if (strcmp(argv[3], "rfc3678")) {
			fprintf(stderr, "invalid API \"%s\"\n", argv[3]);
			help(argv[0]);
			exit(1);
		}
	}

	sd = create_client_multirecv_socket(api, argv[1], argv[2]);
	if(sd < 0) {
		perror("create_client_multirecv_socket failed");
		exit(1);
	}

	while(1)
		sleep(100);

	close(sd);

	return 0;
}
