/*
 * sidscanner.h: IPTV plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __SIDSCANNER_H
#define __SIDSCANNER_H

#include <vdr/channels.h>
#include <vdr/filter.h>

class cSidScanner : public cFilter {
private:
  tChannelID channelIdM;
  bool sidFoundM;
  bool nidFoundM;
  bool tidFoundM;
  bool isActiveM;

protected:
  virtual void Process(u_short pidP, u_char tidP, const u_char *dataP, int lengthP);

public:
  cSidScanner(void);
  ~cSidScanner();
  void SetChannel(const tChannelID &channelIdP);
  void Open() { isActiveM = true; }
  void Close() { isActiveM = false; }
};

#endif // __SIDSCANNER_H
