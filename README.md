# networkMonitor

An application to monitor network activity.

# Install and build dependencies

## libtins
http://libtins.github.io/download/

## spdlog
https://github.com/gabime/spdlog

# Build application

mkdir build && cd build

cmake ../ && make

# Notes 
Socket access requires sudo privileges. $sudo su ./NetworkMonitor 
Run ping.py in the background for testing if the network is passive (e.g. idle WSL)
