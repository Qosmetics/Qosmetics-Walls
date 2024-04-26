#include "UI/PreviewViewController.hpp"
#include "CustomTypes/WallModelContainer.hpp"
#include "assets.hpp"
#include "config.hpp"
#include "logging.hpp"
#include "qosmetics-core/shared/Utils/DateUtils.hpp"
#include "qosmetics-core/shared/Utils/RainbowUtils.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastVector3.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "ConstStrings.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"
#include "PropertyID.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"

DEFINE_TYPE(Qosmetics::Walls, PreviewViewController);

static UnityEngine::Vector3 operator*(UnityEngine::Vector3 vec, float v)
{
    return {
        vec.x * v,
        vec.y * v,
        vec.z * v};
}

namespace Qosmetics::Walls
{
    bool PreviewViewController::justChangedProfile = false;

    void PreviewViewController::Inject(WallModelContainer* wallModelContainer, GlobalNamespace::PlayerDataModel* playerDataModel)
    {
        this->wallModelContainer = wallModelContainer;
        this->playerDataModel = playerDataModel;
    }

    void PreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (currentPrefab)
            currentPrefab->SetActive(false);
    }

    StringW PreviewViewController::get_gaydient() { return Qosmetics::Core::RainbowUtils::randomGradient(); }
    bool PreviewViewController::get_gay() { return Qosmetics::Core::DateUtils::isMonth(6); }

    void PreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            auto parser = BSML::parse_and_construct(Assets::Views::PreviewView_bsml, get_transform(), this);
            auto params = parser->parserParams.get();
            auto objectBG = params->GetObjectsWithTag("objectBG").at(0)->GetComponent<BSML::Backgroundable*>();
            auto imageView = objectBG->background;
            imageView->_skew = 0;
            imageView->set_gradient(true);
            imageView->_gradientDirection = 1;
            imageView->set_color(Sombrero::FastColor::white());
            auto color = Sombrero::FastColor::get_black();
            color.a = 0.3f;
            imageView->set_color0(color);
            color.a = 0.7f;
            imageView->set_color1(color);
            imageView->_curvedCanvasSettingsHelper->Reset();

            ShowLoading(true);
            UpdatePreview(true);
        }
        else
        {
            if (currentPrefab)
                currentPrefab->SetActive(true);
            DEBUG("Reactivating preview view controller with justChangedProfile: {}", justChangedProfile ? "true" : "false");
            UpdatePreview(justChangedProfile);

            justChangedProfile = false;
        }
    }

    void PreviewViewController::SetTitleText(StringW text)
    {
        if (!(title && title->m_CachedPtr))
            return;
        if (Qosmetics::Core::DateUtils::isMonth(6))
        {
            text = "<i>" + Qosmetics::Core::RainbowUtils::gayify(static_cast<std::string>(text)) + "</i>";
            title->set_text(text);
        }
        else
            title->set_text(u"<i>" + text + u"</i>");
    }

    void PreviewViewController::ShowLoading(bool isLoading)
    {
        if (!(loadingIndicator && loadingIndicator->m_CachedPtr))
            return;

        loadingIndicator->get_gameObject()->SetActive(isLoading);
        if (isLoading)
            SetTitleText("Loading...");
    }
    void PreviewViewController::UpdatePreview(bool reinstantiate)
    {
        if (!currentPrefab || reinstantiate)
        {
            DEBUG("Reinstantiating prefab");
            ClearPrefab();
            InstantiatePrefab();
        }

        ShowLoading(false);
        if (!currentPrefab)
        {
            DEBUG("No prefab found, must be default!");
            SetTitleText("Default Box (No preview)");
            return;
        }

        DEBUG("Getting variables");
        auto config = wallModelContainer->GetWallConfig();
        auto& globalConfig = Config::get_config();

        auto& descriptor = wallModelContainer->GetDescriptor();
        auto name = descriptor.get_name();
        SetTitleText(name);

        auto t = currentPrefab->get_transform();
        auto core = t->Find(ConstStrings::Core());
        auto frame = t->Find(ConstStrings::Frame());

        if (core)
            core->get_gameObject()->SetActive(!(globalConfig.forceCoreOff || config.get_disableCore()));
        if (frame)
            frame->get_gameObject()->SetActive(!(globalConfig.forceFrameOff || config.get_disableFrame()));

        int childCount = t->get_childCount();
        for (int i = 0; i < childCount; i++)
        {
            auto child = t->GetChild(i);
            if (child != core && child != frame)
                child->get_gameObject()->SetActive(false);
        }

        t->set_localScale(Sombrero::FastVector3(1.5f, 1.0f, 0.5f) / 0.03f);

        auto renderers = currentPrefab->GetComponentsInChildren<UnityEngine::Renderer*>(true);
        auto scale = t->get_lossyScale() * 0.5f;
        UnityEngine::Vector4 sizeParams = UnityEngine::Vector4(scale.x, scale.y, scale.z, 0.05f);

        // TODO: set colors from selected colorscheme
        int rendererCount = renderers.size();
        for (auto renderer : renderers)
        {
            auto materials = renderer->GetMaterialArray();
            for (auto material : materials)
            {
                if (material->HasProperty(PropertyID::_SizeParams()))
                    material->SetVector(PropertyID::_SizeParams(), sizeParams);
            }
        }
    }
    void PreviewViewController::InstantiatePrefab()
    {
        if (wallModelContainer->CurrentWallObject)
        {
            DEBUG("Found a new wall object, instantiating it! name: {}", wallModelContainer->CurrentWallObject->get_name());
            currentPrefab = UnityEngine::Object::Instantiate(wallModelContainer->CurrentWallObject, get_transform());
            currentPrefab->SetActive(true);
            auto t = currentPrefab->get_transform();
            auto s = get_transform()->get_lossyScale();
            t->set_localScale(Sombrero::FastVector3::one() / 0.03f);
            t->set_localPosition(Sombrero::FastVector3(-30.0f, 2.5f, -70.0f));
            t->set_localEulerAngles(Sombrero::FastVector3(0.0f, 60.0f, 0.0f));
            DEBUG("Instantiated and inited new prefab!");
        }
        else
        {
            ERROR("wallModelContainer->currentWallObject was null but we tried using it!");
        }
    }

    void PreviewViewController::ClearPrefab()
    {
        DEBUG("Clearing Prefab...");
        if (currentPrefab)
        {
            DEBUG("Prefab existed!");
            UnityEngine::Object::DestroyImmediate(currentPrefab);
        }
        currentPrefab = nullptr;
    }

}
