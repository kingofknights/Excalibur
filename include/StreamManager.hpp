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
using StreamManagerPtrT = std::vector<LadderBuilderPrtT>;
using TokenListT		= std::vector<int>;

using FunctionPointerT = std::function<void(const char*)>;

class StreamManager final {
public:
	StreamManager(int size_, const TokenListT& tokenList_);
	void process(const char* buffer_, int size_);

protected:
	void newOrder(const char* buffer_);
	void modifyOrder(const char* buffer_);
	void cancelOrder(const char* buffer_);
	void tradeOrder(const char* buffer_);

private:
	StreamManagerPtrT _manager;
	int				  _sequence = 1;
	FunctionPointerT  _function[26];
};

#endif	// EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_
