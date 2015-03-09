#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/blackadder-module.h>
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("topology");
int main(int argc, char *argv[]) {
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

   Ptr<ClickBridge> click0 = CreateObject<ClickBridge > ();
   node0->AggregateObject(click0);
   click0->SetClickFile("/tmp//00000001.conf");
   Ptr<ServiceModel> servModel0 = CreateObject<ServiceModel > (); 
   node0->AggregateObject(servModel0); 
   Ptr<ClickBridge> click1 = CreateObject<ClickBridge > ();
   node1->AggregateObject(click1);
   click1->SetClickFile("/tmp//00000002.conf");
   Ptr<ServiceModel> servModel1 = CreateObject<ServiceModel > (); 
   node1->AggregateObject(servModel1); 
   Ptr<ClickBridge> click2 = CreateObject<ClickBridge > ();
   node2->AggregateObject(click2);
   click2->SetClickFile("/tmp//00000003.conf");
   Ptr<ServiceModel> servModel2 = CreateObject<ServiceModel > (); 
   node2->AggregateObject(servModel2); 
   Ptr<ClickBridge> click3 = CreateObject<ClickBridge > ();
   node3->AggregateObject(click3);
   click3->SetClickFile("/tmp//00000004.conf");
   Ptr<ServiceModel> servModel3 = CreateObject<ServiceModel > (); 
   node3->AggregateObject(servModel3); 

   Ptr<TopologyManager> tm = CreateObject<TopologyManager > ();
   tm->SetStartTime(Seconds(0.)); 
   tm->SetAttribute("Topology", StringValue("/tmp/topology.graphml"));
   node0->AddApplication(tm); 

   Ptr<Publisher> app0_0 = CreateObject<Publisher> ();
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

   Simulator::Run();
   Simulator::Destroy();
   return 0;
}
