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
    *   text : the content of dialogue. You can use <br> as break point, where player has to click "next" to continue the content. 
    *   option(this is array, up to 4 elements)
        *   text : the text to be displayed
        *   next : the next dialogue name following this option
---   
Below have not been supported yet        
*   item 
    *   name
    *   description
    *   icon