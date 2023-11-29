//
// Created by kingofknights on 11/21/23.
//

#ifndef EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
#define EXCALIBUR_INCLUDE_EXCALIBUR_HPP_

#include <memory>
#include <thread>

#include "EpollSocket.hpp"
#include "Recovery.hpp"

class EpollSocket;
class Recovery;
class StreamManager;

using StreamManagerPtrT		= std::shared_ptr<StreamManager>;
using StreamContainerT		= std::array<StreamManagerPtrT, MaxStream>;
using EpollSocketPrtT		= std::unique_ptr<EpollSocket>;
using EpollSocketContainerT = std::array<EpollSocketPrtT, MaxStream>;
using RecoveryPtrT			= std::unique_ptr<Recovery>;

class Excalibur {
public:
	Excalibur();

	void runRecoveryThread(std::stop_token stopToken_);

protected:
	void readConfig();

private:
	EpollSocketContainerT _container;
	StreamContainerT	  _streamContainer;
	RecoveryPtrT		  _recovery;

	std::unique_ptr<std::jthread> _recoveryThread;

};

#endif	// EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
