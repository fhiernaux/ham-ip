#!/bin/bash
sudo setcap CAP_NET_ADMIN+ep hamip
killall hamip
./hamip --callsign ON4SEB --interface_name ham0 &
sudo ip link set ham0 up
sudo ip addr add 10.0.0.1/24 dev ham0

