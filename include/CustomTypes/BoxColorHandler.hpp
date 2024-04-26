#pragma once

#include "GlobalNamespace/ObstacleControllerBase.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"
#include "sombrero/shared/FastColor.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, BoxColorHandler, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD_PRIVATE(Sombrero::FastColor, lastColor);
                      DECLARE_INSTANCE_FIELD_PRIVATE(ArrayW<UnityEngine::Material*>, customColorMaterials);

                      DECLARE_INSTANCE_METHOD(void, Awake);
                      DECLARE_INSTANCE_METHOD(void, FetchCCMaterials);
                      DECLARE_DEFAULT_CTOR();

                      public
                      :

                      void SetColor(const Sombrero::FastColor& color);

)
