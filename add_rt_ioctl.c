/*
 * reference:
 * net-tools/lib/inet6_sr.c
 *
 * use ioctl() to add entry to routing table
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/route.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int sockfd;
    struct in6_rtmsg rt;
    struct ifreq ifr;

    /* Clean out the RTREQ structure. */
    memset((char *)&rt, 0, sizeof(struct in6_rtmsg));

    inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &rt.rtmsg_dst);

    /* Fill in the other fields. */
    rt.rtmsg_flags = RTF_UP;
	rt.rtmsg_flags |= RTF_HOST;
    rt.rtmsg_metric = 1;
    rt.rtmsg_dst_len = 128;

	// create the socket
	if ( (sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket");
		exit(1);
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, "enp0s10", sizeof(ifr.ifr_name));

	if (ioctl(sockfd, SIOGIFINDEX, &ifr) < 0) {
		perror("SIOGIFINDEX");
		exit(1);
	}
	rt.rtmsg_ifindex = ifr.ifr_ifindex;

	if (ioctl(sockfd, SIOCADDRT, &rt) < 0) {
	    perror("SIOCADDRT");
	    close(sockfd);
		exit(1);
	}

    /* Close the socket. */
    (void) close(sockfd);
    return (0);
}
