#ifndef EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_
#define EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_

#pragma once
#include <sys/epoll.h>

#include <string_view>
#include <thread>

constexpr int MaxEvents	 = 1024;
constexpr int BufferSize = 999999;

class EpollSocket final {
public:
	/** Connect to Multicast. */
	int construct(int fd_, int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_);

	void bindSocket(std::stop_token& stopToken_);

protected:
	/** Prepare Socket. */
	static int prepareMulticastSocket(int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_);

private:
	int			_epollFd{};
	epoll_event _events[MaxEvents]{};
	char		_buffer[BufferSize]{};
};

#endif	// EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_
