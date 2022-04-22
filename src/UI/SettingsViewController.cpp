#include "UI/SettingsViewController.hpp"
#include "config.hpp"
#include "diglett/shared/Localization.hpp"
#include "diglett/shared/Util.hpp"
#include "qosmetics-core/shared/ConfigRegister.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "sombrero/shared/FastVector2.hpp"

#include "UnityEngine/RectTransform.hpp"

DEFINE_TYPE(Qosmetics::Walls, SettingsViewController);

using namespace QuestUI::BeatSaberUI;

namespace QuestUI::BeatSaberUI
{
    HMUI::SimpleTextDropdown* CreateDropdownInternal(UnityEngine::Transform* parent, StringW dropdownName, int selectedIndex, List<StringW>* values, std::function<void(HMUI::SimpleTextDropdown*, int)> onValueChange);
}

#define TOGGLE(name, key)                                                              \
    name##Toggle = CreateToggle(containerT, localization->get(key), globalConfig.name, \
                                [&](auto v)                                            \
                                {                                                      \
                                    Config::get_config().name = v;                     \
                                    Qosmetics::Core::Config::SaveConfig();             \
                                    previewViewController->UpdatePreview(false);       \
                                });                                                    \
    AddHoverHint(name##Toggle, localization->get(key "HoverHint"))

#define SLIDER(name, key, ...)                                                                             \
    name##Slider = CreateSliderSetting(containerT, localization->get(key), __VA_ARGS__, globalConfig.name, \
                                       [&](auto v)                                                         \
                                       {                                                                   \
                                           Config::get_config().name = v;                                  \
                                           Qosmetics::Core::Config::SaveConfig();                          \
                                           previewViewController->UpdatePreview(false);                    \
                                       });                                                                 \
    name##Slider->FormatString = [](auto v) -> std::string { return std::to_string(v).substr(0, 4); };     \
    AddHoverHint(name##Slider, localization->get(key "HoverHint"))

namespace Qosmetics::Walls
{
    bool SettingsViewController::justChangedProfile = false;

    void SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        auto& globalConfig = Config::get_config();
        if (firstActivation)
        {
            auto localization = Diglett::Localization::get_instance();
            Qosmetics::Core::UIUtils::AddHeader(get_transform(), localization->get("QosmeticsBoxs:Settings:Settings"), Sombrero::FastColor::red());
            auto container = CreateScrollableSettingsContainer(this);

            auto externalComponents = container->GetComponent<QuestUI::ExternalComponents*>();
            auto scrollTransform = externalComponents->Get<UnityEngine::RectTransform*>();
            scrollTransform->set_sizeDelta(Sombrero::FastVector2::zero());

            auto containerT = container->get_transform();
            TOGGLE(forceFakeGlowOff, "QosmeticsBoxs:Settings:FakeGlowOff");
            TOGGLE(forceCoreOff, "QosmeticsBoxs:Settings:CoreOff");
            TOGGLE(forceFrameOff, "QosmeticsBoxs:Settings:FrameOff");
            TOGGLE(disableReflections, "QosmeticsBoxs:Settings:DisableReflections");
        }
        else if (justChangedProfile)
        {
            justChangedProfile = false;
            forceFakeGlowOffToggle->set_isOn(globalConfig.forceFakeGlowOff);
            forceCoreOffToggle->set_isOn(globalConfig.forceCoreOff);
            forceFrameOffToggle->set_isOn(globalConfig.forceFrameOff);
            disableReflectionsToggle->set_isOn(globalConfig.disableReflections);
        }
    }

    void SettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
    }
}