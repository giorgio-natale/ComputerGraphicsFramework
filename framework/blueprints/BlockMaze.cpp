//
// Created by drgio on 19/08/2023.
//

#include "../components/scripts/MazeRepresentation.h"
#include "../components/mesh/MeshComponent.h"
#include "../components/texture/TextureComponent.h"
#include "../components/materials/SimplePhongMaterial.h"
#include "../GameEngine.h"
#include "BlockMaze.h"
#include "../components/materials/GGXMaterial.h"

namespace fmwk {
    void BlockMaze::buildEntity() {
        auto gameEngine = GameEngine::getInstance();
        auto mazeEntity = std::make_unique<fmwk::Entity>("Maze");
        char *mazeIn[] = {

                (char*)( "#######################"),
                (char*)( "##        #  #  #     #"),
                (char*)( "#  # #### ## #  #  #  #"),
                (char*)( "## # ## #    ##    #  #"),
                (char*)( "#  #     #####  #    ##"),
                (char*)( "# ###  #     #  #  #  #"),
                (char*)( "#  #########       #  #"),
                (char*)( "## #       ############"),
                (char*)( "#  # #####   #   #    #"),
                (char*)( "# ## #   ### # # # ## #"),
                (char*)("#  # ###   # ### # #  #"),
                (char*)("## #   ### #      ## ##"),
                (char*)("#  ###      ## ### #  #"),
                (char*)("# ## ## ###        ## #"),
                (char*)("#  # ##  ##### ### #  #"),
                (char*)("##   ###     # # # # ##"),
                (char*)("## #   ### # # # # #  #"),
                (char*)("#  # #     # # #   ## #"),
                (char*)("# ## ##### # # ###    #"),
                (char*)("#     #      ###   ## #"),
                (char*)("#######################")
        };
        int row = 21;
        int col = 23;

        std::unique_ptr<MazeRepresentation> mazeRepresentation = std::make_unique<MazeRepresentation>(row, col, 6.0f, mazeIn);
        auto [vertices, faces] = mazeRepresentation->buildMesh();
        gameEngine->addModel<fmwk::VertexWithNormalAndTan>("MazeModel", VERTEX_WITH_NORMAL_AND_TANGENT, vertices, faces, glm::vec3(0), glm::quat(1,0,0,0), glm::vec3(1));

        mazeEntity->addComponent(std::make_unique<fmwk::MeshComponent>(gameEngine->getModelByName("MazeModel")));
        mazeEntity->addComponent(std::make_unique<fmwk::TextureComponent>(gameEngine->getBoundTextureByName("rockColor")));
        mazeEntity->addComponent(std::make_unique<fmwk::GGXMaterial>(gameEngine->getBoundTextureByName("rockNormal").getTexture(), gameEngine->getBoundTextureByName("rockRoughness").getTexture()));
        mazeEntity->addComponent(std::move(mazeRepresentation));

        gameEngine->enqueueEntity(std::move(mazeEntity));
    }
} // fmwk