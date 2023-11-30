Check out this link to learn about audioconv64 to convert audio/video into a format
that is useable by libdragon:
https://n64squid.com/homebrew/libdragon/dfs/audioconv64/

I added audioconv64.exe to my PATH, and then just ran audioconv64 with my input file
to convert a video file into an m1v and wav64 file. Then, in the source, you can
see that we're calling up those resources from the ROM and playing them using 
libdragon's video player.