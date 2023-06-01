//
// Created by VIKLOD on 31-05-2023.
//
#pragma once

#include "Structure.hpp"

class LadderBuilder {
public:
	void newOrder(Price price_, Quantity quantity_, Side side_);

	void modifyOrder(Price price_, Quantity previousQuantity_, Quantity quantity_, Side side_);

	void modifyOrder(Price previousPrice_, Price price_, Quantity previousQuantity_, Quantity quantity_, Side side_);

	void cancelOrder(Price price_, Quantity quantity_, Side side_);

	void tradeOrder(Price price_, Quantity quantity_, Side side_);

private:
	ContainerT<std::less<>>	   _buyContainer;
	ContainerT<std::greater<>> _sellContainer;
};
