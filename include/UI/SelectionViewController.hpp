#pragma once

#include "HMUI/ViewController.hpp"
#include "UI/PreviewViewController.hpp"
#include "custom-types/shared/macros.hpp"

#include "bsml/shared/BSML/Components/CustomListTableData.hpp"
#include "qosmetics-core/shared/UI/Components/DeletionConfirmationModal.hpp"
#include "qosmetics-core/shared/UI/Components/QosmeticObjectTableCell.hpp"
#include "qosmetics-core/shared/UI/Components/QosmeticObjectTableData.hpp"

#include "lapiz/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Walls, SelectionViewController, HMUI::ViewController,
                      DECLARE_INSTANCE_FIELD_PRIVATE(WallModelContainer*, wallModelContainer);
                      DECLARE_INSTANCE_FIELD_PRIVATE(PreviewViewController*, previewViewController);
                      DECLARE_INSTANCE_FIELD_PRIVATE(BSML::CustomListTableData*, descriptorListTableData);
                      DECLARE_INSTANCE_FIELD_PRIVATE(Qosmetics::Core::QosmeticObjectTableData*, descriptorList);
                      DECLARE_INSTANCE_FIELD_PRIVATE(Qosmetics::Core::DeletionConfirmationModal*, deletionConfirmationModal);

                      DECLARE_INJECT_METHOD(void, Inject, PreviewViewController* previewViewController, WallModelContainer* wallModelContainer);
                      DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                      DECLARE_INSTANCE_METHOD(void, Refresh);
                      DECLARE_INSTANCE_METHOD(void, PostParse);
                      DECLARE_INSTANCE_METHOD(void, Default);
                      DECLARE_INSTANCE_METHOD(int, GetSelectedCellIdx);
                      DECLARE_DEFAULT_CTOR();
                      public
                      :

                      void OnDeleteCell(Qosmetics::Core::QosmeticObjectTableCell* cell);
                      void OnSelectDescriptor(Qosmetics::Core::QosmeticObjectTableCell* cell);
                      void ReloadDescriptorList();
                      void OnObjectLoadFinished();
                      void RefreshAfterWallConversion();

)
