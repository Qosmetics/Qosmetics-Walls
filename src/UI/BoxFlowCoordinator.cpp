#include "UI/BoxFlowCoordinator.hpp"
#include "UI/PreviewViewController.hpp"
#include "UI/SelectionViewController.hpp"
#include "UI/SettingsViewController.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/TitleViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "qosmetics-core/shared/Utils/DateUtils.hpp"
#include "qosmetics-core/shared/Utils/RainbowUtils.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"
DEFINE_TYPE(Qosmetics::Walls, BoxFlowCoordinator);

using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::Walls
{
    void BoxFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            previewViewController = CreateViewController<Qosmetics::Walls::PreviewViewController*>();
            settingsViewController = CreateViewController<Qosmetics::Walls::SettingsViewController*>();
            reinterpret_cast<Qosmetics::Walls::SettingsViewController*>(settingsViewController)->previewViewController = reinterpret_cast<Qosmetics::Walls::PreviewViewController*>(previewViewController);
            selectionViewController = CreateViewController<Qosmetics::Walls::SelectionViewController*>();
            reinterpret_cast<Qosmetics::Walls::SelectionViewController*>(selectionViewController)->previewViewController = reinterpret_cast<Qosmetics::Walls::PreviewViewController*>(previewViewController);
            ProvideInitialViewControllers(selectionViewController, settingsViewController, previewViewController, nullptr, nullptr);

            set_showBackButton(true);

            SetTitle(Qosmetics::Core::DateUtils::isMonth(6) ? Qosmetics::Core::RainbowUtils::gayify("Box") : "Box", HMUI::ViewController::AnimationType::In);
        }

        HMUI::TitleViewController* titleView = Object::FindObjectOfType<HMUI::TitleViewController*>();
        Qosmetics::Core::UIUtils::SetTitleColor(titleView, UnityEngine::Color::get_magenta());
    }

    void BoxFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        this->parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

}