#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/pool/pool_alloc.hpp>

using Price		 = int;
using Quantity	 = int;
using Comaprator = std::less<>;
using AllocatorT = boost::fast_pool_allocator<std::pair<Price, Quantity>, boost::default_user_allocator_malloc_free, boost::details::pool::null_mutex, 8, 32>;

template <typename Comparator>
using ContainerT = boost::container::flat_map<Price, Quantity, Comparator, AllocatorT>;

using OrderID = double;
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

using StreamHeader = struct SteamHeader {
	short _len;
	short _streamId;
	int	  _sequence;
	char  _type;
};

using OrderMessage = struct OrderMessage {
	double _timestamp;
	double _orderId;
	int	   _token;
	char   _orderType;
	int	   _price;
	int	   _quantity;
};

struct TradeMessage {
	double Timestamp;
	double _buyOrderId;
	double _sellOrderId;
	int	   _token;
	int	   _price;
	int	   _quantity;
};

using StreamData = struct StreamData {
	SteamHeader _header;
	char		_type;
	char		_data[40];
	int			_counter;
};

using RecoveryRequest = struct RecoveryRequest {
	char  _msgType;
	short _streamId;
	int	  _startSeqNo;
	int	  _endSeqNo;
};

using RecoveryResponse = struct RecoveryResponse {
	short _msgLen;
	short _streamId;
	int	  _seqNo;
	char  _msgType;
	char  _requestStatus;
};

#pragma pack(pop)

enum MessageType : char {
	NEW		= 'N',
	REPLACE = 'M',
	CANCEL	= 'X',
	TRADE	= 'T'
};
