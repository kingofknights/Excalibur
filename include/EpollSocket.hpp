#ifndef EXCALIBAR_INCLUDE_EPOLL_SOCKET_HPP_
#define EXCALIBAR_INCLUDE_EPOLL_SOCKET_HPP_

#pragma once
#include <sys/epoll.h>

#include <string_view>
#include <thread>

constexpr int MaxEvents	 = 1024;
constexpr int BufferSize = 999999;

class EpollSocket final {
public:
	/** Connect to Multicast. */
	int construct(int fd_, int streamID_, std::string_view lanIp_, std::string_view multicastIp_, int port_);

	void bindSocket(std::stop_token& stopToken_);

protected:
	/** Prepare Socket. */
	int prepareMulticastSocket(int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_);

private:
	int			_epollFd{};
	epoll_event events[MaxEvents]{};
	char		buffer[BufferSize]{};
};

#endif	// EXCALIBAR_INCLUDE_EPOLL_SOCKET_HPP_
