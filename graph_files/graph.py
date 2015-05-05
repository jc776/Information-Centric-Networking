#!/usr/bin/python

from read_pcap import read_pcap
from rolling_mean import rolling_mean
import pandas as pd

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt


# (bytes in the last second) per millisecond


def Bps(file,pic):
    # ,"ip.src","ppp.protocol"
    framelen=read_pcap(file, ["frame.len"], timeseries=True)
    bytes_split_sec=framelen.resample("S", how="sum").dropna()
    plt.plot(bytes_split_sec);
    plt.savefig(pic);
    #return bytes_per_s.plot()

def Bpms(file,pic):
    # ,"ip.src","ppp.protocol"
    framelen=read_pcap(file, ["frame.len"], timeseries=True)
    #return framelen
    bytes_per_ms=framelen.resample("100L", how="sum")
    throughput = rolling_mean(bytes_per_ms, window_i_s="1S")
    #throughput = pd.rolling_mean(bytes_per_ms, window=1000, min_periods=1)
    #return throughput
    plt.plot(throughput);
    #plt.plot(bytes_per_ms);
    plt.savefig(pic);
    #return bytes_per_s.plot()

single_BA_A  = Bpms("./QoS/BA/single_wire-0-0.pcap","./QoS_single.png");
#single_BA_B  = Bpms("./QoS/BA/single_wire-1-0.pcap");
single_TCP_A = Bpms("./QoS/TCP/single_wire-0-0.pcap","./TCP_single.png");
#single_TCP_B = Bpms("./QoS/TCP/single_wire-1-0.pcap");

double_BA_A  = Bpms("./QoS/BA/double_wire-0-0.pcap","./QoS_double.png");
#double_BA_B  = Bpms("./QoS/BA/double_wire-1-0.pcap");
double_TCP_A  = Bpms("./QoS/TCP/double_wire-0-0.pcap","./TCP_double.png");
#double_TCP_B  = Bpms("./QoS/TCP/double_wire-1-0.pcap");

print(single_TCP_A);
print(double_BA_A);



