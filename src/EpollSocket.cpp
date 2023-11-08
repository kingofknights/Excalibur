#include "../include/EpollSocket.hpp"

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int EpollSocket::prepareMulticastSocket(int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_) {
	/*
		Connect to socket.
	*/
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		printf("Opening MC datagram socket num = %d error", streamId_);
		exit(EXIT_FAILURE);
	}
	printf("Opening MC datagram socket num = %d....OK.\n", streamId_);

	/*
		Enable SO_REUSEADDR to allow multiple instances of this
		application to receive copies of the multicast datagrams.
	*/
	int reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
		printf("Setting SO_REUSEADDR for MC datagram socket num = %d error!!!", streamId_);
		close(fd);
		exit(EXIT_FAILURE);
	}
	printf("Setting SO_REUSEADDR on MC socket num = %d...OK.\n", streamId_);

	/*
		Bind to the proper port number with the IP address
		specified as INADDR_ANY.
	*/
	struct sockaddr_in localSock {};
	memset((char *)&localSock, 0, sizeof(localSock));
	localSock.sin_family	  = AF_INET;
	localSock.sin_addr.s_addr = inet_addr(multicastIp_.data());
	localSock.sin_port		  = htons(port_);

	if (::bind(fd, (struct sockaddr *)&localSock, sizeof(struct sockaddr))) {
		printf("Binding MC datagram socket num = %d error", streamId_);
		close(fd);
		exit(EXIT_FAILURE);
	}

	printf("Binding MC datagram socket num = %d...OK.\n", streamId_);

	/*
		Join the multicast group on the local 1.1.1.19  interface.
		Note that this IP_ADD_MEMBERSHIP option must be called for each local interface over which the multicast
		datagrams are to be received.
	*/
	struct ip_mreq group {};
	group.imr_multiaddr.s_addr = inet_addr(multicastIp_.data());
	group.imr_interface.s_addr = inet_addr(lanIp_.data());
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
		printf("Adding multicast group for socket num = %d error", streamId_);
		close(fd);
		exit(EXIT_FAILURE);
	}

	printf("Adding multicast group for socket num = %d...OK.\n", fd);

	return fd;
}

int EpollSocket::construct(int fd_, int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_) {
	int sfd_replay_a = prepareMulticastSocket(streamId_, lanIp_, multicastIp_, port_);
	if (sfd_replay_a == -1) {
		perror("1 prepare_mc_socket");
		exit(EXIT_FAILURE);
	}

	struct epoll_event eva {};
	eva.events	= EPOLLIN;
	eva.data.fd = sfd_replay_a;

	if (epoll_ctl(fd_, EPOLL_CTL_ADD, sfd_replay_a, &eva) == -1) {
		perror("epoll_ctl");
		exit(EXIT_FAILURE);
	}
	std::cout << " streamID " << streamId_ << " connected successfully" << std::endl;
	return sfd_replay_a;
}


void EpollSocket::bindSocket(std::stop_token& stopToken_) {
	while (not stopToken_.stop_requested()) {
		int count = epoll_wait(_epollFd, events, MaxEvents, -1);
		if (count == -1) continue;

		for (int n = 0; n < count; ++n) {
			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) || (!(events[n].events & EPOLLIN))) {
				fprintf(stderr, "epoll error\n");
				close(events[n].data.fd);
				continue;
			}
			if (events[n].events & EPOLLIN) {
				ssize_t len = recv(events[n].data.fd, buffer, BufferSize, 0);
				if (len < 0) {
					close(events[n].data.fd);
				} else {
				}
			}
		}
	}
}