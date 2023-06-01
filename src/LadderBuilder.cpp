//
// Created by VIKLOD on 31-05-2023.
//

#include "../include/LadderBuilder.hpp"

void LadderBuilder::newOrder(Price price_, Quantity quantity_, Side side_) {
	if (side_ == Side_BUY) {
		auto iterator = _buyContainer.find(price_);
		if (iterator != _buyContainer.end()) {
			iterator.get_ptr() += quantity_;
		} else {
			_buyContainer.emplace(price_, quantity_);
		}
	} else {
		auto iterator = _sellContainer.find(price_);
		if (iterator != _sellContainer.end()) {
			iterator.get_ptr() += quantity_;
		} else {
			_sellContainer.emplace(price_, quantity_);
		}
	}
}
void LadderBuilder::modifyOrder(Price price_, Quantity previousQuantity_, Quantity quantity_, Side side_) {
	if (side_ == Side_BUY) {
		auto iterator = _buyContainer.find(price_);
		if (iterator != _buyContainer.end()) {
			iterator.get_ptr() -= previousQuantity_;
			iterator.get_ptr() += quantity_;
		} else {
			newOrder(price_, quantity_, side_);
		}
	} else {
		auto iterator = _sellContainer.find(price_);
		if (iterator != _sellContainer.end()) {
			iterator.get_ptr() -= previousQuantity_;
			iterator.get_ptr() += quantity_;
		} else {
			newOrder(price_, quantity_, side_);
		}
	}
}
void LadderBuilder::modifyOrder(Price previousPrice_, Price price_, Quantity previousQuantity_, Quantity quantity_, Side side_) {
	if (side_ == Side_BUY) {
		auto iterator = _buyContainer.find(previousPrice_);
		if (iterator != _buyContainer.end()) {
			iterator.get_ptr() -= previousQuantity_;
		}
	} else {
		auto iterator = _sellContainer.find(previousPrice_);
		if (iterator != _sellContainer.end()) {
			iterator.get_ptr() -= previousQuantity_;
		}
	}
	newOrder(price_, quantity_, side_);
}
void LadderBuilder::cancelOrder(Price price_, Quantity quantity_, Side side_) {
	if (side_ == Side_BUY) {
		auto iterator = _buyContainer.find(price_);
		if (iterator != _buyContainer.end()) {
			iterator.get_ptr() -= quantity_;
			if (iterator.get_ptr() == 0) _buyContainer.erase(iterator);
		}
	} else {
		auto iterator = _sellContainer.find(price_);
		if (iterator != _sellContainer.end()) {
			iterator.get_ptr() -= quantity_;
			if (iterator.get_ptr() == 0) _sellContainer.erase(iterator);
		}
	}
}
void LadderBuilder::tradeOrder(Price price_, Quantity quantity_, Side side_) {
	if (side_ == Side_BUY) {
		auto iterator = _buyContainer.find(price_);
		if (iterator != _buyContainer.end()) {
			iterator.get_ptr() -= quantity_;
			if (iterator.get_ptr() == 0) _buyContainer.erase(iterator);
		}
	} else {
		auto iterator = _sellContainer.find(price_);
		if (iterator != _sellContainer.end()) {
			iterator.get_ptr() -= quantity_;
			if (iterator.get_ptr() == 0) _sellContainer.erase(iterator);
		}
	}
}
