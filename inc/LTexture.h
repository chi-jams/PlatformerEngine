#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include <string>
#include "SDL2/SDL.h"




// Texture Wrapper Class
class LTexture
{
    public: 
        // Initializes variables to default value
        LTexture();

        // Deallocates memory
        ~LTexture();

        // Loades texture from file
        bool loadFromFile( std::string path );

        // Deallocates Texture
        void freeLTexture();

        // Changes the color of an object
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        // Set blending
        void setBlendMode( SDL_BlendMode blending );

        // Set alpha modulation
        void setAlpha( Uint8 alpha );

        // Renders a part of the texture with the left corner at input point
        void render( int x, int y, SDL_Rect* clip = NULL );

        // Getters
        int getWidth();
        int getHeight();

    private:

        // The stored texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth,
            mHeight;

};

#endif
