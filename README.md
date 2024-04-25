# Final-Project

## How to build environment
1.  Run `sudo apt update`
2.  Run `sudo apt install libsdl2-dev`
3.  Run `sudo apt-get install libsdl2-image-dev`
4.  Run `sudo apt-get install libsdl2-ttf-dev`
5.  Run `sudo apt-get install libsdl2-mixer-dev`

## How to write your script (still a work-in-progress)
> The first event of your script must be named "event.start"
*   event
    *   scene
    *   dialogue
*   scene
    *   image
    *   music
*   character
    *   inventory
        *   item.(item name)
            *   number
    *   attributes
        *   name
        *   value
    *   name
    *   image
*   dialogue
    *   (name here)
        *   character
        *   text
        *   options
            *   text
            *   next
---           
*   item
    *   name
    *   description
    *   icon
*   image
    *   path
*   music
    *   path