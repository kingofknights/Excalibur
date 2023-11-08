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
	void generateLadders(int count_) {
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
			if(buyLadder._quantity == sellLadder._quantity){
				buyLadder  = getLadder(_buyContainer, buyIterator);
				sellLadder = getLadder(_sellContainer, sellIterator);

				++index;
				continue;
			}
			
			if (buyLadder._quantity > sellLadder._quantity){
				buyLadder._quantity -= sellLadder._quantity;
				sellLadder = getLadder(_sellContainer, sellIterator);
				goto again;
			}
			
			if(buyLadder._quantity < sellLadder._quantity){
				sellLadder._quantity -= buyLadder._quantity;
				buyLadder = getLadder(_buyContainer, buyIterator);
				goto again;
			}
		}
	}

	template <class Container>
	Ladder getLadder(const Container& container_, typename Container::iterator& iterator_);

	template <class Container>
	void updateContainer(Container& container_, Price price_, Quantity quantity_);

private:
	Price		_bestBuy  = 0;
	Price		_bestSell = 0;
	int			Token	  = 0;
	LadderDepth _ladderDepth;

	ContainerT<std::greater<>> _buyContainer;
	ContainerT<std::less<>>	   _sellContainer;
};

#endif	// EXCALIBUR_INCLUDE_LADDERBUILDER_HPP_
