#pragma once
#include "scotland2/shared/loader.hpp"

namespace Qosmetics::Walls::Disabling
{
    void RegisterDisablingModInfo(modloader::ModInfo info);
    void UnregisterDisablingModInfo(modloader::ModInfo info);

    bool GetAnyDisabling();
}
