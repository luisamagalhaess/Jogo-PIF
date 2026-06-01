#ifndef TIPOS_H
#define TIPOS_H

#define TOTAL_ONDAS 5
 
#define LINHAS  5
#define COLUNAS 9
 
#define DEFESA_GERADOR  0
#define DEFESA_TORRETA  1
#define DEFESA_MURO     2
#define DEFESA_BOMBA    3
 
#define ALIEN_INVASOR   0
#define ALIEN_BLINDADO  1
#define ALIEN_KAMIKAZE  2

#define ROUNDED_LINES(rec, r, s, cor) DrawRectangleRoundedLines(rec, r, s, 2.0f, cor)

#ifdef _WIN32
    #define LIMPAR system("cls")
#else
    #define LIMPAR system("clear")
#endif
 
#endif