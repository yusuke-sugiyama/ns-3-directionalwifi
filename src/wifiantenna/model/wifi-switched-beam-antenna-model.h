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


#ifndef WIFI_SWITCHED_BEAM_ANTENNA_MODEL_H
#define WIFI_SWITCHED_BEAM_ANTENNA_MODEL_H

#include <ns3/object.h>
#include <ns3/wifi-antenna-model.h>

namespace ns3 {


class WifiSwitchedBeamAntennaModel : public WifiAntennaModel
{
public:
  enum ANTENNAMODE{
    OMNI           = 0,
    DIRECTIONAL0   = 1,
    DIRECTIONAL90  = 2,
    DIRECTIONAL180 = 3,
    DIRECTIONAL270 = 4
  };
  // inherited from Object
  static TypeId GetTypeId ();

  // sets the antenna gain in Db
  void SetGainInsidePattern (double gain);
  double GetGainInsidePattern (void) const;

  void SetGainOutsidePattern (double gain);
  double GetGainOutsidePattern (void) const;

  void SetGainOmniMode (double gain);
  double GetGainOmniMode (void) const;

  // in radians
  void SetAzimuthBeamwidth (double bw);
  double GetAzimuthBeamwidth (void) const;
  
  // in radians
  void SetElevationBeamwidth (double bw);
  double GetElevationBeamwidth (void) const;

  int GetNextAntennaMode (Angles bet);
  void SetAntennaMode (int mode);
  void SetAntennaMode (Angles bet);

private:
  double m_innerGain;
  double m_outerGain;
  double m_omniGain;
  double m_aziBW;
  double m_elvBW;
 
  //Angles m_orientation;
  virtual double DoGetGainDb (Angles a);
};

}

#endif // WIFI_SWITCHED_BEAM_ANTENNA_MODEL_H
