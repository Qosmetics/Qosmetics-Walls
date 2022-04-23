#pragma once

#include "CustomTypes/BoxHandler.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ObstacleControllerBase.hpp"
#include "System/Action_1.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"
#include "sombrero/shared/FastColor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, BoxParent, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD(::System::Action_1<::GlobalNamespace::ObstacleControllerBase*>*, didInitDelegate);
                      DECLARE_INSTANCE_FIELD(GlobalNamespace::ObstacleControllerBase*, obstacleController);
                      DECLARE_INSTANCE_FIELD(GlobalNamespace::ColorManager*, colorManager);
                      DECLARE_INSTANCE_FIELD(BoxHandler*, handler);

                      DECLARE_INSTANCE_METHOD(void, Awake);
                      DECLARE_INSTANCE_METHOD(void, OnDestroy);
                      DECLARE_INSTANCE_METHOD(void, ObstacleDidInit, GlobalNamespace::ObstacleControllerBase* obstacleController);

                      public
                      :

                      static void Colorize(GlobalNamespace::ObstacleControllerBase* obstacleController, const Sombrero::FastColor& color);
                      void ColorizeSpecific(const Sombrero::FastColor& color);

)