/*
 * Copyright (C) 2010-2012  George Parisis and Dirk Trossen
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of
 * the BSD license.
 *
 * See LICENSE and COPYING for more details.
 */

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/blackadder-module.h>
#include "publisher.h"
#include "subscriber.h"

// jc776
#include <string>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Example1");

/*This topology is a single node with no connectivity - Blackadder is used for interprocess communication only*/
int
main(int argc, char *argv[]) {
    std::string aux(argv[0]);
    // jc776 - UNIX-like only, but so is the rest of this so far.
    int pos = aux.rfind('/');

    std::string path = aux.substr(0,pos+1);
    //NS_LOG_UNCOND ("Path: " + path);
    
    NodeContainer nodes;
    nodes.Create(1);

    Ptr<ClickBridge> click0 = CreateObject<ClickBridge > ();
    nodes.Get(0)->AggregateObject(click0);
    click0->SetClickFile(path + "test.click");
    Ptr<ServiceModel> servModel0 = CreateObject<ServiceModel > ();
    nodes.Get(0)->AggregateObject(servModel0);

    Ptr<Publisher> pub = CreateObject<Publisher > ();
    pub->SetStartTime(Seconds(2.));
    pub->SetStopTime(Seconds(8.));
    nodes.Get(0)->AddApplication(pub);

    Ptr<Subscriber> sub = CreateObject<Subscriber > ();
    sub->SetStartTime(Seconds(0.));
    sub->SetStopTime(Seconds(5));
    nodes.Get(0)->AddApplication(sub);

    Ptr<TopologyManager> tm = CreateObject<TopologyManager > ();
    tm->SetStartTime(Seconds(0.));
    tm->SetStopTime(Seconds(20));
    tm->SetAttribute("Topology", StringValue("/home/pursuit/topology.graphml"));
    nodes.Get(0)->AddApplication(tm);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
