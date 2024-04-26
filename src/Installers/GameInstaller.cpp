#include "Installers/GameInstaller.hpp"
#include "ConstStrings.hpp"
#include "Disabling.hpp"
#include "PropertyID.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "CustomTypes/BoxColorHandler.hpp"
#include "CustomTypes/BoxHandler.hpp"
#include "CustomTypes/BoxParent.hpp"

#include "GlobalNamespace/BoolSO.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ConditionalActivation.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/FakeMirrorObjectsInstaller.hpp"

#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/DiContainer.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Transform.hpp"

#include "lapiz/shared/objects/Beatmap.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastQuaternion.hpp"
#include "sombrero/shared/FastVector3.hpp"

#if __has_include("chroma/shared/ObstacleAPI.hpp")
#include "chroma/shared/ObstacleAPI.hpp"
#ifndef HAS_CHROMA
#define HAS_CHROMA
#endif
#endif

DEFINE_TYPE(Qosmetics::Walls, GameInstaller);

namespace Qosmetics::Walls
{
    void GameInstaller::ctor(WallModelContainer* wallModelContainer, GlobalNamespace::GameplayCoreSceneSetupData* gameplayCoreSceneSetupData)
    {
        _wallModelContainer = wallModelContainer;
        _gameplayCoreSceneSetupData = gameplayCoreSceneSetupData;
    }

    void GameInstaller::InstallBindings()
    {
        if (Disabling::GetAnyDisabling())
            return;

        auto& config = _wallModelContainer->GetWallConfig();
#ifdef HAS_CHROMA
        auto& globalConfig = Qosmetics::Walls::Config::get_config();
        bool hasCustoms = _wallModelContainer->currentWallObject;

        Chroma::ObstacleAPI::setObstacleColorable(hasCustoms);
#endif
        auto container = get_Container();

        Lapiz::Objects::Registration<GlobalNamespace::ObstacleController*, GlobalNamespace::BeatmapObjectsInstaller*>::New_ctor("obstaclePrefab", std::bind(&GameInstaller::DecorateWall, this, std::placeholders::_1), DECORATION_PRIORITY)->RegisterRedecorator(container);
        Lapiz::Objects::Registration<GlobalNamespace::MirroredObstacleController*, GlobalNamespace::FakeMirrorObjectsInstaller*>::New_ctor("mirroredObstacleControllerPrefab", std::bind(&GameInstaller::DecorateMirroredWall, this, std::placeholders::_1), DECORATION_PRIORITY)->RegisterRedecorator(container);
    }

    GlobalNamespace::ObstacleController* GameInstaller::DecorateWall(GlobalNamespace::ObstacleController* original)
    {
        auto t = original->get_transform();
        auto obstacleCoreT = t->Find(ConstStrings::ObstacleCore());
        auto depthWriteT = obstacleCoreT->Find(ConstStrings::DepthWrite());

        depthWriteT->get_gameObject()->GetComponent<UnityEngine::Renderer*>()->set_enabled(false);

        auto hideWrapperT = t->Find(ConstStrings::HideWrapper());
        auto obstacleFrameT = hideWrapperT->Find(ConstStrings::ObstacleFrame());
        auto obstacleFakeGlowT = hideWrapperT->Find(ConstStrings::ObstacleFakeGlow());

        auto boxParent = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxParent*>();
        auto boxHandler = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxHandler*>();
        boxParent->Handler = boxHandler;
        auto boxColorHandler = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxColorHandler*>();

        auto& globalConfig = Qosmetics::Walls::Config::get_config();

        auto frameFilter = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
        auto fakeGlowFilter = obstacleFakeGlowT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
        auto coreFilter = obstacleCoreT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();

        auto conditionalActivation = obstacleFrameT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalActivation*>();
        // conditionalActivation->value->set_value(true);
        conditionalActivation->_activateOnFalse = !conditionalActivation->_activateOnFalse;

        auto conditionalActivation2 = obstacleFakeGlowT->get_gameObject()->GetComponent<GlobalNamespace::ConditionalActivation*>();
        // conditionalActivation2->value->set_value(true);
        conditionalActivation2->_activateOnFalse = !conditionalActivation2->_activateOnFalse;
        if (_wallModelContainer->CurrentWallObject)
        {
            DEBUG("There's a wall selected");
            auto& config = _wallModelContainer->GetWallConfig();

            auto frameRenderer = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
            auto coreRenderer = obstacleCoreT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();

            auto customCoreT = _wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Core());
            auto customFrameT = _wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Frame());

#ifdef HAS_CHROMA
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
                    frameRenderer->SetMaterialArray(customFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->GetMaterialArray().convert());
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
                    coreRenderer->SetMaterialArray(customCoreT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>()->GetMaterialArray().convert());
                }
            }
            auto colorScheme = _gameplayCoreSceneSetupData->colorScheme;
            boxHandler->SetColor(colorScheme->get_obstaclesColor());
        }
        else
        {
            DEBUG("no custom wall");
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

        return original;
    }

    GlobalNamespace::MirroredObstacleController* GameInstaller::DecorateMirroredWall(GlobalNamespace::MirroredObstacleController* original)
    {
        DEBUG("original: {}", fmt::ptr(original));
        auto t = original->get_transform();

        auto obstacleFrameT = t->Find(ConstStrings::ObstacleFrame());

        auto boxParent = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxParent*>();
        auto boxHandler = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxHandler*>();
        boxParent->Handler = boxHandler;
        auto boxColorHandler = original->get_gameObject()->AddComponent<Qosmetics::Walls::BoxColorHandler*>();

        auto& globalConfig = Qosmetics::Walls::Config::get_config();
        auto& config = _wallModelContainer->GetWallConfig();

        auto frameFilter = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshFilter*>();
        if (globalConfig.disableReflections)
        {
            frameFilter->set_mesh(nullptr);
        }
        else if (_wallModelContainer->CurrentWallObject && config.get_isMirrorable())
        {
            DEBUG("There's a wall selected");

            auto frameRenderer = obstacleFrameT->get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();

            auto customFrameT = _wallModelContainer->CurrentWallObject->get_transform()->Find(ConstStrings::Frame());

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
                conditionalActivation->_value->set_value(true);
                conditionalActivation->_activateOnFalse = false;

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
                    frameRenderer->SetMaterialArray(materials.convert());
                }
            }
            auto colorScheme = _gameplayCoreSceneSetupData->colorScheme;

            boxHandler->SetColor(colorScheme->get_obstaclesColor());
        }
        else
        {
            if (globalConfig.forceFrameOff) // frame needs to be off
                frameFilter->set_mesh(nullptr);
        }

        return original;
    }
}
