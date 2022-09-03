#include "UI/BoxFlowCoordinator.hpp"
#include "custom-types/shared/register.hpp"
#include "hooking.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "static-defines.hpp"

#include "assets.hpp"
#include "diglett/shared/Conversions.hpp"
#include "diglett/shared/Register.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

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
    QuestUI::Init();

    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::de_xml), Diglett::Language::GERMAN);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::en_xml), Diglett::Language::ENGLISH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::es_xml), Diglett::Language::SPANISH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::fr_xml), Diglett::Language::FRENCH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::ja_xml), Diglett::Language::JAPANESE);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::ko_xml), Diglett::Language::KOREAN);

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<Qosmetics::Walls::AppInstaller*>(Lapiz::Zenject::Location::App);
    zenjector->Install<Qosmetics::Walls::MenuInstaller*>(Lapiz::Zenject::Location::Menu);
    zenjector->Install<Qosmetics::Walls::GameInstaller*>(Lapiz::Zenject::Location::Player | Lapiz::Zenject::Location::Tutorial);
}