# vrpm

# how to play

- run `build/vrpm`
- use `pacat --format=s32le --rate=48000 --channels=1 record.raw`
- adjust channel and rate settings accordingly

# convert pcm to wav

`ffmpeg -f s16le -ar 44.1k -ac 2 -i file.pcm file.wav`

- -f s16le … signed 16-bit little endian samples
- -ar 44.1k … sample rate 44.1kHz
- -ac 2 … 2 channels (stereo)
- -i file.pcm … input file
- file.wav … output file

# isolating fundamental frequency

- initially tried with fft. unable to recognize the fundamental mode
- From https://tunelab-world.com/enginerpm.pdf, autocorrelation was tried
- enhanced autocorrelation in audacity gave accurate results over a small sample
- *Enhanced Autocorrelation* is described in a paper by Tolonen and Karjalainen
-- Run a normal autocorrelation over the signal
-- Clip all results that are below zero
-- Results should be stretched by a factor of two and subtracted from original signal
-- Clip all results below zero

# benchmarks

For 88200 values (sample for ~2 seconds at 48KHz sampling rate), enhanced standard autocorrelation is done

- **with openmp, O3, autocorrelation from fourier transform** -> 0.11 seconds 
- **with openmp, O3** -> 2.31 seconds 
- **without openmp** -> 4.66 seconds
- **without openmp, O3** -> 13.8 seconds
