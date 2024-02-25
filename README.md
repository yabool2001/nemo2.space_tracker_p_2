# SYSTEM MODES
System modes can be change by 2-position hardware switch SW1-2:

## Production mode.
Position `SW1=0` `SW2=0`  
- Accelometer enabled
- GNSS verbose OFF


## Simulation mode

Position `SW1=1` `SW2=0`  
Accelometer disabled
GNSS verbose ON
GNSS power off
STOP time short

## Test mode

Position `SW1=0` `SW2=1`  
Accelometer disabled
GNSS verbose ON
GNSS never power off
STOP time short
FIX_ACQ_THS 600
MIN_TNS 3
MIN_TNS_TIME_THS


Production Mode has 2 missions: Active and Sustainable

Active mission acquire fix under 2 conditions: Movement is detected & A certain MIN period has passed. 

Sustainable mission acquire fix under 2 conditions: A certain MIN period has passed & Movement has been detected within the certain MIN period .


Uplink Message payload_id = 0 format:

`[%u] my_astro_payload_id`,
`[%.1f] fix3d.pdop`,
`[%d] fix3d.acq_time`,
`[%lu] (uint32_t) ( fix3d.acq_total_time / 60 )`,
`[%s] fv`

Uplink Message payload_id > 0 format:

`[%u] my_astro_payload_id,`
`[%.1f] fix3d.pdop,`
`[%d] fix3d.acq_time,`
`[%lu] (uint32_t) ( fix3d.acq_total_time / 60 ),`
`[%ld] fix3d.latitude_astro_geo_wr,`
`[%ld] fix3d.longitude_astro_geo_wr`

Downlink Command format:

`[%u] my_astro_payload_id,`
`[%.1f] fix3d.pdop,`
`[%d] fix3d.acq_time,`
`[%lu] (uint32_t) ( fix3d.acq_total_time / 60 ),`
`[%ld] fix3d.latitude_astro_geo_wr,`
`[%ld] fix3d.longitude_astro_geo_wr`

Downlink Commands:

Maximum time of one cycle of tracker 
1,<time>
time = number of seconds for watchdog timer. One cycle of tracker can not last longer than this threshold for watchdog
min time = 300 seconds (5 minutes)
max time = 3600 seconds (1 hour)

Time of keepalive
2,<time>
time = number of seconds for deep sleep time (alarmA). Deep sleep can not last longer than this threshold. 
min time = 300 seconds (5 minutes)
max time = 8 640 000 seconds (100 days)

Time of fix acquisition threshold
3,<time>
time = maximum number of seconds for fix acquisition. 
min time = 45 seconds (5 minutes)
max time = 600 seconds (10 minutes)

Time threshold for finding 3 satellites
4,<time>
time = maximum number of seconds for findind 3 satellites 
min time = 15 seconds (5 minutes)
max time = 600 seconds (10 minutes)

PDOP Expected fix precion
5,<time>
precision = maximum number of seconds for findind 3 satellites 
min precision = 0.01 meters (10 centimeter)
max precision = 100 meters
