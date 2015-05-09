#!/bin/bash

# Deploy topology and start FN(+RV) on each node.
./Project/repos/blackadder/deployment/deploy -c ./Project/repos/Information-Centric-Networking/deploy_scripts/3nodes_vm_U14_ips_pursuit.cfg

# Start the Topology Manager in a new window.
sudo x-terminal-emulator -e "./Project/repos/blackadder/TopologyManager/tm" /tmp/topology.graphml &

# (Not sure, waiting for TM to start)
sleep 2

# Start the Publisher.
# (Replace with delaying one)
cd ./Project/repos/blackadder/examples/video_streaming/VideoDemoApp/
sudo ant g_publisher
