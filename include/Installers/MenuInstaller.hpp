#pragma once

#include "Zenject/Installer.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, MenuInstaller, ::Zenject::Installer,
                      DECLARE_OVERRIDE_METHOD_MATCH(void, InstallBindings, &::Zenject::Installer::InstallBindings);
                      DECLARE_DEFAULT_CTOR();

)
