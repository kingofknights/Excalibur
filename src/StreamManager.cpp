//
// Created by kingofknights on 11/8/23.
//

#include "../include/StreamManager.hpp"

#include "../include/LadderBuilder.hpp"

StreamManager::StreamManager(int size, const TokenListT& tokenList_) : _manager(size) {
	for (int token_ : tokenList_) {
		_manager.at(token_) = std::make_unique<LadderBuilder>(token_);
	}
}
