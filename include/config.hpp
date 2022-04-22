#pragma once
#include "qosmetics-core/shared/ConfigRegister.hpp"

namespace Qosmetics::Walls
{
    struct Config
    {
        std::string lastUsedBox;
        bool forceFakeGlowOff = false;
        bool forceCoreOff = false;
        bool forceFrameOff = false;
        bool disableReflections = false;
        static Config& get_config();
    };

    struct WallConfigRegistration : public Qosmetics::Core::Config::Registration
    {
        WallConfigRegistration(std::string_view memberName) : Qosmetics::Core::Config::Registration(memberName){};
        void SaveConfig(rapidjson::Value& member, rapidjson::Document::AllocatorType& allocator) const override;
        bool LoadConfig(const rapidjson::Value& member) const override;
        void OnProfileSwitched() const override;
    };
}