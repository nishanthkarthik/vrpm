# vrpm

# how to play

- run `build/vrpm`
- use `pacat --format=s32le --rate=48000 --channels=1 record.raw`
- adjust channel and rate settings accordingly

# convert pcm to mp3

`ffmpeg -f s16le -ar 44.1k -ac 2 -i file.pcm file.wav`

- -f s16le … signed 16-bit little endian samples
- -ar 44.1k … sample rate 44.1kHz
- -ac 2 … 2 channels (stereo)
- -i file.pcm … input file
- file.wav … output file

