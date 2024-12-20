import time
from neopixel import Neopixel
import colors
import random
from machine import Pin
from utime import sleep_ms, sleep
import os
from dfplayer import DFPlayer
import _thread

# 
df=DFPlayer(uart_id=1,tx_pin_id=4,rx_pin_id=5)
# #wait some time till the DFPlayer is ready
time.sleep(0.2)
# #change the volume (0-30). The DFPlayer doesn't remember these settings
df.volume(25)
# time.sleep(0.2)
#play file ./01/001.mp3

def second_thread():
    df.play(1,1)
    time.sleep(5)
    df.stop()
    
button = Pin(14, Pin.IN, Pin.PULL_DOWN)

numpix = 50
pixels = Neopixel(numpix, 0, 13, "GRB")
pixels.brightness(10)

def interpolation(d, x):
    output = d[0][1] + (x - d[0][0]) * ((d[1][1] - d[0][1])/(d[1][0] - d[0][0]))
    output = round(output, 3)
    return output

data_fast = [[1, 0.001],[10, 0.007]]
data=[[1, 0.1],[10, 0.7]]

segments = [
    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9],
    [10, 11, 12, 13, 14, 15, 16, 17 ],
    [20, 21, 22, 23, 24, 25, 26, 27 ],
    [30, 31, 32, 33, 34, 35, 36, 37 ],
    [40, 41, 42, 43, 44, 45, 46, 47 ],
   
]

# pre calculate the stop index
index = random.randint(1,len(segments))
print(index)

def light_segment(segment, color):
    for pixel in segment:
        pixels.set_pixel(pixel, color)
    pixels.show()
    
def clear_segment(segment):
    for pixel in segment:
        pixels.set_pixel(pixel, colors.Black)
    pixels.show()

def initialize():
    for segment in segments:
        clear_segment(segment)
        
def rotate_anim(delay, color):
    for segment in segments:
            light_segment(segment, color)
            time.sleep(delay)
            clear_segment(segment)
            
def rotate(times, data, color):
    for x in range(1, times):
        rotate_anim(interpolation(data, x), color)
       # print(interpolation(data, x))
        
def stop_at_random():
    initialize()
    print("STOPPING NOW")
    segments_sliced = segments[0:index]
    for segment in segments_sliced:
        light_segment(segment, colors.White)
        time.sleep(1)
        clear_segment(segment)
        if segments_sliced.index(segment) != (len(segments_sliced)-1):
            light_segment(segment, colors.White)
        else:
            for i in range(10):
                light_segment(segment, colors.Green)
                time.sleep(0.25)
                light_segment(segment, colors.Black)
                time.sleep(0.25)
        initialize()
    
def spinwheel():
    spinning = True
    # Initialize the LEDs
    initialize()
    _thread.start_new_thread(second_thread, ())
    rotate(0, data_fast, colors.Blue)
    rotate(5, data, colors.Red)
    stop_at_random()
    df.stop()
    spinning = False
    
    


prev_state = button.value()
print(prev_state)
initialize()

while True:
    cur_state = button.value()
    if cur_state != prev_state:
        if cur_state:
            index = random.randint(1,len(segments))
            spinwheel()
        else:
            df.stop()

    prev_state = cur_state    

        










