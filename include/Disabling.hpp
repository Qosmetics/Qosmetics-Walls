#pragma once
#include "modloader/shared/modloader.hpp"

namespace Qosmetics::Walls::Disabling
{
    void RegisterDisablingModInfo(ModInfo info);
    void UnregisterDisablingModInfo(ModInfo info);

    bool GetAnyDisabling();
}