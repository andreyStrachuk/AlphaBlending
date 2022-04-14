#include "Blending.hpp"

void Blend (Screen *scr, const u_int8_t *pxl1, const u_int8_t *pxl2, sf::Vector2u topSize, sf::Vector2u downSize) {
    assert (scr);
    assert (pxl1);
    assert (pxl2);

    memcpy ((void *)scr->pxl, (void *)pxl1, downSize.x * downSize.y * 4);

    for (size_t height = 0; height < topSize.y * 4; height += 4) { 
        for (size_t width = 0; width < topSize.x * 4; width += 4) {

            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 0] = (pxl2[height * topSize.x + width + 0] * pxl2[height * topSize.x + width + 3] + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 0] * (255 - pxl2[height * topSize.x + width + 3])) >> 8;
            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 1] = (pxl2[height * topSize.x + width + 1] * pxl2[height * topSize.x + width + 3] + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 1] * (255 - pxl2[height * topSize.x + width + 3])) >> 8;
            scr->pxl[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 2] = (pxl2[height * topSize.x + width + 2] * pxl2[height * topSize.x + width + 3] + pxl1[scr->downStartX + (height + scr->downStartY) * downSize.x + width + 2] * (255 - pxl2[height * topSize.x + width + 3])) >> 8;

        }
    }
}

void OnClick (Screen *scr) {
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right))   { scr->downStartX += 4; if (scr->downStartX > 599 * 4) scr->downStartX = 599 * 4; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))    { scr->downStartX -= 4; if (scr->downStartX < 0) scr->downStartX = 0; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))      { scr->downStartY -= 4; if (scr->downStartY < 0) scr->downStartY = 0; }
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))    { scr->downStartY += 4; if (scr->downStartY > 449 * 4) scr->downStartY = 449 * 4; }
}

void ScreenInit (Screen *scr, int x, int y, sf::Vector2u *size) {
    assert (scr);

    scr->downStartX = 4 * x;
    scr->downStartY = 4 * y;

    scr->pxl = (sf::Uint8 *)calloc (size->x * size->y * 4, sizeof (u_int8_t));
}