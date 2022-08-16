#include "CustomTypes/BoxHandler.hpp"

#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxHandler);

#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

static FieldInfo* find_field_in_object_klasses(Il2CppClass* klass, const char* fieldName)
{
    if (!klass)
        return nullptr;
    auto finfo = il2cpp_functions::class_get_field_from_name(klass, fieldName);
    return finfo ? finfo : find_field_in_object_klasses(klass->parent, fieldName);
}

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
            static auto finfo = find_field_in_object_klasses(obstacleController->klass, "_stretchableObstacle");
            auto stretchableObstacle = reinterpret_cast<GlobalNamespace::StretchableObstacle*>(((void**)obstacleController)[finfo->offset]);
            auto frameController = stretchableObstacle->obstacleFrame;
            stretchableObstacle->SetSizeAndColor(frameController->width, frameController->height, frameController->length, color);
        }
    }

}