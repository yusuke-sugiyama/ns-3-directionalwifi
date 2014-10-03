
#ifndef GEOGRAPHY_TAG_H
#define GEOGRAPHY_TAG_H

#include "ns3/packet.h"
#include "ns3/vector.h"

namespace ns3 {

class Tag;

class GeographyTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a position with the default position (0, 0, 0) 
   */
  GeographyTag();

  /**
   * Create a GeographyTag with the given position value
   * \param position the given position value
   */
  GeographyTag(const Vector &position);

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  /**
   * Set the node position to the given value.
   *
   * \param x the value of the node position
   */
  void Set (const Vector &position);
  /**
   * Return the node position.
   *
   * \return the node position.
   */
  double GetX (void) const;

  /**
   * Return the node position.
   *
   * \return the node position.
   */
  double GetY (void) const;
  /**
   * Return the node position.
   *
   * \return the node position.
   */
  double GetZ (void) const;
  /**
   * Return the node position.
   *
   * \return the node position.
   */
  Vector Get (void) const;
private:
  double m_x;
  double m_y;
  double m_z;
};


}
#endif /* SNR_TAG_H */
