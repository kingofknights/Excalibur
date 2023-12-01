//
// Created by kingofknights on 11/21/23.
//

#include "../include/Excalibur.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/StreamManager.hpp"

Excalibur::Excalibur() { readConfig(); }

void Excalibur::readConfig() {
	std::ifstream file("Excalibur.json");
	if (not file.is_open()) return;

	nlohmann::json json		= nlohmann::json::parse(file);
	const auto	   recovery = json.at("recovery");
	{
		const auto address = recovery.at("address").get<std::string>();
		const auto port	   = recovery.at("port").get<short>();
		std::cout << "recovery => address : " << address << " port : " << port << std::endl;
		_recovery = std::make_unique<Recovery>(address, port);
	}

	const auto connect = json.at("connect");
	const auto streams = json.at("streams");

	std::cout << "connect " << connect << std::endl;
	std::cout << "streams " << streams << std::endl;

	using StreamInfoT = std::array<nlohmann::json::value_type, MaxStream>;

	StreamInfoT streamInfo;
	for (const auto& info : streams) {
		int index			 = info.at("id").get<int>();
		streamInfo.at(index) = info;
		std::cout << "Info : " << index << " " << info << std::endl;
	}

	for (const auto& thread : connect.items()) {
		std::cout << "binding these stream into one epoll : " << thread.value() << std::endl;

		auto epoll = std::make_shared<EpollSocket>();
		for (const auto& item : thread.value().items()) {
			const auto& info	= streamInfo.at(item.value());
			const auto	lan		= info.at("lan").get<std::string>();
			const auto	address = info.at("address").get<std::string>();
			const auto	port	= info.at("port").get<short>();
			const auto	id		= info.at("id").get<int>();

			_streamContainer.at(id) = epoll->construct(id, lan, address, port);
			std::cout << id << " " << lan << " " << address << " " << port << std::endl;
		}
		_container.push_back(std::move(epoll));
	}
}
void Excalibur::runRecoveryThread(std::stop_token stopToken_) {
	while (not stopToken_.stop_requested()) {
	}
}
void Excalibur::run() {
	auto runThread = [](const EpollSocketPrtT& ptr_) {
		std::jthread thread([&](std::stop_token token_) { ptr_->bindSocket(token_); });
		return thread;
	};

	for (const auto& item : _container) {
		_threadGroup.push_back(runThread(item));
	}

	for (std::jthread& thread_ : _threadGroup) {
		if (thread_.joinable()) thread_.join();
	}
}
