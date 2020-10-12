import numpy as np
import cv2
import os
import pygame
import pygame.midi
import WavPlay
from time import sleep
 
cap = cv2.VideoCapture(1) # video capture source camera (Here webcam of laptop) 
ret,frame = cap.read() # return a single frame in variable `frame`
 
notes = {1:'1',2:'2',3:'3',4:'4',5:'5'}
note = ' '
 
while(True):
    cv2.imshow('img1',frame) #display the captured image
    if cv2.waitKey(1) & 0xFF == ord('y'): #save on pressing 'y' 
        img2 = frame.copy()
        
        cv2.imwrite('c2.png',frame)
        cv2.imshow('original', frame)
        
        img2 =  cv2.GaussianBlur(img2, (3,3), 0)
        img_gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
        cv2.imshow('img2', img2)
        cv2.imshow('gray', img_gray)
        
        circles = cv2.HoughCircles(img_gray, cv2.HOUGH_GRADIENT,1,10,param1=60, param2=50, minRadius=0, maxRadius=0)
        circles = np.uint16(np.around(circles))
        
        for i in circles[0, :]:
            x, y, radius = i
            print "ball position x = " + str(x) + ", y = " + str(y) + ", radius = " + str(radius)
            cv2.circle(frame, (i[0],i[1]), i[2],(255,255,0), 2)
            cv2.putText(frame, str(x)+', '+str(y),(i[0]-20,i[1]), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            
            #Save note info to Dictionary in order
            if 0<x<120:
                if 280>y>260:
                    note = 'DO1'
                elif 245>y>225:
                    note = 'RE'
                elif 210>y>190:
                    note = 'MI'
                elif 178>y>158:
                    note = 'FA'
                elif 152>y>132:
                    note = 'SOL'
                elif 126>y>106:
                    note = 'RA'
                elif 100>y>80:
                    note = 'SI'     
                notes[1]=note
                cv2.putText(frame,str(note),(i[0]-10,i[1]+40), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            elif 120<x<240:
                if 280>y>260:
                    note = 'DO1'
                elif 245>y>225:
                    note = 'RE'
                elif 210>y>190:
                    note = 'MI'
                elif 178>y>158:
                    note = 'FA'
                elif 152>y>132:
                    note = 'SOL'
                elif 126>y>106:
                    note = 'RA'
                elif 100>y>80:
                    note = 'SI'
                notes[2]=note
                cv2.putText(frame,str(note),(i[0]-10,i[1]+40), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            elif 240<x<360:
                if 280>y>260:
                    note = 'DO1'
                elif 245>y>225:
                    note = 'RE'
                elif 210>y>190:
                    note = 'MI'
                elif 178>y>158:
                    note = 'FA'
                elif 152>y>132:
                    note = 'SOL'
                elif 126>y>106:
                    note = 'RA'
                elif 100>y>80:
                    note = 'SI'
                notes[3]=note
                cv2.putText(frame,str(note),(i[0]-10,i[1]+40), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            elif 360<x<480:
                if 280>y>260:
                    note = 'DO1'
                elif 245>y>225:
                    note = 'RE'
                elif 210>y>190:
                    note = 'MI'
                elif 178>y>158:
                    note = 'FA'
                elif 152>y>132:
                    note = 'SOL'
                elif 130>y>106:
                    note = 'RA'
                elif 100>y>80:
                    note = 'SI'
                notes[4]=note
                cv2.putText(frame,str(note),(i[0]-10,i[1]+40), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            elif 480<x<600:
                if 280>y>260:
                    note = 'DO1'
                elif 245>y>225:
                    note = 'RE'
                elif 210>y>190:
                    note = 'MI'
                elif 178>y>158:
                    note = 'FA'
                elif 152>y>132:
                    note = 'SOL'
                elif 126>y>106:
                    note = 'RA'
                elif 100>y>80:
                    note = 'SI'
                notes[5]=note
                cv2.putText(frame,str(note),(i[0]-10,i[1]+40), cv2.FONT_HERSHEY_PLAIN, 1.0, 255, 2)
            
        cv2.imshow('houghCircles', frame)
        print(notes)
        
        #play notes
        midi = WavPlay.Midi()
        
        for i in notes:
            print(notes[i])
            midi.playNote(notes[i], 1)
        cv2.destroyAllWindows()
cap.release()

# [출처] [Python][OpenCV][Midi] 악보 인식 후 계이름 연주 프로토타입 2|작성자 로봇매냐
# https://blog.naver.com/monkey5255/220662430284






