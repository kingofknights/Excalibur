#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/pool/pool_alloc.hpp>

using Price		 = int;
using Quantity	 = int;
using Comaprator = std::less<>;
using AllocatorT = boost::fast_pool_allocator<Price, boost::default_user_allocator_new_delete, boost::details::pool::default_mutex, 64, 128>;

template <typename Comparator>
using ContainerT = boost::container::flat_map<Price, Quantity, Comparator, AllocatorT>;

enum Side {
	Side_BUY = 0,
	Side_SELL
};