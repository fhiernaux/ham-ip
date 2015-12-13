#!/bin/bash
sudo ./ham-ip &
sudo ip link set ham0 up
sudo ip addr add 10.0.0.1/24 dev ham0

