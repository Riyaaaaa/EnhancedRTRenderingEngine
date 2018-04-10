#pragma once

#include <boost/optional.hpp>

#include "NuklearWrapper.h"
#include "Constant/InputKey.h"
#include "Structure/Aliases.h"


class NuklearInputMediator
{
public:
    NuklearInputMediator(NuklearWrapper* nuklear);

    void ProcessInputDown(InputKey key, boost::optional<Index> pos);
    void ProcessInputUp(InputKey key, boost::optional<Index> pos);
    void ProcessInputMotion(Index Delta, Index pos, InputKey key);

    void BeginInput();
    void EndInput();

protected:
    NuklearWrapper* _nuklear;
};

