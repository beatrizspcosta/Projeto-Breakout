/**
 * @file Bolinha.c
 * @author Prof. Dr. David Buzatto
 * @brief Bolinha implementation.
 * @copyright Copyright (c) 2026
 */
#include "raylib/raylib.h"
#include "Bolinha.h"

/**
 * Atualiza a posição da bolinha na tela e resolve a colisão da mesma com as
 * extremidades da janela (exceto a borda inferior, tratada pelo GameWorld).
 */
void atualizarBolinha( Bolinha *bolinha, float delta ) {

    bolinha->centro.x += bolinha->vel.x * delta;
    bolinha->centro.y += bolinha->vel.y * delta;

    /* borda esquerda */
    if ( bolinha->centro.x - bolinha->raio <= 0 ) {
        bolinha->centro.x = bolinha->raio;
        bolinha->vel.x    = -bolinha->vel.x;
    }
    /* borda direita */
    else if ( bolinha->centro.x + bolinha->raio >= GetScreenWidth() ) {
        bolinha->centro.x = GetScreenWidth() - bolinha->raio;
        bolinha->vel.x    = -bolinha->vel.x;
    }

    /* borda superior */
    if ( bolinha->centro.y - bolinha->raio <= 0 ) {
        bolinha->centro.y = bolinha->raio;
        bolinha->vel.y    = -bolinha->vel.y;
    }
    /* borda inferior e tratada em GameWorld.c */
}

/**
 * Desenha a bolinha.
 */
void desenharBolinha( Bolinha *bolinha ) {
    DrawCircleV( bolinha->centro, bolinha->raio, bolinha->cor );
}

void aumentarVelocidade( Bolinha *bolinha ) {

    if ( bolinha->vel.x > 0 ) {
        bolinha->vel.x = bolinha->vel.x + 200;
    } else { 
        bolinha->vel.x = bolinha->vel.x - 200;
    }

    bolinha->cor = PINK;

}