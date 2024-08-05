#include "UI/BoxFlowCoordinator.hpp"

#include "HMUI/TitleViewController.hpp"

#include "qosmetics-core/shared/Utils/DateUtils.hpp"
#include "qosmetics-core/shared/Utils/RainbowUtils.hpp"
#include "qosmetics-core/shared/Utils/UIUtils.hpp"

#include "assets.hpp"
#include "logging.hpp"

#include "bsml/shared/Helpers/utilities.hpp"

DEFINE_TYPE(Qosmetics::Walls, BoxFlowCoordinator);

namespace Qosmetics::Walls
{
    void BoxFlowCoordinator::ctor()
    {
        static auto baseKlass = classof(Qosmetics::Core::QosmeticsBaseFlowCoordinator*);
        custom_types::InvokeBaseCtor(baseKlass, this);

        name = "Boxes";
    }

    void BoxFlowCoordinator::Awake()
    {
        inActiveSprite = BSML::Utilities::LoadSpriteRaw(Assets::Icons::WallIcon_png);
        activeSprite = BSML::Utilities::LoadSpriteRaw(Assets::Icons::WallIconSelected_png);
    }

    void BoxFlowCoordinator::Inject(PreviewViewController* previewViewController, SelectionViewController* selectionViewController, SettingsViewController* settingsViewController)
    {
        this->previewViewController = previewViewController;
        this->selectionViewController = selectionViewController;
        this->settingsViewController = settingsViewController;
    }

    void BoxFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        HMUI::TitleViewController* titleView = Object::FindObjectOfType<HMUI::TitleViewController*>();
        if (firstActivation)
        {
            ProvideInitialViewControllers(selectionViewController, settingsViewController, previewViewController, nullptr, nullptr);

            set_showBackButton(true);

            SetTitle("Boxes", HMUI::ViewController::AnimationType::In);

            if (Qosmetics::Core::DateUtils::isMonth(6))
            {
                titleGradientUpdater = get_gameObject()->AddComponent<BSML::TextGradientUpdater*>();
                titleGradientUpdater->set_gradient(BSML::Gradient::Parse(Qosmetics::Core::RainbowUtils::randomGradient()));
                titleGradientUpdater->text = titleView->_text;
                titleGradientUpdater->scrollSpeed = 0.2;
                titleGradientUpdater->fixedStep = true;
                titleGradientUpdater->stepSize = 2;
            }
        }

        if (titleGradientUpdater && titleGradientUpdater->m_CachedPtr.m_value)
            titleGradientUpdater->set_enabled(true);

        Qosmetics::Core::UIUtils::SetTitleColor(titleView, UnityEngine::Color::get_magenta());
    }

    void BoxFlowCoordinator::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (!titleGradientUpdater || !titleGradientUpdater->m_CachedPtr.m_value)
            return;
        titleGradientUpdater->set_enabled(false);
    }

    void BoxFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        this->_parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

}
