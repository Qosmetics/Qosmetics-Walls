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
    auto inactive_data = WallIcon_png::getData();
    auto inactive = QuestUI::BeatSaberUI::VectorToSprite(std::vector<uint8_t>(inactive_data, inactive_data + WallIcon_png::getLength()));
    auto active_data = WallIconSelected_png::getData();
    auto active = QuestUI::BeatSaberUI::VectorToSprite(std::vector<uint8_t>(active_data, active_data + WallIconSelected_png::getLength()));
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

    Diglett::RegisterAsset(ASSET_TO_STR(de_xml), Diglett::Language::GERMAN);
    Diglett::RegisterAsset(ASSET_TO_STR(en_xml), Diglett::Language::ENGLISH);
    Diglett::RegisterAsset(ASSET_TO_STR(es_xml), Diglett::Language::SPANISH);
    Diglett::RegisterAsset(ASSET_TO_STR(fr_xml), Diglett::Language::FRENCH);
    Diglett::RegisterAsset(ASSET_TO_STR(ja_xml), Diglett::Language::JAPANESE);
    Diglett::RegisterAsset(ASSET_TO_STR(ko_xml), Diglett::Language::KOREAN);
}