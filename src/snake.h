#include <matrix_setup.h>
#include <GameController.h>
#include <vector>

int8_t headX = (kMatrixWidth / 2);
int8_t headY = (kMatrixHeight / 2);
//int8_t snakeBody[kMatrixWidth][kMatrixHeight] = {{headX,headY}};
std::vector<uint16_t> snakeBody;
uint16_t lastElement[1] = {};
int8_t foodPosX = -1, foodPosY = -1;
int8_t prev_headX, prev_headY;
String direction = "UP";
int speed = 200;
unsigned long previousMillis = 0;
bool addBody = false;
int8_t snakeLength = 1;
int blinkNumber = 0;
bool gameOver = false;
CRGB background = CRGB::Black;
CRGB wall_color = CRGB::PaleGreen;
int camp_level = 1; //campaign level

uint16_t letterA[] = {XY(6, 1), XY(6, 2), XY(6, 3), XY(6, 4), XY(6, 5), XY(7, 6), XY(8, 6), XY(9, 6), XY(10, 5), XY(10, 4), XY(10, 3), XY(10, 2), XY(10, 1), XY(7, 3), XY(8, 3), XY(9, 3)};

std::vector<uint16_t> wall;

void level(std::vector<uint16_t> &level, int lvl, CRGB color)
{
    if (lvl == 1)
    {
        if (!level.empty())
            level.clear();
    }

    if (level.empty())
        for (int i = 0; i < kMatrixWidth; i++)
            for (int j = 0; j < kMatrixHeight; j++)
            {
                if (lvl == 2)
                    if ((i == 0 || i == (kMatrixWidth - 1) || j == 0 || j == (kMatrixHeight - 1)) && leds[XY(i, j)] == background)
                        level.push_back(XY(i, j));
                if (lvl == 3)
                    if ((((i >= 0 && i <= 4) || (i >= 12 && i <= (kMatrixWidth - 1))) && (j == 0 || j == (kMatrixHeight - 1))) || (((j >= 0 && j <= 4) || (j >= 16 && j <= (kMatrixHeight - 1))) && (i == 0 || i == (kMatrixWidth - 1))) || ((i == 6 || i == 10) && (j >= 6 && j <= 14)))
                        level.push_back(XY(i, j));
                if (lvl == 4)
                    if (((i >= 0 && i <= 7) && j == 7) || ((i >= 9 && i <= (kMatrixWidth - 1)) && j == 13) || ((j >= 0 && j <= 8) && i == 12) || ((j >= 12 && j <= (kMatrixHeight - 1)) && i == 4))
                        level.push_back(XY(i, j));
            }

    for (int i = 0; i < level.size(); i++)
        leds[level[i]] = color;
}

void controlHeadPosition(int8_t &x, int8_t &y, int &button, String &dir, int speed)
{
    if (dir != "UP" && dir != "DOWN")
    {
        if (button == 5)
            dir = "UP";
        if (button == 6)
            dir = "DOWN";
    }

    if (dir != "RIGHT" && dir != "LEFT")
    {
        if (button == 7)
            dir = "RIGHT";
        if (button == 8)
            dir = "LEFT";
    }

    if (button != 0)
        button = 0;

    if (millis() >= previousMillis + speed)
    {
        previousMillis = millis();

        //make tail follow the head
        if (snakeBody.size() > 1)
        {
            for (int i = snakeBody.size() - 1; i > 0; i--)
                snakeBody[i] = snakeBody[i - 1];
        }

        if (dir == "UP")
        {
            y += 1;
            if (y > kMatrixHeight - 1)
                y = 0;
        }
        if (dir == "DOWN")
        {
            y -= 1;
            if (y < 0)
                y = kMatrixHeight - 1;
        }
        if (dir == "RIGHT")
        {
            x += 1;
            if (x > kMatrixWidth - 1)
                x = 0;
        }
        if (dir == "LEFT")
        {
            x -= 1;
            if (x < 0)
                x = kMatrixWidth - 1;
        }
    }
}

void randomFoodGenerator(int8_t &x, int8_t &y, CRGB foodColor)
{
    if (x == -1 && y == -1)
    {
        do
        {
            x = random(kMatrixWidth - 1);
            y = random(kMatrixHeight - 1);
        } while (leds[XY(x, y)] != background);
    }

    leds[XY(x, y)] = foodColor;

    if (headX == x && headY == y)
    {
        snakeBody.push_back(snakeBody[snakeBody.size() - 1]); //add one tile if snake eat food
        snakeLength += 1;
        x = -1;
        y = -1;
        speed -= 15;
    }
}

void gameOverLetter(String letter, int x, int y, CRGB color)
{
    uint16_t g[] = {XY(x + 2, y + 4), XY(x + 1, y + 4), XY(x, y + 4), XY(x, y + 3), XY(x, y + 2), XY(x, y + 1), XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2)};
    uint16_t a[] = {XY(x, y), XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4), XY(x + 2, y + 3), XY(x + 2, y + 2), XY(x + 2, y + 1), XY(x + 2, y), XY(x + 1, y + 1)};
    uint16_t m[] = {XY(x, y), XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 3), XY(x + 1, y + 2), XY(x + 2, y + 4), XY(x + 2, y + 3), XY(x + 2, y + 2), XY(x + 2, y + 1), XY(x + 2, y)};
    uint16_t e[] = {XY(x, y), XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 2, y), XY(x + 1, y), XY(x + 1, y + 2), XY(x + 1, y + 4), XY(x + 2, y + 4)};
    uint16_t o[] = {XY(x, y), XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 1, y), XY(x + 2, y + 4), XY(x + 2, y + 3), XY(x + 2, y + 2), XY(x + 2, y + 1), XY(x + 2, y)};
    uint16_t v[] = {XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 2, y + 4), XY(x + 2, y + 3), XY(x + 2, y + 2), XY(x + 2, y + 1), XY(x + 1, y)};
    uint16_t r[] = {XY(x, y), XY(x, y + 1), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 3), XY(x + 2, y + 2), XY(x + 1, y + 1), XY(x + 2, y)};

    if (letter == "g")
        for (int i = 0; i < (sizeof(g) / sizeof(g[0])); i++)
            leds[g[i]] = color;

    if (letter == "a")
        for (int i = 0; i < (sizeof(a) / sizeof(a[0])); i++)
            leds[a[i]] = color;

    if (letter == "m")
        for (int i = 0; i < (sizeof(m) / sizeof(m[0])); i++)
            leds[m[i]] = color;

    if (letter == "e")
        for (int i = 0; i < (sizeof(e) / sizeof(e[0])); i++)
            leds[e[i]] = color;

    if (letter == "o")
        for (int i = 0; i < (sizeof(o) / sizeof(o[0])); i++)
            leds[o[i]] = color;

    if (letter == "v")
        for (int i = 0; i < (sizeof(v) / sizeof(v[0])); i++)
            leds[v[i]] = color;

    if (letter == "r")
        for (int i = 0; i < (sizeof(r) / sizeof(r[0])); i++)
            leds[r[i]] = color;
}

void checkGameOver() //check if the snake head eat it's body
{
    for (int i = 2; i < snakeBody.size(); i++)
    {
        if (snakeBody[0] == snakeBody[i])
            gameOver = true;
    }

    for (int i = 0; i < wall.size(); i++)
    {
        if (snakeBody[0] == wall[i])
            gameOver = true;
    }
}

void blinkSnake() //blink snake when game over and clear matrix
{
    if (millis() >= previousMillis + 100)
    {
        previousMillis = millis();
        blinkNumber += 1;
    }
}
void resetParameters()
{
    foodPosX = -1;
    foodPosY = -1;
    snakeBody.clear(); //delete all snake body
    wall.clear();
    headX = (kMatrixWidth / 2);
    headY = (kMatrixHeight / 2);
    speed = 200;
    direction = "UP";
    previousMillis = 0;
    FastLED.clear();
}

void displaySnake(CRGB headColor, CRGB tailColor)
{
    checkGameOver();
    if (!gameOver && snakeBody.size() < 11)
    {
        level(wall, camp_level, wall_color);
        controlHeadPosition(headX, headY, controller_button, direction, speed);
        randomFoodGenerator(foodPosX, foodPosY, CRGB::Yellow);

        //create new snake if there is noone or save head position
        if (snakeBody.empty())
            snakeBody.push_back(XY(headX, headY));
        else
            snakeBody[0] = XY(headX, headY);
    }
    else
    {
        if (blinkNumber < 11)
            blinkSnake();

        if (blinkNumber > 10)
        {
            resetParameters();
            if (gameOver)
            {
                camp_level = 1;
                gameOverLetter("g", 1, 15, CRGB::LightSkyBlue);
                gameOverLetter("a", 5, 15, CRGB::LightSkyBlue);
                gameOverLetter("m", 9, 15, CRGB::LightSkyBlue);
                gameOverLetter("e", 13, 15, CRGB::LightSkyBlue);
                gameOverLetter("o", 1, 9, CRGB::LightSkyBlue);
                gameOverLetter("v", 5, 9, CRGB::LightSkyBlue);
                gameOverLetter("e", 9, 9, CRGB::LightSkyBlue);
                gameOverLetter("r", 13, 9, CRGB::LightSkyBlue);

                for (int i = 0; i < (sizeof(letterA) / sizeof(letterA[0])); i++)
                    leds[letterA[i]] = CRGB::MediumOrchid;

                if (controller_button == 3)
                {
                    gameOver = false;
                    blinkNumber = 0;
                }

                if (controller_button != 0)
                    controller_button = 0;
            }
            else
            {
                camp_level++;
                if (camp_level > 4)
                    camp_level = 1;
            }
        }
    }

    //colorize snake and blink the snake if game over
    if (blinkNumber % 2 == 0 || !gameOver)
    {
        if (snakeBody.size() > 1)
            for (int i = 1; i < snakeBody.size(); i++)
                leds[snakeBody[i]] = tailColor;

        leds[snakeBody[0]] = headColor;
    }
    else if (blinkNumber % 2 == 1)
        for (int i = 0; i < snakeBody.size(); i++)
            leds[snakeBody[i]] = background;
}