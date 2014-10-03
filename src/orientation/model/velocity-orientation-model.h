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
#ifndef VELOCITY_ORIENTATION_MODEL_H
#define VELOCITY_ORIENTATION_MODEL_H

#include "orientation-model.h"

namespace ns3 {

/**
 * \ingroup orientation
 * \brief Orientation model for which the current orientation does not 
 * change once it has been set and until it is set again explicitly to a 
 * new value.
 */
class VelocityOrientationModel : public OrientationModel
{
public:
  static TypeId GetTypeId (void);
  VelocityOrientationModel ();
  virtual ~VelocityOrientationModel ();

private:
  /**
   * \return the current orientation.
   */
  virtual Angles DoGetOrientation (void) const;
  /**
   * \param orientation the orientation to set.
   */
  virtual void DoSetOrientation (const Angles &orientation);

  Angles m_orientation;
};

} // namespace ns3

#endif /* VELOCITY_ORIENTATION_MODEL_H */
