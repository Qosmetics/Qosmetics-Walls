#include "CustomTypes/BoxHandler.hpp"

#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxHandler);

namespace Qosmetics::Walls
{

    void BoxHandler::Awake()
    {
        colorHandler = GetComponentInChildren<BoxColorHandler*>();
        obstacleController = GetComponent<GlobalNamespace::ObstacleController*>();
    }

    void BoxHandler::SetColor(const Sombrero::FastColor& color)
    {
        if (colorHandler)
            colorHandler->SetColor(color);

        if (obstacleController)
        {
            auto stretchableObstacle = obstacleController->dyn__stretchableObstacle();
            auto frameController = stretchableObstacle->dyn__obstacleFrame();
            stretchableObstacle->SetSizeAndColor(frameController->dyn_width(), frameController->dyn_height(), frameController->dyn_length(), color);
        }
    }

}