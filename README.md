# WS2812B-matrix-dashboard
LED dashboard matrix with resolution of 21x17 pixels
In progress...

## Why did you do that?
I'm very fascinated with the idea of creating something. If the effect of creation can be visualised, it's a perfect match for me.
Moreover, thanks to the WiFi connection of ESP32 it can communicate with IOT devices. Sounds great :)

## Used tools
- ESP32 microcontroller
- 357 WS2812B leds
- some electronics needed to make the leds work
- 5V power supply or step-up voltage converter
- old NES pad (I used version with original connector, not the one with USB)
- photo frame (I suggest big and pretty thick one)

## Description
A base for this project are WS2812B leds which are programmable. Basically, you can do whatever you want with them.

The matrix is powered with 12V power supply which is later converted to 5V, because of the type of used leds and microcontroller (both of them need at least 5V voltage logic).

Controller was made from an old NES pad, that sends serial data. ESP32 decodes that signals with the special part of code.

Matrix is divided on 6 tiles (menu), each sygnifying different task. You can navigate through it by NES pad and thanks to a backlight the user knows in which part of menu he is.

## What is done and what are the future plans?
For now, i've made:
- fully functional menu with animations
- **digital clock** - a clock that connects to a web server and gets the actual time value
- **snake** - a simple snake game with different levels/maps and increasing speed to make the play a bit challenging

Future plans:
- **arcanoid game** - actually there is a menu tile already prepared
- **tetris** - classic, old and good tetris (maybe with music, but that needs another soldering to add piezo buzzer)
- **settings** - there is also manu tile for that, it'd be nice to be able to actually change brightness or collors
- **weather station** - if I can connect to a server and get the actually time, why wouldn't I get the information about weather from my location and visualise it!
- **room temperature** - if there is possibility to connect to an IOT devices, I can make it communicate with my Xiaomi Temperature Monitor and visualise it on the matrix
