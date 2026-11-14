/**
 * @file Alvo.c
 * @author Prof. Dr. David Buzatto
 * @brief Alvo implementation.
 * @copyright Copyright (c) 2026
 */
#include "raylib/raylib.h"
#include "Alvo.h"

/**
 * Desenha um alvo.
 */
void desenharAlvo( Alvo *alvo ) {
    if ( alvo->hp > 0 ) {
        DrawRectangleRounded( (Rectangle) { 
            alvo->ret.x, 
            alvo->ret.y, 
            alvo->ret.width, 
            alvo->ret.height 
        }, 
        0.3, 
        4, 
        alvo->cor );
    }
}

/**
 * Desenha todos os alvos de um array que tem tamanho igual a quantidade.
 */
void desenharAlvos( Alvo *alvos, int quantidade ) {
    for ( int i = 0; i < quantidade; i++ ) {
        desenharAlvo( &alvos[i] );
    }
}