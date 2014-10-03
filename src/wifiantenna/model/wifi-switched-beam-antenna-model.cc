/* -*- Mode:C++; -*- */
/*
 * Copyright (c) 2014 Yusuke Sugiyama
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
 * Foundation, Inc., Saruwatari Lab, Shizuoka University, Japan
 *
 * Author: Yusuke Sugiyama <sugiyama@aurum.cs.inf.shizuoka.ac.jp>
 */


#include <ns3/log.h>
#include <ns3/double.h>
#include <limits>
#include <iostream>
#include <iomanip>

#include "wifi-antenna-model.h"
#include "wifi-switched-beam-antenna-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiSwitchedBeamAntennaModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiSwitchedBeamAntennaModel);

TypeId
WifiSwitchedBeamAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiSwitchedBeamAntennaModel")
    .SetParent<WifiAntennaModel> ()
    .AddConstructor<WifiSwitchedBeamAntennaModel> ()
    .AddAttribute ("OmniGain",
                   "The gain omni mode (dbm)",
                   DoubleValue (0),
                   MakeDoubleAccessor (
                        &WifiSwitchedBeamAntennaModel::SetGainOmniMode, 
                        &WifiSwitchedBeamAntennaModel::GetGainOmniMode),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("InsideGain",
                   "The gain inside the radiation pattern (dbm)",
                   DoubleValue (3),
                   MakeDoubleAccessor (
                        &WifiSwitchedBeamAntennaModel::SetGainInsidePattern, 
                        &WifiSwitchedBeamAntennaModel::GetGainInsidePattern),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("OutsideGain",
                   "The gain outside the radiation pattern (dbm)",
                   DoubleValue (-80),
                   MakeDoubleAccessor (
                        &WifiSwitchedBeamAntennaModel::SetGainOutsidePattern, 
                        &WifiSwitchedBeamAntennaModel::GetGainOutsidePattern),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("AzimuthBeamwidth",
                   "The estimated Azimuth Beamwidth of the radiation pattern (radians)",
                   DoubleValue (2*M_PI),
                   MakeDoubleAccessor (
                        &WifiSwitchedBeamAntennaModel::SetAzimuthBeamwidth, 
                        &WifiSwitchedBeamAntennaModel::GetAzimuthBeamwidth),
                   MakeDoubleChecker<double> (0, 2*M_PI))
    .AddAttribute ("ElevationBeamwidth",
                   "The estimated Elevation Beamwidth of the radiation pattern (radians)",
                   DoubleValue (M_PI),
                   MakeDoubleAccessor (
                        &WifiSwitchedBeamAntennaModel::SetElevationBeamwidth, 
                        &WifiSwitchedBeamAntennaModel::GetElevationBeamwidth),
                   MakeDoubleChecker<double> (0, M_PI)) 
    ;
  return tid;
}

double
WifiSwitchedBeamAntennaModel::DoGetGainDb (Angles a)
{
  //  double phi   = GetOrientation().phi;
  //  double theta = GetOrientation().theta;
  NS_LOG_FUNCTION (this << a);
  if(m_antennaMode == OMNI) {
    return m_omniGain;
  }
  if((GetAzimuthBeamwidth()/2) > 0 && (GetAzimuthBeamwidth()/2) < M_PI/2)
  {
    if(a.phi > (GetAzimuthBeamwidth()/2) && a.phi <= M_PI){
      return m_outerGain;
    }
    if(a.phi < ((M_PI*2)-(GetAzimuthBeamwidth()/2)) && a.phi >= M_PI) {
      return m_outerGain;
    }
  }

  if(a.theta > (GetElevationBeamwidth()/2)){
    return m_outerGain;
  }
   
  return m_innerGain;
}

void 
WifiSwitchedBeamAntennaModel::SetGainInsidePattern (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_innerGain = gain;
}

double
WifiSwitchedBeamAntennaModel::GetGainInsidePattern (void) const
{
  return m_innerGain;
}

void 
WifiSwitchedBeamAntennaModel::SetGainOutsidePattern (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_outerGain = gain;
}

double
WifiSwitchedBeamAntennaModel::GetGainOutsidePattern (void) const
{
  return m_outerGain;
}

void 
WifiSwitchedBeamAntennaModel::SetGainOmniMode (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_omniGain = gain;
}

double
WifiSwitchedBeamAntennaModel::GetGainOmniMode (void) const
{
  return m_omniGain;
}

void
WifiSwitchedBeamAntennaModel::SetAzimuthBeamwidth (double bw)
{
  NS_LOG_FUNCTION (this << bw);
  m_aziBW = NormalizeOverTwoPI(bw);
}


double
WifiSwitchedBeamAntennaModel::GetAzimuthBeamwidth (void) const
{
  return m_aziBW;
}

void
WifiSwitchedBeamAntennaModel::SetElevationBeamwidth (double bw)
{
  NS_LOG_FUNCTION (this << bw);
  m_elvBW = NormalizeOverPI(bw);
}

double
WifiSwitchedBeamAntennaModel::GetElevationBeamwidth (void) const
{
  return m_elvBW;
}

int
WifiSwitchedBeamAntennaModel::GetNextAntennaMode (Angles bet){

  double ang0   = 0;
  double ang90  = M_PI / 2;
  double ang180 = M_PI;
  double ang270 = 3 * M_PI / 2;

  if(bet.phi >= ang0 && bet.phi < ang90){
    return DIRECTIONAL0;
  }else if(bet.phi >= ang90 && bet.phi < ang180){
    return DIRECTIONAL90;
  }else if(bet.phi >= ang180 && bet.phi < ang270){
    return DIRECTIONAL180;
  }else{
    return DIRECTIONAL270;
  }
}

void
WifiSwitchedBeamAntennaModel::SetAntennaMode (int mode)
{
  m_antennaMode = mode;

  Angles ang0(0, 0);
  Angles ang45((double)M_PI / 4, (double)0);
  Angles ang135((double)3 * M_PI / 4, (double)0);
  Angles ang225((double)5 * M_PI / 4, (double)0);
  Angles ang315((double)7 * M_PI / 4, (double)0);

  switch(mode){
  case OMNI:
    SetAzimuthBeamwidth (0);
    SetOrientation (ang0);
    break;
  case DIRECTIONAL0:
    SetAzimuthBeamwidth (M_PI / 2);
    SetOrientation (ang45);
    break;
  case DIRECTIONAL90:
    SetAzimuthBeamwidth (M_PI / 2);
    SetOrientation (ang135);
    break;
  case DIRECTIONAL180:
    SetAzimuthBeamwidth (M_PI / 2);
    SetOrientation (ang225);
    break;
  case DIRECTIONAL270:
    SetAzimuthBeamwidth (M_PI / 2);
    SetOrientation (ang315);
    break;
  }

  for (Listeners::const_iterator i = m_listeners.begin (); i != m_listeners.end (); i++)
    {
      (*i)->NotifyChangeAntennaMode (mode);
    }
}
void
WifiSwitchedBeamAntennaModel::SetAntennaMode (Angles bet)
{
  double ang0   = 0;
  double ang90  = M_PI / 2;
  double ang180 = M_PI;
  double ang270 = 3 * M_PI / 2;

  if(bet.phi >= ang0 && bet.phi < ang90){
    SetAntennaMode (DIRECTIONAL0);
  }else if(bet.phi >= ang90 && bet.phi < ang180){
    SetAntennaMode (DIRECTIONAL90);
  }else if(bet.phi >= ang180 && bet.phi < ang270){
    SetAntennaMode (DIRECTIONAL180);
  }else{
    SetAntennaMode (DIRECTIONAL270);
  }
}

}
