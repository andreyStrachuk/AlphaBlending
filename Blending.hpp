#ifndef blending
#define blending

#include <immintrin.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

struct Screen {
    u_int8_t *pxl;
    int downStartX;
    int downStartY;
};

enum ERRORS {
    OK,
    NULLPTR,
    UNABLETOOPENIMAGE,
    UNABLETOFINDFONT,
    ALLOC_FAILED
};

void PrintErrors (const int type);

void OnClick (Screen *scr);

void Blend (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize);

void BlendSSE (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize);

int ScreenInit (Screen *scr, const int x, const int y, sf::Vector2u *size);

int MakeColor (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize, int width, int height);

#endif