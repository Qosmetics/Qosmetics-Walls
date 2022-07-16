#include "UI/BoxFlowCoordinator.hpp"
#include "custom-types/shared/register.hpp"
#include "hooking.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "qosmetics-core/shared/FlowCoordinatorRegister.hpp"
#include "static-defines.hpp"

#include "assets.hpp"
#include "diglett/shared/Conversions.hpp"
#include "diglett/shared/Register.hpp"

#include "questui/shared/BeatSaberUI.hpp"

QOSMETICS_FLOWCOORDINATOR_REGISTER(Boxes, Qosmetics::Walls::BoxFlowCoordinator*)
{
    auto inactive = QuestUI::BeatSaberUI::ArrayToSprite(IncludedAssets::WallIcon_png);
    auto active = QuestUI::BeatSaberUI::ArrayToSprite(IncludedAssets::WallIconSelected_png);
    return std::make_pair(inactive, active);
}

ModInfo modInfo = {MOD_ID, VERSION};

extern "C" void setup(ModInfo& info)
{
    info = modInfo;
}

extern "C" void load()
{
    mkpath(box_path);
    auto& logger = Qosmetics::Walls::Logging::getLogger();
    Hooks::InstallHooks(logger);
    custom_types::Register::AutoRegister();

    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::de_xml), Diglett::Language::GERMAN);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::en_xml), Diglett::Language::ENGLISH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::es_xml), Diglett::Language::SPANISH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::fr_xml), Diglett::Language::FRENCH);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::ja_xml), Diglett::Language::JAPANESE);
    Diglett::RegisterAsset(static_cast<std::string_view>(IncludedAssets::ko_xml), Diglett::Language::KOREAN);
}