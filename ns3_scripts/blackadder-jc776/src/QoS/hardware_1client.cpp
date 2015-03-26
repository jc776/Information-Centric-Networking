// INCOMPLETE
// - both "pcap"s are recording ALL data instead of their specific ones

// Changes from "original":
// - Imported (new) publisher/subscriber
// - Local file paths instead of "/tmp" or "/home/pursuit"
// - Switching "PointToPointNetDevice"/"Channel" to "PointToPointHelper" so that I can use Tracing
// - *Tracing traffic on wire*

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/blackadder-module.h>

#include "lib/ns3_video_publisher_app.h"
#include "lib/subscriber.h"

// jc776
#include <string>


using namespace ns3;
NS_LOG_COMPONENT_DEFINE("topology");
int main(int argc, char *argv[]) {
   // LogComponentEnable ("Ns2MobilityHelper",LOG_LEVEL_DEBUG);
   
   // jc776 - Find source file's path.
   //UNIX-like only, but so is the rest of this so far.
   std::string aux(argv[0]);
   int pos = aux.rfind('/');
   
   //   /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/build/examples/blackadder-jc776/
   //-> /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/examples/blackadder-jc776/conf/QoS_double_client/
   std::string path = aux.substr(0,pos+1) + "../../../examples/blackadder-jc776/conf/QoS_double_client/";

   // Generated "hardware" setup
/*
   Ptr<Node> node0 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev0_0 = Create<PointToPointNetDevice>();
   dev0_0->SetAddress (Mac48Address("7f:f1:57:17:1b:18"));
   dev0_0->SetDataRate (DataRate("100Mbps"));
   dev0_0->SetMtu (1500);
   node0->AddDevice(dev0_0);
   Ptr<DropTailQueue> queue0_0 = CreateObject<DropTailQueue > ();
   dev0_0->SetQueue(queue0_0);

   Ptr<Node> node1 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev1_0 = Create<PointToPointNetDevice>();
   dev1_0->SetAddress (Mac48Address("7f:f1:57:17:1b:19"));
   dev1_0->SetDataRate (DataRate("100Mbps"));
   dev1_0->SetMtu (1500);
   node1->AddDevice(dev1_0);
   Ptr<DropTailQueue> queue1_0 = CreateObject<DropTailQueue > ();
   dev1_0->SetQueue(queue1_0);
   Ptr<PointToPointNetDevice> dev1_1 = Create<PointToPointNetDevice>();
   dev1_1->SetAddress (Mac48Address("7f:f1:57:17:1b:1a"));
   dev1_1->SetDataRate (DataRate("100Mbps"));
   dev1_1->SetMtu (1500);
   node1->AddDevice(dev1_1);
   Ptr<DropTailQueue> queue1_1 = CreateObject<DropTailQueue > ();
   dev1_1->SetQueue(queue1_1);
   Ptr<PointToPointNetDevice> dev1_2 = Create<PointToPointNetDevice>();
   dev1_2->SetAddress (Mac48Address("7f:f1:57:17:1b:1c"));
   dev1_2->SetDataRate (DataRate("100Mbps"));
   dev1_2->SetMtu (1500);
   node1->AddDevice(dev1_2);
   Ptr<DropTailQueue> queue1_2 = CreateObject<DropTailQueue > ();
   dev1_2->SetQueue(queue1_2);

   Ptr<Node> node2 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev2_0 = Create<PointToPointNetDevice>();
   dev2_0->SetAddress (Mac48Address("7f:f1:57:17:1b:1b"));
   dev2_0->SetDataRate (DataRate("100Mbps"));
   dev2_0->SetMtu (1500);
   node2->AddDevice(dev2_0);
   Ptr<DropTailQueue> queue2_0 = CreateObject<DropTailQueue > ();
   dev2_0->SetQueue(queue2_0);

   Ptr<Node> node3 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev3_0 = Create<PointToPointNetDevice>();
   dev3_0->SetAddress (Mac48Address("7f:f1:57:17:1b:1d"));
   dev3_0->SetDataRate (DataRate("100Mbps"));
   dev3_0->SetMtu (1500);
   node3->AddDevice(dev3_0);
   Ptr<DropTailQueue> queue3_0 = CreateObject<DropTailQueue > ();
   dev3_0->SetQueue(queue3_0);

   Ptr<PointToPointChannel> channel0 = CreateObject<PointToPointChannel>();
   channel0->SetAttribute("Delay", StringValue("10ms"));
   dev0_0->Attach(channel0);
   dev1_0->Attach(channel0);

   Ptr<PointToPointChannel> channel1 = CreateObject<PointToPointChannel>();
   channel1->SetAttribute("Delay", StringValue("10ms"));
   dev1_1->Attach(channel1);
   dev2_0->Attach(channel1);

   Ptr<PointToPointChannel> channel2 = CreateObject<PointToPointChannel>();
   channel2->SetAttribute("Delay", StringValue("10ms"));
   dev1_2->Attach(channel2);
   dev3_0->Attach(channel2);
*/
   
   // "Hardware" setup
   // - Should be the same as above.
   // - This lets the "ASCII tracer" work.
   NodeContainer nodes;
   nodes.Create (4);
   
   /*
    *    0   <-- Server, video publisher
    *    |   <-- Recording traffic.
    *    1   <-- Router
    *   / \
    *  2   3 <-- Clients, both Subscribed
    */
   
   Ptr<Node> node0 = nodes.Get (0);
   Ptr<Node> node1 = nodes.Get (1);
   Ptr<Node> node2 = nodes.Get (2);
   Ptr<Node> node3 = nodes.Get (3);
   
   NodeContainer n0n1(node0,node1);
   NodeContainer n1n2(node1,node2);
   NodeContainer n1n3(node1,node3);
   
   // These are the same kind of thing, but separated so I can log server/router by itself.
   PointToPointHelper p2p_server_router;
   p2p_server_router.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));
   p2p_server_router.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
   p2p_server_router.SetDeviceAttribute ("Mtu", UintegerValue (1500));
   
   PointToPointHelper p2p_router_client;
   p2p_router_client.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));
   p2p_router_client.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
   p2p_router_client.SetDeviceAttribute ("Mtu", UintegerValue (1500));
   
   p2p_server_router.Install(n0n1);
   p2p_router_client.Install(n1n2);
   p2p_router_client.Install(n1n3);
   
   // Setting MAC addresses to match cfg/conf files.
   // I'm not sure if there's a better way to do this
   // I'd like to set them with Install, not relying on GetDevice()/deterministic order of Install()
   
   // 0-1
   Ptr<NetDevice> dev0_0 = node0->GetDevice(0); // generated was "PointToPointNetDevice"
   Ptr<NetDevice> dev1_0 = node1->GetDevice(0);
   dev0_0->SetAddress (Mac48Address("7f:f1:57:17:1b:18"));
   dev1_0->SetAddress (Mac48Address("7f:f1:57:17:1b:19"));
   
   // 1-2
   Ptr<NetDevice> dev1_1 = node1->GetDevice(1);
   Ptr<NetDevice> dev2_0 = node2->GetDevice(0);
   dev1_1->SetAddress (Mac48Address("7f:f1:57:17:1b:1a"));
   dev2_0->SetAddress (Mac48Address("7f:f1:57:17:1b:1b"));

   // 1-3
   Ptr<NetDevice> dev1_2 = node1->GetDevice(2);
   Ptr<NetDevice> dev3_0 = node3->GetDevice(0);
   dev1_2->SetAddress (Mac48Address("7f:f1:57:17:1b:1c"));
   dev3_0->SetAddress (Mac48Address("7f:f1:57:17:1b:1d"));
   
   NS_LOG_INFO("0->1: " << dev0_0);
   
   
   
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
   Ptr<ClickBridge> click2 = CreateObject<ClickBridge > ();
   node2->AggregateObject(click2);
   click2->SetClickFile(path + "00000003.conf");
   Ptr<ServiceModel> servModel2 = CreateObject<ServiceModel > (); 
   node2->AggregateObject(servModel2); 
   Ptr<ClickBridge> click3 = CreateObject<ClickBridge > ();
   node3->AggregateObject(click3);
   click3->SetClickFile(path + "00000004.conf");
   Ptr<ServiceModel> servModel3 = CreateObject<ServiceModel > (); 
   node3->AggregateObject(servModel3); 

   Ptr<TopologyManager> tm = CreateObject<TopologyManager > ();
   tm->SetStartTime(Seconds(0.)); 
   tm->SetAttribute("Topology", StringValue(path + "topology.graphml"));
   node0->AddApplication(tm); 

   // "User Application" setup
   Ptr<VideoPublisher> app0_0 = CreateObject<VideoPublisher> ();
   app0_0->SetStartTime(Seconds(2.34)); 
   app0_0->SetStopTime(Seconds(14.87)); 
   node0->AddApplication(app0_0);

   Ptr<Subscriber> app2_0 = CreateObject<Subscriber> ();
   app2_0->SetStartTime(Seconds(2.34)); 
   app2_0->SetStopTime(Seconds(14.87)); 
   node2->AddApplication(app2_0);

   Ptr<Subscriber> app3_0 = CreateObject<Subscriber> ();
   app3_0->SetStartTime(Seconds(2.34)); 
   app3_0->SetStopTime(Seconds(14.87)); 
   node3->AddApplication(app3_0);

   //jc776: Set up tracing on the up/down wire.
   AsciiTraceHelper ascii;
   // both of these are recording ALL data, instead of just their specific ones
   // srv/rtr: merge 0_0, 1_0
   // rtr/cls: merge 1_1, 1_2, 2_1, 3_1
   p2p_server_router.EnableAsciiAll (ascii.CreateFileStream ("logs/QoS/double_server_router.tr"));
   p2p_server_router.EnablePcapAll ("logs/QoS/double_server_router");
   p2p_router_client.EnableAsciiAll (ascii.CreateFileStream ("logs/QoS/double_router_clients.tr"));
   p2p_router_client.EnablePcapAll ("logs/QoS/double_router_clients");

   Simulator::Run();
   Simulator::Destroy();
   return 0;
}
