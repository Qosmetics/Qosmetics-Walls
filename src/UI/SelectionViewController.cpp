#include "UI/SelectionViewController.hpp"
#include "CustomTypes/WallModelContainer.hpp"
#include "assets.hpp"
#include "config.hpp"
#include "diglett/shared/Localization.hpp"
#include "diglett/shared/Util.hpp"
#include "logging.hpp"
#include "qosmetics-core/shared/ConfigRegister.hpp"
#include "qosmetics-core/shared/Utils/FileUtils.hpp"
#include "qosmetics-core/shared/Utils/ZipUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/List/QuestUITableView.hpp"
#include "static-defines.hpp"

#include "HMUI/TableView.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"

#include "QwallConversion.hpp"
#include <algorithm>

DEFINE_TYPE(Qosmetics::Walls, SelectionViewController);

using namespace QuestUI::BeatSaberUI;

namespace Qosmetics::Walls
{
    void SelectionViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            auto vertical = CreateVerticalLayoutGroup(get_transform());
            auto buttonHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());

            auto localization = Diglett::Localization::get_instance();
            auto defaultObjectBtn = CreateUIButton(buttonHorizontal->get_transform(), localization->get("QosmeticsCore:QosmeticsTable:Default"), std::bind(&SelectionViewController::OnSelectDefault, this));
            auto refreshBtn = CreateUIButton(buttonHorizontal->get_transform(), localization->get("QosmeticsCore:QosmeticsTable:Refresh"), [this]()
                                             { QwallConversion::ConvertOldQwalls(std::bind(&SelectionViewController::RefreshAfterWallConversion, this)); });

            deletionConfirmationModal = Qosmetics::Core::DeletionConfirmationModal::Create(get_transform());
            descriptorList = CreateScrollableCustomSourceList<Qosmetics::Core::QosmeticObjectTableData*>(vertical->get_transform(), UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(100.0f, 76.0f), nullptr);
            descriptorList->deletionConfirmationModal = deletionConfirmationModal;
            descriptorList->onSelect = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnSelectDescriptor), this, std::placeholders::_1);
            descriptorList->onDelete = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnDeleteCell), this, std::placeholders::_1);
            // TODO: make default sprite for walls:
            // descriptorList->defaultSprite = VectorToSprite(std::vector<uint8_t>(_binary_PlaceholderIcon_png_start, _binary_PlaceholderIcon_png_end));
        }

        QwallConversion::ConvertOldQwalls(std::bind(&SelectionViewController::RefreshAfterWallConversion, this));
    }

    void SelectionViewController::RefreshAfterWallConversion()
    {
        // don't do shit when not enabled
        if (!get_enabled() || !get_gameObject()->get_activeSelf())
            return;

        ReloadDescriptorList();
    }

    void SelectionViewController::ReloadDescriptorList()
    {
        std::vector<std::string> boxs = {};
        Qosmetics::Core::FileUtils::GetFilesInFolderPath("box", box_path, boxs);
        auto tableView = reinterpret_cast<QuestUI::TableView*>(descriptorList->tableView);
        int scrolledRow = tableView->get_scrolledRow();

        auto& descriptorSet = descriptorList->objectDescriptors;
        int current = 0;
        for (auto& box : boxs)
        {
            current++;

            std::string filePath = fmt::format("{}/{}", box_path, box);
            auto orig = std::find_if(descriptorSet.begin(), descriptorSet.end(), [filePath](auto& d)
                                     { return d.get_filePath() == filePath; });
            // check if the box was already parsed
            if (orig != descriptorSet.end())
                continue;

            // get bytes from the zip file that a box is
            std::vector<uint8_t> packageData;
            if (Qosmetics::Core::ZipUtils::GetBytesFromZipFile(filePath, "package.json", packageData))
            {
                DEBUG("Got package: {}", std::string(packageData.begin(), packageData.end()).c_str());
                rapidjson::Document doc;
                doc.Parse(std::string(packageData.begin(), packageData.end()));
                // add to the set
                try
                {
                    descriptorSet.emplace(doc["descriptor"], filePath);
                }
                catch (const std::runtime_error& e)
                {
                    ERROR("error thrown while parsing descriptor from package: {}", e.what());
                }
            }
        }

        // check each descriptor to see if it still exists on disk, if not it should be removed from the list
        for (auto it = descriptorSet.begin(); it != descriptorSet.end(); /* nothing */)
        {
            auto itr = std::find(boxs.begin(), boxs.end(), Qosmetics::Core::FileUtils::GetFileName(it->get_filePath()));
            if (itr == boxs.end())
                it = descriptorSet.erase(it);
            else
                it++;
        }

        tableView->ReloadData();
        tableView->RefreshCells(true, true);
        tableView->ClearSelection();
    }

    void SelectionViewController::OnSelectDefault()
    {
        auto wallModelContainer = WallModelContainer::get_instance();

        // if we do not PROPERLY switch to default, don't clear the preview
        if (wallModelContainer->Default())
        {
            OnObjectLoadFinished();
            auto tableView = reinterpret_cast<QuestUI::TableView*>(descriptorList->tableView);
            tableView->ClearSelection();
        }
    }

    void SelectionViewController::OnSelectDescriptor(Qosmetics::Core::QosmeticObjectTableCell* cell)
    {
        if (cell)
        {
            auto& descriptor = cell->descriptor;
            if (!fileexists(descriptor.get_filePath()))
            {
                ReloadDescriptorList();
                return;
            }

            if (WallModelContainer::get_instance()->LoadObject(descriptor, std::bind(&SelectionViewController::OnObjectLoadFinished, this)))
            {
                previewViewController->ClearPrefab();
                previewViewController->ShowLoading(true);
            }
            else
            {
                previewViewController->UpdatePreview(false);
            }
        }
    }
    void SelectionViewController::OnDeleteCell(Qosmetics::Core::QosmeticObjectTableCell* cell)
    {
        auto& descriptor = cell->descriptor;

        if (!fileexists(descriptor.get_filePath()))
        {
            return;
        }

        if (descriptor.get_filePath() == WallModelContainer::get_instance()->GetDescriptor().get_filePath())
            OnSelectDefault();

        deletefile(descriptor.get_filePath());
        ReloadDescriptorList();
    }

    void SelectionViewController::OnObjectLoadFinished()
    {
        // something to do after we changed the object, like update preview
        auto wallModelContainer = WallModelContainer::get_instance();

        Qosmetics::Walls::Config::get_config().lastUsedBox = wallModelContainer->GetWallConfig().get_isDefault() ? "" : Qosmetics::Core::FileUtils::GetFileName(wallModelContainer->GetDescriptor().get_filePath(), true);
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(true);
    }
}