import soundfile as sf
from pathlib import Path
from scipy import signal
from scipy.fft import fftshift
import numpy as np
import matplotlib.pyplot as plt

def plot_spectrogram(sound, sample_rate):
    f, t, Sxx = signal.spectrogram(sound, sample_rate)
    plt.pcolormesh(t, f, Sxx, shading='gouraud')

def main():
    fig, (ax1, ax2) = plt.subplots(ncols=2, sharex=True)
    
    file1 = 'esp32 Hellooo.wav'
    sound1, sample_rate1 = sf.read(Path('WAV') / file1)
    if sound1.ndim > 1:
        sound1 = sound1.mean(1)
    print(f'Sample rate: {sample_rate1}')
    plt.subplot(1, 2, 1)
    plot_spectrogram(sound1, sample_rate1)

    file2 = 'ggwave_hello_example_edited.wav'
    sound2, sample_rate2 = sf.read(Path('WAV') / file2)
    if sound2.ndim > 1:
        sound2 = sound2.mean(1)
    print(f'Sample rate: {sample_rate2}')
    plt.subplot(1, 2, 2)
    plot_spectrogram(sound2, sample_rate2)

    plt.show()


if __name__ == '__main__':
    main()