//
// Created by kingofknights on 11/21/23.
//

#ifndef EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
#define EXCALIBUR_INCLUDE_EXCALIBUR_HPP_

#include <memory>
#include "EpollSocket.hpp"
#include "Recovery.hpp"

class StreamManager;
class EpollSocket;
class Recovery;

using EpollSocketPrtT		= std::unique_ptr<EpollSocket>;
using EpollSocketContainerT = std::array<EpollSocketPrtT, MaxStream>;
using RecoveryPtrT			= std::unique_ptr<Recovery>;

class Excalibur {

public:
	Excalibur();

protected:
	void readConfig();

private:
	EpollSocketContainerT _container;
	RecoveryPtrT		  _recovery;
};

#endif	// EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
