// INCOMPLETE
// Doesn't measure anything yet.

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

  //  LogComponentEnable("TcpL4Protocol", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpSocketImpl", LOG_LEVEL_ALL);
  //  LogComponentEnable("PacketSink", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpLargeTransfer", LOG_LEVEL_ALL);
   
   // jc776 - Find source file's path.
   //UNIX-like only, but so is the rest of this so far.
   // Conf files not needed yet.
   //std::string aux(argv[0]);
   //int pos = aux.rfind('/');
   
   //   /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/build/examples/blackadder-jc776/
   //-> /home/jack/Desktop/Project/repos/ns-allinone-3.15/ns-3.15/examples/blackadder-jc776/conf/QoS_double_client/
   //std::string path = aux.substr(0,pos+1) + "../../../examples/blackadder-jc776/conf/QoS_double_client/";

   
   // "Hardware" setup
   // - This is the same as the generated one, but usable with the logging tools.
   // - Common to both QoS Single Client files.
   // - Still haven't made it a common file.
   NodeContainer nodes;
   nodes.Create (3);
   
   /*
    *    0   <-- Server, OnOffApplication (Sends at 720p towards 2)
    *    |   <-- Recording traffic.
    *    1   <-- Router
    *    | 
    *    2   <-- Client, PacketSinkApplication (Recieves from 0)
    */
   
   // Nodes
   Ptr<Node> node0 = nodes.Get (0);
   Ptr<Node> node1 = nodes.Get (1);
   Ptr<Node> node2 = nodes.Get (2);
   
   NodeContainer n0n1(node0,node1);
   NodeContainer n1n2(node1,node2);
   
   // Wire attributes
   PointToPointHelper p2p;
   p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mbps")));
   p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (10)));
   p2p.SetDeviceAttribute ("Mtu", UintegerValue (1500));
   
   // Install wires
   NetDeviceContainer wires_server_router = p2p.Install(n0n1);
   NetDeviceContainer wires_router_client = p2p.Install(n1n2);
   
   // MAC Addresses (not used by TCP)
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
   
  // Underlying software setup
  InternetStackHelper internet;
  internet.InstallAll ();

  // Assigned IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (wires_server_router);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer rcInterfaces = ipv4.Assign (wires_router_client);

  // IP-level connectivity.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
   
   // "User Application" setup

  // The client (2) has a 'packet sink'.
  uint16_t servPort = 8080;
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer sinkApp = sinkHelper.Install (node2);
  sinkApp.Start (Seconds (2.34));
  sinkApp.Stop (Seconds (14.87));

  // 1316B/600us = 1316MB/600s = 2.193MB/s

  // The server (0) sends at a constant rate, towards the client.
  // rcInterfaces.GetAddress(1) is 'the second half of the router-client interface' so the client...
  Address remoteAddress (InetSocketAddress (rcInterfaces.GetAddress(1), servPort));
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", remoteAddress);
  clientHelper.SetConstantRate(DataRate ("2.2MB/s"));
  ApplicationContainer clientApp = clientHelper.Install (node0);
  clientApp.Start (Seconds (2.34));
  clientApp.Stop (Seconds (14.87));

   //jc776: Set up tracing on the up/down wire.
   AsciiTraceHelper ascii;
   // both of these are recording ALL data, instead of just their specific ones
   // srv/rtr: merge 0_0, 1_0
   // rtr/cls: merge 1_1, 1_2, 2_1, 3_1
   //p2p_server_router.EnableAsciiAll (ascii.CreateFileStream ("logs/QoS/single_server.tr"));
   //p2p_server_router.EnablePcapAll ("logs/QoS/single_server_router");
   
   //p2p.EnableAsciiAll (ascii.CreateFileStream ("logs/QoS/single_server_all.tr"));
   //p2p.EnablePcapAll ("logs/QoS/single_server_all");

   p2p.EnableAscii (ascii.CreateFileStream ("logs/QoS/single_server_wire.tr"),wires_server_router);
   p2p.EnablePcap ("logs/QoS/single_server_wire",wires_server_router);

   Simulator::Run();
   Simulator::Destroy();
   return 0;
}
