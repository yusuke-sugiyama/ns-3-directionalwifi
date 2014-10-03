/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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
#include <ns3/double.h>
#include <ns3/angles.h>
#include <cmath>

#include "wifi-antenna-model.h"
#include "wifi-parabolic-antenna-model.h"


NS_LOG_COMPONENT_DEFINE ("WifiParabolicAntennaModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiParabolicAntennaModel);


TypeId 
WifiParabolicAntennaModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::WifiParabolicAntennaModel")
    .SetParent<WifiAntennaModel> ()
    .AddConstructor<WifiParabolicAntennaModel> ()
    .AddAttribute ("Beamwidth",
                   "The 3dB beamwidth (degrees)",
                   DoubleValue (60),
                   MakeDoubleAccessor (&WifiParabolicAntennaModel::SetBeamwidth,
                                       &WifiParabolicAntennaModel::GetBeamwidth),
                   MakeDoubleChecker<double> (0, 180))
    .AddAttribute ("MaxAttenuation",
                   "The maximum attenuation (dB) of the antenna radiation pattern.",
                   DoubleValue (20.0),
                   MakeDoubleAccessor (&WifiParabolicAntennaModel::m_maxAttenuation),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

void 
WifiParabolicAntennaModel::SetBeamwidth (double beamwidthDegrees)
{ 
  NS_LOG_FUNCTION (this << beamwidthDegrees);
  m_beamwidthRadians = DegreesToRadians (beamwidthDegrees);
}

double
WifiParabolicAntennaModel::GetBeamwidth () const
{
  return RadiansToDegrees (m_beamwidthRadians);
}

double 
WifiParabolicAntennaModel::DoGetGainDb (Angles a)
{
  NS_LOG_FUNCTION (this << a);
  // azimuth angle w.r.t. the reference system of the antenna
  double phi = a.phi;

  // make sure phi is in (-pi, pi]
  while (phi <= -M_PI)
    {
      phi += M_PI+M_PI;
    }
  while (phi > M_PI)
    {
      phi -= M_PI+M_PI;
    }

  NS_LOG_LOGIC ("phi = " << phi );

  double gainDb = -std::min (12 * pow (phi / m_beamwidthRadians, 2), m_maxAttenuation);

  NS_LOG_LOGIC ("gain = " << gainDb);
  return gainDb;
}


}

