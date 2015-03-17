// Changes from "original":
// - Imported (new) publisher/subscriber
// - Local file paths instead of "/tmp" or "/home/pursuit"
// - Switching "PointToPointNetDevice"/"Channel" to "PointToPointHelper" so that I can use Tracing
// - *Tracing traffic on wire*

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/blackadder-module.h>

#include "../../lib/ns3_video_publisher_app.h"
#include "../../lib/subscriber.h"

// jc776
#include <string>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("topology");

int main(int argc, char *argv[]) {
   // jc776 - Find source file's path.
   //UNIX-like only, but so is the rest of this so far.
   std::string aux(argv[0]);
   int pos = aux.rfind('/');
   
   //   /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/build/examples/blackadder-jc776/
   //-> /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/examples/blackadder-jc776/conf/2nodes/
   std::string path = aux.substr(0,pos+1) + "../../../examples/blackadder-jc776/conf/2nodes/";
   
   NodeContainer n0n1;
   n0n1.Create (2);
   
   Ptr<Node> node0 = n0n1.Get (0);
   Ptr<Node> node1 = n0n1.Get (1);
   
   //jc776: Using helper for trace.
   // I'm using "PointToPointHelper"
   // This allows some kind of automated setup of 'pcap' stats
   // This should create the same kind of setup as the deploy script's generated ones
   // ("PointToPointNetDevice", "DropTailQueue")
   // 'allocated' MAC address instead of custom: I need to reset this to match conf files
   
   // "Hardware" setup
   PointToPointHelper p2p;
   
   p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));
   p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
   p2p.SetDeviceAttribute ("Mtu", UintegerValue (1500));
   
   p2p.Install(n0n1);
   
   Ptr<NetDevice> dev0_0 = node0->GetDevice(0); // generated was "PointToPointNetDevice"
   Ptr<NetDevice> dev1_0 = node1->GetDevice(0);
   
   dev0_0->SetAddress (Mac48Address("7f:fa:28:48:cb:18"));
   dev1_0->SetAddress (Mac48Address("7f:fa:28:48:cb:19"));
   
   // "Background Software" setup
   Ptr<ClickBridge> click0 = CreateObject<ClickBridge > ();
   node0->AggregateObject(click0);
   click0->SetClickFile(path + "00000001.conf");
   Ptr<ServiceModel> servModel0 = CreateObject<ServiceModel > (); 
   node0->AggregateObject(servModel0); 
   Ptr<ClickBridge> click1 = CreateObject<ClickBridge > ();
   node1->AggregateObject(click1);
   click1->SetClickFile(path + "00000002.conf");
   Ptr<ServiceModel> servModel1 = CreateObject<ServiceModel > (); 
   node1->AggregateObject(servModel1); 

   Ptr<TopologyManager> tm = CreateObject<TopologyManager > ();
   tm->SetStartTime(Seconds(0.)); 
   tm->SetAttribute("Topology", StringValue(path + "topology.graphml"));
   node0->AddApplication(tm); 
   
   // "User Application" setup
   Ptr<VideoPublisher> app0_0 = CreateObject<VideoPublisher> ();
   app0_0->SetStartTime(Seconds(3)); 
   app0_0->SetStopTime(Seconds(14.87)); 
   node0->AddApplication(app0_0);

   Ptr<Subscriber> app1_0 = CreateObject<Subscriber> ();
   app1_0->SetStartTime(Seconds(5)); 
   app1_0->SetStopTime(Seconds(10)); 
   node1->AddApplication(app1_0);

   //jc776: Recording packets/destinations
   //Ptr<FlowMonitor> flowMonitor;
   //FlowMonitorHelper flowHelper;
   //flowMonitor = flowHelper.InstallAll();
   
   //jc776: Set up tracing on the up/down wires.
   AsciiTraceHelper ascii;
   p2p.EnableAsciiAll (ascii.CreateFileStream ("logs/2nodes/video/video.tr"));
   p2p.EnablePcapAll ("logs/2nodes/video/");

   Simulator::Run();
   Simulator::Destroy();
   
   return 0;
}
