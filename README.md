SYSTEM MODES
System modes can be change by 2-position hardware switch SW1-2:

Production mode

Position SW1=0 SW2=0
Accelometer enabled

Simulation mode

Position SW1=1 SW2=0
Accelometer disabled
STOP time short

Test mode

Position SW1=0 SW2=1
Accelometer disabled
GNSS never power_off
STOP time short
FIX_ACQ_THS 600
MIN_TNS 3
MIN_TNS_TIME_THS


Production Mode has 2 missions: Active and Sustainable

Active mission acquire fix every time when movement is detected

Sustainable mission acquire fix after 2 conditions: A certain period has passed & Movement has been detected within the certain period .
