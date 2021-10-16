//#include <matrix_setup.h>
#include <GameController.h>
#include <clock.h>
#include <snake.h>

int period = 500;
unsigned long time_now_clock = 0;
unsigned long time_now_snake = 0;
unsigned long time_now_arca = 0;
int clock_index = 1;
int snake_index = 9;
int arca_index = 0;
int dir_ball = 1;

//MENU
int menuPos = 1, menuPos_actual;
int menuPosX = 2, menuPosY = 13;  //actual menu position
int menuPosX_prev, menuPosY_prev; //previous menu position
bool menu_ON = false;
unsigned long time_now_menu = 0;

Timezone myTZ;
bool displayClock_flag = false;
bool displaySnake_flag = false;

void drawBorder()
{
    for (int i = 0; i < kMatrixWidth; i++)
    {
        for (int j = 0; j < kMatrixHeight; j++)
        {
            if (i == 0 || i == (kMatrixWidth - 1) || j == 0 || j == (kMatrixHeight - 1))
            {
                leds[XY(i, j)] = CRGB::RoyalBlue;
                leds[XY(0, 0)] = CRGB::RoyalBlue;
            }
        }
    }
}

void drawClock(int x, int y)
{
    CRGB color;
    uint16_t clock_border[] = {XY(x + 1, y), XY(x + 2, y), XY(x + 3, y),
                               XY(x, y + 1), XY(x, y + 2), XY(x, y + 3),
                               XY(x + 1, y + 4), XY(x + 2, y + 4), XY(x + 3, y + 4),
                               XY(x + 4, y + 1), XY(x + 4, y + 2), XY(x + 4, y + 3)};

    //clock_face has specific order that makes it simpler to animate
    uint16_t clock_face[] = {XY(x + 2, y + 2), XY(x + 1, y + 1), XY(x + 1, y + 2),
                             XY(x + 1, y + 3), XY(x + 2, y + 3), XY(x + 3, y + 3),
                             XY(x + 3, y + 2), XY(x + 3, y + 1), XY(x + 2, y + 1)};

    for (int i = 0; i < (sizeof(clock_border) / sizeof(clock_border[0])); i++)
    {
        leds[clock_border[i]] = CRGB::SaddleBrown;
    }

    leds[clock_face[0]] = CRGB::DarkRed;

    if (leds[XY(x, y)].r == 20)
    {
        color.r = 20;
        color.g = 20;
        color.b = 20;
    }
    else
        color = CRGB::Black;

    if (millis() > time_now_clock + period)
    {
        time_now_clock = millis();
        leds[clock_face[clock_index]] = CRGB::DarkRed;
        if (clock_index != 1)
        {
            leds[clock_face[clock_index - 1]] = color;
        }
        if (clock_index == 1)
        {
            leds[clock_face[8]] = color;
        }
        clock_index++;
        if (clock_index > 8)
        {
            clock_index = 1;
        }
    }
}

void drawSnake(int x, int y)
{
    CRGB color;
    uint16_t snake_body[] = {XY(x + 4, y + 4), XY(x + 3, y + 4), XY(x + 2, y + 4),
                             XY(x + 1, y + 3), XY(x + 2, y + 2), XY(x + 3, y + 2),
                             XY(x + 4, y + 1), XY(x + 3, y), XY(x + 2, y),
                             XY(x + 1, y), XY(x + 1, y + 1), XY(x, y + 1), XY(x, y)};

    //head of the snake - RED
    leds[snake_body[0]] = CRGB::DarkRed;
    //rest of the body - DARKGREEN
    for (int i = 1; i < (sizeof(snake_body) / sizeof(snake_body[0]) - 4); i++)
    {
        leds[snake_body[i]] = CRGB::DarkGreen;
    }

    if (leds[XY(x, y + 2)].r == 20)
    {
        color.r = 20;
        color.g = 20;
        color.b = 20;
    }
    else
        color = CRGB::Black;

    //animate tail
    if (millis() > time_now_snake + (period - 300))
    {
        time_now_snake = millis();
        leds[snake_body[snake_index]] = CRGB::DarkGreen;
        if (snake_index == 9)
        {
            leds[snake_body[10]] = color;
            leds[snake_body[11]] = color;
            leds[snake_body[12]] = CRGB::DarkGreen;
        }
        if (snake_index == 10)
        {
            leds[snake_body[9]] = color;
            leds[snake_body[11]] = color;
            leds[snake_body[12]] = CRGB::DarkGreen;
        }
        if (snake_index == 11)
        {
            leds[snake_body[9]] = color;
            leds[snake_body[10]] = CRGB::DarkGreen;
            leds[snake_body[12]] = color;
        }
        if (snake_index == 12)
        {
            leds[snake_body[9]] = CRGB::DarkGreen;
            leds[snake_body[10]] = color;
            leds[snake_body[11]] = CRGB::DarkGreen;
            leds[snake_body[12]] = color;
        }
        snake_index++;
        if (snake_index > 12)
        {
            snake_index = 9;
        }
    }
}

void drawArcanoid(int x, int y)
{
    CRGB color;
    uint16_t paddle[] = {XY(x + 1, y), XY(x + 2, y), XY(x + 3, y), XY(x + 4, y)};

    uint16_t ball[] = {XY(x + 3, y + 1), XY(x + 4, y + 2), XY(x + 3, y + 3),
                       XY(x + 2, y + 4), XY(x + 1, y + 3), XY(x, y + 2), XY(x + 1, y + 1), XY(x + 2, y + 2)};

    //the last tile of paddle was blinking and I didn't know how to solve that, thats why
    //there is '-1' in for loop, visually it looks ok
    for (int i = 0; i < (sizeof(paddle) / sizeof(paddle[0]) - 1); i++)
    {
        leds[paddle[i]] = CRGB::DarkBlue;
    }

    if (leds[XY(x, y)].r == 20)
    {
        color.r = 20;
        color.g = 20;
        color.b = 20;
    }
    else
        color = CRGB::Black;

    if (millis() > time_now_arca + (period - 350))
    {
        time_now_arca = millis();
        leds[ball[arca_index]] = CRGB::DarkOrchid;

        if (arca_index == 0) //from right to left
            dir_ball = 1;
        else if (arca_index == 6) //from left to right
            dir_ball = 2;

        if (dir_ball == 1)
        {
            leds[ball[arca_index]] = CRGB::DarkOrchid;
            leds[ball[arca_index - 1]] = color;
            leds[ball[arca_index + 1]] = color;
            arca_index++;
        }
        if (dir_ball == 2)
        {
            leds[ball[arca_index]] = CRGB::DarkOrchid;
            leds[ball[arca_index + 1]] = color;
            leds[ball[arca_index - 1]] = color;
            arca_index--;
        }
    }
}

void drawSettings(int x, int y)
{
    uint16_t settings[] = {XY(x, y + 1), XY(x + 1, y + 2), XY(x + 2, y + 3), XY(x + 2, y + 4), XY(x + 3, y + 4),
                           XY(x, y), XY(x + 1, y + 1), XY(x + 2, y + 2),
                           XY(x + 1, y), XY(x + 2, y + 1), XY(x + 3, y + 2), XY(x + 4, y + 2), XY(x + 4, y + 3)};

    for (int i = 0; i < (sizeof(settings) / sizeof(settings[0]) - 8); i++)
    {
        leds[settings[i]] = CRGB::DimGray;
    }

    for (int i = 5; i < (sizeof(settings) / sizeof(settings[0]) - 5); i++)
    {
        leds[settings[i]] = CRGB::Gray;
    }

    for (int i = 8; i < (sizeof(settings) / sizeof(settings[0])); i++)
    {
        leds[settings[i]] = CRGB::LightGrey;
    }
}

void controlMenuPos()
{
    readNES();
    if (controller_button == 1 && menu_ON == false)
    {
        menu_ON = true;
        displayClock_flag = false;
        displaySnake_flag = false;
        FastLED.clear();
        controller_button = 0;
    }

    if (menu_ON)
    {
        if (menuPos % 2 == 1) //left side of the menu
        {
            // if ((controller_button == 7) && readNES() != 0)
            if (controller_button == 7) //right pressed
            {
                menuPos += 1;
                controller_button = 0;
            }
        }
        else if (menuPos % 2 == 0) //right side of the menu
        {
            //if ((controller_button == 8) && readNES() != 0)
            if (controller_button == 8) //left pressed
            {
                menuPos -= 1;
                controller_button = 0;
            }
        }

        //upper side of menu
        // if ((controller_button == 5) && readNES() != 0) //UP pressed
        if (controller_button == 5)
        {
            menuPos -= 2;
            if (menuPos == -1)
                menuPos = 5;
            if (menuPos == 0)
                menuPos = 6;
            controller_button = 0;
        }

        //lower side of menu
        // if ((controller_button == 6) && readNES() != 0) //DOWN pressed
        if (controller_button == 6)
        {
            menuPos += 2;
            if (menuPos == 7)
                menuPos = 1;
            if (menuPos == 8)
                menuPos = 2;
            controller_button = 0;
        }
    }
}

//function draws all the menu icons and provides controll over menu
void drawMenu()
{
    controlMenuPos();
    if (menu_ON)
    {
        if (menuPos != menuPos_actual)
        {
            for (int i = menuPosX; i < menuPosX + 7; i++)
            {
                for (int j = menuPosY; j < menuPosY + 7; j++)
                {
                    if (leds[XY(i, j)].r == 20 && leds[XY(i, j)].g == 20 && leds[XY(i, j)].b == 20)
                    {
                        leds[XY(i, j)].r = 0;
                        leds[XY(i, j)].g = 0;
                        leds[XY(i, j)].b = 0;
                    }
                }
            }
        }

        switch (menuPos)
        {
        case 1:
            menuPosX = 2;
            menuPosY = 13;
            menuPos_actual = menuPos;
            if (controller_button == 3)
            {
                displayClock_flag = true;
                menu_ON = false;
                controller_button = 0;
            }
            break;
        case 2:
            menuPosX = 8;
            menuPosY = 13;
            menuPos_actual = menuPos;
            if (controller_button == 3)
            {
                displaySnake_flag = true;

                controller_button = 0;
                menu_ON = false;
            }
            break;
        case 3:
            menuPosX = 2;
            menuPosY = 7;
            menuPos_actual = menuPos;
            break;
        case 4:
            menuPosX = 8;
            menuPosY = 7;
            menuPos_actual = menuPos;
            break;
        case 5:
            menuPosX = 2;
            menuPosY = 1;
            menuPos_actual = menuPos;
            break;
        case 6:
            menuPosX = 8;
            menuPosY = 1;
            menuPos_actual = menuPos;
            break;

        default:
            break;
        }

        if (menuPos == menuPos_actual)
        {
            for (int i = menuPosX; i < menuPosX + 7; i++)
            {
                for (int j = menuPosY; j < menuPosY + 7; j++)
                {
                    if ((leds[XY(i, j)].r == 0 && leds[XY(i, j)].g == 0 && leds[XY(i, j)].b == 0) /* ||
                    (leds[XY(i, j)].r == 20 && leds[XY(i, j)].g == 20 && leds[XY(i, j)].b == 20) */
                    )
                    {
                        leds[XY(i, j)].r = 20;
                        leds[XY(i, j)].g = 20;
                        leds[XY(i, j)].b = 20;
                    }
                }
            }
        }

        drawBorder();
        drawClock(3, 14);
        drawSnake(9, 14);
        drawArcanoid(3, 8);
        drawSettings(3, 2);
    }

    if (menuPos == 1 && displayClock_flag == true) //display clock
    {
        if (minuteChanged() || secondChanged())
        {
            FastLED.clear();
            displayClock(myTZ, CRGB::Beige);
        }
    }

    if (menuPos == 2 && displaySnake_flag == true) //display snake game
    {
        FastLED.clear();
        displaySnake(CRGB::DarkRed, CRGB::DarkGreen);
    }
}

/* void drawChristmasTree()
{
    for (int i = 0; i < kMatrixWidth; i++)
    {
        for (int j = 0; j < kMatrixHeight; j++)
        {
            if (((j == 1 || j == 2) && i != 0 && i != 16) ||
                ((j == 3 || j == 4) && i != 0 && i != 1 && i != 15 && i != 16) ||
                ((j >= 5 && j <= 7) && (i >= 3 && i <= 13)) ||
                ((j == 8 || j == 9) && (i >= 4 && i <= 12)) ||
                ((j >= 10 && j <= 12) && (i >= 5 && i <= 11)) ||
                ((j >= 13 && j <= 15) && (i >= 6 && i <= 10)) ||
                ((j == 16 || j == 17) && (i >= 7 && i <= 9)) ||
                ((j == 18 || j == 19) && (i == 8)))
                leds[XY(i, j)] = CRGB::DarkGreen;
        }
    }

    uint16_t rainbow_strip[] = {XY(7, 16), XY(8, 15), XY(9, 15), XY(10, 14), XY(11, 13)};
} */