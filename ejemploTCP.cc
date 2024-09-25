// Network topology
//
//           10Mb/s, 10ms       
//       n0-----------------n1


#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"

using namespace ns3;

int
main (int argc, char *argv[])
{

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("20Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("10ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  
  uint16_t port = 50000; // Puerto en el que escucha el servidor
  
  PacketSinkHelper tcpserver ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (),port));
  ApplicationContainer serverApps = tcpserver.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  
  BulkSendHelper tcpClient ("ns3::TcpSocketFactory", InetSocketAddress (interfaces.GetAddress (1), port));
  tcpClient.SetAttribute ("MaxBytes", UintegerValue (4294967296)); //enviar max 4GB 
  ApplicationContainer clientApps = tcpClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));


  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("tcp-prueba.tr"));
  
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
  
  
  
