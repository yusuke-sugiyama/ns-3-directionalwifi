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

#include "geography-tag.h"
#include "ns3/tag.h"
#include "ns3/double.h"
#include "ns3/log.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GeographyTag);
NS_LOG_COMPONENT_DEFINE ("GeographyTag");

TypeId
GeographyTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GeographyTag")
    .SetParent<Tag> ()
    .AddConstructor<GeographyTag> ()
    .AddAttribute ("x", "The node position",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&GeographyTag::GetX),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("y", "The node position",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&GeographyTag::GetY),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("z", "The node position",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&GeographyTag::GetZ),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}
TypeId
GeographyTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

GeographyTag::GeographyTag ()
  : m_x (0),
    m_y (0),
    m_z (0)
{
}
GeographyTag::GeographyTag (const Vector &position)
  : m_x (position.x),
    m_y (position.y),
    m_z (position.z)
{
}


uint32_t
GeographyTag::GetSerializedSize (void) const
{
  return sizeof (double) + sizeof (double);
}
void
GeographyTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_x);
  i.WriteDouble (m_y);
  //  i.WriteDouble (m_z);
}
void
GeographyTag::Deserialize (TagBuffer i)
{
  m_x = i.ReadDouble ();
  m_y = i.ReadDouble ();
  //  m_z = i.ReadDouble ();
}
void
GeographyTag::Print (std::ostream &os) const
{
  os << "x=" << m_x << ", y= " << m_y << ", z=" << m_z;
}

void
GeographyTag::Set (const Vector &position)
{
  m_x = position.x;
  m_y = position.y;
  m_z = position.z;
}
double
GeographyTag::GetX (void) const
{
  return m_x;
}
double
GeographyTag::GetY (void) const
{
  return m_y;
}
double
GeographyTag::GetZ (void) const
{
  return m_z;
}
Vector
GeographyTag::Get (void) const
{
  return Vector (m_x, m_y, m_z);
}


}
