#!/bin/bash
# Author: Matt Bunting

VIN=$(cat /etc/libpanda.d/vin)
VIN_CURRENT=$(cat /etc/hostname)

if [ "$VIN" == "$VIN_CURRENT" ];
then
	exit
fi

VIN_SIZE=${#VIN}
#echo "VIN_SIZE=${VIN_SIZE}"

if [ $VIN_SIZE -gt 0 ]; # Ensure it's not ablank file, otherwise everything will break
then
#    echo "VIN size is greater than 0"
#    echo "VIN=${VIN}"

	echo $VIN > /etc/hostname
	sed -i "/^127.0.1.1/ c\127.0.1.1  $VIN" /etc/hosts
	sed -i "/^ssid=/ c\ssid=$VIN" /etc/hostapd/hostapd.conf

	if grep -Fxq "$VIN" /etc/wpa_supplicant/wpa_supplicant.conf;
	then
		echo "This pi already has the mifi configured"
	else
		echo "Configuring mifi..."
		addWifiApSimple -s mifi-$VIN -p circles1
	fi

	echo "vinToHostname: This is a new VIN!  Need to reboot to apply"
	if ping -q -c 1 -W 8.8.8.8 > /dev/null;then
		python3 ~/libpanda/scripts/vinParser.py
	fi
	reboot
fi
