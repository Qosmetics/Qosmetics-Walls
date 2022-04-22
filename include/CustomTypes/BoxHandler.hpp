#pragma once

#include "CustomTypes/BoxColorHandler.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"
#include "sombrero/shared/FastColor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, BoxHandler, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD(BoxColorHandler*, colorHandler);
                      DECLARE_INSTANCE_FIELD(GlobalNamespace::ObstacleController*, obstacleController);

                      DECLARE_INSTANCE_METHOD(void, Awake);

                      public
                      :

                      void SetColor(const Sombrero::FastColor& color);

)
