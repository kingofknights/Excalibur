//
// Created by kingofknights on 11/8/23.
//

#include "../include/LadderBuilder.hpp"

template <class Container>
LadderT GetLadder(const Container& container_, typename Container::const_iterator& iterator_) {
	if (iterator_ != container_.cend()) {
		LadderT ladder{iterator_->first, iterator_->second};
		++iterator_;
		return ladder;
	}
	return {0, 0};
}

template <class Container>
void UpdateContainer(Container& container_, PriceT price_, QuantityT quantity_) {
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

void LadderBuilder::update(bool side_, PriceT price_, QuantityT quantity_) {
	if (price_ < 0 or price_ == std::numeric_limits<PriceT>::max()) return;

	if (side_)
		UpdateContainer(_buyContainer, price_, quantity_);
	else
		UpdateContainer(_sellContainer, price_, quantity_);
}

void LadderBuilder::setOrder(OrderIdT orderId_, PriceT price_, QuantityT quantity_) { _orderContainer.insert_or_assign(orderId_, OrderT{price_, quantity_}); }

OrderT LadderBuilder::getOrder(OrderIdT orderId_) const {
	const auto iterator = _orderContainer.find(orderId_);
	return iterator != _orderContainer.cend() ? OrderT{iterator->_price, iterator->_quantity} : OrderT{0, 0};
}

void LadderBuilder::clearOrder(OrderIdT orderId_) { _orderContainer.erase(orderId_); }

void LadderBuilder::clear() {
	_buyContainer.clear();
	_sellContainer.clear();
}

void LadderBuilder::generateLadders(int count_) {
	auto buyIterator  = _buyContainer.cbegin();
	auto sellIterator = _sellContainer.cbegin();

	int	 index		= 0;
	auto buyLadder	= GetLadder(_buyContainer, buyIterator);
	auto sellLadder = GetLadder(_sellContainer, sellIterator);

	while (index < count_) {
		if (buyLadder._price < sellLadder._price or sellLadder._price == 0) [[likely]] {
			_ladderDepth._bid[index] = buyLadder;
			_ladderDepth._ask[index] = sellLadder;

			buyLadder  = GetLadder(_buyContainer, buyIterator);
			sellLadder = GetLadder(_sellContainer, sellIterator);

			++index;
			continue;
		}
	again:
		if (buyLadder._quantity == sellLadder._quantity) {
			buyLadder  = GetLadder(_buyContainer, buyIterator);
			sellLadder = GetLadder(_sellContainer, sellIterator);

			++index;
			continue;
		}

		if (buyLadder._quantity > sellLadder._quantity) {
			buyLadder._quantity -= sellLadder._quantity;
			sellLadder			 = GetLadder(_sellContainer, sellIterator);
			goto again;
		}

		if (buyLadder._quantity < sellLadder._quantity) {
			sellLadder._quantity -= buyLadder._quantity;
			buyLadder			  = GetLadder(_buyContainer, buyIterator);
			goto again;
		}
	}

	_bestBuy  = _ladderDepth._bid[0]._price;
	_bestSell = _ladderDepth._ask[0]._price;
}

PriceT LadderBuilder::getBestBuy() const { return _bestBuy; }
PriceT LadderBuilder::getBestSell() const { return _bestSell; }
int	  LadderBuilder::getToken() const { return _token; }
