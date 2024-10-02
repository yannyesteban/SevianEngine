#include <iostream>
#include "VulkanRenderer.h"
#include "VulkanFactory.h"
#include "Game.h"
using namespace SEVIAN;

int main () {

    GameInfo info = { VK, "Race", 1300 ,600 };

    std::unique_ptr<Game> race = std::make_unique<Game> ( info );

    race->play ();

    return 0;
}


/*#include <iostream>
#include "VulkanRenderer.h"
#include "VulkanFactory.h"
#include "App1.h"
using namespace SEVIAN;

int main()
{
    std::unique_ptr<RendererFactory> factory;
  
    factory = std::make_unique<VulkanFactory> ();

    App1 myGame ( std::move ( factory ) );

    myGame.run ();


    return 0;
}
*/