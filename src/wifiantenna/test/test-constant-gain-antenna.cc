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

#include <ns3/log.h>
#include <ns3/test.h>
#include <ns3/double.h>
#include <ns3/constant-gain-antenna-model.h>
#include <ns3/orientation-module.h>
#include <ns3/mobility-module.h>
#include <ns3/simulator.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>


NS_LOG_COMPONENT_DEFINE ("TestConstantGainAntennaModel");

namespace ns3 {

enum ConstantGainAntennaModelGainTestCondition  {
  EQUAL = 0,
  LESSTHAN = 1
};

class ConstantGainAntennaModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Angles a, double ab, double eb, Angles o, double ig, double og);
  ConstantGainAntennaModelTestCase (Angles a, double ab, double eb, Angles o, double ig, double og, double expectedGainDb, ConstantGainAntennaModelGainTestCondition cond);


private:
  virtual void DoRun (void);

  Angles m_a;
  double m_ab;
  double m_eb;
  Angles m_o;
  double m_ig;
  double m_og;
  double m_expectedGain;
  ConstantGainAntennaModelGainTestCondition m_cond;
};

std::string ConstantGainAntennaModelTestCase::BuildNameString (Angles a, double ab, double eb, Angles o, double ig, double og)
{
  std::ostringstream oss;
  oss <<  "theta=" << a.theta << " , phi=" << a.phi 
      << ", azi beamdwidth=" << ab << " rad"
      << ", elv beamdwidth=" << eb << " rad"
      << ", ori theta=" << o.theta 
      << ", ori phi=" << o.phi 
      << ", insideGain=" << ig << " dB"
      << ", outsideGain=" << og << " dB";
  return oss.str ();
}


ConstantGainAntennaModelTestCase::ConstantGainAntennaModelTestCase (Angles a, double ab, double eb, Angles o, double ig, double og, double expectedGainDb, ConstantGainAntennaModelGainTestCondition cond)
  : TestCase (BuildNameString (a, ab, eb, o, ig, og)),
    m_a (a),
    m_ab (ab),
    m_eb (eb),
    m_o (o),
    m_ig (ig),
    m_og (og),
    m_expectedGain (expectedGainDb),
    m_cond (cond)
{
}

void
ConstantGainAntennaModelTestCase::DoRun ()
{
  NS_LOG_FUNCTION (this << BuildNameString (m_a, m_ab, m_eb, m_o, m_ig, m_og));

  Ptr<ConstantGainAntennaModel> a = CreateObject<ConstantGainAntennaModel> ();
  a->SetAttribute ("InsideGain", DoubleValue (m_ig));
  a->SetAttribute ("OutsideGain", DoubleValue (m_og));
  a->SetAttribute ("AzimuthBeamwidth", DoubleValue (m_ab));
  a->SetAttribute ("ElevationBeamwidth", DoubleValue (m_eb));

  Ptr<OrientationModel> ori = CreateObject<ConstantOrientationModel> ();
  ori->SetAttribute ("Orientation", AnglesValue(m_o));
  a->AggregateObject (ori);

  Vector v = CalculateNewPoint (Vector (0, 0, 0), m_a, 1);
  Ptr<ConstantPositionMobilityModel> cm1 = CreateObject<ConstantPositionMobilityModel> ();
  cm1->SetAttribute ("Position", VectorValue(Vector (0, 0, 0)));
  Ptr<ConstantPositionMobilityModel> cm2 = CreateObject<ConstantPositionMobilityModel> ();
  cm2->SetAttribute ("Position", VectorValue(v));

  double actualGain = a->GetGainDb (cm1, cm2);
  switch (m_cond) 
    {
    case EQUAL:
      NS_TEST_EXPECT_MSG_EQ_TOL (actualGain, m_expectedGain, 0.001, "wrong value of the radiation pattern");
      break;
    case LESSTHAN:
      NS_TEST_EXPECT_MSG_LT (actualGain, m_expectedGain, "gain higher than expected");
      break;
    default:
      break;
    }
}




class ConstantGainAntennaModelTestSuite : public TestSuite
{
public:
  ConstantGainAntennaModelTestSuite ();
};

ConstantGainAntennaModelTestSuite::ConstantGainAntennaModelTestSuite ()
  : TestSuite ("constant-gain-antenna-model", UNIT)
{ 
  // to calculate the azimut angle offset for a given gain in db:
  // phideg = (2*acos(10^(targetgaindb/(20*n))))*180/pi
  // e.g., with a 60 deg beamwidth, gain is -20dB at +- 74.945 degrees from boresight

  //                                                                             phi,                  theta,                azi bw,                elv bw,   orientation, innerGain, outerGain,  expectedGain,   condition
  // Test Elevation Beamwidth
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians   (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (90)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (180)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (-90)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (-35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (-30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (-15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
                  
  // Test Azimuth Beamwidth (Pointing directly at the other)
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (45), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-45), DegreesToRadians  (0)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));

  // Test Azimuth Beamwidth
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (45), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-45), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (90), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-90), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (180), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians (-180), DegreesToRadians (15)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (45), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-45), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (90), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-90), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (180), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians (-180), DegreesToRadians (30)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (45), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-45), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (90), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-90), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (180), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians (-180), DegreesToRadians (35)), DegreesToRadians (60), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));

  // Test different beamwidths
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (15)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));

  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             3,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (30)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));

  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-15), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-30), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians   (35), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));
  AddTestCase (new ConstantGainAntennaModelTestCase (Angles (DegreesToRadians  (-35), DegreesToRadians (35)), DegreesToRadians (30), DegreesToRadians (60), Angles (0, 0),         3,         0,             0,     EQUAL));

};

static ConstantGainAntennaModelTestSuite staticConstantGainAntennaModelTestSuiteInstance;




} // namespace ns3
