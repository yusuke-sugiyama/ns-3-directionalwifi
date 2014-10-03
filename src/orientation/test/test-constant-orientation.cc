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


NS_LOG_COMPONENT_DEFINE ("TestConstantOrientationModel");

namespace ns3 {

enum ConstantOrientationModelGainTestCondition  {
  EQUAL = 0,
  LESSTHAN = 1
};

class ConstantOrientationModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Angles a);
  ConstantOrientationModelTestCase (Angles a, Angles expectedOri, ConstantOrientationModelGainTestCondition cond);


private:
  virtual void DoRun (void);

  Angles m_a;
  Angles m_expectedOri;
  ConstantOrientationModelGainTestCondition m_cond;
};

std::string ConstantOrientationModelTestCase::BuildNameString (Angles a)
{
  std::ostringstream oss;
  oss <<  "theta=" << a.theta << " , phi=" << a.phi;
  return oss.str ();
}


ConstantOrientationModelTestCase::ConstantOrientationModelTestCase (Angles a, Angles expectedOri, ConstantOrientationModelGainTestCondition cond)
  : TestCase (BuildNameString (a)),
    m_a (a),
    m_expectedOri (expectedOri),
    m_cond (cond)
{
}

void
ConstantOrientationModelTestCase::DoRun ()
{
  NS_LOG_FUNCTION (this << BuildNameString (m_a));

  Ptr<OrientationModel> a = CreateObject<ConstantOrientationModel> ();
  a->SetAttribute ("Orientation", AnglesValue(m_a));


  Angles ori = a->GetOrientation();
  switch (m_cond) 
    {
    case EQUAL:
      NS_TEST_EXPECT_MSG_EQ_TOL (ori.phi, m_expectedOri.phi, 0.001, "wrong value of phi");
      NS_TEST_EXPECT_MSG_EQ_TOL (ori.theta, m_expectedOri.theta, 0.001, "wrong value of theta");
      break;
    default:
      break;
    }
}




class ConstantOrientationModelTestSuite : public TestSuite
{
public:
  ConstantOrientationModelTestSuite ();
};

ConstantOrientationModelTestSuite::ConstantOrientationModelTestSuite ()
  : TestSuite ("constant-orientation-model", UNIT)
{ 
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians   (0)), Angles( DegreesToRadians    (0), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians   (0)), Angles( DegreesToRadians   (15), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians   (30), DegreesToRadians   (0)), Angles( DegreesToRadians   (30), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (15)), Angles( DegreesToRadians    (0), DegreesToRadians  (15)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (30)), Angles( DegreesToRadians    (0), DegreesToRadians  (30)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians   (15), DegreesToRadians  (15)), Angles( DegreesToRadians   (15), DegreesToRadians  (15)), EQUAL));
  
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (179), DegreesToRadians   (0)), Angles( DegreesToRadians  (179), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (180), DegreesToRadians   (0)), Angles( DegreesToRadians  (180), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (181), DegreesToRadians   (0)), Angles( DegreesToRadians  (181), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (359), DegreesToRadians   (0)), Angles( DegreesToRadians  (359), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (360), DegreesToRadians   (0)), Angles( DegreesToRadians    (0), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians  (361), DegreesToRadians   (0)), Angles( DegreesToRadians    (1), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians   (-1), DegreesToRadians   (0)), Angles( DegreesToRadians  (359), DegreesToRadians   (0)), EQUAL));

  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (179)), Angles( DegreesToRadians    (0), DegreesToRadians (179)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (180)), Angles( DegreesToRadians    (0), DegreesToRadians (180)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians (181)), Angles( DegreesToRadians    (0), DegreesToRadians (179)), EQUAL));
  AddTestCase (new ConstantOrientationModelTestCase (Angles (DegreesToRadians    (0), DegreesToRadians  (-1)), Angles( DegreesToRadians    (0), DegreesToRadians   (1)), EQUAL));


};

static ConstantOrientationModelTestSuite staticConstantOrientationModelTestSuiteInstance;




} // namespace ns3
