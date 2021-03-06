#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"

#include <glad/gl.h>
#include <vector>
#include <algorithm>

namespace our
{
    
    // The render command stores command that tells the renderer that it should draw
    // the given mesh at the given localToWorld matrix using the given material
    // The renderer will fill this struct using the mesh renderer components
    struct RenderCommand {
        glm::mat4 localToWorld;
        glm::vec3 center;
        Mesh* mesh;
        Material* material;
    };

    // A forward renderer is a renderer that draw the object final color directly to the framebuffer
    // In other words, the fragment shader in the material should output the color that we should see on the screen
    // This is different from more complex renderers that could draw intermediate data to a framebuffer before computing the final color
    // In this project, we only need to implement a forward renderer
    class ForwardRenderer {
        // These are two vectors in which we will store the opaque and the transparent commands.
        // We define them here (instead of being local to the "render" function) as an optimization to prevent reallocating them every frame
        std::vector<RenderCommand> opaqueCommands;
        std::vector<RenderCommand> transparentCommands;
    public:
        // This function should be called every frame to draw the given world
        // Both viewportStart and viewportSize are using to define the area on the screen where we will draw the scene
        // viewportStart is the lower left corner of the viewport (in pixels)
        // viewportSize is the width & height of the viewport (in pixels). It is also used to compute the aspect ratio
        void render(World* world, glm::ivec2 viewportStart, glm::ivec2 viewportSize){
            // First of all, we search for a camera and for all the mesh renderers
            CameraComponent* camera = nullptr;
            opaqueCommands.clear();
            transparentCommands.clear();
            for(auto entity : world->getEntities()){
                // If we hadn't found a camera yet, we look for a camera in this entity
                if(!camera) camera = entity->getComponent<CameraComponent>();
                // If this entity has a mesh renderer component
                if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                    // We construct a command from it
                    RenderCommand command;
                    command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                    command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                    command.mesh = meshRenderer->mesh;
                    command.material = meshRenderer->material;
                    // if it is transparent, we add it to the transparent commands list
                    if(command.material->transparent){
                        transparentCommands.push_back(command);
                    } else {
                    // Otherwise, we add it to the opaque command list
                        opaqueCommands.push_back(command);
                    }
                }
            }

            // If there is no camera, we return (we cannot render without a camera)
            if(camera == nullptr) return;

            glm::vec3 cameraForward = camera->getViewMatrix()[2]; // Let cameraForward store the "forward" vector (third column in the view matrix)
            //The 3rd Column is the forward pointing vector in the 4x4 view Matrix since our view system is camera to world Space.
            std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
                // HINT: the following return should return true "first" should be drawn before "second". 
                return first.center[2] < second.center[2];  // Sort descending (farthest then the nearest)
                
            });

            // Get the camera ViewProjection matrix and store it in VP
            glm::mat4 VP = camera->getProjectionMatrix(viewportSize) * camera->getViewMatrix();
            // Set the OpenGL viewport using viewportStart and viewportSize
            glViewport(viewportStart.x, viewportStart.y, viewportSize.x, viewportSize.y);
            // Set the clear color to black and the clear depth to 1
            glClearColor(0.0f, 0.0f ,0.0f ,0.0f);
            glClearDepth(1.0f);
            // Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
            glColorMask(true,true,true,true);
            glDepthMask(true);
            // Clear the color and depth buffers
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);
            // Draw all the opaque commands followed by all the transparent commands
            // Set the "transform" uniform to be equal the model-view-projection matrix for each render command
            for(auto command : opaqueCommands)
            {
                command.material->setup();
                command.material->shader->set("transform", VP * command.localToWorld);  // set the transfom uniform to a model-view-projection matrix
                command.mesh->draw();
            }

            for (auto command : transparentCommands)
            {
                command.material->setup();
                command.material->shader->set("transform", VP * command.localToWorld);
                command.mesh->draw();
            }
            
        };


    };

}