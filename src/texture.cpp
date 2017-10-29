

#include <string>
#include <iostream>
#include "texture.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_Extra_Operations.h"


extern SDL_Renderer* mainRenderer;

Texture::Texture()
{
    aTexture = NULL;
    tWidth = 0;
    tHeight = 0;
}

Texture::~Texture()
{
    freeTexture();
}

bool Texture::loadFromFile( std::string path )
{
    // Make sure we're not overwriting the texture
    // Freeing first ensures that we don't have a memory leak
    freeTexture();

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        std::cout << "Failed to load surface! SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Color key the image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) ); 

        
        aTexture = SDL_CreateTextureFromSurface( mainRenderer, loadedSurface );
        if( aTexture == NULL )
        {
            std::cout << "Failed to create texture! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            // Get dimensions of surface
            tWidth = loadedSurface->w;
            tHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    return aTexture != NULL;
}

void Texture::freeTexture()
{
    // Makes sure that there is a texture to destroy before destroying it.
    // We don't want to be deleting something random in memory.
    if( aTexture != NULL )
    {
        SDL_DestroyTexture( aTexture );
        aTexture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    // Modulate texture
    SDL_SetTextureColorMod( aTexture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( aTexture, blending );
}

void Texture::setAlpha( Uint8 alpha )
{

    SDL_SetTextureAlphaMod( aTexture, alpha );
}

void Texture::render( int x, int y, SDL_Rect* clip, SDL_RendererFlip flip )
{
    // Size and position of output texture.
    SDL_Rect renderRect = { x, y, tWidth, tHeight };
    
    // If we are taking only part of a texture,
    // then reduce the size of the output texture
    // to the piece of the texture.
    if( clip != NULL )
    {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }

    SDL_RenderCopyEx( mainRenderer, aTexture, clip, &renderRect, 0, NULL, flip );
}
