//
// Created by kingofknights on 11/9/23.
//

#include "../include/Recovery.hpp"

#include <unistd.h>

#include <cstring>
#include <thread>

#include "../include/Structure.hpp"

Recovery::Recovery(std::string_view address_, int port_) : _address(address_), _port(port_), _fd(0) {}

bool Recovery::makeConnection() {
	close(_fd);
	std::this_thread::sleep_for(std::chrono::microseconds(10));
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) exit(EXIT_FAILURE);

	struct sockaddr_in sockaddrIn {};
	memset((char *)&sockaddrIn, 0, sizeof(sockaddr_in));
	sockaddrIn.sin_family	   = AF_INET;
	sockaddrIn.sin_addr.s_addr = inet_addr(_address.data());
	sockaddrIn.sin_port		   = htons(_port);
	bool status				   = connect(_fd, (struct sockaddr *)&sockaddrIn, sizeof(sockaddr_in));
	return status == 0;
}

bool Recovery::sendDataToExchange(const char *buffer_, int size_) {
	while (makeConnection() and not isConnected(_fd))
		;

	return send(_fd, buffer_, size_, 0) == size_;
}

bool Recovery::isConnected(int fd_) {
	int error  = 0;
	int len	   = sizeof(error);
	int option = getsockopt(fd_, SOL_SOCKET, SO_ERROR, &error, reinterpret_cast<socklen_t *>(&len));
	return not(option != 0 || error != 0);
}

bool Recovery::recoveryRequest(short streamId_, int start_, int end_) {
	RecoveryRequestT request{._msgType = MessageType::RECOVERY, ._streamId = streamId_, ._startSeqNo = start_, ._endSeqNo = end_};
	char			buffer[sizeof(RecoveryRequestT)];
	memset(buffer, 0, sizeof(RecoveryRequestT));
	memcpy(buffer, &request, sizeof(RecoveryRequestT));
	return sendDataToExchange(buffer, sizeof(RecoveryRequestT));
}
