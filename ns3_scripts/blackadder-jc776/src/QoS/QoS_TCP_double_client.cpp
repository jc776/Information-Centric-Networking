// Changes from "original":
// - Imported (new) publisher/subscriber
// - Local file paths instead of "/tmp" or "/home/pursuit"
// - Switching "PointToPointNetDevice"/"Channel" to "PointToPointHelper" so that I can use Tracing
// - *Tracing traffic on wire*

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
//#include <ns3/blackadder-module.h>

#include <ns3/internet-module.h>
#include <ns3/applications-module.h>

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
   // - This is the same as the generated one, but usable with the logging tools.
   // - Common to both QoS Single Client files.
   // - Still haven't made it a common file.
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

   NodeContainer clients(node2,node3);
   
   // Wire Attributes
   PointToPointHelper p2p;
   p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));
   p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
   p2p.SetDeviceAttribute ("Mtu", UintegerValue (1500));
   
   NetDeviceContainer wires_server_router  = p2p.Install(n0n1);
   NetDeviceContainer wires_router_client1 = p2p.Install(n1n2);
   NetDeviceContainer wires_router_client2 = p2p.Install(n1n3);
   
   // Setting MAC addresses to match cfg/conf files.
   // I'm not sure if there's a better way to do this
   // I'd like to set them with Install, not relying on GetDevice()/deterministic order of Install()
   // TCP doesn't use MAC addresses.
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
   
  // Underlying software setup
  InternetStackHelper internet;
  internet.InstallAll ();

  // Assigned IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (wires_server_router);
  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer rcInterface1 = ipv4.Assign (wires_router_client1);
  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer rcInterface2 = ipv4.Assign (wires_router_client2);

  // IP-level connectivity.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   
   // "User Application" setup

  // Both clients have a 'packet sink'.
  uint16_t servPort = 8080;
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer sinkApp = sinkHelper.Install (clients); // difference, EACH client
  sinkApp.Start (Seconds (2.34));
  sinkApp.Stop (Seconds (14.87));

  // 1316B/600us = 1316MB/600s = 2.193MB/s

  // The server has an application sending at a constant rate for each client.
  // rcInterfaces.GetAddress(1) is 'the second half of the router-client interface' so the client...
  Address remoteAddress (InetSocketAddress (rcInterface1.GetAddress(1), servPort));
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", remoteAddress);
  clientHelper.SetConstantRate(DataRate ("2.2MB/s"));
  ApplicationContainer clientApp = clientHelper.Install (node0);
  clientApp.Start (Seconds (2.34));
  clientApp.Stop (Seconds (14.87));

  Address remoteAddress2 (InetSocketAddress (rcInterface2.GetAddress(1), servPort));
  OnOffHelper clientHelper2 ("ns3::TcpSocketFactory", remoteAddress2);
  clientHelper2.SetConstantRate(DataRate ("2.2MB/s"));
  ApplicationContainer clientApp2 = clientHelper2.Install (node0);
  clientApp2.Start (Seconds (2.34));
  clientApp2.Stop (Seconds (14.87));

   //Set up tracing on the up/down wire.
   AsciiTraceHelper ascii;
   p2p.EnableAscii (ascii.CreateFileStream ("logs/QoS/TCP/double_wire.tr"),wires_server_router);
   p2p.EnablePcap ("logs/QoS/TCP/double_wire",wires_server_router);
   //p2p.EnableAsciiAll (ascii.CreateFileStream ("logs/QoS/TCP/double_all.tr"));
   //p2p.EnablePcapAll ("logs/QoS/TCP/single_all",true);

   Simulator::Run();
   Simulator::Destroy();
   return 0;
}
