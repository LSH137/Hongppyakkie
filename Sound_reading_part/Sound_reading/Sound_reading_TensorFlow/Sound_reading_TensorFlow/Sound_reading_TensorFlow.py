import tensorflow as tf
from scipy.io import wavfile
from scipy import signal
import matplotlib.pyplot as plt
import collections
import os
import typing

learn_data_dir: str = 'learing_data/'

def Read_File_Name(dir: str):
    file_list: list = os.listdir(dir)

    return file_list

def Read_Learing_data(note: str):
    dir:str = learn_data_dir + note
    sample_rate, samples = wavfile.read(dir)

    frequencies, times, spectrogram = signal.spectrogram(samples[:, 1], sample_rate, nfft = 20000, nperseg = 2048)
    spectrogram_list: list = spectrogram.tolist()

    return spectrogram_list




