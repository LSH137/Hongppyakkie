# non stable version
# last debuging: LSH 2020/10/13
# 음의 범위가 b3 ~ f5를 벗어나는 것에 대한 수정 필요

from scipy.io import wavfile
from scipy import signal
import matplotlib.pyplot as plt
import collections

def scale(File, note):

    error = 5
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

    numDict = [i for i in orderedDict] # 리스트에 위 주파수를 순서대로 입력

    for i in range(len(numDict)):
        if note > orderedDict[numDict[len(numDict) - 1]]: # F5보다 음이 높으면 break
            break
        elif note < orderedDict[numDict[0]]: # B3보다 음이 낮으면 break
            break
        elif (orderedDict[numDict[i]] - error <= note and orderedDict[numDict[i]] + error > note): # +- error 만큼의 주파수 오차 범위 내에서 일치하는지 체크
            File.write(str(numDic[i]))
            return numDict[i]
        '''
        elif i == 0 and (orderedDict[numDict[i]] <= note and (orderedDict[numDict[i]] + orderedDict[numDict[i+1]])/2 > note):
            return numDict[i]

        if i == (len(numDict)-1) and ((orderedDict[numDict[i-1]] + orderedDict[numDict[i]])/2 <= note and orderedDict[numDict[i]] >= note):
            return  numDict[i]
        elif ((orderedDict[numDict[i-1]] + orderedDict[numDict[i]])/2 <= note and (orderedDict[numDict[i]] + orderedDict[numDict[i+1]])/2 > note):
            return  numDict[i]
        '''
###########################################################################################################################

def makeGraph(freq, time, spect):
    plt.pcolormesh(time, freq, spect)
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.ylim(0, 700)
    plt.show()

'''
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
'''
##################################################################################################

if __name__ == '__main__':
    flag = 1
    try:
        f = open("data/pianoTest.txt", "w")
        sample_rate, samples = wavfile.read('data/sample_LSH.wav') # 파일 이름 변경[LSH]

    except:
        print("error occur while opening sample file")
        flag = 0

    if flag == 1:
        #frequencies, times, spectrogram = signal.spectrogram(samples[:, 1], sample_rate, nfft=16392, nperseg=1024)
        frequencies, times, spectrogram = signal.spectrogram(samples[:, 1], sample_rate, nfft = 16392, nperseg = 2048)
        print(samples)
        print("frequence: ", frequencies, "spectrogram: ", spectrogram, "times: ", times)

        makeGraph(frequencies, times, spectrogram)

        #changeNote(f, frequencies, times, spectrogram)
        f.close()



    # 출처: https://fishneverdies.tistory.com/28