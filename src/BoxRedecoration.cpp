#include "logging.hpp"

#include "ConstStrings.hpp"
#include "CustomTypes/BoxColorHandler.hpp"
#include "CustomTypes/BoxHandler.hpp"
#include "CustomTypes/BoxParent.hpp"
#include "CustomTypes/WallModelContainer.hpp"
#include "Disabling.hpp"
#include "PropertyID.hpp"
#include "config.hpp"

#include "GlobalNamespace/BeatmapObjectsInstaller.hpp"
#include "GlobalNamespace/BoolSO.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ConditionalActivation.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/FakeMirrorObjectsInstaller.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"
#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Transform.hpp"

#if __has_include("chroma/shared/ObstacleAPI.hpp")
#include "chroma/shared/ObstacleAPI.hpp"
#ifndef HAS_CHROMA
#define HAS_CHROMA 1
#endif
#endif

/*
    ObstaclePrefab
        L HideWrapper
        |   L ObstacleFrame
        |   L ObstacleFakeGlow
        L ObstacleCore
            L Collider
            L DepthWrite
*/

/*
REDECORATION_REGISTRATION(obstaclePrefab, 10, true, GlobalNamespace::ObstacleController*, GlobalNamespace::BeatmapObjectsInstaller*)
{
    if (Qosmetics::Walls::Disabling::GetAnyDisabling())
        return obstaclePrefab;

    INFO("Redecorating Wall");
    auto t = obstaclePrefab->get_transform();
    auto obstacleCoreT = t->Find(ConstStrings::ObstacleCore());
    auto depthWriteT = obstacleCoreT->Find(ConstStrings::DepthWrite());

    depthWriteT->get_gameObject()->GetComponent<UnityEngine::Renderer*>()->set_enabled(false);

    auto hideWrapperT = t->Find(ConstStrings::HideWrapper());
    auto obstacleFrameT = hideWrapperT->Find(ConstStrings::ObstacleFrame());
    auto obstacleFakeGlowT = hideWrapperT->Find(ConstStrings::ObstacleFakeGlow());

    auto boxParent = obstaclePrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxParent*>();
    auto boxHandler = obstaclePrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxHandler*>();
    boxParent->handler = boxHandler;
    auto boxColorHandler = obstaclePrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxColorHandler*>();

    auto wallModelContainer = Qosmetics::Walls::WallModelContainer::get_instance();
    auto& globalConfig = Qosmetics::Walls::Config::get_config();

    auto frameFilter = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    auto fakeGlowFilter = obstacleFakeGlowT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    auto coreFilter = obstacleCoreT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();

    auto conditionalActivation = obstacleFrameT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalActivation*>();
    // conditionalActivation->value->set_value(true);
    conditionalActivation->activateOnFalse = !conditionalActivation->activateOnFalse;

    auto conditionalActivation2 = obstacleFakeGlowT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalActivation*>();
    // conditionalActivation2->value->set_value(true);
    conditionalActivation2->activateOnFalse = !conditionalActivation2->activateOnFalse;

    if (wallModelContainer->currentWallObject)
    {
        DEBUG("There's a wall selected");
        auto& config = wallModelContainer->GetWallConfig();

        auto frameRenderer = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
        auto coreRenderer = obstacleCoreT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();

        auto customCoreT = wallModelContainer->currentWallObject->get_transform()->Find(ConstStrings::Core());
        auto customFrameT = wallModelContainer->currentWallObject->get_transform()->Find(ConstStrings::Frame());

#ifdef HAS_CHROMA
        Chroma::ObstacleAPI::setObstacleColorable(true);

        auto callbackOpt = Chroma::ObstacleAPI::getObstacleChangedColorCallbackSafe();
        if (callbackOpt)
        {
            auto& callbackRef = callbackOpt.value().get();
            callbackRef += &Qosmetics::Walls::BoxParent::Colorize;
        }
#endif

        if (config.get_disableFrame() || globalConfig.forceFrameOff) // frame needs to be off
        {
            DEBUG("Disable frame");
            frameFilter->set_mesh(nullptr);
            fakeGlowFilter->set_mesh(nullptr);
        }
        else
        {
            DEBUG("keep frame");
            // doing this lets us keep the frame on

            if (config.get_disableFakeGlow() || globalConfig.forceFakeGlowOff)
            {
                DEBUG("Disable fake glow");
                fakeGlowFilter->set_mesh(nullptr);
            }
            if (config.get_replaceFrameMesh())
            {
                DEBUG("Replace mesh");
                frameFilter->set_mesh(customFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>()->get_sharedMesh());
            }
            if (config.get_replaceFrameMaterial())
            {
                DEBUG("Replace frame material");
                frameRenderer->SetMaterialArray(customFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->GetMaterialArray());
            }
        }

        if (config.get_disableCore() || globalConfig.forceCoreOff) // core needs to be off
        {
            DEBUG("Disable core");
            coreFilter->set_mesh(nullptr);
        }
        else
        {
            DEBUG("keep core");
            if (config.get_replaceCoreMesh())
            {
                DEBUG("Replace mesh");
                coreFilter->set_mesh(customCoreT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>()->get_sharedMesh());
            }
            if (config.get_replaceCoreMaterial())
            {
                DEBUG("Replace core material");
                UnityEngine::Object::DestroyImmediate(obstacleCoreT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalMaterialSwitcher*>());
                coreRenderer->SetMaterialArray(customCoreT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->GetMaterialArray());
            }
        }
        auto gameplayCoreSceneSetupData = container->TryResolve<GlobalNamespace::GameplayCoreSceneSetupData*>();
        auto colorScheme = gameplayCoreSceneSetupData->colorScheme;

        boxHandler->SetColor(colorScheme->get_obstaclesColor());
    }
    else
    {
        DEBUG("no custom wall");
#ifdef HAS_CHROMA
        Chroma::ObstacleAPI::setObstacleColorable(false);
#endif
        if (globalConfig.forceFrameOff) // frame needs to be off
        {
            DEBUG("Frame Off");
            frameFilter->set_mesh(nullptr);
            fakeGlowFilter->set_mesh(nullptr);
        }
        else if (globalConfig.forceFakeGlowOff) // just fake glow off
        {
            DEBUG("Fake Glow Off");
            fakeGlowFilter->set_mesh(nullptr);
        }
        if (globalConfig.forceCoreOff) // core needs to be off
        {
            DEBUG("Core Off");
            coreFilter->set_mesh(nullptr);
        }
    }

    return obstaclePrefab;
}

REDECORATION_REGISTRATION(mirroredObstacleControllerPrefab, 10, true, GlobalNamespace::MirroredObstacleController*, GlobalNamespace::FakeMirrorObjectsInstaller*)
{
    if (Qosmetics::Walls::Disabling::GetAnyDisabling())
        return mirroredObstacleControllerPrefab;

    auto t = mirroredObstacleControllerPrefab->get_transform();

    auto obstacleFrameT = t->Find(ConstStrings::ObstacleFrame());

    auto boxParent = mirroredObstacleControllerPrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxParent*>();
    auto boxHandler = mirroredObstacleControllerPrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxHandler*>();
    boxParent->handler = boxHandler;
    auto boxColorHandler = mirroredObstacleControllerPrefab->get_gameObject()->AddComponent<Qosmetics::Walls::BoxColorHandler*>();

    auto wallModelContainer = Qosmetics::Walls::WallModelContainer::get_instance();
    auto& globalConfig = Qosmetics::Walls::Config::get_config();
    auto& config = wallModelContainer->GetWallConfig();

    auto frameFilter = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
    if (globalConfig.disableReflections)
    {
        frameFilter->set_mesh(nullptr);
    }
    else if (wallModelContainer->currentWallObject && config.get_isMirrorable())
    {
        DEBUG("There's a wall selected");

        auto frameRenderer = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();

        auto customFrameT = wallModelContainer->currentWallObject->get_transform()->Find(ConstStrings::Frame());

        if (config.get_disableFrame() || globalConfig.forceFrameOff) // frame needs to be off
        {
            DEBUG("Disable frame");
            frameFilter->set_mesh(nullptr);
        }
        else
        {
            DEBUG("keep frame");
            // doing this lets us keep the frame on
            auto conditionalActivation = obstacleFrameT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalActivation*>();
            conditionalActivation->value->set_value(true);
            conditionalActivation->activateOnFalse = false;

            if (config.get_replaceFrameMesh())
            {
                DEBUG("Replace mesh");
                frameFilter->set_mesh(customFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>()->get_sharedMesh());
            }
            if (config.get_replaceFrameMaterial())
            {
                DEBUG("Replace frame material");
                auto materials = customFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->GetSharedMaterialArray();
                for (auto material : materials)
                {
                    material->set_renderQueue(1951);
                    if (material->HasProperty(PropertyID::_Alpha()))
                        material->SetFloat(PropertyID::_Alpha(), 0.05f);
                    if (material->HasProperty(PropertyID::_StencilRefID()))
                        material->SetFloat(PropertyID::_StencilRefID(), 1);
                    if (material->HasProperty(PropertyID::_StencilComp()))
                        material->SetFloat(PropertyID::_StencilComp(), 3);
                    if (material->HasProperty(PropertyID::_StencilOp()))
                        material->SetFloat(PropertyID::_StencilOp(), 0);

                    if (material->HasProperty(PropertyID::_BlendDstFactor()))
                        material->SetFloat(PropertyID::_BlendDstFactor(), 10);
                    if (material->HasProperty(PropertyID::_BlendDstFactorA()))
                        material->SetFloat(PropertyID::_BlendDstFactorA(), 0);
                    if (material->HasProperty(PropertyID::_BlendSrcFactor()))
                        material->SetFloat(PropertyID::_BlendSrcFactor(), 5);
                    if (material->HasProperty(PropertyID::_BlendSrcFactorA()))
                        material->SetFloat(PropertyID::_BlendSrcFactorA(), 0);
                }
                frameRenderer->SetMaterialArray(materials);
            }
        }
        auto gameplayCoreSceneSetupData = container->TryResolve<GlobalNamespace::GameplayCoreSceneSetupData*>();
        auto colorScheme = gameplayCoreSceneSetupData->colorScheme;

        boxHandler->SetColor(colorScheme->get_obstaclesColor());
    }
    else
    {
        if (globalConfig.forceFrameOff) // frame needs to be off
            frameFilter->set_mesh(nullptr);
    }

    return mirroredObstacleControllerPrefab;
}
*/