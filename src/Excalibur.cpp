//
// Created by kingofknights on 11/21/23.
//

#include "../include/Excalibur.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/EpollSocket.hpp"
#include "../include/Recovery.hpp"
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
	int			index = 0;
	StreamInfoT streamInfo;
	for (const auto& info : streams) {
		streamInfo.at(index)  = info;
		index				 += 1;
	}

	for (const auto& thread : connect.items()) {
		std::cout << "binding these stream into one epoll : " << thread.value() << std::endl;

		for (const auto& item : thread.value().items()) {
			const auto& info	= streamInfo.at(item.value());
			const auto	lan		= info.at("lan").get<std::string>();
			const auto	address = info.at("address").get<std::string>();
			const auto	port	= info.at("port").get<short>();

			const auto id	  = info.at("id").get<int>();
			_container.at(id) = std::make_unique<EpollSocket>();
			const auto streamManager = _container.at(id)->construct(id, lan, address, port);
		}
	}
}
