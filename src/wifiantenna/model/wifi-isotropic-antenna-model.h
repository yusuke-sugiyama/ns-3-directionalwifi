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

#ifndef WIFI_ISOTROPIC_ANTENNA_MODEL_H
#define WIFI_ISOTROPIC_ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/wifi-antenna-model.h>

namespace ns3 {

/**
 * \brief Isotropic antenna model
 * 
 * This is the simplest antenna model. The gain of this antenna is unitary (=0dB) in all directions.
 */
class WifiIsotropicAntennaModel : public WifiAntennaModel
{
public:

  WifiIsotropicAntennaModel ();
  // inherited from Object
  static TypeId GetTypeId ();

private:

  // inherited from WifiAntennaModel
  virtual double DoGetGainDb (Angles a);

};



} // namespace ns3


#endif // WIFIISOTROPIC_ANTENNA_MODEL_H
