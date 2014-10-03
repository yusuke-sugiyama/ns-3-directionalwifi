/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
 *
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#include <ns3/log.h>
#include <cmath>
#include "wifi-antenna-model.h"
#include "ns3/orientation-model.h"
#include "ns3/antenna-model.h"
#include "ns3/mobility-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiAntennaModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiAntennaModel);

WifiAntennaListener::~WifiAntennaListener ()
{
}

TypeId 
WifiAntennaModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiAntennaModel")
    .SetParent<Object> ()
    ;
  return tid;
}

WifiAntennaModel::WifiAntennaModel ()
{
}

WifiAntennaModel::~WifiAntennaModel ()
{
}

void
WifiAntennaModel::SetOrientationModel (Ptr<OrientationModel> orientation){
  m_orientation = orientation;
}

Angles
WifiAntennaModel::GetOrientation (){
  return m_orientation->GetOrientation ();
}

void
WifiAntennaModel::SetOrientation (const Angles &orientation){
  m_orientation->SetOrientation(orientation);
}

double
WifiAntennaModel::GetGainDb (Ptr<MobilityModel> src, Ptr<MobilityModel> dest){
  Angles oriSum;
  /*
  Ptr<OrientationModel> antOri = this->GetObject<OrientationModel>();
  if(antOri != 0){
    oriSum.phi += antOri->GetOrientation().phi;
    oriSum.theta += antOri->GetOrientation().theta;
  }
  */
  if(m_orientation != 0){
    oriSum.phi += m_orientation->GetOrientation ().phi;
    oriSum.theta += m_orientation->GetOrientation ().theta;
  }
  Angles bet(dest->GetPosition(), src->GetPosition());
  oriSum.phi = NormalizeOverTwoPI(bet.phi - oriSum.phi);
  oriSum.theta = NormalizeOverTwoPI(bet.theta - oriSum.theta);
  return DoGetGainDb (oriSum);
}

void
WifiAntennaModel::SetAntennaMode (int mode){
  m_antennaMode = mode;
  return;
}

void
WifiAntennaModel::SetAntennaMode (Angles bet){
  return;
}

int
WifiAntennaModel::GetNextAntennaMode (Angles bet){
  return m_antennaMode;
}

int
WifiAntennaModel::GetAntennaMode (){
  return m_antennaMode;
}

void
WifiAntennaModel::RegisterListener (WifiAntennaListener *listener)
{
  NS_LOG_FUNCTION(this << listener);
  m_listeners.push_back (listener);
}

void
WifiAntennaModel::NotifyChangeAntennaMode (int mode)
{
  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++)
    {
      NS_LOG_DEBUG(this << " " << (*i));
      (*i)->NotifyChangeAntennaMode (mode);
    }
}

}
