//
// Created by kingofknights on 11/8/23.
//

#ifndef EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
#define EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_

#include "Structure.hpp"
class LadderBuilder final {
public:
	explicit LadderBuilder(int token_);

	void update(Side side_, Price price_, Quantity quantity_);

	void clear();

protected:
	void generateLadders(int count_);

public:
	[[nodiscard]] Price getBestBuy() const;
	[[nodiscard]] Price getBestSell() const;
	[[nodiscard]] int	getToken() const;

private:
	Price		_bestBuy  = 0;
	Price		_bestSell = 0;
	int			_token	  = 0;
	LadderDepth _ladderDepth;

	ContainerT<std::greater<>> _buyContainer;
	ContainerT<std::less<>>	   _sellContainer;
};

#endif	// EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
