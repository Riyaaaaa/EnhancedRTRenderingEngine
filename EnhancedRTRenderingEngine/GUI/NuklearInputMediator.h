#pragma once

#include <boost/optional.hpp>

#include "NuklearWrapper.h"
#include "Constant/InputKey.h"
#include "Structure/Aliases.h"


class NuklearInputMediator
{
public:
    NuklearInputMediator(const std::shared_ptr<NuklearWrapper>& nuklear);

    bool ProcessInputDown(InputKey key, boost::optional<Index> pos);
    bool ProcessInputUp(InputKey key, boost::optional<Index> pos);
    bool ProcessInputMotion(Index Delta, Index pos, InputKey key);

    void BeginInput();
    void EndInput();

protected:
    std::shared_ptr<NuklearWrapper> _nuklear;

    bool _swallows = false;
};

