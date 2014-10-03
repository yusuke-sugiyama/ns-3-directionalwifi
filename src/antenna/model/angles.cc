/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 CTTC
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


#include <ns3/log.h>
#include <cmath>
#include "angles.h"


NS_LOG_COMPONENT_DEFINE ("Angles");

namespace ns3 {

ATTRIBUTE_HELPER_CPP (Angles);

double DegreesToRadians (double degrees)
{
  return degrees * M_PI / 180.0;

}

double RadiansToDegrees (double radians)
{
  return radians * 180.0 / M_PI;
}

double NormalizeOverTwoPI (double a){
  double twoPI = 2 * M_PI;
  while(a < 0) a += twoPI;
  while(a > twoPI) a -= twoPI;
  return a;
}

double NormalizeOverPI (double a){
  double twoPI = 2 * M_PI;
  while(a < 0) a += twoPI;
  while(a > twoPI) a -= twoPI;
  if(a > M_PI) a = twoPI - a; 
  return a;
}

Vector CalculateNewPoint (Vector cur, Angles ang, double dist){
    double x = cur.x + dist * cos(ang.phi) * cos(ang.theta);
    double y = cur.y + dist * cos(ang.phi) * cos(ang.theta);
    double z = cur.z + dist * cos(ang.phi) * cos(ang.theta);
    return Vector (x, y, z);
}

std::ostream& operator<< (std::ostream& os, const Angles& a)
{
  os << "(" << a.phi << ", " << a.theta << ")";
  return os;
}

std::istream &operator >> (std::istream &is, Angles &a)
{
  char c;
  is >> a.phi >> c >> a.theta;
  if (c != ':')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}


Angles::Angles ()
  : phi (0),
    theta (0)
{
}


Angles::Angles (double p, double t)
  : phi (NormalizeOverTwoPI(p)),
    theta (NormalizeOverTwoPI(t))
{
}


Angles::Angles (Vector v)
  : phi (NormalizeOverTwoPI(std::atan2 (v.y, v.x))),
    theta (NormalizeOverTwoPI(std::acos (v.z / sqrt (v.x*v.x + v.y*v.y + v.z*v.z))))
{
}

Angles::Angles (Vector v, Vector o)
  : phi (NormalizeOverTwoPI(std::atan2 (v.y - o.y, v.x - o.x))),
    theta (NormalizeOverTwoPI(std::acos ((v.z - o.z) / CalculateDistance (v, o))))
{
}

Angles
Angles::Inverse (void)
{
  return Angles(NormalizeOverTwoPI(phi + M_PI), NormalizeOverPI(theta + M_PI));
}


}

