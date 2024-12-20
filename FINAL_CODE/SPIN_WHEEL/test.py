import time
from dfplayer import DFPlayer
df=DFPlayer(uart_id=1,tx_pin_id=4,rx_pin_id=5)
#wait some time till the DFPlayer is ready
time.sleep(0.2)
#change the volume (0-30). The DFPlayer doesn't remember these settings
df.volume(25)
time.sleep(0.2)
#play file ./01/001.mp3
df.play(1,1)
df.stop()