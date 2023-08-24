#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Global.h"
#include "DrawText.h"
#include "Pacman.h"
#include "Ghost.h"
#include "GhostManager.h"
#include "ConvertSketch.h"
#include "DrawMap.h"
#include "MapCollision.h"
#include "Sound.h"
#include<fstream>
#include"MainMenu.h"
#include"Howtoplay.h"
#include<iostream>
#include "SnowAnimation.h"
#include <iostream>
#include"SnowLayer.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int disappear = 200;
const int notdisappear = 100;

const int SNOW_EFFECT_DELAY = 5; // 3 minutes in seconds
const int SNOW_EFFECT_DURATION = 30;
const int NUM_SNOWFLAKES = 100;


enum class GameState {
    Menu,
    Playing,
    GameOver
};

int main()
{
    // ... (Your initialization code)
    int a = 0, b = 0;
    bool game_won = false;
    int lives = 3;
    unsigned lag = 0;
    unsigned char level = 0;
    int night;
    int k = 0;
    int l = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;

    bool freeze = false;





    std::array<std::string, MAP_HEIGHT> map_sketch = {
        // ... Your map sketch data ...
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #.................# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " ####.### # ###.#### ",
        "    #.#   0   #.#    ",
        "#####.# ##=## #.#####",
        "     .  #123#  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#.....P.....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #.................# ",
        " ################### "

    };

    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};
    std::array<Position, 4> ghost_positions;
    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * (MAP_WIDTH)*SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * (MAP_HEIGHT + 3)) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    //window.setView(sf::View(sf::FloatRect(0, -CELL_SIZE * 2, CELL_SIZE * (MAP_WIDTH), FONT_HEIGHT + CELL_SIZE * (MAP_HEIGHT + 3))));
    MainMenu mainMenu(window);
    a = mainMenu.draw();

    if (b == 1) {
        a = mainMenu.draw();
    }
    if (a == 1)
    {
        bool snow_animation_active = false;
        sf::Clock snow_animation_clock;

        window.setView(sf::View(sf::FloatRect(0, -CELL_SIZE * 2, CELL_SIZE * (MAP_WIDTH), FONT_HEIGHT + CELL_SIZE * (MAP_HEIGHT + 3))));
        window.setFramerateLimit(60);


        SnowAnimation snowAnimation(WINDOW_WIDTH, WINDOW_HEIGHT, NUM_SNOWFLAKES);
        sf::Music music;

        std::chrono::time_point<std::chrono::steady_clock> start_snow_time = std::chrono::steady_clock::now() + std::chrono::seconds(SNOW_EFFECT_DELAY);

        std::chrono::time_point<std::chrono::steady_clock> start_night_time = start_snow_time + std::chrono::seconds(5);
        std::chrono::time_point<std::chrono::steady_clock> start_fog_time = start_night_time + std::chrono::seconds(20);
        bool night_mode_active = false;

        GhostManager ghost_manager;

        std::array<Ghost, 4>& ghosts = ghost_manager.getGhosts();
        Pacman pacman;
        sf::Vector2f snowPosition(160.0f, 204.0f);
        SnowLayer snowlayer(snowPosition, "Resources/Images/snow.jpg");
        // SnowLayer snowLayer(WINDOW_WIDTH, WINDOW_HEIGHT, NUM_SNOWFLAKES);
        sf::Texture snowmanTexture;
        if (!snowmanTexture.loadFromFile("Resources/Images/snowman.jpg")) {
            std::cout << "snowman no open";
        }
        SoundManager sound;
        srand(static_cast<unsigned>(time(0)));
        map = convert_sketch(map_sketch, ghost_positions, pacman);
        ghost_manager.reset(level, ghost_positions);
        previous_time = std::chrono::steady_clock::now();
        sf::Texture overlayTexture;
        if (!overlayTexture.loadFromFile("Resources/Images/snowbg.png")) {
            //	// Handle loading error
            //
            std::cout<<"cant open";
        }


   
        sf::Clock waitingClock;
        enum class GameStartState {
            WaitingForKeyPress,
            Playing
        };
        GameStartState startState = GameStartState::WaitingForKeyPress;
        if (!music.openFromFile("pacman_beginning.ogg"))
        {
            std::cout << "cant open";
        }

        else {
            music.play();

        }
        window.setFramerateLimit(60);



        while (window.isOpen())
        {
            // ... (Your event handling code)

           // window.draw(overlaySprite);
            unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
            lag += delta_time;
            previous_time += std::chrono::microseconds(delta_time);

            window.setVerticalSyncEnabled(true);
            while (FRAME_DURATION <= lag) {
                lag -= FRAME_DURATION;
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        // ... (Your event handling code)
                    case sf::Event::Closed:
                        window.close();
                        break;
                    }
                }

                auto current_time = std::chrono::steady_clock::now();
                auto elapsed_snow_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_snow_time).count();
                auto elapsed_night_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_night_time).count();
                auto elapsed_fog_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_fog_time).count();
                if (elapsed_snow_time >= SNOW_EFFECT_DELAY) {
                    window.clear(sf::Color::Black);

                    if ((elapsed_snow_time - SNOW_EFFECT_DELAY) % 20 >= 10) {
                        snow_animation_active = false;
                    }
                    else {
                        snow_animation_active = true;
                    }
                }
                else {
                    snow_animation_active = false;
                }

                if (elapsed_snow_time >= 2 * 20) {
                    start_snow_time = current_time;

                }
                

                if (elapsed_night_time >= 30 && elapsed_night_time <= 50) {
                    sf::Texture texture;
                    if (!texture.loadFromFile("Resources/Images/moon.jpg")) {
                        std::cout << "cant open";
                    }
                    night = 1;

                    // Create a sprite and set its texture
                    sf::Sprite moon(texture);
                    moon.setPosition(290, -35);
                    moon.setTextureRect(sf::IntRect(0, 0, 43, 43));

                    // Calculate the alpha value based on elapsed time
                    float alpha = (elapsed_night_time - 10) / 30.0f * 255.0f;
                    if (alpha >= 255.0f) {
                        alpha = 255.0f;
                    }

                    // Set the sprite's color with the calculated alpha
                  //  sf::Color moonColor = moon.getColor();
                  // moonColor.a = static_cast<sf::Uint8>(alpha);
                   // moon.setColor(moonColor);

                    // Draw the moon sprite
                    window.draw(moon);

                    // Draw the disappearing text with the calculated alpha
                    sf::Font font;
                    if (!font.loadFromFile("static/arial.ttf")) {
                        std::cout << "Failed to load font" << std::endl;
                    }
                    sf::Text disappearingText("CAUTION: The ghosts are disappearing....", font, 10);

                    // Calculate the color for the scary effect (red tint)
                    sf::Color scaryColor(255, 0, 0, static_cast<sf::Uint8>(alpha));

                    // Apply the scary color to the text
                    disappearingText.setFillColor(scaryColor);

                    // Position the text at the left of the moon sprite
                    sf::FloatRect moonBounds = moon.getGlobalBounds();
                    sf::FloatRect textBounds = disappearingText.getGlobalBounds();
                    disappearingText.setPosition(moonBounds.left - textBounds.width - 10, moonBounds.top + 20);

                    // Add flickering effect to the text color
                    if (static_cast<int>(elapsed_night_time) % 2 == 0) {
                        disappearingText.setFillColor(scaryColor);
                    }
                    else {
                        // Return to normal color when not flickering
                        disappearingText.setFillColor(sf::Color::White);
                    }

                    window.draw(disappearingText);
                }
                else {
                    night = 0;
                }



                if (elapsed_fog_time >= 50 && elapsed_fog_time <= 100) {
                    sf::Texture texture;
                    if (!texture.loadFromFile("Resources/Images/fog_texture.png")) {
                        std::cout << "cant open";
                    }
                    sf::Sprite fog(texture);
                    fog.setPosition(0, 0);
                    window.draw(fog);

                }




                if (snow_animation_active && !game_won) {

                    sf::Sprite snowman(snowmanTexture);
                    
                    sf::Sprite overlaySprite(overlayTexture);
                    auto elapsed_snow_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_snow_time).count();
                    float alpha = static_cast<float>(elapsed_snow_time) / 5000.0f; // 3000 milliseconds = 3 seconds

                    if (alpha >= 1.0f) {
                        alpha = 1.0f; // Ensure that alpha doesn't exceed 1.0
                    }

                    // Apply the calculated alpha to the overlay sprite's color
                    sf::Color overlayColor = overlaySprite.getColor();
                    overlayColor.a = static_cast<sf::Uint8>(alpha * 255);
                    overlaySprite.setColor(overlayColor);

                    // Clear the window
                    window.clear();

                    // Draw the overlay sprite
                    window.draw(overlaySprite);
                    sf::Texture texture;
                    if (!texture.loadFromFile("Resources/Images/snowbg.png")) {
                        std::cout << "cant open";
                    }

                    // Create a sprite and set its texture
                    sf::Sprite snowlayer(texture);
                    // window.draw(snowlayer);
                    snowAnimation.update(2.0f / 60.0f); // Pass the elapsed time
                    snowAnimation.draw(window);

                    // draw_map(map, window);

                    // pacman.draw(game_won, window, freeze);
                    //pacman.snowupdate(level, map, delta_time, freeze);
                    /// ghost_manager.draw(true, window, freeze);
                     //ghost_manager.update(level, map, pacman);
                    // std::cout << " >>" << delta_time << std::endl;


                     //pacman.update

                    if (snow_animation_clock.getElapsedTime() >= sf::seconds(20)) {
                        waitingClock.restart();
                        snow_animation_active = false;
                        window.clear();
                    }

                }



                waitingClock.restart();
                // ... (Your game logic and collision detection code)

                if (startState == GameStartState::WaitingForKeyPress && music.getStatus() == sf::SoundSource::Stopped) {
                    // ... (Your waiting for keypress code)
                    previous_time = std::chrono::steady_clock::now();
                    waitingClock.restart();
                    startState = GameStartState::Playing;

                }

                if (startState == GameStartState::Playing && !game_won && !pacman.get_dead()) {
                    // ... (Your game logic and collision detection code)
                    {
                        bool pelletsLeft = false;
                        if (!snow_animation_active) {
                            pacman.update(level, map, delta_time, pacman.getFreeze());
                        }
                        else {
                            pacman.snowupdate(level, map, delta_time, pacman.getFreeze());
                        }
                        ghost_manager.update(level, map, pacman);

                        for (const std::array<Cell, MAP_HEIGHT>& column : map)
                        {
                            for (const Cell& cell : column)
                            {
                                if (Cell::Pellet == cell)
                                {
                                    pelletsLeft = true;
                                    break;
                                }
                            }

                            if (pelletsLeft)
                            {
                                break;
                            }
                        }

                        if (!pelletsLeft)
                        {
                            game_won = true;
                        }
                        else if (pacman.get_dead())
                        {
                            lives--;
                            std::cout << "lives left: " << lives << std::endl;

                            if (pacman.Score() > pacman.getHighScore()) {
                                pacman.updateHighScore(pacman.Score());
                                std::ofstream MyFile("highScore.txt", std::ios::out | std::ios::trunc);

                                if (!MyFile.is_open()) {
                                    std::cout << "Failed to open the file." << std::endl;

                                }

                                else {
                                    MyFile << pacman.getHighScore();



                                }

                                MyFile.close();

                            }

                            if (lives <= 0)
                            {
                                sound.Dead();
                                game_won = false;
                                std::cout << "Game Over" << std::endl;
                            }
                            else
                            {
                                window.clear();
                                sound.Dead();
                                sf::sleep(sf::milliseconds(1000));
                                pacman.draw_death_animation(window);
                                window.display();
                                window.clear();
                                while (sound.IsPlayingDeathSound()) {
                                    continue;
                                }
                                pacman.reset();
                                ghost_manager.reset(level, ghost_positions);

                                previous_time = std::chrono::steady_clock::now();
                                startState = GameStartState::WaitingForKeyPress;


                                waitingClock.restart();
                            }
                        }




                    }
                }

                if (startState == GameStartState::Playing && !game_won && !pacman.get_dead()) {
                    // Draw the maze
                    draw_map(map, window);
                    if (night == 0) ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window, pacman.getFreeze());
                    else
                    {
                        if (k == disappear)
                        {
                            ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window, pacman.getFreeze());
                            l++;
                            std::cout << l << std::endl;
                            if (l == notdisappear)
                            {
                                k = 0;
                                l = 0;
                            }
                        }
                        else
                            k++;

                    }
                    draw_text(0, 0, -FONT_HEIGHT, "Score:" + std::to_string(pacman.Score()), window);
                    draw_text(0, 0, -2 * FONT_HEIGHT, "High Score" + std::to_string(pacman.getHighScore()), window);
                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);
                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT + FONT_HEIGHT, "Lives: " + std::to_string(lives), window);
                    pacman.draw(game_won, window, pacman.getFreeze());

                }
                else if (startState == GameStartState::WaitingForKeyPress) {
                    // Draw the maze
                    draw_map(map, window);
                    if (night == 0) ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window, pacman.getFreeze());
                    else
                    {
                        if (k == disappear)
                        {
                            ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window, pacman.getFreeze());
                            l++;
                            if (l == notdisappear)
                            {
                                k = 0;
                                l = 0;
                            }
                        }
                        else
                            k++;

                    }
                    draw_text(0, 0, -FONT_HEIGHT, "Game Starting...", window);
                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);
                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT + FONT_HEIGHT, "Lives: " + std::to_string(lives), window);
                    pacman.pacman_sprite(window);
                }

                if (!pacman.get_dead() || !pacman.get_animation_over()) {
                    pacman.draw(game_won, window, pacman.getFreeze());
                }

                if (pacman.get_animation_over()) {
                    // ... (Your continue game or next level code)
                    if (game_won)
                    {
                        draw_text(1, 0, 0, "Next level! Press Enter key to continue", window);
                    }
                    else
                    {
                        draw_text(1, 0, 0, "Game over. Press Enter key to continue", window);
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    {
                        game_won = false;
                        if (pacman.get_dead())
                        {
                            level = 0;
                        }
                        else
                        {
                            level++;
                        }

                        map = convert_sketch(map_sketch, ghost_positions, pacman);
                        ghost_manager.reset(level, ghost_positions);
                        pacman.reset();
                        lives = 3;
                        pacman.Score() = 0;

                        startState = GameStartState::WaitingForKeyPress; // Place it here
                        if (startState == GameStartState::WaitingForKeyPress) {
                            std::cout << "waiting for keypress state";
                        }
                        music.play();

                        waitingClock.restart();


                    }

                    window.display();


                }


                window.display();
                window.clear();
            }

        }
    }

    return 0;
}