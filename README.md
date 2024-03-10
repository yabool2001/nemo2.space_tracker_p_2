# SYSTEM MODES
System modes can be change by 2-position hardware switch SW1-2:

## Production mode.
Position `SW1=0` `SW2=0`  
- Accelometer enabled
- GNSS verbose OFF
- PDOP 2 m

## Simulation mode

Position `SW1=1` `SW2=0`  
- Accelometer disabled
- GNSS verbose ON
- GNSS power off
- STOP time short
- PDOP 2 m

## Test mode

Position `SW1=0` `SW2=1`  
- Accelometer disabled
- GNSS verbose ON
- GNSS never power off
- STOP time short
- FIX_ACQ_THS 600
- MIN_TNS 3
- MIN_TNS_TIME_THS
- PDOP 10 m

Production Mode has 2 missions: Active and Sustainable

Active mission acquire fix under 2 conditions: Movement is detected & A certain MIN period has passed. 

Sustainable mission acquire fix under 2 conditions: A certain MIN period has passed & Movement has been detected within the certain MIN period .


Uplink Message payload_id = 0 format:

`[%u] my_astro_payload_id`,
`[%.1f] fix3d.pdop`,
`[%d] fix3d.acq_time`,
`[%lu] (uint32_t) ( fix3d.acq_total_time / 60 )`,
`[%s] fv`
`[%u] sys_mode`

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

Set Maximum time of one cycle of tracker (sys_watchdog_time_ths)
1,<time>
time = number of seconds for watchdog timer. One cycle of tracker can not last longer than this threshold for watchdog
min time = 300 seconds (5 minutes)
max time = 3600 seconds (1 hour)

Set Time of keepalive (my_rtc_alarmA_time)
2,<time>
time = number of seconds for deep sleep time (alarmA). Deep sleep can not last longer than this threshold. 
min time = 300 seconds (5 minutes)
max time = 8 640 000 seconds (100 days)

Set Time of fix acquisition threshold (fix_acq_ths)
3,<time>
time = maximum number of seconds for fix acquisition. 
min time = 45 seconds (5 minutes)
max time = 600 seconds (10 minutes)

Set Time threshold for finding 3 satellites (min_tns_time_ths)
4,<time>
time = maximum number of seconds for findind 3 satellites 
min time = 15 seconds (5 minutes)
max time = 600 seconds (10 minutes)

Set PDOP Expected fix precion (pdop_ths)
5,<time>
precision = maximum number of seconds for findind 3 satellites 
min precision = 0.01 meters (10 centimeter)
max precision = 100 meters

Get System
8,1 Configuration
fv , sys_mode , sys_watchdog_time_ths , my_rtc_alarmA_time , fix_acq_ths , min_tns_time_ths , pdop_ths ,
8,2 Status
total gnss minutes of work , temperature measure by accelerometer

SYSTEM Software Reset
9,9

