/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {
    rm.textureExample = LoadTexture( "resources/images/mario.png" );
    rm.soundExample = LoadSound( "resources/sfx/bola.mp3" );
    rm.musicExample = LoadMusicStream( "resources/musics/musiquinha.mp3" );
    rm.musicVitoria = LoadMusicStream( "resources/musics/vitoria.mp3" );
    rm.musicGameOver = LoadMusicStream( "resources/musics/gameover.mp3" );
}

void unloadResourcesResourceManager( void ) {
    UnloadTexture( rm.textureExample );
    UnloadSound( rm.soundExample );
    UnloadMusicStream( rm.musicExample );
    UnloadMusicStream( rm.musicVitoria );
}