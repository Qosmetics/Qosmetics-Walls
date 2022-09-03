#include "Installers/GameInstaller.hpp"
#include "ConstStrings.hpp"
#include "Disabling.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "GlobalNamespace/FakeMirrorObjectsInstaller.hpp"

#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/DiContainer.hpp"

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
        return original;
    }

    GlobalNamespace::MirroredObstacleController* GameInstaller::DecorateMirroredWall(GlobalNamespace::MirroredObstacleController* original)
    {
        return original;
    }
}