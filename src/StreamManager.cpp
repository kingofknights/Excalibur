//
// Created by kingofknights on 11/8/23.
//

#include "../include/StreamManager.hpp"

#include "../include/LadderBuilder.hpp"
#include "../include/Structure.hpp"

StreamManager::StreamManager(int size, const TokenListT& tokenList_) : _manager(size) {
	for (int token_ : tokenList_) _manager.at(token_) = std::make_unique<LadderBuilder>(token_);

	for (char a = 'A'; a <= 'Z'; ++a) _function[a - 'A'] = [](const char*) {};

	_function[MessageType::NEW - 'A']	  = [this](const char* buffer_) { newOrder(buffer_); };
	_function[MessageType::REPLACE - 'A'] = [this](const char* buffer_) { modifyOrder(buffer_); };
	_function[MessageType::CANCEL - 'A']  = [this](const char* buffer_) { cancelOrder(buffer_); };
	_function[MessageType::TRADE - 'A']	  = [this](const char* buffer_) { tradeOrder(buffer_); };
}
void StreamManager::process(const char* buffer_, int size_) {
	const StreamHeader* streamHeader = (SteamHeader*)(buffer_);
	size_t				size		 = sizeof(SteamHeader);
	if (_sequence + 1 == streamHeader->_sequence) [[likely]] {
		_function[streamHeader->_type - 'A'](buffer_ + size);
	}
}

void StreamManager::newOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessage*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);
	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}

void StreamManager::modifyOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessage*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);

	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}
void StreamManager::cancelOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessage*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->generateLadders();
	ladder->clearOrder(orderMessage->_orderId);
}
void StreamManager::tradeOrder(const char* buffer_) {}
