#include "CustomTypes/BoxColorHandler.hpp"
#include "MaterialUtils.hpp"
#include "PropertyID.hpp"
#include "logging.hpp"

#include "UnityEngine/Renderer.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxColorHandler);

namespace Qosmetics::Walls
{
    void BoxColorHandler::Awake()
    {
        FetchCCMaterials();
    }

    void BoxColorHandler::FetchCCMaterials()
    {
        if (customColorMaterials)
            return;
        auto renderers = GetComponentsInChildren<UnityEngine::Renderer*>(true);
        std::vector<UnityEngine::Material*> customColorMaterialsVec = {};

        for (auto renderer : renderers)
        {
            auto materials = renderer->get_materials();
            for (auto material : materials)
                if (MaterialUtils::ShouldCC(material) && material->HasProperty(PropertyID::_Color()))
                    customColorMaterialsVec.push_back(material);
        }
        DEBUG("Found {} custom colors materials", customColorMaterialsVec.size());
        customColorMaterials = il2cpp_utils::vectorToArray(customColorMaterialsVec);
    }

    void BoxColorHandler::SetColor(const Sombrero::FastColor& color)
    {
        if (lastColor.operator==(color))
            return;

        DEBUG("{}: setting colors!", fmt::ptr(this));
        DEBUG("color: {:.2f}, {:.2f}, {:.2f}, {:.2f}", color.r, color.g, color.b, color.a);

        lastColor = color;

        for (auto* mat : customColorMaterials)
            mat->SetColor(PropertyID::_Color(), color);
    }

}