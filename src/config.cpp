#include "config.hpp"
#include "logging.hpp"

#include "CustomTypes/WallModelContainer.hpp"
#include "UI/PreviewViewController.hpp"
#include "UI/SelectionViewController.hpp"
#include "UI/SettingsViewController.hpp"
#include "static-defines.hpp"

#define GET_JSON_STRING(identifier)                                                                                           \
    auto identifier##Itr = member.FindMember(#identifier);                                                                    \
    if (identifier##Itr != member.MemberEnd() && identifier##Itr->value.IsString())                                           \
    {                                                                                                                         \
        actual_config.identifier = std::string(identifier##Itr->value.GetString(), identifier##Itr->value.GetStringLength()); \
    }                                                                                                                         \
    else                                                                                                                      \
        foundEverything = false;

#define SET_JSON_STRING(identifier)                                            \
    auto identifier##Itr = member.FindMember(#identifier);                     \
    if (identifier##Itr != member.MemberEnd())                                 \
    {                                                                          \
        identifier##Itr->value.SetString(actual_config.identifier, allocator); \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        member.AddMember(#identifier, actual_config.identifier, allocator);    \
    }

#define GET_JSON_BOOL(identifier)                                                 \
    auto identifier##Itr = member.FindMember(#identifier);                        \
    if (identifier##Itr != member.MemberEnd() && identifier##Itr->value.IsBool()) \
    {                                                                             \
        actual_config.identifier = identifier##Itr->value.GetBool();              \
    }                                                                             \
    else                                                                          \
        foundEverything = false;

#define SET_JSON_BOOL(identifier)                                           \
    auto identifier##Itr = member.FindMember(#identifier);                  \
    if (identifier##Itr != member.MemberEnd())                              \
    {                                                                       \
        identifier##Itr->value.SetBool(actual_config.identifier);           \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        member.AddMember(#identifier, actual_config.identifier, allocator); \
    }

#define GET_JSON_DOUBLE(identifier)                                                 \
    auto identifier##Itr = member.FindMember(#identifier);                          \
    if (identifier##Itr != member.MemberEnd() && identifier##Itr->value.IsDouble()) \
    {                                                                               \
        actual_config.identifier = identifier##Itr->value.GetDouble();              \
    }                                                                               \
    else                                                                            \
        foundEverything = false;

#define SET_JSON_DOUBLE(identifier)                                         \
    auto identifier##Itr = member.FindMember(#identifier);                  \
    if (identifier##Itr != member.MemberEnd())                              \
    {                                                                       \
        identifier##Itr->value.SetDouble(actual_config.identifier);         \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        member.AddMember(#identifier, actual_config.identifier, allocator); \
    }

#define GET_JSON_ENUM(identifier)                                                                       \
    auto identifier##Itr = member.FindMember(#identifier);                                              \
    if (identifier##Itr != member.MemberEnd() && identifier##Itr->value.IsInt())                        \
    {                                                                                                   \
        actual_config.identifier = (decltype(actual_config.identifier))identifier##Itr->value.GetInt(); \
    }                                                                                                   \
    else                                                                                                \
        foundEverything = false;

#define SET_JSON_ENUM(identifier)                                                \
    auto identifier##Itr = member.FindMember(#identifier);                       \
    if (identifier##Itr != member.MemberEnd())                                   \
    {                                                                            \
        identifier##Itr->value.SetInt((int)actual_config.identifier);            \
    }                                                                            \
    else                                                                         \
    {                                                                            \
        member.AddMember(#identifier, (int)actual_config.identifier, allocator); \
    }

namespace Qosmetics::Walls
{
    Config actual_config;

    Config& Config::get_config() { return actual_config; }
    void WallConfigRegistration::SaveConfig(rapidjson::Value& member, rapidjson::Document::AllocatorType& allocator) const
    {
        INFO("Saving config...");
        SET_JSON_STRING(lastUsedBox);
        SET_JSON_BOOL(forceFakeGlowOff);
        SET_JSON_BOOL(forceCoreOff);
        SET_JSON_BOOL(forceFrameOff);
        SET_JSON_BOOL(disableReflections);
        INFO("Config Saved!");
    }

    bool WallConfigRegistration::LoadConfig(const rapidjson::Value& member) const
    {
        bool foundEverything = true;
        INFO("Loading config...");
        GET_JSON_STRING(lastUsedBox);
        GET_JSON_BOOL(forceFakeGlowOff);
        GET_JSON_BOOL(forceCoreOff);
        GET_JSON_BOOL(forceFrameOff);
        GET_JSON_BOOL(disableReflections);

        if (foundEverything)
            INFO("Config Loaded!");
        return foundEverything;
    }

    void WallConfigRegistration::OnProfileSwitched() const
    {

        INFO("Just Switched profiles!");
        Qosmetics::Walls::SettingsViewController::justChangedProfile = true;
        Qosmetics::Walls::PreviewViewController::justChangedProfile = true;

        auto wallModelContainer = Qosmetics::Walls::WallModelContainer::get_instance();
        if (actual_config.lastUsedBox == "" || actual_config.lastUsedBox == "Default")
        {
            wallModelContainer->Default();
            return;
        }

        std::string filePath = fmt::format("{}/{}.box", box_path, actual_config.lastUsedBox);
        if (!fileexists(filePath))
        {
            wallModelContainer->Default();
            return;
        }

        auto manifest = Qosmetics::Core::Manifest<Qosmetics::Walls::WallObjectConfig>(filePath);
        wallModelContainer->LoadObject(manifest, nullptr);
    }

    QOSMETICS_CONFIG_REGISTER(WallConfigRegistration, "boxConfig");
}