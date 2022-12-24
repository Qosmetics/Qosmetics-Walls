#include "Installers/AppInstaller.hpp"

#include "CustomTypes/WallModelContainer.hpp"
#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "lapiz/shared/utilities/ZenjectExtensions.hpp"

DEFINE_TYPE(Qosmetics::Walls, AppInstaller);

namespace Qosmetics::Walls
{
    void AppInstaller::InstallBindings()
    {
        auto container = get_Container();

        ::Lapiz::Zenject::ZenjectExtensions::FromNewComponentOnNewGameObject(container->BindInterfacesAndSelfTo<Qosmetics::Walls::WallModelContainer*>())->AsSingle()->NonLazy();
    }
}