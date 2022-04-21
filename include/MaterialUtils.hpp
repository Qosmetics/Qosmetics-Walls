#pragma once

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"

namespace Qosmetics::Walls::MaterialUtils
{
    bool ShouldCC(UnityEngine::Material* material);
    void PrewarmAllShadersOnObject(UnityEngine::GameObject* object);
}