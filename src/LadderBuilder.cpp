//
// Created by kingofknights on 11/8/23.
//

#include "../include/LadderBuilder.hpp"

template <class Container>
Ladder getLadder(const Container& container_, typename Container::iterator& iterator_) {
	if (iterator_ != container_.end()) {
		Ladder ladder{iterator_->first, iterator_->second};
		++iterator_;
		return ladder;
	}
	return {0, 0};
}

template <class Container>
void updateContainer(Container& container_, Price price_, Quantity quantity_) {
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

LadderBuilder::LadderBuilder(int token_) : _token(token_) { _ladderDepth._token = token_; }

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

void LadderBuilder::clear() {
	_buyContainer.clear();
	_sellContainer.clear();
}

void LadderBuilder::generateLadders(int count_) {
	auto buyIterator  = _buyContainer.begin();
	auto sellIterator = _sellContainer.begin();

	int	 index		= 0;
	auto buyLadder	= getLadder(_buyContainer, buyIterator);
	auto sellLadder = getLadder(_sellContainer, sellIterator);

	while (index < count_) {
		if (buyLadder._price < sellLadder._price or sellLadder._price == 0) [[likely]] {
			_ladderDepth._bid[index] = buyLadder;
			_ladderDepth._ask[index] = sellLadder;

			buyLadder  = getLadder(_buyContainer, buyIterator);
			sellLadder = getLadder(_sellContainer, sellIterator);

			++index;
			continue;
		}
	again:
		if (buyLadder._quantity == sellLadder._quantity) {
			buyLadder  = getLadder(_buyContainer, buyIterator);
			sellLadder = getLadder(_sellContainer, sellIterator);

			++index;
			continue;
		}

		if (buyLadder._quantity > sellLadder._quantity) {
			buyLadder._quantity -= sellLadder._quantity;
			sellLadder			 = getLadder(_sellContainer, sellIterator);
			goto again;
		}

		if (buyLadder._quantity < sellLadder._quantity) {
			sellLadder._quantity -= buyLadder._quantity;
			buyLadder			  = getLadder(_buyContainer, buyIterator);
			goto again;
		}
	}

	_bestBuy  = _ladderDepth._bid[0]._price;
	_bestSell = _ladderDepth._ask[0]._price;
}

Price LadderBuilder::getBestBuy() const { return _bestBuy; }
Price LadderBuilder::getBestSell() const { return _bestSell; }
int	  LadderBuilder::getToken() const { return _token; }
