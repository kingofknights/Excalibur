//
// Created by kingofknights on 11/9/23.
//

#ifndef EXCALIBUR_INCLUDE_RECOVERY_HPP_
#define EXCALIBUR_INCLUDE_RECOVERY_HPP_

#include <arpa/inet.h>

#include <string>

class Recovery {
public:
	Recovery(std::string_view address_, int port_);

	[[nodiscard]] bool		  makeConnection();
	[[nodiscard]] bool		  sendDataToExchange(const char* buffer_, int size_);
	[[nodiscard]] static bool isConnected(int fd_);
	[[nodiscard]] bool		  recoveryRequest(short streamId_, int start_, int end_);

private:
	static constexpr size_t _maxSize		  = 999999;
	char					_buffer[_maxSize] = {};

	std::string _address;
	int			_port;

	int _fd;
};

#endif	// EXCALIBUR_INCLUDE_RECOVERY_HPP_
