//
// Created by kingofknights on 11/8/23.
//

#ifndef EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
#define EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_

#include "Structure.hpp"
class LadderBuilder final {
public:
	void update(Side side_, Price price_, Quantity quantity_);

protected:
	void generateLadders(int count_);

private:
	Price		_bestBuy  = 0;
	Price		_bestSell = 0;
	int			Token	  = 0;
	LadderDepth _ladderDepth;

	ContainerT<std::greater<>> _buyContainer;
	ContainerT<std::less<>>	   _sellContainer;
};

#endif	// EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
