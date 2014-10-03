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
#ifndef ORIENTATION_MODEL_H
#define ORIENTATION_MODEL_H

#include "ns3/angles.h"
#include "ns3/vector.h"
#include "ns3/object.h"

namespace ns3 {

class MobilityModel;

/**
 * \ingroup orientation
 * \brief Keep track of the current orientation of an object.
 *
 * Object orientation is described by unit angles
 *
 * This is a base class for all specific orientation models.
 */
class OrientationModel : public Object
{
public:
  static TypeId GetTypeId (void);
  OrientationModel ();
  virtual ~OrientationModel () = 0;

  /**
   * \return the current orientation
   */
  Angles GetOrientation (void) const;
  /**
   * \param orientation the orientation to set.
   */
  void SetOrientation (const Angles &orientation);

private:
  /**
   * \return the current orientation.
   *
   * Concrete subclasses of this base class must 
   * implement this method.
   */
  virtual Angles DoGetOrientation (void) const = 0;
  /**
   * \param orientation the orientation to set.
   *
   * Concrete subclasses of this base class must 
   * implement this method.
   */
  virtual void DoSetOrientation (const Angles &orientation) = 0;
};

} // namespace ns3

#endif /* ORIENTATION_MODEL_H */
