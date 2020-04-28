# Campfire: Submission for LD46.

![alt text](https://static.jam.vg/raw/827/13/z/303a4.gif)

Campfire is a 2D platformer where you carry a lamp through a forest. As your lamp dies out, you lose vision of your surroundings. Rush from campfire to campfire to keep your lamp alive as you make your way to the end.

Campfire was built from (almost) scratch in C++ by a team of two programmers. 
Most of our time (about 60/72 hours) was spent writing the engine code (platform layer, renderer, animation systems, basic lighting, basic collision, tools) 
and learning how to do both pixel art (Aseprite) and music (Bosca Ceoil). 
Although, it is more of a tech-demo, demonstrating what we managed to achieve in 72 hours, than a actual game, we have managed to construct one level to showcase all of the basic mechanics.

## External Content Credits
All of Campfire's content, including art, music, and code, were authored by us except for 
* GLFW
* stb_image & stb_truetype 
* XAudio

### On Our Code Library Choices
* GLFW was used as a cross-platform layer for creating windows and getting user-input. Ultimately, we bound ourselves to Windows due to the use of XAudio; however, supposing we planned on a port, GLFW certainly simplifies things immensely.
* stb_image and stb_truetype were used for processing png image files and ttf fonts respectively. Both libraries were purely used to load in image/font data in a uncompressed and/or easy to reason about format, hence all the rendering was done manually via a OpenGL abstraction layer.
* XAudio was a somewhat last minute addition. Ideally, something like OpenAL would have been used to make a cross-platform release possible, but given XAudio was the only API we had any familiarity with, it made sense to just stick with it as our Audio API of choice.

## Controls
* Arrow Keys to move Left and Right 
* ‘C’ to jump 
* ‘Q’ to kill your character and restart at the last checkpoint

![alt text](https://static.jam.vg/raw/827/13/z/2ff6d.png)
![alt text](https://static.jam.vg/raw/827/13/z/2ffda.png)
