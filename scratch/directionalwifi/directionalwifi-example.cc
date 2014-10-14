/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Joe Kopena <tjkopena@cs.drexel.edu>
 *
 * These applications are used in the WiFi Distance Test experiment,
 * described and implemented in test02.cc.  That file should be in the
 * same place as this file.  The applications have two very simple
 * jobs, they just generate and receive packets.  We could use the
 * standard Application classes included in the NS-3 distribution.
 * These have been written just to change the behavior a little, and
 * provide more examples.
 *
 */

#include <ctime>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/stats-module.h"
#include "ns3/aodv-module.h"
#include "ns3/dsr-module.h"
#include "ns3/netanim-module.h"
#include "ns3/wifiantenna-module.h"

#include "directionalwifi-apps.h"

#define DATA_TYPE  5
#define DATA       0
#define ACK        1
#define RTS        2
#define CTS        3
#define OTHER      4

#define MAX_TIME 10000 

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("DirectionalWifiSimulator");

Time startAnalysisTime = Seconds (0);

/**********************************************************
                   Trace data
 ***********************************************************/

void
AppSenderRx (Ptr<TimeMinMaxAvgTotalCalculator> endTime, std::string path,
	     uint32_t count, uint32_t numPkts)
{
  if(!numPkts == 0){
    if(100 == count ){
      startAnalysisTime = Simulator::Now();
    }

    if(count >= numPkts){
      endTime->Update (Simulator::Now() - startAnalysisTime);
      Simulator::Stop();
    }
  }

  NS_LOG_INFO (path << "[numPkts]=" << numPkts << " [count]=" << count);
}

void
MacMissedAckCallback (Ptr<CounterCalculator<uint32_t> > datac,
		      std::string path, Mac48Address address) {
  NS_LOG_INFO (path << "[address]=" << address);
  if(startAnalysisTime > Seconds (0)){
    datac->Update ();
  }
}


void
PhyTxBeginCallback (Ptr<TimeMinMaxAvgTotalCalculator> endTime,
		    Ptr<CounterCalculator<uint32_t> > datac[DATA_TYPE],
		    std::string path,
		    Ptr<const Packet> packet)
{

  if(Simulator::Now() >= Seconds(MAX_TIME)){
    endTime->Update (Simulator::Now());
    Simulator::Stop();
  }

  uint32_t packetSize = packet->GetSize ();
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr);

  if(startAnalysisTime > Seconds (0)){
    if(hdr.IsRts ()){
      datac[RTS]->Update();
    }else if (hdr.IsCts ()){
      datac[CTS]->Update();
    }else if (hdr.GetType() == WIFI_MAC_DATA){
      if (packetSize <= 1000) {datac[OTHER]->Update();}
      else                    {datac[DATA]->Update();}
    }else if(hdr.GetType() == WIFI_MAC_CTL_ACK){
      datac[ACK]->Update();
    }
  }
}

void
PhyRxBeginCallback (std::string path, Ptr<const Packet> packet)
{
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr);
}

void
PhyRxEndCallback (std::string path, Ptr<const Packet> packet)
{
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr);
}

void
PhyRxHeaderEndCallback (std::string path, Ptr<const Packet> packet)
{
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr);
}

void
PhyRxOk (std::string path, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble)
{
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr << " [snr]=" << snr << " [mode]=" << mode << " [preamble]" << preamble);
}

void
PhyRxError (std::string path, Ptr<const Packet> packet, double snr)
{
  WifiMacHeader hdr;
  packet->PeekHeader (hdr);
  NS_LOG_INFO (path << "[hdr]=" << hdr << " [snr]=" << snr);
}



//----------------------------------------------
//-- main
//----------------------------------------------
int main (int argc, char *argv[]) {


  ns3::Packet::EnablePrinting();

  double   distance = 90;
  int nodeAmount = 4;
  double   rate = 0.002;
  string format ("omnet");

  string experiment ("change_node-amount_transmission-rate");
  string strategy ("csmaca-dataack");
  string animFile ("my-wifi-anime.xml");
  string input;
  string runID;
  
  {
    stringstream sstr;
    sstr << "run-" << time (NULL);
    runID = sstr.str ();
  }

  ns3::Packet::EnablePrinting();

  // Set up command line parameters used to control the experiment.
  CommandLine cmd;
  cmd.AddValue ("distance", "Distance apart to place nodes (in meters).", distance);
  cmd.AddValue ("format", "Format to use for data output.",format);
  cmd.AddValue ("experiment", "Identifier for experiment.", experiment);
  cmd.AddValue ("strategy", "Identifier for strategy.", strategy);
  cmd.AddValue ("run", "Identifier for run.", runID);
  cmd.AddValue ("nodeAmount", "Number of nodes", nodeAmount);
  cmd.AddValue ("rate", "rate", rate);
  cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);
  cmd.Parse (argc, argv);

  if (format != "omnet" && format != "db") {
    NS_LOG_ERROR ("Unknown output format '" << format << "'");
    return -1;
  }

#ifndef STATS_HAS_SQLITE3
  if (format == "db") {
    NS_LOG_ERROR ("sqlite support not compiled in.");
    return -1;
  }
#endif
  {
    stringstream sstr ("");
    sstr << nodeAmount << "_" << rate;
    input = sstr.str ();
  }

  //------------------------------------------------------------
  //-- Create nodes and network stacks
  //--------------------------------------------
  NS_LOG_INFO ("Creating nodes.");
  NodeContainer nodes;
  nodes.Create (nodeAmount);

  NS_LOG_INFO ("Installing WiFi and Internet stack.");
  WifiHelper wifi = WifiHelper::Default ();
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"));
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("1000"));
  NetDeviceContainer nodeDevices = wifi.Install (wifiPhy, wifiMac, nodes);

  //------------------------------------------------------------
  //-- Setup physical layout
  //--------------------------------------------
  NS_LOG_INFO ("Installing static mobility; distance " << distance << " .");
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  for(int i = 0; i < nodeAmount; i++){
    positionAlloc->Add (Vector (0, distance * i, 0.0)); 
  }

  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (nodes);

  /* AODV */
  AodvHelper aodv;
  InternetStackHelper internet;
  internet.SetRoutingHelper(aodv);
  internet.Install (nodes);

  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("192.168.0.0", "255.255.255.0");
  ipAddrs.Assign (nodeDevices);


  //------------------------------------------------------------
  //-- Setup Animation
  //--------------------------------------------

  AnimationInterface::SetNodeDescription (nodes, "Nodes"); // Optional
  AnimationInterface::SetNodeColor (nodes, 0, 255, 0);     // Optional
  AnimationInterface anim ("wireless-animation.xml");      // Mandatory
  anim.EnablePacketMetadata (); // Optional
  anim.EnableIpv4RouteTracking ("routingtable-wireless.xml", Seconds (0), Seconds (5), Seconds (0.25)); //Optional

  //------------------------------------------------------------
  //-- Create a custom traffic source and sink
  //--------------------------------------------
  NS_LOG_INFO ("Create traffic source & sink.");
  Ptr<Node>         node[nodeAmount];
  Ptr<Sender>     sender[nodeAmount];
  Ptr<Receiver> receiver[nodeAmount];

  for(int i = 0; i < nodeAmount; i++){
    node    [i] = NodeList::GetNode (i);
    sender  [i] = CreateObject<Sender>();
    receiver[i] = CreateObject<Receiver>();
    node[i]->AddApplication (sender[i]);
    node[i]->AddApplication (receiver[i]);
    sender  [i]->SetStartTime (Seconds (0));
    receiver[i]->SetStartTime (Seconds (0));
  }

  std::string strSourceSender1 ("/NodeList/1/ApplicationList/*/$Sender/");
  std::string strSourceSender2 ("/NodeList/2/ApplicationList/*/$Sender/");

  std::ostringstream rateString;
  rateString << rate;
  std::string rateSyntax = "ns3::ConstantRandomVariable[Constant=";
  rateSyntax += rateString.str() + "]";
  std::string numPacketSyntax = "/NodeList/";
  std::ostringstream nodeAmountString;
  nodeAmountString << nodeAmount - 1;
  numPacketSyntax += nodeAmountString.str() + "/ApplicationList/*/$Receiver/NumPackets";
  std::string EndAddress = "192.168.0.";
  std::ostringstream nodeAmountString2;
  nodeAmountString2 << nodeAmount;
  EndAddress += nodeAmountString2.str();

  Config::Set (strSourceSender1 + "Interval", StringValue (rateSyntax));
  Config::Set (strSourceSender1 + "PacketSize", UintegerValue (1500));
  Config::Set (strSourceSender1 + "Destination", Ipv4AddressValue ("192.168.0.1"));
  Config::Set (strSourceSender2 + "Interval", StringValue (rateSyntax));
  Config::Set (strSourceSender2 + "PacketSize", UintegerValue (1500));
  Config::Set (strSourceSender2 + "Destination", Ipv4AddressValue (EndAddress.c_str()));
  Config::Set ("/NodeList/3/ApplicationList/*/$Receiver/NumPackets" , UintegerValue (1100));

  //------------------------------------------------------------
  //-- Setup stats and data collection
  //--------------------------------------------

  DataCollector data;
  data.DescribeRun (experiment, strategy, input, runID);

  /* Phy level trace */

  Ptr<CounterCalculator<uint32_t> > phyTotalTxBegin[DATA_TYPE];
  phyTotalTxBegin[DATA]   = CreateObject<CounterCalculator<uint32_t> >();
  phyTotalTxBegin[ACK]    = CreateObject<CounterCalculator<uint32_t> >();
  phyTotalTxBegin[RTS]    = CreateObject<CounterCalculator<uint32_t> >();
  phyTotalTxBegin[CTS]    = CreateObject<CounterCalculator<uint32_t> >();
  phyTotalTxBegin[OTHER]  = CreateObject<CounterCalculator<uint32_t> >();
  Ptr<TimeMinMaxAvgTotalCalculator> endTime = CreateObject<TimeMinMaxAvgTotalCalculator>();

  std::string strPhy("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/");
  Config::Connect (strPhy + "PhyTxBegin", MakeBoundCallback (&PhyTxBeginCallback, endTime, phyTotalTxBegin));
  Config::Connect (strPhy + "PhyRxBegin", MakeCallback (&PhyRxBeginCallback));
  Config::Connect (strPhy + "PhyRxEnd",   MakeCallback (&PhyRxEndCallback));
  Config::Connect (strPhy + "PhyRxHeaderEnd",  MakeCallback (&PhyRxHeaderEndCallback));
  Config::Connect (strPhy + "$ns3::YansWifiPhy/State/RxOk",    MakeCallback (&PhyRxOk));
  Config::Connect (strPhy + "$ns3::YansWifiPhy/State/RxError", MakeCallback (&PhyRxError));

  std::string types[DATA_TYPE];
  types[DATA]   = "data";
  types[ACK]    = "ack";
  types[RTS]    = "rts";
  types[CTS]    = "cts";
  types[OTHER]  = "other";
  for(int i = 0; i < DATA_TYPE; i++){
    phyTotalTxBegin[i]->SetKey ("phy-tx-" + types[i]);
    phyTotalTxBegin[i]->SetContext ("node[*]");
    data.AddDataCalculator (phyTotalTxBegin[i]); 
  }
  endTime->SetKey ("endTime");
  endTime->SetContext (".");
  data.AddDataCalculator (endTime);


  /* Mac level trace */
  Ptr<PacketCounterCalculator> macTotalMissedAck = CreateObject<PacketCounterCalculator>();

  std::string strMacHigh("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/");
  Config::Connect (strMacHigh + "MacTxDataFailed", MakeBoundCallback (&MacMissedAckCallback, macTotalMissedAck));

  macTotalMissedAck->SetKey ("mac-total-missed-ack");
  macTotalMissedAck->SetContext ("node[*]");
  data.AddDataCalculator (macTotalMissedAck);


  /* Application level trace */
  Ptr<PacketSizeMinMaxAvgTotalCalculator> appTxPkts = CreateObject<PacketSizeMinMaxAvgTotalCalculator>();
  Ptr<TimeMinMaxAvgTotalCalculator> delayStat = CreateObject<TimeMinMaxAvgTotalCalculator>();
  std::string strReceiver("/NodeList/*/ApplicationList/*/$Receiver/");
  Config::Connect (strReceiver + "Rx", MakeBoundCallback (&AppSenderRx, endTime));

  delayStat->SetKey ("delay");
  delayStat->SetContext (".");
  receiver[nodeAmount - 1]->SetDelayTracker (delayStat);
  data.AddDataCalculator (delayStat);

  
  //------------------------------------------------------------
  //-- Run the simulation
  //--------------------------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();

  //------------------------------------------------------------
  //-- Generate statistics output.
  //--------------------------------------------

  // Pick an output writer based in the requested format.
  Ptr<DataOutputInterface> output = 0;
  if (format == "omnet") {
    NS_LOG_INFO ("Creating omnet formatted data output.");
    output = CreateObject<OmnetDataOutput>();
  } else if (format == "db") {
#ifdef STATS_HAS_SQLITE3
    NS_LOG_INFO ("Creating sqlite formatted data output.");
    output = CreateObject<SqliteDataOutput>();
#endif
  } else {
    NS_LOG_ERROR ("Unknown output format " << format);
  }
  
  // Finally, have that writer interrogate the DataCollector and save
  // the results.
  if (output != 0) {
    output->Output (data);
  }

  // Free any memory here at the end of this example.
  Simulator::Destroy ();
  
  // end main
}
