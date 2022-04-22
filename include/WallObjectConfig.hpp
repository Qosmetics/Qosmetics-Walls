#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "qosmetics-core/shared/Data/BasicConfig.hpp"

#ifndef GET_BOOL
#define GET_BOOL(identifier)                                                              \
    auto identifier##Itr = value.FindMember(#identifier);                                 \
    if (identifier##Itr != value.MemberEnd())                                             \
    {                                                                                     \
        identifier = identifier##Itr->value.IsBool() && identifier##Itr->value.GetBool(); \
    }

#endif

#ifndef ADD_BOOL
#define ADD_BOOL(identifier) val.AddMember(#identifier, identifier, allocator)
#endif

#ifndef GET_DOUBLE
#define GET_DOUBLE(identifier)                                                     \
    auto identifier##Itr = value.FindMember(#identifier);                          \
    if (identifier##Itr != value.MemberEnd() && identifier##Itr->value.IsDouble()) \
    {                                                                              \
        identifier = identifier##Itr->value.GetDouble();                           \
    }

#endif

#ifndef ADD_DOUBLE
#define ADD_DOUBLE(identifier) val.AddMember(#identifier, identifier, allocator)
#endif

#ifndef GET_ENUM
#define GET_ENUM(identifier)                                                    \
    auto identifier##Itr = value.FindMember(#identifier);                       \
    if (identifier##Itr != value.MemberEnd() && identifier##Itr->value.IsInt()) \
    {                                                                           \
        identifier = (decltype(identifier))identifier##Itr->value.GetInt();     \
    }

#endif

#ifndef ADD_ENUM
#define ADD_ENUM(identifier) val.AddMember(#identifier, (int)identifier, allocator)
#endif

#ifndef CONST_GETTER
#define CONST_GETTER(identifier) \
    auto get_##identifier() const { return identifier; }
#endif

namespace Qosmetics::Walls
{
    class WallObjectConfig : public Qosmetics::Core::BasicConfig
    {
    public:
        WallObjectConfig() : Qosmetics::Core::BasicConfig(){};
        WallObjectConfig(bool replaceCoreMaterial, bool replaceFrameMaterial, bool replaceCoreMesh, bool replaceFrameMesh, bool disableFrame, bool disableFakeGlow) : Qosmetics::Core::BasicConfig(), replaceCoreMaterial(replaceCoreMaterial), replaceFrameMaterial(replaceFrameMaterial), replaceCoreMesh(replaceCoreMesh), replaceFrameMesh(replaceFrameMesh), disableFrame(disableFrame), disableFakeGlow(disableFakeGlow), isLegacy(true){};
        WallObjectConfig(const rapidjson::Value& value) : Qosmetics::Core::BasicConfig(value)
        {
            GET_BOOL(replaceCoreMaterial);
            GET_BOOL(replaceFrameMaterial);
            GET_BOOL(replaceCoreMesh);
            GET_BOOL(replaceFrameMesh);
            GET_BOOL(disableCore);
            GET_BOOL(disableFrame);
            GET_BOOL(disableFakeGlow);
            GET_BOOL(isLegacy);
            isDefault = false;
        }

        rapidjson::Value ToJson(rapidjson::Document::AllocatorType& allocator) override
        {
            rapidjson::Value val;
            val.SetObject();
            ADD_BOOL(replaceCoreMaterial);
            ADD_BOOL(replaceFrameMaterial);
            ADD_BOOL(replaceCoreMesh);
            ADD_BOOL(replaceFrameMesh);
            ADD_BOOL(disableCore);
            ADD_BOOL(disableFrame);
            ADD_BOOL(disableFakeGlow);
            ADD_BOOL(isLegacy);
            return val;
        }

        CONST_GETTER(replaceCoreMaterial);
        CONST_GETTER(replaceFrameMaterial);
        CONST_GETTER(replaceCoreMesh);
        CONST_GETTER(replaceFrameMesh);
        CONST_GETTER(disableCore);
        CONST_GETTER(disableFrame);
        CONST_GETTER(disableFakeGlow);
        CONST_GETTER(isLegacy);
        CONST_GETTER(isDefault);

    private:
        bool replaceCoreMaterial = false;
        bool replaceFrameMaterial = false;
        bool replaceCoreMesh = false;
        bool replaceFrameMesh = false;
        bool disableCore = false;
        bool disableFrame = false;
        bool disableFakeGlow = false;
        bool isLegacy = false;
        bool isDefault = true;
    };
}