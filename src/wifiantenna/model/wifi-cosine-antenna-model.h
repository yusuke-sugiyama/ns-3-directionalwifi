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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef WIFI_COSINE_ANTENNA_MODEL_H
#define WIFI_COSINE_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/wifi-antenna-model.h>

namespace ns3 {

/**
 * 
 * \brief Cosine Antenna Model 
 *
 * This class implements the cosine model as described in:
 *
 * Li Chunjian, "Efficient Antenna Patterns for Three-Sector WCDMA Systems"
 *
 * Note that only the element factor of the above model is
 * considered. Also, an additional constant gain is added to model the
 * radiation pattern on the vertical plane (to account for the fact
 * that the elevation angle is not included in the model).
 */
class WifiCosineAntennaModel : public WifiAntennaModel
{
public:

  // inherited from Object
  static TypeId GetTypeId ();

  // attribute getters/setters
  void SetBeamwidth (double beamwidthDegrees);
  double GetBeamwidth () const;

private:

  /**
   * this is the variable "n" in the paper by Chunjian
   * 
   */
  double m_exponent; 

  double m_beamwidthRadians;

  double m_maxGain;
  
  // inherited from AntennaModel
  virtual double DoGetGainDb (Angles a);
};



} // namespace ns3


#endif // WIFI_COSINE_ANTENNA_MODEL_H
