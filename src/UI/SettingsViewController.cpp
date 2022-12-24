#include "UI/SettingsViewController.hpp"
#include "assets.hpp"
#include "config.hpp"
#include "qosmetics-core/shared/ConfigRegister.hpp"
#include "qosmetics-core/shared/Utils/DateUtils.hpp"
#include "qosmetics-core/shared/Utils/RainbowUtils.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"

#include "UnityEngine/RectTransform.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Settings/BaseSetting.hpp"

DEFINE_TYPE(Qosmetics::Walls, SettingsViewController);

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

    void SettingsViewController::Inject(PreviewViewController* previewViewController)
    {
        this->previewViewController = previewViewController;
    }

    void SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            BSML::parse_and_construct(IncludedAssets::SettingsView_bsml, get_transform(), this);
        }
        else if (justChangedProfile)
        {
            justChangedProfile = false;
            auto settings = GetComponentsInChildren<BSML::BaseSetting*>(true);

            for (auto setting : settings)
                il2cpp_utils::RunMethod(setting, "ReceiveValue");
        }
    }

    bool SettingsViewController::get_gay()
    {
        return Qosmetics::Core::DateUtils::isMonth(6);
    }

    bool SettingsViewController::get_forceFakeGlowOff() { return Config::get_config().forceFakeGlowOff; }
    void SettingsViewController::set_forceFakeGlowOff(bool value)
    {
        Config::get_config().forceFakeGlowOff = value;
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(false);
    }

    bool SettingsViewController::get_forceCoreOff() { return Config::get_config().forceCoreOff; }
    void SettingsViewController::set_forceCoreOff(bool value)
    {
        Config::get_config().forceCoreOff = value;
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(false);
    }

    bool SettingsViewController::get_forceFrameOff() { return Config::get_config().forceFrameOff; }
    void SettingsViewController::set_forceFrameOff(bool value)
    {
        Config::get_config().forceFrameOff = value;
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(false);
    }

    bool SettingsViewController::get_disableReflections() { return Config::get_config().disableReflections; }
    void SettingsViewController::set_disableReflections(bool value)
    {
        Config::get_config().disableReflections = value;
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(false);
    }

}