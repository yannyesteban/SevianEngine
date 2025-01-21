#include <iostream>
#include "Renderer.h"
#include "VulkanFactory.h"
#include "Game.h"
#include <locale>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace SEVIAN;

int main () {

    //std::ifstream f ( "one.json" );
    /*
    std::ifstream file ( "one.json", std::ios::binary ); // Abre el archivo en modo binario
    if (!file.is_open ()) {
        std::cerr << "Error al abrir el archivo\n";
        return 1;
    }

    try {
        nlohmann::json json_data;
        file >> json_data; // Carga el archivo JSON
        std::cout << json_data.dump ( 4 ) << std::endl; // Imprime el contenido del JSON formateado
    }
    catch (const std::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what () << std::endl;
    }

    return 0;*/

    GameInfo info = { VK, "Race", 2000 ,1000 };

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