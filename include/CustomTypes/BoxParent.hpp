#pragma once

#include "CustomTypes/BoxHandler.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ObstacleControllerBase.hpp"
#include "System/Action_1.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"
#include "sombrero/shared/FastColor.hpp"
#include <map>

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, BoxParent, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD_PRIVATE(::System::Action_1<::GlobalNamespace::ObstacleControllerBase*>*, didInitDelegate);
                      DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::ObstacleControllerBase*, obstacleController);
                      DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::ColorManager*, colorManager);
                      DECLARE_INSTANCE_FIELD_PRIVATE(BoxHandler*, handler);

                      DECLARE_INSTANCE_METHOD(void, Awake);
                      DECLARE_INSTANCE_METHOD(void, OnDestroy);
                      DECLARE_INSTANCE_METHOD(void, ObstacleDidInit, GlobalNamespace::ObstacleControllerBase* obstacleController);

                      DECLARE_DEFAULT_CTOR();
                      public
                      :

                      static std::map<GlobalNamespace::ObstacleControllerBase*, BoxParent*>
                          obstacleControllerToBoxParentMap;
                      static void Colorize(GlobalNamespace::ObstacleControllerBase* obstacleController, const Sombrero::FastColor& color);
                      void ColorizeSpecific(const Sombrero::FastColor& color);

                      __declspec(property(get = get_Handler, put = set_Handler)) BoxHandler * Handler;
                      BoxHandler * get_Handler() const;
                      void set_Handler(BoxHandler* handler);

)
