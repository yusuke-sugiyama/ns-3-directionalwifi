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
 * Author: Brian Panneton <brian.c.panneton.ctr@us.army.mil>
 */

#include <ns3/log.h>
#include <ns3/double.h>
#include <limits>
#include <iostream>
#include <iomanip>

#include "wifi-antenna-model.h"
#include "wifi-constant-gain-antenna-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiConstantGainAntennaModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiConstantGainAntennaModel);

TypeId
WifiConstantGainAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiConstantGainAntennaModel")
    .SetParent<WifiAntennaModel> ()
    .AddConstructor<WifiConstantGainAntennaModel> ()
    .AddAttribute ("InsideGain",
                   "The gain inside the radiation pattern (dbm)",
                   DoubleValue (1),
                   MakeDoubleAccessor (
                        &WifiConstantGainAntennaModel::SetGainInsidePattern, 
                        &WifiConstantGainAntennaModel::GetGainInsidePattern),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("OutsideGain",
                   "The gain outside the radiation pattern (dbm)",
                   DoubleValue (0),
                   MakeDoubleAccessor (
                        &WifiConstantGainAntennaModel::SetGainOutsidePattern, 
                        &WifiConstantGainAntennaModel::GetGainOutsidePattern),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("AzimuthBeamwidth",
                   "The estimated Azimuth Beamwidth of the radiation pattern (radians)",
                   DoubleValue (2*M_PI),
                   MakeDoubleAccessor (
                        &WifiConstantGainAntennaModel::SetAzimuthBeamwidth, 
                        &WifiConstantGainAntennaModel::GetAzimuthBeamwidth),
                   MakeDoubleChecker<double> (0, 2*M_PI))
    .AddAttribute ("ElevationBeamwidth",
                   "The estimated Elevation Beamwidth of the radiation pattern (radians)",
                   DoubleValue (M_PI),
                   MakeDoubleAccessor (
                        &WifiConstantGainAntennaModel::SetElevationBeamwidth, 
                        &WifiConstantGainAntennaModel::GetElevationBeamwidth),
                   MakeDoubleChecker<double> (0, M_PI)) 
    ;
  return tid;
}

double
WifiConstantGainAntennaModel::DoGetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
  if((GetAzimuthBeamwidth()/2) > 0 && (GetAzimuthBeamwidth()/2) < M_PI/2)
  {
      if(a.phi > (GetAzimuthBeamwidth()/2)+.000001 && a.phi <= M_PI)
        return m_outerGain;
      if(a.phi < ((M_PI*2)-(GetAzimuthBeamwidth()/2))-.000001 && a.phi >= M_PI) 
        return m_outerGain;
  }

  if(a.theta > (GetElevationBeamwidth()/2)+.000001)
    return m_outerGain;
   
  return m_innerGain;
}

void 
WifiConstantGainAntennaModel::SetGainInsidePattern (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_innerGain = gain;
}

double
WifiConstantGainAntennaModel::GetGainInsidePattern (void) const
{
  return m_innerGain;
}

void 
WifiConstantGainAntennaModel::SetGainOutsidePattern (double gain)
{
  NS_LOG_FUNCTION (this << gain);
  m_outerGain = gain;
}

double
WifiConstantGainAntennaModel::GetGainOutsidePattern (void) const
{
  return m_outerGain;
}

void
WifiConstantGainAntennaModel::SetAzimuthBeamwidth (double bw)
{
  NS_LOG_FUNCTION (this << bw);
  m_aziBW = NormalizeOverTwoPI(bw);
}

double
WifiConstantGainAntennaModel::GetAzimuthBeamwidth (void) const
{
  return m_aziBW;
}

void
WifiConstantGainAntennaModel::SetElevationBeamwidth (double bw)
{
  NS_LOG_FUNCTION (this << bw);
  m_elvBW = NormalizeOverPI(bw);
}

double
WifiConstantGainAntennaModel::GetElevationBeamwidth (void) const
{
  return m_elvBW;
}

}




