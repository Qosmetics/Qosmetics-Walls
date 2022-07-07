#include "CustomTypes/BoxHandler.hpp"

#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxHandler);

namespace Qosmetics::Walls
{

    void BoxHandler::Awake()
    {
        colorHandler = GetComponentInChildren<BoxColorHandler*>();
        obstacleController = GetComponent<GlobalNamespace::ObstacleControllerBase*>();
    }

    void BoxHandler::SetColor(const Sombrero::FastColor& color)
    {
        if (colorHandler)
            colorHandler->SetColor(color);

        if (obstacleController)
        {
            auto stretchableObstacle = CRASH_UNLESS(il2cpp_utils::GetFieldValue<GlobalNamespace::StretchableObstacle*>(obstacleController, "_stretchableObstacle"));
            auto frameController = stretchableObstacle->obstacleFrame;
            stretchableObstacle->SetSizeAndColor(frameController->width, frameController->height, frameController->length, color);
        }
    }

}