#include "CustomTypes/BoxParent.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxParent);

namespace Qosmetics::Walls
{
    void BoxParent::Awake()
    {
    }

    void BoxParent::OnDestroy()
    {
    }

    void BoxParent::ObstacleDidInit(GlobalNamespace::ObstacleControllerBase* obstacleController)
    {
    }

    void BoxParent::Colorize(GlobalNamespace::ObstacleControllerBase* obstacleController, const Sombrero::FastColor& color)
    {
    }

    void BoxParent::ColorizeSpecific(const Sombrero::FastColor& color)
    {
        if (handler)
            handler->SetColor(color);
    }

}