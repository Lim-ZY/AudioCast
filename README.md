# AudioCast: Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags

## Repository Organization

```
├── LICENSE                
├── README.md
├── frequency_scanner         # Utility to scan unallocated FM frequencies
├── hardware                  # PCB hardware design files for baseband generator and frontend
├── assets                    # Images, videos, posters, etc.
└── transmitter_firmware      # Firmware code for baseband generator, FM tuner, etc.
```

## AudioCast System Preview

### FM Modulation vs Mixing

With increasing frequency of baseband / message signal, the value of the modulation index reduces and it becomes narrowband frequency modulation. A narrowband frequency modulation can be approximated as a mixing process, with upper sidebands becoming negligible which is shown in the following video:

https://github.com/user-attachments/assets/ccb81c37-590c-4e0f-b894-82cb153416e6

Alternatively: Watch it [here](https://www.youtube.com/watch?v=7RvvvsP36FI)

### Indoor NLoS Audio Evaluation

Non-line-of-sight (NLoS) evaluation of AudioCast transmitter, showing the transmitted audio snippets and corresponding received audio snippets at 7 different locations. We transmitted audio snippets of 30s but for the video we depict only 5s to make it brief. Furthermore, the calculated PESQ score for evaluating the received audio quality is also shown in the following video:

https://github.com/user-attachments/assets/ebbb9d8d-553b-4ae6-ada7-a8c252bf67cc

Alternatively: Watch it [here](https://www.youtube.com/watch?v=k4Gf5YFvplg)

> The above videos can also be previewed [here](./assets/videos/).
