#pragma once

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "HMUI/ViewController.hpp"
#include "UI/PreviewViewController.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "bsml/shared/macros.hpp"
#include "custom-types/shared/macros.hpp"

#ifndef DECLARE_OVERRIDE_METHOD_MATCH
#define DECLARE_OVERRIDE_METHOD_MATCH(retval, name, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, name, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)
#endif

#include "private_field.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, SettingsViewController, HMUI::ViewController,
                      DECLARE_INSTANCE_PRIVATE_FIELD(PreviewViewController*, previewViewController);

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