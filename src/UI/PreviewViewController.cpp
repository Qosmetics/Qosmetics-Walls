#include "UI/PreviewViewController.hpp"
#include "CustomTypes/WallModelContainer.hpp"
#include "config.hpp"
#include "diglett/shared/Localization.hpp"
#include "diglett/shared/Util.hpp"
#include "logging.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastVector3.hpp"

#include "UnityEngine/UI/LayoutElement.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"

DEFINE_TYPE(Qosmetics::Walls, PreviewViewController);

using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::Walls
{
    bool PreviewViewController::justChangedProfile = false;
    void PreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (currentPrefab)
            currentPrefab->SetActive(false);
    }

    void PreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            title = Qosmetics::Core::UIUtils::AddHeader(get_transform(), "", Sombrero::FastColor::red());
            reinterpret_cast<UnityEngine::RectTransform*>(title->get_transform()->get_parent()->get_parent())->set_anchoredPosition({0.0f, 30.0f});
            auto backgroundLayout = CreateVerticalLayoutGroup(this);
            auto horizontalBackgroundLayout = CreateHorizontalLayoutGroup(backgroundLayout);
            auto layoutElem = horizontalBackgroundLayout->get_gameObject()->GetComponent<UnityEngine::UI::LayoutElement*>();
            layoutElem->set_preferredWidth(75.0f);
            layoutElem->set_preferredHeight(50.0f);

            auto bg = horizontalBackgroundLayout->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();
            bg->ApplyBackgroundWithAlpha("title-gradient", 1.0f);

            auto imageView = bg->get_gameObject()->GetComponent<HMUI::ImageView*>();
            imageView->set_gradient(true);
            imageView->dyn__gradientDirection() = 1;
            imageView->set_color(Sombrero::FastColor::white());
            auto color = Sombrero::FastColor::get_black();
            color.a = 0.3f;
            imageView->set_color0(color);
            color.a = 0.7f;
            imageView->set_color1(color);
            imageView->dyn__curvedCanvasSettingsHelper()->Reset();

            loadingIndicator = Qosmetics::Core::UIUtils::CreateLoadingIndicator(get_transform());
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
        title->set_text(u"<i>" + text + u"</i>");
    }

    void PreviewViewController::ShowLoading(bool isLoading)
    {
        loadingIndicator->SetActive(isLoading);
        if (isLoading)
        {
            SetTitleText(Diglett::Localization::get_instance()->get("QosmeticsCore:Misc:Loading") + u"...");
        }
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
            SetTitleText(Diglett::Localization::get_instance()->get("QosmeticsDodgers:Preview:Default"));
            return;
        }

        DEBUG("Getting variables");
        auto noteModelContainer = WallModelContainer::get_instance();
        auto config = noteModelContainer->GetWallConfig();
        auto& globalConfig = Config::get_config();

        auto& descriptor = noteModelContainer->GetDescriptor();
        auto name = descriptor.get_name();
        SetTitleText(name);

        // TODO: implement proper positioning, size n stuff
    }
    void PreviewViewController::InstantiatePrefab()
    {
        auto wallModelContainer = WallModelContainer::get_instance();
        if (wallModelContainer->currentWallObject)
        {
            DEBUG("Found a new saber object, instantiating it! name: {}", wallModelContainer->currentWallObject->get_name());
            currentPrefab = UnityEngine::Object::Instantiate(wallModelContainer->currentWallObject, get_transform());
            currentPrefab->SetActive(true);
            auto t = currentPrefab->get_transform();
            auto s = get_transform()->get_lossyScale();
            DEBUG("LossyScale: {}, {}, {}", s.x, s.y, s.z);
            t->set_localScale(Sombrero::FastVector3::one() / 0.03f);
            t->set_localPosition(Sombrero::FastVector3(-30.0f, 2.5f, -85.0f));
            t->set_localEulerAngles(Sombrero::FastVector3(0.0f, -30.0f, 0.0f));
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