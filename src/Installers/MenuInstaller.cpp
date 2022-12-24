#include "Installers/MenuInstaller.hpp"

#include "UI/BoxFlowCoordinator.hpp"
#include "UI/PreviewViewController.hpp"
#include "UI/SelectionViewController.hpp"
#include "UI/SettingsViewController.hpp"

#include "lapiz/shared/arrayutils.hpp"
#include "lapiz/shared/utilities/ZenjectExtensions.hpp"

#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"

DEFINE_TYPE(Qosmetics::Walls, MenuInstaller);

using namespace ::Lapiz::Zenject::ZenjectExtensions;

namespace Qosmetics::Walls
{
    void MenuInstaller::InstallBindings()
    {
        auto container = get_Container();

        FromNewComponentOnNewGameObject(container->Bind<Qosmetics::Core::QosmeticsBaseFlowCoordinator*>()->To(Lapiz::ArrayUtils::TypeArray<Qosmetics::Walls::BoxFlowCoordinator*>()))->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<Qosmetics::Walls::PreviewViewController*>())->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<Qosmetics::Walls::SettingsViewController*>())->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<Qosmetics::Walls::SelectionViewController*>())->AsSingle();
    }
}