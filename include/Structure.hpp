#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/pool/pool_alloc.hpp>

using Price		 = int;
using Quantity	 = int;
using Comaprator = std::less<>;
using AllocatorT = boost::fast_pool_allocator<std::pair<Price, Quantity>, boost::default_user_allocator_malloc_free, boost::details::pool::null_mutex, 8, 32>;

template <typename Comparator>
using ContainerT = boost::container::flat_map<Price, Quantity, Comparator, AllocatorT>;

using OrderID = uint64_t;
using Order	  = struct Order {
	  Price	   _price;
	  Quantity _quantity;
};
using OrderPool		  = boost::fast_pool_allocator<Order>;
using OrderContainerT = boost::container::flat_map<OrderID, Order, Comaprator, OrderPool>;

#pragma pack(push, 1)
using Ladder = struct Ladder {
	Price	 _price	   = 0;
	Quantity _quantity = 0;
};
using LadderDepth = struct LadderDepth {
	int	   _token = 0;
	Ladder _bid[5];
	Ladder _ask[5];
};
#pragma pack(pop)

enum Side  : bool{
	Side_BUY = false,
	Side_SELL = true
};

enum MessageType : char {
	NEW		= 'N',
	REPLACE = 'M',
	CANCEL	= 'X',
	TRADE	= 'T'
};
