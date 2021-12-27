#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

glm::ivec2 our::texture_utils::loadImage(Texture2D &texture, const char *filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *data = stbi_load(filename, &size.x, &size.y, &channels, 4);
    if (data == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return {0, 0};
    }
    //Bind the texture such that we upload the image data to its storage
    //HINT: The steps should be as follows: bind the texture, send the pixel data to the GPU, then generate the mipmap (if requested).
    texture.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
    //Allows elements of an image array to be read by shaders where the parameters are the target,                                                                                             
    // level (0 is the base image level), the number of color components in the texture,
    // width and height  of the texture image
    // The last parameter is the actual data of the image
    if (generate_mipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D); //Generates mipmaps for the specified texture object that is bound to the target
    }

    stbi_image_free(data); //Free image data after uploading to GPU
    return size;
}