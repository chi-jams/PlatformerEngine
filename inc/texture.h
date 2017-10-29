#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include "SDL2/SDL.h"

// Texture Wrapper Class
class Texture
{
    private:

        // The stored texture
        SDL_Texture* aTexture;

        // Image dimensions
        int tWidth,
            tHeight;
    public: 
        // Initializes variables to default value
        Texture();

        // Deallocates memory
        ~Texture();

        // Loades texture from file
        bool loadFromFile( std::string path );

        // Deallocates Texture
        void freeTexture();

        // Changes the color of an object
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        // Set blending
        void setBlendMode( SDL_BlendMode blending );

        // Set alpha modulation
        void setAlpha( Uint8 alpha );

        // Renders a part of the texture with the left corner at input point
        void render( int x, int y, SDL_Rect* clip = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        // Getters
        SDL_Texture* getTexture() { return aTexture; }
        int getWidth() { return tWidth; }
        int getHeight() { return tHeight; }

};

#endif
