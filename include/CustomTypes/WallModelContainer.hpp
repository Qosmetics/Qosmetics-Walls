#pragma once

#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "WallObjectConfig.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include "qosmetics-core/shared/Data/Descriptor.hpp"
#include "qosmetics-core/shared/Data/Manifest.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, WallModelContainer, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_METHOD(void, Start);
                      DECLARE_INSTANCE_METHOD(void, OnDestroy);
                      DECLARE_INSTANCE_FIELD(UnityEngine::AssetBundle*, bundle);
                      DECLARE_INSTANCE_FIELD(bool, isLoading);
                      DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, currentWallObject);

                      DECLARE_CTOR(ctor);
                      DECLARE_SIMPLE_DTOR();

                      public
                      :

                      using Manifest = Qosmetics::Core::Manifest<Qosmetics::Walls::WallObjectConfig>;

                      static WallModelContainer * get_instance();
                      bool LoadObject(const Manifest& manifest, std::function<void(WallModelContainer*)> onFinished);
                      bool LoadObject(const Qosmetics::Core::Descriptor& descriptor, std::function<void(WallModelContainer*)> onFinished = nullptr);
                      const WallObjectConfig& GetWallConfig();
                      const Qosmetics::Core::Descriptor& GetDescriptor();
                      bool Default();

                      private
                      :

                      custom_types::Helpers::Coroutine LoadBundleRoutine(std::function<void(WallModelContainer*)> onFinished);
                      void Unload();

                      Manifest currentManifest;

                      static WallModelContainer * instance;

)