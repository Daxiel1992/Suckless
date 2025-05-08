#!/bin/sh
df_check_location='/home'
# Hard Drive Space
DF=$(df -h $df_check_location | tail -n 1)
STOUSED=$(awk '{print $3}' <<< $DF)
STOTOT=$(awk '{print $2}' <<< $DF)
# Memory Usage
FREE=$(free -h | grep Mem)
MEMUSED=$(awk '{print $3}' <<< $FREE)
MEMTOT=$(awk '{print $2}' <<< $FREE)
# CPU Stats
CPU=$(top -bn1 | grep Cpu | awk '{print $2}')%
CPUTEMP=$(sensors -f | grep Package | awk '{print $4}')
# Battery
ACPI=$(acpi | grep 'Battery 0:')
BATTSTATUS=$(awk '{print $3}' <<< $ACPI | sed 's/.$//')
BATTPER=$(awk '{print $4}' <<< $ACPI | cut -d, -f1)
if [ $BATTSTATUS = 'Discharging' ]
then 
	BATTICON=""
else
	BATTICON=""
fi
printf " %s/%s  %s/%s  %s %s %s" "$STOUSED" "$STOTOT" "$MEMUSED" "$MEMTOT" "$CPU" "$BATTICON" "$BATTPER"
