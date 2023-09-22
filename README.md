# Play MP3 with FFmpeg and SDL2 

This is a code example to show how to play MP3 using SDL2 
for playing audio and FFMpeg for decoding. 

## How to compile
SDL is added as a git submodule so you have to init
its submodule. After cloning this repo you should do: <br>
> <code>git submodule update --init</code >

And then `cd` to `libs/sdl` dir and make sure 
you are on `SDL2` branch:

```bash 
cd ./libs/sdl
git fetch 
git switch SDL2
```

Then you will need [FFmpeg](https://github.com/FFmpeg/FFmpeg)
in somewhere that cmake can find it with `find_package()`
You can do this with vcpkg package manager. 

After all dependencies are met you can do :
```
cmake -S ./ -B ./cmake-build
```