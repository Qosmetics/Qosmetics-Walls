#include "CustomTypes/WallModelContainer.hpp"

#include "config.hpp"
#include "logging.hpp"
#include "qosmetics-core/shared/Utils/BundleUtils.hpp"
#include "qosmetics-core/shared/Utils/FileUtils.hpp"
#include "qosmetics-core/shared/Utils/ZipUtils.hpp"
#include "static-defines.hpp"
#include <utility>

#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "UnityEngine/Coroutine.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/TextAsset.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Text.hpp"

#include "ConstStrings.hpp"

#include "MaterialUtils.hpp"
#include "QwallConversion.hpp"

DEFINE_TYPE(Qosmetics::Walls, WallModelContainer);

void SetLayerRecursively(UnityEngine::Transform* obj, int layer)
{
    obj->get_gameObject()->set_layer(layer);
    int childCount = obj->get_childCount();
    for (int i = 0; i < childCount; i++)
        SetLayerRecursively(obj->GetChild(i), layer);
}

void LegacyFixups(UnityEngine::GameObject* loadedObject) {}

void AddHandlers(UnityEngine::GameObject* loadedObject)
{
}

namespace Qosmetics::Walls
{
    WallModelContainer* WallModelContainer::instance = nullptr;
    WallModelContainer* WallModelContainer::get_instance()
    {
        if (instance)
            return instance;
        auto go = UnityEngine::GameObject::New_ctor(StringW(___TypeRegistration::get()->name()));
        UnityEngine::Object::DontDestroyOnLoad(go);
        return go->AddComponent<WallModelContainer*>();
    }

    void WallModelContainer::ctor()
    {
        DEBUG("Created WallModelContainer instance: {}", fmt::ptr(this));
        instance = this;
        bundle = nullptr;
        isLoading = false;
        currentWallObject = nullptr;
        currentManifest = Qosmetics::Core::Manifest<Qosmetics::Walls::WallObjectConfig>();
    }

    void WallModelContainer::Start()
    {
        auto lastUsedDodger = Qosmetics::Walls::Config::get_config().lastUsedDodger;
        if (lastUsedDodger == "" || lastUsedDodger == "Default")
            return;

        std::string filePath = fmt::format("{}/{}.dodger", dodger_path, lastUsedDodger);
        if (!fileexists(filePath))
            return;
        currentManifest = Qosmetics::Core::Manifest<WallObjectConfig>(filePath);
        INFO("Loading Wall Object {}", currentManifest.get_descriptor().get_name());
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(LoadBundleRoutine(nullptr)));
    }

    bool WallModelContainer::LoadObject(const Qosmetics::Core::Manifest<Qosmetics::Walls::WallObjectConfig>& manifest, std::function<void(WallModelContainer*)> onFinished)
    {
        if (isLoading)
        {
            ERROR("Was still loading object, not loading new object...");
            return false;
        }
        if (manifest.get_filePath() == currentManifest.get_filePath())
        {
            ERROR("Tried loading object {} again, not loading new object", manifest.get_filePath());
            return false;
        }

        currentManifest = manifest;
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(LoadBundleRoutine(std::move(onFinished))));
        return true;
    }

    bool WallModelContainer::LoadObject(const Qosmetics::Core::Descriptor& descriptor, std::function<void(WallModelContainer*)> onFinished)
    {
        if (isLoading)
            return false;
        if (descriptor.get_filePath() == currentManifest.get_filePath())
            return false;
        INFO("Loading Wall Object {}", descriptor.get_name());
        currentManifest = Qosmetics::Core::Manifest<WallObjectConfig>(descriptor.get_filePath());
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(LoadBundleRoutine(std::move(onFinished))));
        return true;
    }

    const WallObjectConfig& WallModelContainer::GetWallConfig()
    {
        return currentManifest.get_config();
    }

    const Qosmetics::Core::Descriptor& WallModelContainer::GetDescriptor()
    {
        return currentManifest.get_descriptor();
    }

    custom_types::Helpers::Coroutine WallModelContainer::LoadBundleRoutine(std::function<void(WallModelContainer*)> onFinished)
    {
        isLoading = true;
        if (currentWallObject)
            Object::DestroyImmediate(currentWallObject);
        if (bundle)
            bundle->Unload(true);

        DEBUG("Loading file {} from dodger {}", currentManifest.get_fileName(), currentManifest.get_filePath());
        co_yield custom_types::Helpers::CoroutineHelper::New(Qosmetics::Core::BundleUtils::LoadBundleFromZipAsync(currentManifest.get_filePath(), currentManifest.get_fileName(), bundle));

        bool isLegacy = currentManifest.get_config().get_isLegacy();
        DEBUG("Loading {}Dodger", isLegacy ? "legacy " : "");
        co_yield custom_types::Helpers::CoroutineHelper::New(Qosmetics::Core::BundleUtils::LoadAssetFromBundleAsync<UnityEngine::GameObject*>(bundle, isLegacy ? "_CustomWall" : "_Dodger", currentWallObject));

        if (!currentWallObject)
            ERROR("Failed to load dodger from bundle!");
        auto name = currentWallObject->get_name();
        currentWallObject = UnityEngine::Object::Instantiate(currentWallObject, get_transform());

        SetLayerRecursively(currentWallObject->get_transform(), 12);
        currentWallObject->set_name(name);
        currentWallObject->SetActive(false);

        // Prewarm all shaders
        MaterialUtils::PrewarmAllShadersOnObject(currentWallObject);

        if (isLegacy)
        {
            DEBUG("Executing legacy object fixups");
            LegacyFixups(currentWallObject);
        }

        DEBUG("Adding handlers to object");
        AddHandlers(currentWallObject);

        isLoading = false;
        if (onFinished)
            onFinished(this);

        co_return;
    }

    bool WallModelContainer::Default()
    {
        if (isLoading)
            return false;
        if (currentWallObject)
        {
            Object::DestroyImmediate(currentWallObject);
            currentWallObject = nullptr;
        }
        if (bundle)
        {
            bundle->Unload(true);
            bundle = nullptr;
        }

        currentManifest = decltype(currentManifest)();
        return true;
    }

    void WallModelContainer::OnDestroy()
    {
        instance = nullptr;
        UnloadBundle();
    }

    void WallModelContainer::UnloadBundle()
    {
        if (bundle)
            bundle->Unload(false);
        bundle = nullptr;
    }

    void WallModelContainer::OnGameRestart()
    {
        if (currentWallObject)
        {
            Object::DestroyImmediate(currentWallObject);
            currentWallObject = nullptr;
        }
        if (bundle)
        {
            bundle->Unload(true);
            bundle = nullptr;
        }

        instance = nullptr;
        UnityEngine::Object::DestroyImmediate(this->get_gameObject());
    }
}