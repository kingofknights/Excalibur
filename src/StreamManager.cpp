//
// Created by kingofknights on 11/8/23.
//

#include "../include/StreamManager.hpp"
#include "../include/Structure.hpp"
#include "../include/LadderBuilder.hpp"

void TradeRemove(LadderBuilderPrtT& ladderBuilder_, OrderIdT orderId_, PriceT price_, QuantityT quantity_) {
	if (quantity_ > 0) {
		ladderBuilder_->setOrder(orderId_, price_, quantity_);
	} else {
		ladderBuilder_->clearOrder(orderId_);
	}
}

StreamManager::StreamManager(int size_, const TokenListT& tokenList_) : _manager(size_) {
	for (int token_ : tokenList_) _manager.at(token_) = std::make_unique<LadderBuilder>(token_);

	for (char a = 'A'; a <= 'Z'; ++a) _function[a - 'A'] = [](const char*) {};

	_function[MessageType::NEW - 'A']	  = [this](const char* buffer_) { newOrder(buffer_); };
	_function[MessageType::REPLACE - 'A'] = [this](const char* buffer_) { modifyOrder(buffer_); };
	_function[MessageType::CANCEL - 'A']  = [this](const char* buffer_) { cancelOrder(buffer_); };
	_function[MessageType::TRADE - 'A']	  = [this](const char* buffer_) { tradeOrder(buffer_); };
}

void StreamManager::process(const char* buffer_, [[maybe_unused]] int size_) {
	const auto* streamHeader = (StreamHeaderT*)(buffer_);
	constexpr size_t	size		 = sizeof(StreamHeaderT);
	if (_sequence + 1 == streamHeader->_sequence) [[likely]] {
		_function[streamHeader->_type - 'A'](buffer_ + size);
		_sequence = streamHeader->_sequence;
		return;
	}
	if (_sequence < streamHeader->_sequence) {
	}
}

void StreamManager::newOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);
	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}

void StreamManager::modifyOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->update(orderMessage->_orderType == 'B', orderMessage->_price, orderMessage->_quantity);

	ladder->generateLadders();
	ladder->setOrder(orderMessage->_orderId, orderMessage->_price, orderMessage->_quantity);
}

void StreamManager::cancelOrder(const char* buffer_) {
	const auto* orderMessage = (const OrderMessageT*)(buffer_);
	auto&		ladder		 = _manager.at(orderMessage->_token);
	if (not ladder) return;

	auto order = ladder->getOrder(orderMessage->_orderId);
	ladder->update(orderMessage->_orderType == 'B', order._price, -order._quantity);
	ladder->generateLadders();
	ladder->clearOrder(orderMessage->_orderId);
}

void StreamManager::tradeOrder(const char* buffer_) {
	const auto* tradeMessage = (TradeMessage*)(buffer_);
	auto&		ladder		 = _manager.at(tradeMessage->_token);
	if (not ladder) return;

	auto buyOrder  = ladder->getOrder(tradeMessage->_buyOrderId);
	auto sellOrder = ladder->getOrder(tradeMessage->_sellOrderId);

	ladder->update(true, buyOrder._price, -tradeMessage->_quantity);
	ladder->update(false, sellOrder._price, -tradeMessage->_quantity);
	ladder->generateLadders();

	TradeRemove(ladder, tradeMessage->_buyOrderId, buyOrder._price, buyOrder._quantity - tradeMessage->_quantity);
	TradeRemove(ladder, tradeMessage->_sellOrderId, sellOrder._price, sellOrder._quantity - tradeMessage->_quantity);
}
