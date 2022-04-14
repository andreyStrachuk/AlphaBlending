#ifndef blending
#define blending

#include <emmintrin.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct Screen {
    u_int8_t *pxl;
    int downStartX;
    int downStartY;
};


void OnClick (Screen *scr);

void Blend (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize);

void ScreenInit (Screen *scr, int x, int y, sf::Vector2u *size);

#endif