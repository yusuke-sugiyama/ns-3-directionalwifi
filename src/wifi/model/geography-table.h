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

#ifndef GEOGRAPHY_TABLE_H
#define GEOGRAPHY_TABLE_H

#include <stdint.h>
#include <vector>

#include "ns3/mac48-address.h"
#include "ns3/object.h"
#include "ns3/vector.h"
#include "ns3/angles.h"

namespace ns3 {

class GeographyItem
{
public:
  ~GeographyItem();
  GeographyItem (Mac48Address address, const Vector &position);
  Mac48Address GetAddress (void);
  void SetPosition (const Vector &position);
  Vector GetPosition ();
  
private:
  Mac48Address m_address;
  Vector m_position;
};

class GeographyTable : public Object
{
public:
  static TypeId GetTypeId (void);
  Angles GetAngle(Mac48Address, const Vector &position, bool *existsAddress);
  void AddItem(Mac48Address address, const Vector &position);
  void InitItem();
  bool IsExistsAddress(Mac48Address address);
  void UpdatePosition(Mac48Address address, const Vector &position);
  void UpdateTable(Mac48Address address, const Vector &position);

  GeographyTable();
  ~GeographyTable();
private:
  std::vector<GeographyItem*> items;
};

} // namespace ns3


#endif /* GEOGRAPHY_TABLE_H */

