import os
from wavePlayer import wavePlayer

player = wavePlayer()

try:
    while True:
        player.play('/sounds/ipl.wav')
except KeyboardInterrupt:
    player.stop()
    print("wave player terminated")