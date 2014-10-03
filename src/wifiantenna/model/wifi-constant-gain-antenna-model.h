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

#ifndef WIFI_CONSTANT_GAIN_ANTENNA_MODEL_H
#define WIFI_CONSTANT_GAIN_ANTENNA_MODEL_H

#include <ns3/object.h>
#include <ns3/wifi-antenna-model.h>

namespace ns3 {

/**
 * Constant gain antenna model
 * 
 * An extremely simple proof of concept antenna model, 
 * provide an orientation and azi/elv beamwidths and if
 * the Angles provided to GetGainDb fall within that
 * simple radiation pattern, return the inside pattern gain otherwise 
 * return the outside pattern gain
 */
class WifiConstantGainAntennaModel : public WifiAntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // sets the antenna gain in Db
  void SetGainInsidePattern (double gain);
  double GetGainInsidePattern (void) const;

  void SetGainOutsidePattern (double gain);
  double GetGainOutsidePattern (void) const;

  // in radians
  void SetAzimuthBeamwidth (double bw);
  double GetAzimuthBeamwidth (void) const;
  
  // in radians
  void SetElevationBeamwidth (double bw);
  double GetElevationBeamwidth (void) const;

private:
  double m_innerGain;
  double m_outerGain;
  double m_aziBW;
  double m_elvBW;
 
  //Angles m_orientation;
  virtual double DoGetGainDb (Angles a);
};

}

#endif // WIFI_CONSTANT_GAIN_ANTENNA_MODEL
