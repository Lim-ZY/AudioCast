# AudioCast: Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags

![banner](./assets/images/audiocast_banner.png)

## Overview

Wireless connectivity challenges hinder the deployment of embedded systems. We introduce AudioCast to address two critical issues: spectrum scarcity-induced contention and high power consumption in transmitters. The widespread availability of broadcast radio receivers (for example, FM radios using the 88–108 MHz spectrum) and access to underutilized lower-frequency spectrum motivate the design of AudioCast. The lower-frequency spectrum offers superior radio-wave propagation characteristics, exhibiting at least 10× lower path loss than the 2.4 GHz and 5 GHz Industrial, Scientific, and Medical (ISM) bands while avoiding congestion and interference. These properties enable reliable and long-distance communication, even for weakly radiated signals. AudioCast builds on these properties and the unique negative resistance of a tunnel diode.

AudioCast rethinks the architecture of radio transmitters using a tunnel diode oscillator to generate carrier signals and self-modulate them with baseband signals. This results in frequency-modulated transmissions at an overall power consumption below 200 μW. Unlike related systems based on the backscatter mechanism, AudioCast does not require an externally generated carrier or rely on ambient signals. We argue that AudioCast represents an example of a new class of transmitters which we conceptualize as Beyond-Backscatter transmitters. Through experiments, we demonstrate that AudioCast achieves a transmission range of up to 130 m in line-of-sight and tens of meters in non-line-of-sight conditions respectively. These transmissions are decodable by ubiquitous commodity FM receivers in cars, homes, and phones. We evaluate AudioCast through theoretical analysis, benchtop experiments, and urban/indoor field deployments. Additionally, we prototype and demonstrate novel applications, including low-power voice transmissions and hand gesture communication, enabled by AudioCast’s range and power efficiency.


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


## Publications

1. [**ACM IMWUT (UbiComp) '25**](https://doi.org/10.1145/3729471) - *AudioCast: Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags*
   > **Full paper** to appear in IMWUT - June '25: **Vol. 9, No. 2, Article 27** and to be presented at ACM UbiComp '25 in Espoo, Finland.
    <details>
    <summary>Click to expand citation</summary>

    ```
    @article{audiocast_imwut25,
        author = {Reddy, C. Rajashekar; Shah, Dhairya Jigar; Ang, Nobel and Varshney, Ambuj (to appear)},
        title = {AudioCast: Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags},
        journal = {IMWUT},
        year = {2025},
        issue_date = {June 2025},
        publisher = {Association for Computing Machinery},
        address = {New York, NY, USA},
        volume = {9},
        number = {2},
        url = {https://doi.org/10.1145/3729471},
        doi = {10.1145/3729471},
        articleno = {27},
        numpages = {32},
    }
    ```

    </details>

 2. [**ACM MobiSys '25**](https://www.sigmobile.org/mobisys/2025/) - *Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags*
    > **Demo** to appear at MobiSys '25 in Anaheim, California, US.
    <details>
    <summary>Click to expand citation</summary>

    ```
    @inproceedings{audiocast_mobisys25,
        author = {Reddy, C. Rajashekar and Shah, Dhairya Jigar and Varshney, Ambuj (to appear)},
        title = {Enabling Ubiquitous Connectivity for Embedded Systems through Audio-Broadcasting Low-power Tags},
        year = {2025},
        publisher = {Association for Computing Machinery},
        address = {New York, NY, USA},
        numpages = {2},
        location = {Anaheim, California, US},
        series = {MOBISYS '25}
    }
    ```
    </details>

 3. [**ACM ENSsys '23**](https://dl.acm.org/doi/10.1145/3628353.3628546) - *Beyond Broadcasting: Revisiting FM Frequency-band for Providing Connectivity to Next Billion Devices*
    > **Paper** presented at EnSsys '23 Workshop which has held in conjunction with ACM SenSys '23 in Istanbul, Turkey. AudioCast builds on this early work.
    <details>
    <summary>Click to expand citation</summary>
    
    ```
    @inproceedings{tunnelradio_enssys23,
        author = {Reddy, C. Rajashekar and Gulati, Manoj and Varshney, Ambuj},
        title = {Beyond Broadcasting: Revisiting FM Frequency-band for Providing Connectivity to Next Billion Devices},
        year = {2023},
        isbn = {9798400704383},
        publisher = {Association for Computing Machinery},
        address = {New York, NY, USA},
        url = {https://doi.org/10.1145/3628353.3628546},
        doi = {10.1145/3628353.3628546},
        booktitle = {Proceedings of the 11th International Workshop on Energy Harvesting \& Energy-Neutral Sensing Systems},
        pages = {30–36},
        numpages = {7},
        keywords = {FM communication, Internet of Things (IoT), Low-Power Communication, Tunnel Diodes},
        location = {Istanbul, Turkiye},
        series = {ENSsys '23}
    }
    ```

## Contributors and Team

AudioCast is developed and maintained by the following researchers from the [WEISER group](https://weiserlab.github.io/ambuj/) from the School of Computing, National University of Singapore (NUS). 

1. [C. Rajashekar Reddy](https://github.com/rez39)
2. [Dhairya Shah](https://github.com/dhairyashah1/)
3. [Prof. Ambuj Varshney](https://github.com/weiserlab/)

> Feel free to contact us at [`rajashekar.c@u.nus.edu`](mailto:rajashekar.c@u.nus.edu) or [`dhairya@u.nus.edu`](mailto:dhairya@u.nus.edu) for any questions or suggestions.


## Acknowledgements

We acknowledge Nobel Ang, [Yuvraj Singh Bhadauria](https://github.com/UvrajSB) and [Manoj Gulati](https://github.com/manojgulati) for their assistance and support during the early stages of this work.


## Funding

This research is fully supported by the Advanced Research and Technology Innovation Centre **(ARTIC)** at the National University of Singapore. In addition, some parts of this work are supported by a **Tier 1 grant** from Ministry of Education. It is also supported by a **Startup Grant** from ODPRT and an unrestricted gift from Google through their **Research Scholar Program**, all of which are hosted at the National University of Singapore (NUS).
