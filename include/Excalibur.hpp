//
// Created by kingofknights on 11/21/23.
//

#ifndef EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
#define EXCALIBUR_INCLUDE_EXCALIBUR_HPP_

#include <memory>
#include <thread>
#include <vector>

#include "EpollSocket.hpp"
#include "Recovery.hpp"

class EpollSocket;
class Recovery;
class StreamManager;

using StreamManagerPtrT		= std::shared_ptr<StreamManager>;
using StreamContainerT		= std::array<StreamManagerPtrT, MaxStream>;
using EpollSocketPrtT		= std::shared_ptr<EpollSocket>;
using EpollSocketContainerT = std::vector<EpollSocketPrtT>;
using RecoveryPtrT			= std::unique_ptr<Recovery>;

class Excalibur {
public:
	Excalibur();

	void run();

protected:
	void readConfig();
	void runRecoveryThread(std::stop_token stopToken_);

private:
	EpollSocketContainerT	  _container;
	StreamContainerT		  _streamContainer;
	RecoveryPtrT			  _recovery;
	std::vector<std::jthread> _threadGroup;
};

#endif	// EXCALIBUR_INCLUDE_EXCALIBUR_HPP_
