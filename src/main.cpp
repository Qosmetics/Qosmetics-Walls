#include "UI/BoxFlowCoordinator.hpp"
#include "custom-types/shared/register.hpp"
#include "hooking.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "static-defines.hpp"

#include "assets.hpp"

#include "Installers/AppInstaller.hpp"
#include "Installers/GameInstaller.hpp"
#include "Installers/MenuInstaller.hpp"
#include "lapiz/shared/AttributeRegistration.hpp"
#include "lapiz/shared/zenject/Zenjector.hpp"

ModInfo modInfo = {MOD_ID, VERSION};

extern "C" void setup(ModInfo& info)
{
    info = modInfo;
}

extern "C" void load()
{
    il2cpp_functions::Init();

    mkpath(box_path);
    auto& logger = Qosmetics::Walls::Logging::getLogger();
    Hooks::InstallHooks(logger);
    custom_types::Register::AutoRegister();
    Lapiz::Attributes::AutoRegister();

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<Qosmetics::Walls::AppInstaller*>(Lapiz::Zenject::Location::App);
    zenjector->Install<Qosmetics::Walls::MenuInstaller*>(Lapiz::Zenject::Location::Menu);
    zenjector->Install<Qosmetics::Walls::GameInstaller*>(Lapiz::Zenject::Location::Player | Lapiz::Zenject::Location::Tutorial);
}