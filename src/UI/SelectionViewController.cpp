#include "UI/SelectionViewController.hpp"
#include "CustomTypes/WallModelContainer.hpp"
#include "assets.hpp"
#include "config.hpp"
#include "logging.hpp"
#include "qosmetics-core/shared/ConfigRegister.hpp"
#include "qosmetics-core/shared/Utils/FileUtils.hpp"
#include "qosmetics-core/shared/Utils/ZipUtils.hpp"
#include "static-defines.hpp"

#include "HMUI/TableView.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/Helpers/utilities.hpp"

#include "QwallConversion.hpp"
#include "System/Collections/Generic/HashSet_1.hpp"
#include <algorithm>

DEFINE_TYPE(Qosmetics::Walls, SelectionViewController);

namespace Qosmetics::Walls
{
    void SelectionViewController::Inject(PreviewViewController* previewViewController, WallModelContainer* wallModelContainer)
    {
        this->previewViewController = previewViewController;
        this->wallModelContainer = wallModelContainer;
    }

    void SelectionViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            BSML::parse_and_construct(Assets::Views::SelectionView_bsml, get_transform(), this);
            /*
            auto vertical = CreateVerticalLayoutGroup(get_transform());
            auto buttonHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());

            auto localization = Diglett::Localization::get_instance();
            auto defaultObjectBtn = CreateUIButton(buttonHorizontal->get_transform(), localization->get("QosmeticsCore:QosmeticsTable:Default"), std::bind(&SelectionViewController::Default, this));
            auto refreshBtn = CreateUIButton(buttonHorizontal->get_transform(), localization->get("QosmeticsCore:QosmeticsTable:Refresh"), [this]()
                                             { QwallConversion::ConvertOldQwalls(std::bind(&SelectionViewController::RefreshAfterWallConversion, this)); });

            deletionConfirmationModal = Qosmetics::Core::DeletionConfirmationModal::Create(get_transform());
            descriptorList = CreateScrollableCustomSourceList<Qosmetics::Core::QosmeticObjectTableData*>(vertical->get_transform(), UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(100.0f, 76.0f), nullptr);
            descriptorList->deletionConfirmationModal = deletionConfirmationModal;
            descriptorList->onSelect = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnSelectDescriptor), this, std::placeholders::_1);
            descriptorList->onDelete = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnDeleteCell), this, std::placeholders::_1);
            descriptorList->defaultSprite = ArrayToSprite(Assets::Icons::PlaceholderIcon_png);
            */
        }

        Refresh();
    }

    void SelectionViewController::PostParse()
    {
        deletionConfirmationModal = Qosmetics::Core::DeletionConfirmationModal::Create(get_transform());

        auto tableView = descriptorListTableData->tableView;
        auto go = descriptorListTableData->get_gameObject();
        Object::DestroyImmediate(descriptorListTableData);
        descriptorListTableData = nullptr;
        descriptorList = go->AddComponent<Qosmetics::Core::QosmeticObjectTableData*>();
        descriptorList->tableView = tableView;
        tableView->SetDataSource(reinterpret_cast<HMUI::TableView::IDataSource*>(descriptorList), false);

        descriptorList->deletionConfirmationModal = deletionConfirmationModal;
        descriptorList->onSelect = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnSelectDescriptor), this, std::placeholders::_1);
        descriptorList->onDelete = std::bind(reinterpret_cast<void (SelectionViewController::*)(HMUI::TableCell*)>(&SelectionViewController::OnDeleteCell), this, std::placeholders::_1);
        descriptorList->defaultSprite = BSML::Utilities::LoadSpriteRaw(Assets::Icons::PlaceholderIcon_png);
    }

    void SelectionViewController::Refresh()
    {
        QwallConversion::ConvertOldQwalls(std::bind(&SelectionViewController::RefreshAfterWallConversion, this));
    }

    void SelectionViewController::RefreshAfterWallConversion()
    {
        // don't do shit when not enabled
        if (!get_enabled() || !get_gameObject()->get_activeSelf())
            return;

        ReloadDescriptorList();
    }

    int SelectionViewController::GetSelectedCellIdx()
    {
        if (!descriptorList || !descriptorList->m_CachedPtr.m_value)
            return -1;
        auto tableView = descriptorList->tableView;
        auto enumerator = tableView->_selectedCellIdxs->GetEnumerator();
        int result = -1;
        if (enumerator.MoveNext())
            result = enumerator.get_Current();
        enumerator.Dispose();
        return result;
    }

    void SelectionViewController::ReloadDescriptorList()
    {
        std::vector<std::string> boxes = {};
        Qosmetics::Core::FileUtils::GetFilesInFolderPath("box", box_path, boxes);

        int row = GetSelectedCellIdx();
        auto& descriptorSet = descriptorList->objectDescriptors;
        for (auto& box : boxes)
        {

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
            auto itr = std::find(boxes.begin(), boxes.end(), Qosmetics::Core::FileUtils::GetFileName(it->get_filePath()));
            if (itr == boxes.end())
                it = descriptorSet.erase(it);
            else
                it++;
        }

        auto tableView = descriptorList->tableView;
        tableView->ReloadData();
        tableView->RefreshCells(true, true);
        tableView->ScrollToCellWithIdx(std::clamp(row, 0, (int)descriptorSet.size() - 1), HMUI::TableView::ScrollPositionType::Center, true);
        tableView->ClearSelection();
    }

    void SelectionViewController::Default()
    {
        // if we do not PROPERLY switch to default, don't clear the preview
        if (wallModelContainer->Default())
        {
            OnObjectLoadFinished();
            descriptorList->tableView->ClearSelection();
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

            if (wallModelContainer->LoadObject(descriptor, std::bind(&SelectionViewController::OnObjectLoadFinished, this)))
            {
                previewViewController->ClearPrefab();
                previewViewController->ShowLoading(true);
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
            Default();

        deletefile(descriptor.get_filePath());
        ReloadDescriptorList();
    }

    void SelectionViewController::OnObjectLoadFinished()
    {
        // something to do after we changed the object, like update preview
        Qosmetics::Walls::Config::get_config().lastUsedBox = wallModelContainer->GetWallConfig().get_isDefault() ? "" : Qosmetics::Core::FileUtils::GetFileName(wallModelContainer->GetDescriptor().get_filePath(), true);
        Qosmetics::Core::Config::SaveConfig();
        previewViewController->UpdatePreview(true);
    }
}
