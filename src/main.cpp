#include "UI/BoxFlowCoordinator.hpp"
#include "_config.h"
#include "custom-types/shared/register.hpp"
#include "hooking.hpp"
#include "logging.hpp"
#include "scotland2/shared/modloader.h"
#include "static-defines.hpp"

#include "assets.hpp"

#include "Installers/AppInstaller.hpp"
#include "Installers/GameInstaller.hpp"
#include "Installers/MenuInstaller.hpp"
#include "lapiz/shared/AttributeRegistration.hpp"
#include "lapiz/shared/zenject/Zenjector.hpp"

modloader::ModInfo modInfo = {MOD_ID, VERSION, 0};

QOSMETICS_WALLS_EXPORT_FUNC void setup(CModInfo* info)
{
    info->id = MOD_ID;
    info->version = VERSION;
    info->version_long = 0;
}

QOSMETICS_WALLS_EXPORT_FUNC void late_load()
{
    il2cpp_functions::Init();

    mkpath(box_path);
    Hooks::InstallHooks();
    custom_types::Register::AutoRegister();
    Lapiz::Attributes::AutoRegister();

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<Qosmetics::Walls::AppInstaller*>(Lapiz::Zenject::Location::App);
    zenjector->Install<Qosmetics::Walls::MenuInstaller*>(Lapiz::Zenject::Location::Menu);
    zenjector->Install<Qosmetics::Walls::GameInstaller*>(Lapiz::Zenject::Location::Player | Lapiz::Zenject::Location::Tutorial);
}
