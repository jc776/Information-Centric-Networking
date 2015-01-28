#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/blackadder-module.h>
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("topology");
int main(int argc, char *argv[]) {
   Ptr<Node> node0 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev0_0 = Create<PointToPointNetDevice>();
   dev0_0->SetAddress (Mac48Address("7f:fa:28:48:cb:18"));
   dev0_0->SetDataRate (DataRate("100Mbps"));
   dev0_0->SetMtu (1500);
   node0->AddDevice(dev0_0);
   Ptr<DropTailQueue> queue0_0 = CreateObject<DropTailQueue > ();
   dev0_0->SetQueue(queue0_0);

   Ptr<Node> node1 = CreateObject<Node>();
   Ptr<PointToPointNetDevice> dev1_0 = Create<PointToPointNetDevice>();
   dev1_0->SetAddress (Mac48Address("7f:fa:28:48:cb:19"));
   dev1_0->SetDataRate (DataRate("100Mbps"));
   dev1_0->SetMtu (1500);
   node1->AddDevice(dev1_0);
   Ptr<DropTailQueue> queue1_0 = CreateObject<DropTailQueue > ();
   dev1_0->SetQueue(queue1_0);

   Ptr<PointToPointChannel> channel0 = CreateObject<PointToPointChannel>();
   channel0->SetAttribute("Delay", StringValue("10ms"));
   dev0_0->Attach(channel0);
   dev1_0->Attach(channel0);

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

   Ptr<TopologyManager> tm = CreateObject<TopologyManager > ();
   tm->SetStartTime(Seconds(0.)); 
   tm->SetAttribute("Topology", StringValue("/tmp/topology.graphml"));
   node0->AddApplication(tm); 

   Ptr<Publisher> app0_0 = CreateObject<Publisher> ();
   app0_0->SetStartTime(Seconds(2.34)); 
   app0_0->SetStopTime(Seconds(14.87)); 
   node0->AddApplication(app0_0);

   Ptr<Subscriber> app1_0 = CreateObject<Subscriber> ();
   app1_0->SetStartTime(Seconds(2.34)); 
   app1_0->SetStopTime(Seconds(14.87)); 
   node1->AddApplication(app1_0);

   Simulator::Run();
   Simulator::Destroy();
   return 0;
}
