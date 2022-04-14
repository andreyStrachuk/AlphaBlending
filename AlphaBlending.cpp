#include "Blending.hpp"

// const char I = 255u,
//            Z = 0x80u;
           
// const __m128i   _0 =                  _mm_set_epi8 (0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
// const __m128i _255 = _mm_meow8_epi16 (_mm_set_epi8 (I,I,I,I, I,I,I,I, I,I,I,I, I,I,I,I));

//-------------------------------------------------------------------------------------------------

int main() {
    sf::Image topIm;
    topIm.loadFromFile ("src/Racket.bmp");

    sf::Image downIm;
    downIm.loadFromFile ("src/Table.bmp");

    sf::Font font;
    font.loadFromFile ("src/Ubuntu-Bold.ttf");

    sf::Vector2u topSize = topIm.getSize ();
    sf::Vector2u downSize = downIm.getSize ();
    
    const sf::Uint8 *pxl2 = topIm.getPixelsPtr ();
    const sf::Uint8 *pxl1 = downIm.getPixelsPtr ();

    Screen scr {};
    ScreenInit (&scr, 300, 225, &downSize);

    sf::RenderWindow window(sf::VideoMode(downSize.x, downSize.y), "SFML window");
    sf::Clock clock;
    sf::Time time;

    while (window.isOpen()) {

        OnClick (&scr);

        for (int i = 0; i < 500; ++i)                       // this is made for noticing fps and further optimization
            Blend (&scr, pxl1, pxl2, topSize, downSize);

        time = clock.getElapsedTime (); 
        sf::Text txt (std::to_string (1 / time.asSeconds()), font);
        clock.restart().asSeconds(); 

        sf::Image newDownIm;
        newDownIm.create (downSize.x, downSize.y, scr.pxl);

        sf::Texture texture;
        texture.create (downSize.x, downSize.y);
        texture.update (newDownIm);

        sf::Sprite spr;
        spr.setTexture (texture);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear ();
        texture.update (newDownIm);
        window.draw (spr);
        window.draw (txt);
        window.display ();
    }

    return 0;
}
    


