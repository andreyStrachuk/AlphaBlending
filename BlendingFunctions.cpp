#include "Blending.hpp"

void Blend (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize) {
    assert (scr);
    assert (pxl1);
    assert (pxl2);

    memcpy ((void *)scr->pxl, (void *)pxl1, downSize.x * downSize.y * 4);

    for (size_t height = 0; height < topSize.y * sizeof (u_int32_t); height += sizeof (u_int32_t)) { 
        for (size_t width = 0; width < topSize.x * sizeof (u_int32_t); width += sizeof (u_int32_t)) {

            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 0] = (pxl2[height * topSize.x + width + 0] * pxl2[height * topSize.x + width + 3]
                                                                                            + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 0]
                                                                                            * (255 - pxl2[height * topSize.x + width + 3])) >> 8;
            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 1] = (pxl2[height * topSize.x + width + 1] * pxl2[height * topSize.x + width + 3]
                                                                                            + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 1]
                                                                                            * (255 - pxl2[height * topSize.x + width + 3])) >> 8;
            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 2] = (pxl2[height * topSize.x + width + 2] * pxl2[height * topSize.x + width + 3]
                                                                                            + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 2]
                                                                                            * (255 - pxl2[height * topSize.x + width + 3])) >> 8;

        }
    }
}

void OnClick (Screen *scr) {
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right))   { scr->downStartX += 16; if (scr->downStartX > 599 * 4) scr->downStartX = 599 * 4; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))    { scr->downStartX -= 16; if (scr->downStartX < 0) scr->downStartX = 0; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))      { scr->downStartY -= 16; if (scr->downStartY < 0) scr->downStartY = 0; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))    { scr->downStartY += 16; if (scr->downStartY > 449 * 4) scr->downStartY = 449 * 4; }
}

int ScreenInit (Screen *scr, const int x, const int y, sf::Vector2u *size) {
    assert (scr);

    scr->downStartX = sizeof (u_int32_t) * x;
    scr->downStartY = sizeof (u_int32_t) * y;

    void *ptr = calloc (size->x * size->y * sizeof (u_int32_t), sizeof (u_int8_t));
    ASSERT_OK (ptr == nullptr, PrintErrors (ALLOC_FAILED); return ALLOC_FAILED);

    scr->pxl = (sf::Uint8 *)ptr;

    return OK;
}

void BlendSSE (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize) {
    assert (scr);
    assert (pxl1);
    assert (pxl2);

    const unsigned char hU= 255u;
    const unsigned char hS = 0x80u;
           
    const __m128i _zero_multiplier = _mm_set1_epi32 (0);
    const __m128i _255_multiplier = _mm_cvtepu8_epi16 (_mm_set_epi8 (hU,hU,hU,hU, hU,hU,hU,hU, hU,hU,hU,hU, hU,hU,hU,hU));

    memcpy ((void *)scr->pxl, (void *)pxl1, downSize.x * downSize.y * 4);

    for (size_t height = 0; height < topSize.y * sizeof (u_int32_t); height += sizeof (u_int32_t)) {
        for (size_t width = 0; width < topSize.x * sizeof (u_int32_t); width += 4 * sizeof (u_int32_t)) {

            __m128i top = _mm_lddqu_si128 ((__m128i *) &pxl2[height * topSize.x + width]);
            __m128i down = _mm_lddqu_si128 ((__m128i *) &pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width]);
            
            __m128i downHighBytes = (__m128i)_mm_movehl_ps((__m128)_zero_multiplier, (__m128)down);
            __m128i topHighBytes = (__m128i)_mm_movehl_ps((__m128)_zero_multiplier, (__m128)top);

            top = _mm_cvtepu8_epi16 (top);
            topHighBytes = _mm_cvtepu8_epi16 (topHighBytes);

            down = _mm_cvtepu8_epi16 (down);
            downHighBytes = _mm_cvtepu8_epi16 (downHighBytes);

            static const __m128i maskForShuffle = _mm_set_epi8 (hS, 14, hS, 14, hS, 14, hS, 14,
                                                                hS, 6, hS, 6, hS, 6, hS, 6);
            __m128i a = _mm_shuffle_epi8 (top, maskForShuffle);
            __m128i A = _mm_shuffle_epi8 (topHighBytes, maskForShuffle);

            top = _mm_mullo_epi16 (top, a);
            topHighBytes = _mm_mullo_epi16 (topHighBytes, A);

            down = _mm_mullo_epi16 (down, _mm_sub_epi16 (_255_multiplier, a));
            downHighBytes = _mm_mullo_epi16 (downHighBytes, _mm_sub_epi16 (_255_multiplier, A));

            __m128i sum = _mm_add_epi16 (top, down);
            __m128i sumHighBytes = _mm_add_epi16 (topHighBytes, downHighBytes);

            static const __m128i lowMask = _mm_set_epi8 (hS, hS, hS, hS, hS, hS, hS, hS, 
                                                            15, 13, 11, 9, 7, 5, 3, 1);

            static const __m128i highMask = _mm_set_epi8 (15, 13, 11, 9, 7, 5, 3, 1, hS, hS, hS, hS, hS, hS, hS, hS);

            sum = _mm_shuffle_epi8 (sum, lowMask);
            sumHighBytes = _mm_shuffle_epi8 (sumHighBytes, highMask);

            __m128i color = _mm_add_epi32 (sum, sumHighBytes);

            _mm_storeu_si128 ((__m128i *) &scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width], color);
        }
    }
}

void PrintErrors (const int type) {
    switch (type) {
        case ALLOC_FAILED: {
            printf ("Allocation failed!\nError code: %d\n", type);

            break;
        }
        case NULLPTR: {
            printf ("Null pointer!\nError code: %d\n", type);

            break;
        }
        case UNABLETOOPENIMAGE: {
            printf ("Unable to open image!\nError code: %d\n", type);

            break;
        }
        case UNABLETOFINDFONT: {
            printf ("Unable to find font!\nError code: %d\n", type);

            break;
        }
        default: {
            printf ("Unknown error!\nError code: %d\n", type);
        }
    }
}
