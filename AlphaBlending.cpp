#include "Blending.hpp"

int main() {
    sf::Image topIm;
    int res = topIm.loadFromFile ("src/Racket.bmp");
    ASSERT_OK (res == 0, PrintErrors (UNABLETOOPENIMAGE); return UNABLETOOPENIMAGE);

    sf::Image downIm;
    res = downIm.loadFromFile ("src/Table.bmp");
    ASSERT_OK (res == 0, PrintErrors (UNABLETOOPENIMAGE); return UNABLETOOPENIMAGE);

    sf::Font font;
    res = font.loadFromFile ("src/Ubuntu-Bold.ttf");
    ASSERT_OK (res == 0, PrintErrors (UNABLETOFINDFONT); return UNABLETOFINDFONT);

    sf::Vector2u topSize = topIm.getSize ();
    sf::Vector2u downSize = downIm.getSize ();
    
    const sf::Uint8 *pxl2 = topIm.getPixelsPtr ();
    const sf::Uint8 *pxl1 = downIm.getPixelsPtr ();

    Screen scr {};
    res = ScreenInit (&scr, 300, 225, &downSize);
    ASSERT_OK (res != OK, return res);

    sf::RenderWindow window(sf::VideoMode(downSize.x, downSize.y), "SFML window");
    sf::Clock clock;
    sf::Time time;

    while (window.isOpen()) {

        OnClick (&scr);

        for (int i = 0; i < 50; ++i)                       // this is made for noticing fps
            BlendSSE (&scr, pxl1, pxl2, topSize, downSize);

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
    


