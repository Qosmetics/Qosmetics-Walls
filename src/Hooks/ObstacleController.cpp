#include "hooking.hpp"

#include "CustomTypes/BoxParent.hpp"
#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

MAKE_AUTO_HOOK_MATCH(ObstacleController_Init, &GlobalNamespace::ObstacleController::Init, void, GlobalNamespace::ObstacleController* self, ::GlobalNamespace::ObstacleData* obstacleData, float worldRotation, ::UnityEngine::Vector3 startPos, ::UnityEngine::Vector3 midPos, ::UnityEngine::Vector3 endPos, float move1Duration, float move2Duration, float singleLineWidth, float height)
{
    ObstacleController_Init(self, obstacleData, worldRotation, startPos, midPos, endPos, move1Duration, move2Duration, singleLineWidth, height);

    auto boxParentItr = Qosmetics::Walls::BoxParent::obstacleControllerToBoxParentMap.find(self);
    if (boxParentItr != Qosmetics::Walls::BoxParent::obstacleControllerToBoxParentMap.end())
        boxParentItr->second->ObstacleDidInit(self);
}

MAKE_AUTO_HOOK_MATCH(MirroredObstacleController_Mirror, &GlobalNamespace::MirroredObstacleController::Mirror, void, GlobalNamespace::MirroredObstacleController* self, GlobalNamespace::ObstacleController* obstacleController)
{
    MirroredObstacleController_Mirror(self, obstacleController);
    auto boxParentItr = Qosmetics::Walls::BoxParent::obstacleControllerToBoxParentMap.find(self);
    if (boxParentItr != Qosmetics::Walls::BoxParent::obstacleControllerToBoxParentMap.end())
        boxParentItr->second->ObstacleDidInit(self);
}