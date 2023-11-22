#ifndef EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_
#define EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_

#pragma once
#include <sys/epoll.h>

#include <memory>
#include <stop_token>
#include <string_view>

constexpr int MaxEvents	 = 1024;
constexpr int BufferSize = 999999;
constexpr int MaxStream	 = 16;

class StreamManager;
using StreamManagerPtrT = std::shared_ptr<StreamManager>;
using StreamContainerT	= std::array<StreamManagerPtrT, MaxStream>;

class EpollSocket final {
public:
	EpollSocket();
	/** Connect to Multicast. */
	[[nodiscard]] StreamManagerPtrT construct(int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_) ;

	void bindSocket(std::stop_token& stopToken_);

protected:
	/** Prepare Socket. */
	static int prepareMulticastSocket(int streamId_, std::string_view lanIp_, std::string_view multicastIp_, int port_);

private:
	int			_epollFd{};
	epoll_event _events[MaxEvents]{};
	char		_buffer[BufferSize]{};

	StreamContainerT _container;
};

#endif	// EXCALIBUR_INCLUDE_EPOLLSOCKET_HPP_
