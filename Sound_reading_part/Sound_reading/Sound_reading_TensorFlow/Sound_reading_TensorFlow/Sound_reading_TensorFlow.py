import tensorflow as tf
from scipy.io import wavfile
from scipy import signal
import matplotlib.pyplot as plt
import collections
import os

learn_data_dir: str = 'learing_data/'

def Read_Learing_data(dir: str):
    file_list = os.listdir(dir)

    for i in file_list:
        dir = dir + i
        sample_rate, samples = wavfile.read(dir)


