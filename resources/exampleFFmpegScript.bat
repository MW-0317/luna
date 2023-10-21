ffmpeg -y -hide_banner -loglevel panic -i "../temp.mp4" -vf palettegen palette.png
ffmpeg -y -hide_banner -loglevel panic -i "../temp.mp4" -i palette.png -filter_complex paletteuse=dither=none temp.gif
gifsicle temp.gif --colors 64 -o temp.gif
Pause