#pragma once

#include <glad/gl.h>

namespace our
{

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D
    {
        // The OpenGL object name of this texture
        GLuint name = 0;

    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name"
        Texture2D()
        {
            glGenTextures(1, &name); //Genreate texture at name
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D()
        {
            glDeleteTextures(1,&name); //Delete texture at name
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, name); //bound the texture name to the target GL_TEXTURE_2D
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0); //Unbind 
        }

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
    };

}