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
#include <ns3/orientation-module.h>
#include <ns3/mobility-module.h>
#include <ns3/simulator.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>


NS_LOG_COMPONENT_DEFINE ("TestVelocityOrientationModel");

namespace ns3 {

enum VelocityOrientationModelTestCondition  {
  EQUAL = 0,
  LESSTHAN = 1
};

class VelocityOrientationModelTestCase : public TestCase
{
public:
  static std::string BuildNameString (Vector v);
  VelocityOrientationModelTestCase (Vector v, Angles expectedOri, VelocityOrientationModelTestCondition cond);


private:
  virtual void DoRun (void);

  Vector m_v;
  Angles m_expectedOri;
  VelocityOrientationModelTestCondition m_cond;
};

std::string VelocityOrientationModelTestCase::BuildNameString (Vector v)
{
  std::ostringstream oss;
  oss << "velocity=" << v;
  return oss.str ();
}


VelocityOrientationModelTestCase::VelocityOrientationModelTestCase (Vector v, Angles expectedOri, VelocityOrientationModelTestCondition cond)
  : TestCase (BuildNameString (v)),
    m_v (v),
    m_expectedOri (expectedOri),
    m_cond (cond)
{
}

void
VelocityOrientationModelTestCase::DoRun ()
{
  NS_LOG_FUNCTION (this << BuildNameString (m_v));

  Ptr<OrientationModel> a = CreateObject<VelocityOrientationModel> ();
  
  Ptr<ConstantVelocityMobilityModel> m = CreateObject<ConstantVelocityMobilityModel> ();
  m->SetVelocity (m_v);
  m->AggregateObject (a);

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




class VelocityOrientationModelTestSuite : public TestSuite
{
public:
  VelocityOrientationModelTestSuite ();
};

VelocityOrientationModelTestSuite::VelocityOrientationModelTestSuite ()
  : TestSuite ("velocity-orientation-model", UNIT)
{ 
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    0,    0,    0), Angles( DegreesToRadians    (0), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    1,    0,    0), Angles( DegreesToRadians    (0), DegreesToRadians  (90)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    0,    1,    0), Angles( DegreesToRadians   (90), DegreesToRadians  (90)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    0,    0,    1), Angles( DegreesToRadians    (0), DegreesToRadians   (0)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (   -1,    0,    0), Angles( DegreesToRadians  (180), DegreesToRadians  (90)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    0,   -1,    0), Angles( DegreesToRadians  (270), DegreesToRadians  (90)), EQUAL));
  AddTestCase (new VelocityOrientationModelTestCase (Vector (    0,    0,   -1), Angles( DegreesToRadians    (0), DegreesToRadians (180)), EQUAL));

};

static VelocityOrientationModelTestSuite staticVelocityOrientationModelTestSuiteInstance;




} // namespace ns3
