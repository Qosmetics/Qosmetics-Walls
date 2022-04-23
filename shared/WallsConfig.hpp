#pragma once

#include <string>

namespace Qosmetics::Walls
{
    struct WallsConfig
    {
    public:
        std::string get_lastUsedBox() const { return lastUsedBox; }
        bool get_forceFakeGlowOff() const { return forceFakeGlowOff; }
        bool get_forceCoreOff() const { return forceCoreOff; }
        bool get_forceFrameOff() const { return forceFrameOff; }
        bool get_disableReflections() const { return disableReflections; }

    private:
        std::string lastUsedBox;
        bool forceFakeGlowOff;
        bool forceCoreOff;
        bool forceFrameOff;
        bool disableReflections;
    };
}