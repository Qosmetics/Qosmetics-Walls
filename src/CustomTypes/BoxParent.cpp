#include "CustomTypes/BoxParent.hpp"

#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxParent);

#if __has_include("chroma/shared/ObstacleAPI.hpp")
#include "chroma/shared/ObstacleAPI.hpp"
#ifndef HAS_CHROMA
#define HAS_CHROMA 1
#endif
#endif

namespace Qosmetics::Walls
{
    std::map<GlobalNamespace::ObstacleControllerBase*, BoxParent*> BoxParent::obstacleControllerToBoxParentMap = {};

    void BoxParent::Awake()
    {
        obstacleController = GetComponent<GlobalNamespace::ObstacleControllerBase*>();
        obstacleControllerToBoxParentMap[obstacleController] = this;

        auto castedObstacleController = il2cpp_utils::try_cast<GlobalNamespace::ObstacleController>(obstacleController);
        if (castedObstacleController)
        {
            colorManager = castedObstacleController.value()->_colorManager;
        }
        else
        {
            auto castedObstacleController = il2cpp_utils::try_cast<GlobalNamespace::MirroredObstacleController>(obstacleController);
            if (castedObstacleController)
            {
                auto obstacleController = castedObstacleController.value()->_followedObstacle;
                if (obstacleController)
                    colorManager = obstacleController->_colorManager;
            }
        }

        std::function<void(GlobalNamespace::ObstacleControllerBase*)> fun = std::bind(&BoxParent::ObstacleDidInit, this, std::placeholders::_1);
    }

    void BoxParent::OnDestroy()
    {
        obstacleControllerToBoxParentMap.erase(obstacleController);
    }

    void BoxParent::ObstacleDidInit(GlobalNamespace::ObstacleControllerBase* obstacleController)
    {
        if (colorManager)
            ColorizeSpecific(colorManager->get_obstaclesColor());
    }

    void BoxParent::Colorize(GlobalNamespace::ObstacleControllerBase* obstacleController, const Sombrero::FastColor& color)
    {
        auto boxItr = obstacleControllerToBoxParentMap.find(obstacleController);
        if (boxItr != obstacleControllerToBoxParentMap.end())
        {
            boxItr->second->ColorizeSpecific(color);
        }
    }

    void BoxParent::ColorizeSpecific(const Sombrero::FastColor& color)
    {
        if (handler)
            handler->SetColor(color);
    }

    BoxHandler* BoxParent::get_Handler() const { return handler; }
    void BoxParent::set_Handler(BoxHandler* handler) { this->handler = handler; }
}
