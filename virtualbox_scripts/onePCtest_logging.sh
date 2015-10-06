#!/bin/bash

#... cleanup.
pkill -9 xterm

sleep 1

BA=./Project/repos/blackadder
ICN=./Project/repos/ICN-test

LOGS=~/Desktop/logs

CLICK_CONF=${BA}/src/sample.conf

TM_EXE=${BA}/TopologyManager/tm
TM_TOPOLOGY=${BA}/TopologyManager/topology.graphml

VIDEO_DIR=${ICN}/blackadder_applications/caching_video_apps/VideoCaching

echo $VIDEO_DIR

run_logged(){
    xterm -hold -e "bash -c '$1 2>&1 | tee $2'" &
}

run_logged "click ${CLICK_CONF}" "${LOGS}/click.txt"

sleep 1

run_logged "${TM_EXE} ${TM_TOPOLOGY}" "${LOGS}/tm.txt"

sleep 1

echo ${VIDEO_DIR}

cd ${VIDEO_DIR}
run_logged "ant g_publisher" "${LOGS}/publisher.txt"
run_logged "ant subscriber" "${LOGS}/subscriber.txt"
run_logged "ant cache" "${LOGS}/cache.txt"
