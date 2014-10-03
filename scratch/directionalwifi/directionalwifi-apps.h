#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/application.h"

#include "ns3/stats-module.h"

using namespace ns3;

//----------------------------------------------------------------------
//------------------------------------------------------
class Sender: public Application {
public:
  static TypeId GetTypeId (void);
  Sender();
  virtual ~Sender();

protected:
  virtual void DoDispose (void);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void SendPacket ();

  uint32_t        m_pktSize;
  Ipv4Address     m_destAddr;
  uint32_t        m_destPort;
  Ptr<ConstantRandomVariable> m_interval;

  Ptr<Socket>     m_socket;
  EventId         m_sendEvent;

  TracedCallback<Ptr<const Packet> > m_txTrace;


  // end class Sender
};




//------------------------------------------------------
class Receiver: public Application {
public:
  static TypeId GetTypeId (void);
  Receiver();
  virtual ~Receiver();

  void SetCounter (Ptr<CounterCalculator<> > calc);
  void SetDelayTracker (Ptr<TimeMinMaxAvgTotalCalculator> delay);

protected:
  virtual void DoDispose (void);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void Receive (Ptr<Socket> socket);

  Ptr<Socket>     m_socket;

  uint32_t        m_port;
  uint32_t        m_count;
  uint32_t        m_numPkts;

  Ptr<CounterCalculator<> > m_calc;
  Ptr<TimeMinMaxAvgTotalCalculator> m_delay;
  TracedCallback<uint32_t, uint32_t > m_rxTrace;
  // end class Receiver
};




//------------------------------------------------------
class TimestampTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  // these are our accessors to our tag structure
  void SetTimestamp (Time time);
  Time GetTimestamp (void) const;

  void Print (std::ostream &os) const;

private:
  Time m_timestamp;

  // end class TimestampTag
};
