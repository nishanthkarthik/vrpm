#!/bin/bash

CHAN=1

# to convert wav to pcm
ffmpeg -i engine.wav -f s32le -ar 48000 -ac $CHAN -acodec pcm_s32le engine.raw

# to play
# pacat --rate=48000 --channels=$CHAN --format=s32le engine.raw

# to convert pcm to mp3
# ffmpeg -f s16le -ar 44.1k -ac 2 -i engine.raw engine.wav