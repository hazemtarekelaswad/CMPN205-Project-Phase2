#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our {

    // This class defined an OpenGL sampler
    class Sampler {
        // The OpenGL object name of this sampler 
        GLuint name;
    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name" 
        //Sampler is used to pass the texture object to the fragment shader
        Sampler() {
            glGenSamplers(1, &name); //Generate 1 sampler and store the generated sampler object name in (name)
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler() { 
            glDeleteSamplers(1, &name); //Deletes 1 sampler 
         }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const {
            glBindSampler(textureUnit, name); //texture unit is the index of the texture unit to which the sampler is bound
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit){
            glBindSampler(textureUnit,0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const {
            glSamplerParameteri(name,parameter,value); //Assign value to the sampler parameter, "name" is the sampler object to be modified 
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const {
            glSamplerParameterf(name,parameter,value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const {
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json& data);

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

}