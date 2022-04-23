#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Mesh.hpp"
#include "WallsConfig.hpp"
#include "conditional-dependencies/shared/main.hpp"
#include <optional>
#if __has_include("qosmetics-core/shared/Data/Descriptor.hpp")
#include "qosmetics-core/shared/Data/Descriptor.hpp"
#ifndef QOSMETICS_CORE
#define QOSMETICS_CORE
#endif
#endif

#define QOSMETICS_WALLS "qosmetics-walls"

namespace Qosmetics::Walls::API
{

// only available if you also have qosmetics core as a dep
#ifdef QOSMETICS_CORE
    /// @brief optionally get the active wall descriptor
    /// @return std::nullopt if not installed, descriptor if installed (check name to see if it's default)
    std::optional<Qosmetics::Core::Descriptor> GetActiveDescriptor()
    {
        static auto function = CondDeps::Find<Qosmetics::Core::Descriptor>(QOSMETICS_WALLS, "GetActiveDescriptor");
        if (function)
            return function.value()();
        return std::nullopt;
    }
#endif

    /// @brief optionally get a const reference to the Qosmetics Config
    /// @return std::nullopt if not installed, const reference wrapper otherwise
    std::optional<std::reference_wrapper<const Qosmetics::Walls::WallsConfig>> GetConfig()
    {
        static auto function = CondDeps::Find<Qosmetics::Walls::WallsConfig*>(QOSMETICS_WALLS, "GetConfig");
        if (function)
            return *function.value()();
        return std::nullopt;
    }

    /// @brief Set the default wall as being active
    void SetDefault()
    {
        static auto function = CondDeps::Find<void>(QOSMETICS_WALLS, "SetDefault");
        if (function)
            function.value()();
    }

    /// @brief Set a specific wall as being active, by passing the path of the object
    /// @param filePath the path of the object
    /// @return std::nullopt if not installed, false if no new object got loaded, true if a new object got loaded
    std::optional<bool> SetActiveFromFilePath(std::string filePath)
    {
        static auto function = CondDeps::Find<bool, std::string>(QOSMETICS_WALLS, "SetActiveFromFilePath");
        if (function)
            return function.value()(filePath);
        return std::nullopt;
    }

    /// @brief Set a specific wall as being active, by passing the filename of the object (with extension!)
    /// @param obj the filename of the object
    /// @return std::nullopt if not installed, false if no new object got loaded, true if a new object got loaded
    std::optional<bool> SetActive(std::string fileName)
    {
        static auto function = CondDeps::Find<bool, std::string>(QOSMETICS_WALLS, "SetActive");
        if (function)
            return function.value()(fileName);
        return std::nullopt;
    }

    /// @brief optionally gets a reference to the core material
    /// @return std::nullopt if not installed, nullptr if not available, value if found
    std::optional<ArrayW<UnityEngine::Material*>> GetCoreMaterials()
    {
        static auto function = CondDeps::Find<ArrayW<UnityEngine::Material*>>(QOSMETICS_WALLS, "GetCoreMaterials");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief optionally gets a reference to the frame material
    /// @return std::nullopt if not installed, nullptr if not available, value if found
    std::optional<ArrayW<UnityEngine::Material*>> GetFrameMaterials()
    {
        static auto function = CondDeps::Find<ArrayW<UnityEngine::Material*>>(QOSMETICS_WALLS, "GetFrameMaterials");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief optionally gets a reference to the core mesh
    /// @return std::nullopt if not installed, nullptr if not available, value if found
    std::optional<UnityEngine::Mesh*> GetCoreMesh()
    {
        static auto function = CondDeps::Find<UnityEngine::Mesh*>(QOSMETICS_WALLS, "GetCoreMesh");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief optionally gets a reference to the frame mesh
    /// @return std::nullopt if not installed, nullptr if not available, value if found
    std::optional<UnityEngine::Mesh*> GetFrameMesh()
    {
        static auto function = CondDeps::Find<UnityEngine::Mesh*>(QOSMETICS_WALLS, "GetFrameMesh");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief optionally get a clone of the entire prefab
    /// @return std::nullopt if not installed, nullptr if default, if there is debris a gameobject pointer to a cloned wall object
    std::optional<UnityEngine::GameObject*> GetPrefabClone()
    {
        static auto function = CondDeps::Find<UnityEngine::GameObject*>(QOSMETICS_WALLS, "GetPrefabClone");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief optionally get the entire prefab (Please don't edit to break things!)
    /// @return std::nullopt if not installed, nullptr if default, if there is something selected a pointer to the original object
    std::optional<UnityEngine::GameObject*> GetPrefab()
    {
        static auto function = CondDeps::Find<UnityEngine::GameObject*>(QOSMETICS_WALLS, "GetPrefab");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief checks if the current wall is a custom one or default one
    /// @return std::nullopt if not installed, false if not custom, true if custom
    std::optional<bool> GetWallIsCustom()
    {
        static auto function = CondDeps::Find<bool>(QOSMETICS_WALLS, "GetWallIsCustom");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief checks if walls are disabled due to a mod registering itself as disabling
    /// @return std::nullopt if not installed, false if not disabled, true if disabled
    std::optional<bool> GetWallsDisabled()
    {
        static auto function = CondDeps::Find<bool>(QOSMETICS_WALLS, "GetWallsDisabled");
        if (function)
            return function.value()();
        return std::nullopt;
    }

    /// @brief Unregister your mod as disabling Walls replacements
    /// @param info your mod info
    void UnregisterWallDisablingInfo(ModInfo info)
    {
        static auto function = CondDeps::Find<void, ModInfo>(QOSMETICS_WALLS, "UnregisterWallDisablingInfo");
        if (function)
            function.value()(info);
    }

    /// @brief register your mod as disabling Walls replacements
    /// @param info your mod info
    void RegisterWallDisablingInfo(ModInfo info)
    {
        static auto function = CondDeps::Find<void, ModInfo>(QOSMETICS_WALLS, "RegisterWallDisablingInfo");
        if (function)
            function.value()(info);
    }

    /// @brief get the wall folder
    /// @return std::nullopt if not installed, else folder path for wall folder
    std::optional<std::string> GetWallFolder()
    {
        static auto function = CondDeps::Find<std::string>(QOSMETICS_WALLS, "GetWallFolder");
        if (function)
            return function.value()();
        return std::nullopt;
    }

}