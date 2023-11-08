//
// Created by kingofknights on 11/8/23.
//

#include "../include/LadderBuilder.hpp"
void LadderBuilder::update(Side side_, Price price_, Quantity quantity_) {
	if (price_ < 0 or price_ == std::numeric_limits<Price>::max()) return;

	switch (side_) {
		case Side::Side_BUY: {
			updateContainer(_buyContainer, price_, quantity_);
		}
		case Side::Side_SELL: {
			updateContainer(_sellContainer, price_, quantity_);
			break;
		}
	}
}
template <class Container>
Ladder LadderBuilder::getLadder(const Container& container_, typename Container::iterator& iterator_) {
	if (iterator_ != container_.end()) {
		Ladder ladder{iterator_.first, iterator_.second};
		++iterator_;
		return ladder;
	}
	return {0, 0};
}

template <class Container>
void LadderBuilder::updateContainer(Container& container_, Price price_, Quantity quantity_) {
	auto iterator = container_.find(price_);
	if (iterator != container_.end()) [[likely]] {
		if ((iterator->second + quantity_) > 0)
			iterator->second += quantity_;
		else {
			container_.erase(iterator);
		}
	} else if (quantity_ > 0) {
		container_.insert(std::make_pair(price_, quantity_));
	}
}
