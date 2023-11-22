//
// Created by kingofknights on 11/8/23.
//

#ifndef EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_
#define EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_

#include <functional>
#include <memory>
#include <vector>

class LadderBuilder;
using LadderBuilderPrtT = std::unique_ptr<LadderBuilder>;
using LadderContainerT	= std::vector<LadderBuilderPrtT>;
using TokenListT		= std::vector<int>;

using FunctionPointerT = std::function<void(const char*)>;

class StreamManager final {
public:
	explicit StreamManager(int size_);
	void process(const char* buffer_, size_t size_);
	void init(const TokenListT& tokenList_);

protected:
	void newOrder(const char* buffer_);
	void modifyOrder(const char* buffer_);
	void cancelOrder(const char* buffer_);
	void tradeOrder(const char* buffer_);

private:
	int				 _sequence = 0;
	LadderContainerT _manager;
	FunctionPointerT _function[26];
};

#endif	// EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_
