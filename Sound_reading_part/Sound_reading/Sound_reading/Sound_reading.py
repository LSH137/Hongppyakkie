# ver 1.0.0
# last debuging: LSH 2020/10/12

from scipy.io import wavfile
from scipy import signal
import matplotlib.pyplot as plt
import collections

def scale(note):
    orderedDict = collections.OrderedDict
    orderedDict = {
        'b3': 246.9417,
        'c4': 261.6256,
        'd4': 293.6648,
        'e4': 329.6276,
        'f4': 349.2282,
        'g4': 195.9977,
        'a4': 440.0000,
        'b4': 493.8833,
        'c5': 523.2511,
        'd5': 587.3295,
        'e5': 659.2551,
        'f5': 698.4565
    }

    numDict = [i for i in orderedDict]

    for i in range(len(numDict)):
        if note > orderedDict[numDict[len(numDict) - 1]]:
            break
        elif note < orderedDict[numDict[0]]:
            break
        elif i == 0 and (orderedDict[numDict[i]] <= note and (orderedDict[numDict[i]] + orderedDict[numDict[i+1]])/2 > note):
            return numDict[i]

        if i == (len(numDict)-1) and ((orderedDict[numDict[i-1]] + orderedDict[numDict[i]])/2 <= note and orderedDict[numDict[i]] >= note):
            return  numDict[i]
        elif ((orderedDict[numDict[i-1]] + orderedDict[numDict[i]])/2 <= note and (orderedDict[numDict[i]] + orderedDict[numDict[i+1]])/2 > note):
            return  numDict[i]

def makeGraph(freq, time, spect):
    plt.pcolormesh(time, freq, spect)
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.show()

def changeNote(File, freq, time, spect):
    spect2 = spect.T
    for t, sp in enumerate(spect2):
        s = 0
        f = 0
        for fr, spe in enumerate(sp):
            if spe == 0:
                s = spe
                f = fr
            elif spe >= s:
                s = spe
                f = fr

        for num, fre in enumerate(freq):
            if num == f:
                File.write(str(scale(fre)) + "\t")

        for num2, tim in enumerate(time):
            if num2 == t:
                File.write(str(tim) + "\t")

        print("clear")
        File.write(str(s) + "\n")

if __name__ == '__main__':
    f = open("data/pianoTest.txt", "w")
    sample_rate, samples = wavfile.read('data/pianoTest3.wav')
    frequencies, times, spectrogram = signal.spectrogram(samples[:, 1], sample_rate, nfft=16392, nperseg=1024)
    #makeGraph(frequencies, times, spectrogram)
    changeNote(f, frequencies, times, spectrogram)
    f.close()



    # 출처: https://fishneverdies.tistory.com/28