#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

#define APP_NAME "Chess Company"
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 496
#define BACKGROUND_COLOR (Color){0xf5, 0xe0, 0xba, 0xff}
#define WHITE_SPACE_COLOR (Color){0xd7, 0xaa, 0x60, 0xff}
#define BLACK_SPACE_COLOR (Color){0x1b, 0x1a, 0x1d, 0xff}
#define WHITE_PIECE_COLOR (Color){0xd0, 0xca, 0xa9, 0xff}
#define BLACK_PIECE_COLOR (Color){0x4b, 0x4f, 0x53, 0xff}
#define SELECTION_SPACE_COLOR (Color){251, 192, 147, 128}

#define CHESS_PIECE_SVG "resources/sprites/chess_pieces.svg"
#define SVG_RES_X 1350
#define SVG_RES_Y 450

extern Texture chessPiecesTexture;
extern Texture chessBoardTexture;
extern Shader chessPieceShader;
extern Shader chessBoardShader;
extern int isWhiteLoc;
extern int chessPieceTextureLoc;
extern int offsetLoc;

#endif