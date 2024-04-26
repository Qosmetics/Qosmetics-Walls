#pragma once

#include "CustomTypes/WallModelContainer.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

#include "Zenject/Installer.hpp"
#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, GameInstaller, ::Zenject::Installer,
                      DECLARE_INSTANCE_FIELD_PRIVATE(WallModelContainer*, _wallModelContainer);
                      DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplayCoreSceneSetupData*, _gameplayCoreSceneSetupData);

                      DECLARE_PRIVATE_METHOD(GlobalNamespace::ObstacleController*, DecorateWall, GlobalNamespace::ObstacleController* original);
                      DECLARE_PRIVATE_METHOD(GlobalNamespace::MirroredObstacleController*, DecorateMirroredWall, GlobalNamespace::MirroredObstacleController* original);

                      DECLARE_OVERRIDE_METHOD_MATCH(void, InstallBindings, &::Zenject::Installer::InstallBindings);
                      DECLARE_CTOR(ctor, WallModelContainer* wallModelContainer, GlobalNamespace::GameplayCoreSceneSetupData* gameplayCoreSceneSetupData);

                      static constexpr const int DECORATION_PRIORITY = 300;

)
