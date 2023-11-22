//
// Created by kingofknights on 11/8/23.
//

#include "../include/StreamManager.hpp"

#include <execution>

#include "../include/LadderBuilder.hpp"

void TradeRemove(const LadderBuilderPrtT& ladderBuilder_, OrderIdT orderId_, PriceT price_, QuantityT quantity_) {
	if (quantity_ > 0) {
		ladderBuilder_->setOrder(orderId_, price_, quantity_);
	} else {
		ladderBuilder_->clearOrder(orderId_);
	}
}

constexpr auto EmptyFunction = []([[maybe_unused]] const char*) {};

StreamManager::StreamManager(int size_) : _manager(size_) {
	for (char a = 'A'; a <= 'Z'; ++a) _function[a - 'A'] = EmptyFunction;

	_function[MessageType::NEW - 'A']	  = [this](const char* buffer_) { newOrder(buffer_); };
	_function[MessageType::REPLACE - 'A'] = [this](const char* buffer_) { modifyOrder(buffer_); };
	_function[MessageType::CANCEL - 'A']  = [this](const char* buffer_) { cancelOrder(buffer_); };
	_function[MessageType::TRADE - 'A']	  = [this](const char* buffer_) { tradeOrder(buffer_); };
}

void StreamManager::process(const char* buffer_, [[maybe_unused]] size_t size_) {
	const auto*		 streamHeader = (StreamHeaderT*)(buffer_);
	constexpr size_t size		  = sizeof(StreamHeaderT);
	if (_sequence + 1 == streamHeader->_sequence) [[likely]] {
		_function[streamHeader->_type - 'A'](buffer_ + size);
		_sequence = streamHeader->_sequence;
		return;
	}
	if (_sequence < streamHeader->_sequence) {
		if (_sequence == 0) {
			_sequence = streamHeader->_sequence;
			return;
		}

		_sequence = streamHeader->_sequence;
		std::for_each(std::execution::par, _manager.cbegin(), _manager.cend(), [](const LadderBuilderPrtT& pointer_) { pointer_->clear(); });
	}
}

void StreamManager::newOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	const auto& ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);
	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}

void StreamManager::modifyOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	const auto& ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);

	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}

void StreamManager::cancelOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	const auto& ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->generateLadders();
	ladder->clearOrder(orderMessage->_orderId);
}

void StreamManager::tradeOrder(const char* buffer_) {
	const auto* tradeMessage = (TradeMessage*)(buffer_);
	const auto& ladder		 = _manager.at(tradeMessage->_token);
	if (not ladder) return;

	auto buyOrder  = ladder->getOrder(tradeMessage->_buyOrderId);
	auto sellOrder = ladder->getOrder(tradeMessage->_sellOrderId);

	ladder->update(true, buyOrder._price, -tradeMessage->_quantity);
	ladder->update(false, sellOrder._price, -tradeMessage->_quantity);
	ladder->generateLadders();

	TradeRemove(ladder, tradeMessage->_buyOrderId, buyOrder._price, buyOrder._quantity - tradeMessage->_quantity);
	TradeRemove(ladder, tradeMessage->_sellOrderId, sellOrder._price, sellOrder._quantity - tradeMessage->_quantity);
}

void StreamManager::init(const TokenListT& tokenList_) {
	for (int token : tokenList_) {
		_manager.at(token) = std::make_unique<LadderBuilder>(token);
	}
}
