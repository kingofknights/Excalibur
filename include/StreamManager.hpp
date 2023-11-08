//
// Created by kingofknights on 11/8/23.
//

#ifndef EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_
#define EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_

#include <memory>
#include <vector>

class LadderBuilder;
using LadderBuilderPrtT = std::unique_ptr<LadderBuilder>;
using StreamManagerPtrT = std::vector<LadderBuilderPrtT>;
using TokenListT		= std::vector<int>;

class StreamManager {
public:
	StreamManager(int size, const TokenListT& tokenList_);
private : StreamManagerPtrT _manager;
};

#endif	// EXCALIBUR_INCLUDE_STREAMMANAGER_HPP_
