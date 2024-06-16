# Final-Project

## How to build environment
>   You can do `make install-SDL` or do the following command:
>   `sudo apt-get update && sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev`

## How to write your script (still a work-in-progress)
> The first event of your script must be named "event.start"
*   event.(name)
    *   scene:the background image to be displayed
    *   next: the next dialogue to start
*   scene.(name)
    *   name
    *   background: the relative path of image file
*   character.(name)
    *   name : name to be displayed
    *   avatar : the relative path of avatar image file
    *   tachie : the relative path of tachie image file
*   dialogue.(name)
    *   character : the character that is speaking
    *   text : the content of dialogue. You can use < br > (without space in brackets) as break point, where player has to click "next" to continue the content. 
    *   option(this is array, up to 4 elements)
        *   text : the text to be displayed
        *   next : the next dialogue name following this option
---   
Below have not been supported yet        
*   item 
    *   name
    *   description
    *   icon

## How to Compile
1. Makesure you have instaled SDL2 library. You can use `./make install-SDL2` to do this under Ubuntu. Note that this requires sudo permission.
2. Compile the source code by `./make`
3. After that you will get an executable `engine`
4. You can run this in bash in this folder by `./engine`

## Game Control
### Start Menu
1. When you run the `engine`, you will see a game menu
2. In the menu, you will see 4 buttons: New game, Choose Save, > (Save Name), and Exit. The meaning is clear. If you want to choose your gamesave, click `Choose Save`; to select, click the 3rd button `> (Save Name)`
### How to Save your game
1. When you are in the game, you can save your game by clicking the cross (X) of your window. There is NO auto save. And you must close your game if you want to save.
2. The saved file(named by the creation time of the file) will be stored in the folder `save`