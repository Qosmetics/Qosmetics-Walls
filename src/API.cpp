#include "API.hpp"
#include "ConstStrings.hpp"
#include "Disabling.hpp"
#include "conditional-dependencies/shared/main.hpp"
#include "config.hpp"
#include "qosmetics-core/shared/Data/Descriptor.hpp"
#include "static-defines.hpp"
#include <fmt/core.h>

#include "CustomTypes/WallModelContainer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"

#define WALLMODELCONTAINER getModelContainer()
#define GET_WALLMODELCONTAINER() auto wallModelContainer = WALLMODELCONTAINER

auto getModelContainer()
{
    return UnityEngine::Resources::FindObjectsOfTypeAll<Qosmetics::Walls::WallModelContainer*>()->FirstOrDefault();
}

EXPOSE_API(GetActiveDescriptor, Qosmetics::Core::Descriptor)
{
    return WALLMODELCONTAINER->GetDescriptor();
}

EXPOSE_API(GetConfig, Qosmetics::Walls::WallsConfig*)
{
    return reinterpret_cast<Qosmetics::Walls::WallsConfig*>(&Qosmetics::Walls::Config::get_config());
}

EXPOSE_API(SetDefault, void)
{
    WALLMODELCONTAINER->Default();
}

EXPOSE_API(SetActiveFromFilePath, bool, std::string filePath)
{
    if (!fileexists(filePath))
        return false;
    auto manifest = Qosmetics::Walls::WallModelContainer::Manifest(filePath);
    return WALLMODELCONTAINER->LoadObject(manifest, nullptr);
}

EXPOSE_API(SetActive, bool, std::string fileName)
{
    std::string filePath = fmt::format("{}/{}", box_path, fileName);
    if (!fileexists(filePath))
        return false;
    auto manifest = Qosmetics::Walls::WallModelContainer::Manifest(filePath);
    return WALLMODELCONTAINER->LoadObject(manifest, nullptr);
}

EXPOSE_API(GetCoreMaterials, ArrayW<UnityW<UnityEngine::Material>>)
{
    GET_WALLMODELCONTAINER();
    if (!wallModelContainer->CurrentWallObject)
        return nullptr;

    auto core = wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Core());
    if (!core)
        return nullptr;
    auto meshRenderer = core->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
    if (!meshRenderer)
        return nullptr;
    return meshRenderer->GetMaterialArray();
}

EXPOSE_API(GetFrameMaterial, ArrayW<UnityW<UnityEngine::Material>>)
{
    GET_WALLMODELCONTAINER();
    if (!wallModelContainer->CurrentWallObject)
        return nullptr;

    auto frame = wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Frame());
    if (!frame)
        return nullptr;
    auto meshRenderer = frame->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
    if (!meshRenderer)
        return nullptr;
    return meshRenderer->GetMaterialArray();
}

EXPOSE_API(GetCoreMesh, UnityEngine::Mesh*)
{
    GET_WALLMODELCONTAINER();
    if (!wallModelContainer->CurrentWallObject)
        return nullptr;

    auto core = wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Core());
    if (!core)
        return nullptr;
    auto meshFilter = core->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    if (!meshFilter)
        return nullptr;
    return meshFilter->get_sharedMesh();
}

EXPOSE_API(GetFrameMesh, UnityEngine::Mesh*)
{
    GET_WALLMODELCONTAINER();
    if (!wallModelContainer->CurrentWallObject)
        return nullptr;

    auto frame = wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Frame());
    if (!frame)
        return nullptr;
    auto meshFilter = frame->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    if (!meshFilter)
        return nullptr;
    return meshFilter->get_sharedMesh();
}

EXPOSE_API(GetPrefabClone, UnityEngine::GameObject*)
{
    GET_WALLMODELCONTAINER();

    if (!wallModelContainer->CurrentWallObject)
        return nullptr;
    return UnityEngine::Object::Instantiate(wallModelContainer->CurrentWallObject);
}

EXPOSE_API(GetPrefab, UnityEngine::GameObject*)
{
    return WALLMODELCONTAINER->CurrentWallObject;
}

EXPOSE_API(GetWallIsCustom, bool)
{
    return WALLMODELCONTAINER->CurrentWallObject != nullptr;
}

EXPOSE_API(GetWallsDisabled, bool)
{
    return Qosmetics::Walls::Disabling::GetAnyDisabling();
}

EXPOSE_API(UnregisterWallDisablingInfo, void, modloader::ModInfo info)
{
    Qosmetics::Walls::Disabling::UnregisterDisablingModInfo(info);
}

EXPOSE_API(RegisterWallDisablingInfo, void, modloader::ModInfo info)
{
    Qosmetics::Walls::Disabling::RegisterDisablingModInfo(info);
}

EXPOSE_API(GetWallFolder, std::string)
{
    return box_path;
}
#pragma GCC diagnostic pop
