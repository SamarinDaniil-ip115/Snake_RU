#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <locale.h>
#include <time.h>
#include <iostream>

using namespace std;
using namespace sf;

struct Snake
{
    int x,y;
};

struct Food
{
    int x,y;
};

int best_size = 0;
int size_snake = 2;

//check best result, if new best result - rewrite .txt
void check_best_result()
{
    // read beast size
    FILE *k = fopen("best_size.txt", "r");
    fscanf(k,"%d",&best_size);
    fclose(k);
    if((size_snake-2) > best_size)
    {
        FILE *f = fopen("best_size.txt", "w");
        fprintf(f, "%d", size_snake-2);
        fclose(k);
    }   
}
//snake logic
void update_move_snake(int direction, Snake *snake, Food &food)
{
    //move snake
    for (int i = size_snake; i > 0; --i)
    {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
    //change direction
    switch(direction)
    {
        case 0:
            snake[0].y = snake[1].y - 20;
            break;
        case 1:
            snake[0].x = snake[1].x - 20;
            break;
        case 2:
            snake[0].y = snake[1].y + 20;
            break;
        case 3:
            snake[0].x = snake[1].x + 20;
            break;
        default:
            break;
    }
    //check food
    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        food.x = 20*(rand()%30) + 120;
        food.y = 20*(rand()%30) + 120;
        size_snake++;
    }
}
//
bool life_snake(Snake *snake)
{
//check if a snake has bitten a snake
    for (int j = 0; j < size_snake - 1; j++)
    {
        for (int i = j + 1; i < size_snake - 1; i++)
        {
            if (snake[i].x == snake[j].x && snake[i].y == snake[j].y)
                return false;
        }
    }
//check if the snake has gone abroad
    if (snake[0].x < 0 || snake[0].x > 1200)
        return false;
    if (snake[0].y < 100 || snake[0].y > 800)
        return false;
return true;
}
//draw snake
void draw_snake(RenderWindow& window, Snake *snake)
{
    RectangleShape one_piece_snake;
    one_piece_snake.setSize(Vector2f(20, 20));
// head snake
    one_piece_snake.setPosition(snake[0].x, snake[0].y);
    one_piece_snake.setFillColor(Color::White);
    window.draw(one_piece_snake);
// body snake
    one_piece_snake.setFillColor(Color::Green);
    for (int i = 1; i < size_snake; i++)
    {
        one_piece_snake.setPosition(snake[i].x, snake[i].y);
        window.draw(one_piece_snake);
    }
}

void draw_food(RenderWindow& window, Food food)
{
    RectangleShape f;
    f.setSize(Vector2f(20, 20));
    f.setFillColor(Color::Red);
    f.setPosition(food.x, food.y);
    window.draw(f);
}
// draw score
void draw_score(RenderWindow& window)
{
    // read beast size
    FILE *k = fopen("best_size.txt", "r");
    fscanf(k,"%d",&best_size);
    fclose(k);
    //add RU
    setlocale(LC_ALL, "ru_RU.UTF-8");
    // add font
    Font font;
    font.loadFromFile("Opel_Sans_Bold.ttf");
    // create text
    Text score;
    score.setFont(font);
    score.setFillColor(Color::White);
    score.setCharacterSize(20);
    // output text
    string score_text = "";
    wstring text_best = L"Счёт в лучшей игре:";
    wstring text = L"Счёт в данной игре:";
    // check
    if ((size_snake-2) >= best_size)
    {
        best_size = size_snake-2;
    }
    // bad code, but i'm lazy.
    score.setPosition(10,30);
    score.setString(text_best);
    window.draw(score);
    score.setPosition(210,30);
    score_text = to_string(best_size);
    score.setString(score_text);
    window.draw(score);
    score.setPosition(210,10);
    score_text = to_string(size_snake-2);
    score.setString(score_text);
    window.draw(score);
    score.setPosition(10,10);
    score.setString(text);
    window.draw(score);
}
// checks if the button is pressed
bool ButtonPressed(
        RenderWindow& window, int x_fpos, int x_spos, int y_fpos, int y_spos)
{
    if (Mouse::getPosition(window).x > x_fpos
        && Mouse::getPosition(window).x < x_spos
        && Mouse::getPosition(window).y > y_fpos
        && Mouse::getPosition(window).y < y_spos) {
        return true;
    }
    return false;
}
// draw button
void draw_Button(
        RenderWindow& window,
        RectangleShape& Button,
        Color bgcolor,
        int x_fpos,
        int y_fpos,
        int xSize,
        int ySize)
{
    Button.setSize(Vector2f(xSize, ySize));
    Button.setFillColor(bgcolor);
    Button.setOutlineColor(Color::Black);
    Button.setOutlineThickness(2);
    Button.setPosition(x_fpos, y_fpos);
    window.draw(Button);
}
//draw text in button
void draw_text_button(
        Font font,
        int x_fpos,
        int y_fpos,
        wstring txt_button,
        RenderWindow& window)
{
    Text text_button;
    text_button.setPosition(x_fpos + 2, y_fpos - 2);
    text_button.setFont(font);
    text_button.setString(txt_button);
    text_button.setFillColor(Color::Red);
    text_button.setCharacterSize(20);
    window.draw(text_button);
}
//
int main() {
    srand(time(NULL));
    Snake snake[100];
    Food food;
    food.x = 300;
    food.y = 300;
    int direction = 0, score = 0, menu_page = 0;
    update_move_snake(direction, snake, food);
    snake[0].y = 200;
    snake[0].x = 200;
    snake[1].y = 220;
    snake[1].x = 200;
    //
    wstring page2 = L"Играть";
    Font font;
    if (!font.loadFromFile("Opel_Sans_Bold.ttf")) {
        return EXIT_FAILURE;
    }
    // create background
    RectangleShape background;
    background.setSize(Vector2f(1200, 800));
    background.setFillColor(Color(25, 5, 33));
    background.setPosition(0, 0);
    // create and setting window
    RenderWindow window;
    Vector2i centerWindow((VideoMode::getDesktopMode().width / 2) - 600,(VideoMode::getDesktopMode().height / 2) - 400);
    window.create(VideoMode(1200, 800), "Snake", Style::Titlebar | Style::Close);
    window.setPosition(centerWindow);
    window.setKeyRepeatEnabled(true);
    // create a line to separate the counter 
    VertexArray lines(Lines, 2);
    lines[0].position = Vector2f(0,100);
    lines[1].position = Vector2f(1200,100);
    lines[0].color = Color::White;
    lines[1].color = Color::White;
    // create timer
    Clock clock;
    float timer = 0, delay = 0.15, times;
    //create button
    RectangleShape Button;
    Color bgcolor = Color::White;
    // open window
    while (window.isOpen()) {
        Event event;
        //event checking
        while (window.pollEvent(event)) {
            // close window
            if (event.type == Event::Closed)
            {
                check_best_result();
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && menu_page == 0)
            {
                if (event.mouseButton.button == Mouse::Left) {
                    if (ButtonPressed(window, 400, 500, 400, 500))
                    {
                        //change menu page
                        menu_page = 1;
                        bgcolor = Color::Green;
                        //reset data
                        size_snake = 2;
                        snake[0].y = 200;
                        snake[0].x = 200;
                        snake[1].y = 220;
                        snake[1].x = 200;
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == Mouse::Left)
                    bgcolor = Color::White;
            }
        }
        if(menu_page == 1)
        {
            // time 
            times = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += times;
            Event Event;
            //change direction
            if(Keyboard::isKeyPressed(Keyboard::Left)) direction = 1;
            if(Keyboard::isKeyPressed(Keyboard::Right)) direction = 3;
            if(Keyboard::isKeyPressed(Keyboard::Up)) direction = 0;
            if(Keyboard::isKeyPressed(Keyboard::Down)) direction = 2;
            //update game
            if (timer > delay)
            {
                timer = 0;
                update_move_snake(direction, snake, food);
            }
        }
        //application and update window
        window.clear();
        window.draw(background);
        switch (menu_page)
        {
        case 0:
            draw_Button(window,Button,bgcolor,400,400,100,100);
            draw_text_button(font, 400, 400, page2, window);
            break;
        case 1:
            if (life_snake(snake))
            {
                window.draw(lines);
                draw_snake(window, snake);
                draw_food(window, food);
                draw_score(window);
            } else {
                check_best_result();
                menu_page = 0;
            }
            break;
        }
        window.display();
    }
}
