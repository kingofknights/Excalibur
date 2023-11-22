//
// Created by kingofknights on 11/8/23.
//

#ifndef EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
#define EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_

#include "Structure.hpp"
class LadderBuilder final {
public:
	explicit LadderBuilder(int token_);

	void update(bool side_, PriceT price_, QuantityT quantity_);
	void clear();
	void setOrder(OrderIdT orderId_, PriceT price_, QuantityT quantity_);
	void clearOrder(OrderIdT orderId_);
	void generateLadders(int count_ = 5);
	void print() const;

	[[nodiscard]] OrderT getOrder(OrderIdT orderId_) const;
	[[nodiscard]] PriceT getBestBuy() const;
	[[nodiscard]] PriceT getBestSell() const;
	[[nodiscard]] int	 getToken() const;

private:
	PriceT		 _bestBuy  = 0;
	PriceT		 _bestSell = 0;
	int			 _token	   = 0;
	LadderDepthT _ladderDepth;

	ContainerT<std::greater<>> _buyContainer;
	ContainerT<std::less<>>	   _sellContainer;
	OrderContainerT			   _orderContainer;
};

#endif	// EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
