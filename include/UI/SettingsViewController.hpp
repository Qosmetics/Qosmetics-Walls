#pragma once

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "HMUI/ViewController.hpp"
#include "UI/PreviewViewController.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "bsml/shared/macros.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, SettingsViewController, HMUI::ViewController,
                      DECLARE_INSTANCE_FIELD_PRIVATE(PreviewViewController*, previewViewController);

                      DECLARE_INJECT_METHOD(void, Inject, PreviewViewController* previewViewController);
                      DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                      DECLARE_INSTANCE_METHOD(StringW, get_gaydient);
                      DECLARE_INSTANCE_METHOD(bool, get_gay);

                      DECLARE_BSML_PROPERTY(bool, forceFakeGlowOff);
                      DECLARE_BSML_PROPERTY(bool, forceCoreOff);
                      DECLARE_BSML_PROPERTY(bool, forceFrameOff);
                      DECLARE_BSML_PROPERTY(bool, disableReflections);

                      public
                      :

                      static bool justChangedProfile;

)
