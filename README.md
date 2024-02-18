SYSTEM MODES
System modes can be change by 2-position hardware switch SW1-2:

Production mode

Position SW1=0 SW2=0
Accelometer enabled
GNSS verbose OFF


Simulation mode

Position SW1=1 SW2=0
Accelometer disabled
GNSS verbose ON
STOP time short

Test mode

Position SW1=0 SW2=1
Accelometer disabled
GNSS verbose ON
GNSS never power_off
STOP time short
FIX_ACQ_THS 600
MIN_TNS 3
MIN_TNS_TIME_THS


Production Mode has 2 missions: Active and Sustainable

Active mission acquire fix under 2 conditions: Movement is detected & A certain MIN period has passed. 

Sustainable mission acquire fix under 2 conditions: A certain MIN period has passed & Movement has been detected within the certain MIN period .


Message payload_id = 0 format:

`[%u]	my_astro_payload_id,`
`[%.1f]	fix3d.pdop,`
`[%d]	fix3d.acq_time,`
`[%lu]	(uint32_t) ( fix3d.acq_total_time / 60 ),`
`[%s]	fv`

Message payload_id > 0 format:

`[%u]	my_astro_payload_id,`
`[%.1f]	fix3d.pdop,`
`[%d]	fix3d.acq_time,`
`[%lu]	(uint32_t) ( fix3d.acq_total_time / 60 ),`
`[%ld]	fix3d.latitude_astro_geo_wr,`
`[%ld]	fix3d.longitude_astro_geo_wr`