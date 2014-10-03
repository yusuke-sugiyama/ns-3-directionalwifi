/* -*- Mode:C++; -*- */
/*
 * Copyright (c) 2014 Yusuke Sugiyama
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
n * published by the Free Software Foundation;
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

#include <vector>
#include "ns3/log.h"
#include "geography-table.h"
#include "ns3/random-variable-stream.h"
#include "ns3/mac48-address.h"
#include "ns3/double.h"

NS_LOG_COMPONENT_DEFINE ("GeographyTable");

namespace ns3 {

TypeId
GeographyTable::GetTypeId (void)
{
  static TypeId tid = TypeId ("GeographyTable")
    .SetParent<Object> ()
    .AddConstructor<GeographyTable> ()
    ;
  return tid;
}

GeographyTable::GeographyTable ()
{
  InitItem();
}

GeographyTable::~GeographyTable ()
{
  InitItem();
}

Angles
GeographyTable::GetAngle(Mac48Address address, const Vector &position, bool *existsAddress)
{
  NS_LOG_FUNCTION(this << address << position);
  for(unsigned int i = 0; i < items.size(); i++)
    {
      if(items[i]->GetAddress() == address)
	{
	  *existsAddress = true;
	  return Angles (items[i]->GetPosition (), position);
	}
    }
  *existsAddress = false;
  return Angles ((double)0, (double)0);
}
  
void
GeographyTable::AddItem(Mac48Address address, const Vector &position)
{
  NS_LOG_FUNCTION(this << address << position);
  items.push_back(new GeographyItem(address, position));
}
  
void
GeographyTable::InitItem()
{
  items.clear();
}
bool
GeographyTable::IsExistsAddress(Mac48Address address)
{
  NS_LOG_FUNCTION(this);
  for(unsigned int i = 0; i < items.size(); i++)
    {
      NS_LOG_DEBUG(this << items[i]->GetAddress());
      if(items[i]->GetAddress() == address)
	{
	  return true;
	}
    }
  return false;
}
void
GeographyTable::UpdatePosition(Mac48Address address,  const Vector &position)
{
  NS_LOG_FUNCTION(this);
  for(unsigned int i = 0; i < items.size(); i++)
    {
      if(items[i]->GetAddress() == address)
	{
	  items[i]->SetPosition(position);
	}
    }
}  
void
GeographyTable::UpdateTable(Mac48Address address, const Vector &position)
{

  NS_LOG_FUNCTION(this << address);
  if(IsExistsAddress(address))
    {
      UpdatePosition(address, position);
    }
  else
    {
      AddItem(address, position);
    }
}


  
GeographyItem::GeographyItem (Mac48Address address, const Vector &position)
  : m_address (address),
    m_position (position)
{
}

GeographyItem::~GeographyItem ()
{
}

Mac48Address
GeographyItem::GetAddress()
{
  return m_address;
}

void
GeographyItem::SetPosition (const Vector &position)
{
  m_position = position;
}

Vector
GeographyItem::GetPosition ()
{
  return m_position;
}

} // namespace ns3
