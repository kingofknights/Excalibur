#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/pool/pool_alloc.hpp>

using PriceT	= int;
using QuantityT = int;
using OrderIdT	= double;

using ComapratorT = std::less<>;
using AllocatorT  = boost::fast_pool_allocator<std::pair<PriceT, QuantityT>, boost::default_user_allocator_malloc_free, boost::details::pool::null_mutex, 8, 32>;

template <typename Comparator>
using ContainerT = boost::container::flat_map<PriceT, QuantityT, Comparator, AllocatorT>;

using OrderT = struct Order {
	PriceT	  _price;
	QuantityT _quantity;
};
using OrderPoolT	  = boost::fast_pool_allocator<OrderT>;
using OrderContainerT = boost::container::flat_map<OrderIdT, OrderT, ComapratorT, OrderPoolT>;

#pragma pack(push, 1)
using LadderT = struct Ladder {
	PriceT	  _price	= 0;
	QuantityT _quantity = 0;
};

using LadderDepthT = struct LadderDepth {
	int		_token = 0;
	LadderT _bid[5];
	LadderT _ask[5];
};

using StreamHeaderT = struct StreamHeader {
	short _len;
	short _streamId;
	int	  _sequence;
	char  _type;
};

using OrderMessageT = struct OrderMessage {
	double _timestamp;
	double _orderId;
	int	   _token;
	char   _orderType;
	int	   _price;
	int	   _quantity;
};

using TradeMessageT = struct TradeMessage {
	double _timeStamp;
	double _buyOrderId;
	double _sellOrderId;
	int	   _token;
	int	   _price;
	int	   _quantity;
};

using StreamDataT = struct StreamData {
	StreamHeaderT _header;
	char		  _type;
	char		  _data[40];
	int			  _counter;
};

using RecoveryRequestT = struct RecoveryRequest {
	char  _msgType;
	short _streamId;
	int	  _startSeqNo;
	int	  _endSeqNo;
};

using RecoveryResponseT = struct RecoveryResponse {
	short _msgLen;
	short _streamId;
	int	  _seqNo;
	char  _msgType;
	char  _requestStatus;
};

#pragma pack(pop)

enum MessageType : char {
	NEW		 = 'N',
	REPLACE	 = 'M',
	CANCEL	 = 'X',
	TRADE	 = 'T',
	RECOVERY = 'R'
};
