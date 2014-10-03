/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 *         Brian Panneton <brian.c.panneton.ctr@us.army.mil>
 */

#include <cmath>

#include "velocity-orientation-model.h"
#include "ns3/mobility-model.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (VelocityOrientationModel);

TypeId 
VelocityOrientationModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::VelocityOrientationModel")
    .SetParent<OrientationModel> ()
    .AddConstructor<VelocityOrientationModel> ()
  ;
  return tid;
}

VelocityOrientationModel::VelocityOrientationModel (void)
{
}

VelocityOrientationModel::~VelocityOrientationModel (void)
{
}

Angles
VelocityOrientationModel::DoGetOrientation (void) const
{
  Ptr<MobilityModel> m = GetObject<MobilityModel>();
  if(m == 0) return Angles (0,0);
  else
  {
    return Angles(m->GetVelocity());
  } 
}

void
VelocityOrientationModel::DoSetOrientation (const Angles &orientation)
{
  // ignore
}

} // namespace ns3
