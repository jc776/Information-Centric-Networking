#!/bin/bash

BA=./Project/repos/blackadder
ICN=./Project/repos/Information-Centric-Networking

CLICK_CONF=${BA}/src/sample.conf

TM_EXE=${BA}/TopologyManager/tm
TM_TOPOLOGY=${BA}/TopologyManager/topology.graphml

VIDEO_DIR=${ICN}/blackadder_applications/caching_video_apps/VideoDemoVLC2

echo $VIDEO_DIR

x-terminal-emulator -e click ${CLICK_CONF} &

sleep 1

x-terminal-emulator -e ${TM_EXE} ${TM_TOPOLOGY} &

sleep 1

echo ${VIDEO_DIR}

cd ${VIDEO_DIR}
x-terminal-emulator -e "ant" g_publisher &> a1.txt &
x-terminal-emulator -e "ant" subscriber &> a2.txt &

ant subscriber
