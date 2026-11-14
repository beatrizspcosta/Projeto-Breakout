/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 *  
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#include "Bolinha.h"
#include "GameWorld.h"
#include "Jogador.h"
#include "Alvo.h"
#include <ResourceManager.h>

/* ── constantes ─────────────────────────────────────────────── */
#define LARGURA_TELA     600
#define ALTURA_TELA      800

#define LARGURA_JOGADOR  130.0f
#define ALTURA_JOGADOR    18.0f
#define Y_JOGADOR        740.0f
#define VEL_JOGADOR      500.0f

#define RAIO_BOLINHA      14.0f
#define VEL_BOLINHA_X    260.0f
#define VEL_BOLINHA_Y   -320.0f //negativo = sobe

#define MARGEM_ALVO_H      8.0f
#define MARGEM_ALVO_V      8.0f
#define OFFSET_ALVO_Y     70.0f
#define ALTURA_ALVO       22.0f
#define PONTOS_POR_LINHA  10

/* ── protótipos internos ─────────────────────────────────────── */
void resetarBolinha( GameWorld *gw );
void resetarJogador( GameWorld *gw );
void inicializarAlvos( GameWorld *gw );
void reiniciarJogo( GameWorld *gw );
void checarColisaoBolinhaJogador( GameWorld *gw );
void checarColisaoBolinhaAlvos( GameWorld *gw );
void checarBordaInferior( GameWorld *gw );
void desenharHUD( GameWorld *gw );
void desenharPausa( GameWorld *gw );
void desenharVitoria( GameWorld *gw );
void desenharGameOver( GameWorld *gw );

/* ══════════════════════════════════════════════════════════════ */
/*  Criação / destruição                                          */
/* ══════════════════════════════════════════════════════════════ */

GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld *) malloc( sizeof( GameWorld ) );
    reiniciarJogo( gw );
    return gw;
}

void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/* ══════════════════════════════════════════════════════════════ */
/*  Update                                                        */
/* ══════════════════════════════════════════════════════════════ */

void updateGameWorld( GameWorld *gw, float delta ) {

/*______________________________MUSICAS______________________________*/

    if ( gw->estado == ESTADO_VITORIA ) { //esse if vai verificar se venceu, e ai parar os outros sons e tocar o de vitoria
        if ( IsMusicStreamPlaying( rm.musicExample ) ) {
            StopMusicStream( rm.musicExample );
        }
        
        if ( !IsMusicStreamPlaying( rm.musicVitoria ) ) {
            PlayMusicStream( rm.musicVitoria );
            SetMusicVolume( rm.musicVitoria, 0.5f );
        } else {
            UpdateMusicStream( rm.musicVitoria );
        }
    } else if (gw->estado == ESTADO_GAME_OVER ) { //esse é igual o anterior, mas com o game over
        if ( IsMusicStreamPlaying( rm.musicExample ) ) {
            StopMusicStream( rm.musicExample );
        }

        if ( !IsMusicStreamPlaying( rm.musicGameOver ) ) {
            PlayMusicStream( rm.musicGameOver );
            SetMusicVolume( rm.musicGameOver, 0.8f ); 
        } else {
            UpdateMusicStream( rm.musicGameOver );
        }
    } else { //aqui é caso nao esteja em nenhum dos outros dois estados, ai roda a musiquinha normal do jogo
        if ( IsMusicStreamPlaying( rm.musicVitoria ) ) {
            StopMusicStream( rm.musicVitoria );
        }

        if ( !IsMusicStreamPlaying( rm.musicExample ) ) {
            PlayMusicStream( rm.musicExample );
            SetMusicVolume( rm.musicExample, 0.2f ); 
        } else {
            UpdateMusicStream( rm.musicExample );
        }
    }

/*______________________________FIM MUSICAS______________________________*/

    switch ( gw->estado ) {

        case ESTADO_AGUARDANDO:
            gw->jogador.velocidadeAtual = 0; //jogador nao pode se mover enquanto estiver aguardando

            if ( IsKeyPressed( KEY_SPACE ) ) {
                gw->estado = ESTADO_JOGANDO;
            }

            break;

        case ESTADO_JOGANDO:
            entradaJogador ( &gw->jogador );
            atualizarJogador( &gw->jogador, delta );
            atualizarBolinha( &gw->bolinha, delta );

            checarColisaoBolinhaJogador( gw );
            checarColisaoBolinhaAlvos  ( gw );
            checarBordaInferior        ( gw );

            if ( gw->alvosRestantes == 0 ) {
                gw->estado = ESTADO_VITORIA;
            }

            break;

        case ESTADO_VITORIA:
        case ESTADO_GAME_OVER:
       
            gw->jogador.velocidadeAtual = 0; //travar jogador para nao se mexer

            if ( IsKeyPressed( KEY_SPACE ) ) {
                reiniciarJogo( gw );
            }

            break;
    }
}

/* ══════════════════════════════════════════════════════════════ */
/*  Draw                                                          */
/* ══════════════════════════════════════════════════════════════ */

void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( (Color){ 255, 210, 225, 255 } );

    desenharAlvos  ( gw->alvos, TOTAL_ALVOS );
    desenharJogador( &gw->jogador );
    desenharBolinha( &gw->bolinha );
    desenharHUD    ( gw );

    //deixar mais escuro quando estiver aguardando
    if ( gw->estado == ESTADO_AGUARDANDO ) {
        DrawRectangle( 0, 0, LARGURA_TELA, ALTURA_TELA, ColorAlpha( WHITE, 0.25f ) ); 
    }

    if ( gw->estado == ESTADO_VITORIA ) {
        DrawRectangle( 0, 0, LARGURA_TELA, ALTURA_TELA, ColorAlpha( GOLD, 0.5f ) ); 
    }

    if ( gw->estado == ESTADO_GAME_OVER ) {
        DrawRectangle( 0, 0, LARGURA_TELA, ALTURA_TELA, ColorAlpha( RED, 0.5f ) ); 
    }


    switch ( gw->estado ) {

        case ESTADO_AGUARDANDO: desenharPausa   ( gw ); break;
        case ESTADO_VITORIA:    desenharVitoria ( gw ); break;
        case ESTADO_GAME_OVER:  desenharGameOver( gw ); break;

        default: break;
    }

    EndDrawing();

}

/* ══════════════════════════════════════════════════════════════ */
/*  Reset / inicialização                                         */
/* ══════════════════════════════════════════════════════════════ */

void resetarJogador( GameWorld *gw ) {

    gw->jogador.ret.width  = LARGURA_JOGADOR;
    gw->jogador.ret.height = ALTURA_JOGADOR;
    gw->jogador.ret.x      = ( LARGURA_TELA - LARGURA_JOGADOR ) / 2.0f;
    gw->jogador.ret.y      = Y_JOGADOR;
    gw->jogador.velocidadeBase  = VEL_JOGADOR;
    gw->jogador.velocidadeAtual = 0.0f;
    gw->jogador.cor = (Color){ 180, 0, 80, 255 };

}

void resetarBolinha( GameWorld *gw ) {

    gw->bolinha.raio     = RAIO_BOLINHA;
    gw->bolinha.centro.x = LARGURA_TELA / 2.0f;
    gw->bolinha.centro.y = Y_JOGADOR - RAIO_BOLINHA - 2.0f;
    gw->bolinha.vel.x    = VEL_BOLINHA_X;
    gw->bolinha.vel.y    = VEL_BOLINHA_Y;
    gw->bolinha.cor      = RED;

    if ( gw->pontuacao >= 240 ) {
        aumentarVelocidade( &gw->bolinha );
    }

    if ( gw->pontuacao >= 480 ) {
        aumentarVelocidade( &gw->bolinha );
    }

}

void inicializarAlvos( GameWorld *gw ) {

    float larguraAlvo = ( LARGURA_TELA - ( COLUNAS_ALVOS + 1 ) * MARGEM_ALVO_H )
                        / (float) COLUNAS_ALVOS;

    Color cores[LINHAS_ALVOS] = {

        (Color){ 210,  30,  90, 255 },
        (Color){ 230,  50, 120, 255 },
        (Color){ 245,  80, 140, 255 },
        (Color){ 190,  30, 110, 255 },
        (Color){ 170,  40, 100, 255 },
        (Color){ 220,  60, 125, 255 },
        (Color){ 240,  70, 130, 255 },
        (Color){ 200,  35, 105, 255 }

    };

    gw->alvosRestantes = 0;

    for ( int i = 0; i < LINHAS_ALVOS; i++ ) {
        for ( int col = 0; col < COLUNAS_ALVOS; col++ ) {

            int lca = i * COLUNAS_ALVOS + col;

            gw->alvos[lca].ret.x      = MARGEM_ALVO_H + col * ( larguraAlvo + MARGEM_ALVO_H );
            gw->alvos[lca].ret.y      = OFFSET_ALVO_Y + i * ( ALTURA_ALVO + MARGEM_ALVO_V );

            gw->alvos[lca].ret.width  = larguraAlvo;
            gw->alvos[lca].ret.height = ALTURA_ALVO;

            gw->alvos[lca].cor        = cores[i];
            gw->alvos[lca].hp         = 1; //health points
            gw->alvos[lca].pontos     = 15;

            gw->alvosRestantes++;

        }
    }
}

void reiniciarJogo( GameWorld *gw ) {

    gw->vidas     = VIDAS_INICIAIS;
    gw->pontuacao = 0;
    gw->estado    = ESTADO_AGUARDANDO;

    gw->usouVelocidade240  = false;
    gw->usouVelocidade480 = false;

    resetarJogador ( gw );
    resetarBolinha ( gw );
    inicializarAlvos( gw );

}

/* ══════════════════════════════════════════════════════════════ */
/*  Colisões                                                      */
/* ══════════════════════════════════════════════════════════════ */

void checarColisaoBolinhaJogador( GameWorld *gw ) {

    Bolinha *b = &gw->bolinha;
    Jogador *j = &gw->jogador;

    if ( !CheckCollisionCircleRec( b->centro, b->raio, j->ret ) ) {
        return;
    }
    if ( b->vel.y < 0 ) { 
        return;
     } //ja subindo, ignora

    /* inverte vertical e empurra para cima do jogador */
    b->vel.y    = -fabsf( b->vel.y );
    b->centro.y = j->ret.y - b->raio - 1.0f;

}

void checarColisaoBolinhaAlvos( GameWorld *gw ) {

    Bolinha *b = &gw->bolinha;

    for ( int i = 0; i < TOTAL_ALVOS; i++ ) {

        Alvo *a = &gw->alvos[i];

        if ( a->hp <= 0 ) {
            continue;
        }

        if ( !CheckCollisionCircleRec( b->centro, b->raio, a->ret ) ) {
            continue;
        }

        /* destroi o alvo */
        a->hp = 0;
        gw->pontuacao += a->pontos; //aumenta a pontuação
        gw->alvosRestantes--;

        /* inverte a direção vertical */
        b->vel.y = -b->vel.y;

        PlaySound( rm.soundExample );

        break; /*apenas um alvo por frame, sai do for*/

    }

    //aumentar a velocidade da bolinha a partir de X pontos
    if ( gw->pontuacao >= 240 && !gw->usouVelocidade240 ) {
        aumentarVelocidade( b );
        gw->usouVelocidade240 = true;
    }

    if ( gw->pontuacao >= 480 && !gw->usouVelocidade480 ) {
        aumentarVelocidade( b );
        gw->usouVelocidade480 = true;
    }

}

void checarBordaInferior( GameWorld *gw ) {

    Bolinha *b = &gw->bolinha;

    if ( b->centro.y - b->raio > GetScreenHeight() ) {

        gw->vidas--;

        if ( gw->vidas <= 0 ) {
            gw->estado = ESTADO_GAME_OVER;
        } else {
            resetarBolinha( gw );
            resetarJogador( gw );
            gw->estado = ESTADO_AGUARDANDO;
        }

    }

}

/* ══════════════════════════════════════════════════════════════ */
/*  Desenhos HUD/telas etc                                        */
/* ══════════════════════════════════════════════════════════════ */

void desenharHUD( GameWorld *gw ) {

    char buf[64];

    //placar de pontos
    sprintf( buf, "PONTOS: %d", gw->pontuacao );
    DrawText( buf, 15, 17, 20, (Color){ 120, 0, 50, 255 } );

    DrawText( "VIDAS: ", LARGURA_TELA - 165, 17, 20, (Color){ 120, 0, 50, 255 } );

    //icones das vidas restantes (circulos para representar coracoes)
    for ( int i = 0; i < gw->vidas; i++ ) {
        int posX = ( LARGURA_TELA - 85 ) + ( i * 25 ); 
        int posY = 20;

        DrawCircle( posX + 8, posY + 5, 10, (Color){ 180, 0, 80, 255 } );
    }

    DrawLine( 0, 50, LARGURA_TELA, 50, (Color){ 180, 100, 130, 255 } );

}

void desenharPausa( GameWorld *gw ) {

    (void) gw;
    int tw = MeasureText( "PRESSIONE ESPACO PARA JOGAR", 25 );
    DrawText( "PRESSIONE ESPACO PARA JOGAR",
              ( LARGURA_TELA - tw ) / 2,
              ALTURA_TELA / 2,
              25, ( (Color){ 255, 20, 147, 255 } ) );

}

void desenharVitoria( GameWorld *gw ) {

    char buf[64];
    sprintf( buf, "PONTUACAO: %d", gw->pontuacao );

    int tw1 = MeasureText( "VOCE VENCEU!", 36 );
    int tw2 = MeasureText( buf, 22 );
    int tw3 = MeasureText( "ESPACO para jogar novamente", 20 );

    DrawText( "VOCE VENCEU!",
              ( LARGURA_TELA - tw1 ) / 2, ALTURA_TELA / 2 - 50, 36, (Color){ 120, 0, 50, 255 } );
    DrawText( buf,
              ( LARGURA_TELA - tw2 ) / 2, ALTURA_TELA / 2,      22, (Color){ 160, 0, 70, 255 } );
    DrawText( "ESPACO para jogar novamente",
              ( LARGURA_TELA - tw3 ) / 2, ALTURA_TELA / 2 + 40, 20, (Color){ 100, 0, 40, 255 } );

}

void desenharGameOver( GameWorld *gw ) {

    char buf[64];
    sprintf( buf, "PONTUACAO: %d", gw->pontuacao );

    int tw1 = MeasureText( "GAME OVER", 40 );
    int tw2 = MeasureText( buf, 22 );
    int tw3 = MeasureText( "ESPACO para tentar novamente", 20 );

    DrawText( "GAME OVER",
              ( LARGURA_TELA - tw1 ) / 2, ALTURA_TELA / 2 - 50, 40, (Color){ 120, 0, 50, 255 } );
    DrawText( buf,
              ( LARGURA_TELA - tw2 ) / 2, ALTURA_TELA / 2,      22, (Color){ 160, 0, 70, 255 } );
    DrawText( "ESPACO para tentar novamente",
              ( LARGURA_TELA - tw3 ) / 2, ALTURA_TELA / 2 + 40, 20, (Color){ 100, 0, 40, 255 } );

}