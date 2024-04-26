# Final-Project

## How to build environment
>   You can do `make install-SDL` or do the following command:
>   `sudo apt-get update && sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev`

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