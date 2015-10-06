#!/usr/bin/python

from read_pcap import read_pcap
from rolling_mean import rolling_mean
import pandas as pd

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#plt.axis([0, 1, 1.1*amin(s), 2*amax(s) ])
plt.xlabel('simulation time')
plt.ylabel('throughput / Mb/s')

# "the axes"
# 111 = "1x1 grid, 1st plot" or something
ax = plt.subplot(111)

# (bytes in the last second) per millisecond

def Bps(file,pic):
    # ,"ip.src","ppp.protocol"
    framelen=read_pcap(file, ["frame.len"], timeseries=True)
    bytes_split_sec=framelen.resample("S", how="sum").dropna()
    plt.plot(bytes_split_sec);
    plt.savefig(pic);
    #return bytes_per_s.plot()

def Bpms(file,lab,style):
    # ,"ip.src","ppp.protocol"
    framelen=read_pcap(file, ["frame.len"], timeseries=True)
    #print framelen.index
    
    # 100ms
    bytes_each_window=framelen.resample("100L", how="sum")
    #print bytes_each_window.index
    # average the last 1S i.e. 10 windows
    throughput = rolling_mean(bytes_each_window, window_i_s="1S",min_periods_i=1).dropna()
    s = throughput['frame.len'] * float(8*10) / float(1000000) # B/100ms to Mb/s
    print s
    #print throughput
    
    # does "plt.plot" w/ time index
    
    handle, = plt.plot(throughput.index.to_pydatetime(),s, label=lab, linestyle=style)
    
    #handle, = pd.Series.plot(throughput_series, ax=axes) #, label=lab, linestyle=style);
    return handle;
    
    #return throughput

single_TCP_A = Bpms("./QoS/TCP/single_wire-0-0.pcap","TCP/IP, one client",'-');
#single_TCP_B = Bpms("./QoS/TCP/single_wire-1-0.pcap");

double_TCP_A  = Bpms("./QoS/TCP/double_wire-0-0.pcap","TCP/IP, two clients",'-');
#double_TCP_B  = Bpms("./QoS/TCP/double_wire-1-0.pcap");

single_BA_A  = Bpms("./QoS/BA/single_wire-0-0.pcap","Blackadder, one client",'-');
#single_BA_B  = Bpms("./QoS/BA/single_wire-1-0.pcap");

double_BA_A  = Bpms("./QoS/BA/double_wire-0-0.pcap","Blackadder, two clients",'--');
#double_BA_B  = Bpms("./QoS/BA/double_wire-1-0.pcap");

box = ax.get_position()
ax.set_position([box.x0,box.y0,box.width,box.height*0.8])

#move the bottom center of the legend to above the plot
plt.legend(loc='lower center',bbox_to_anchor=(0.5,1.1),ncol=2)


#frame = pd.DataFrame([single_TCP_A,double_TCP_A,single_BA_A,double_BA_A])
#frame.plot(x='frame.time_relative',y='frame.len')

plt.savefig("./dedup_graph.png");



