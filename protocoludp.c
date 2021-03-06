/*
 * protocoludp.c: IPTV plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

#include <vdr/device.h>

#include "common.h"
#include "config.h"
#include "protocoludp.h"
#include "socket.h"

cIptvProtocolUdp::cIptvProtocolUdp()
: isIGMPv3M(false),
  sourceAddrM(strdup("")),
  streamAddrM(strdup("")),
  streamPortM(0)
{
  debug("cIptvProtocolUdp::%s()", __FUNCTION__);
}

cIptvProtocolUdp::~cIptvProtocolUdp()
{
  debug("cIptvProtocolUdp::%s()", __FUNCTION__);
  // Drop the multicast group and close the socket
  cIptvProtocolUdp::Close();
  // Free allocated memory
  free(streamAddrM);
  free(sourceAddrM);
}

bool cIptvProtocolUdp::Open(void)
{
  debug("cIptvProtocolUdp::%s(%s)", __FUNCTION__, streamAddrM);
  OpenSocket(streamPortM, streamAddrM, sourceAddrM, isIGMPv3M);
  if (!isempty(streamAddrM)) {
     // Join a new multicast group
     JoinMulticast();
     }
  return true;
}

bool cIptvProtocolUdp::Close(void)
{
  debug("cIptvProtocolUdp::%s(%s)", __FUNCTION__, streamAddrM);
  if (!isempty(streamAddrM)) {
     // Drop the multicast group
     OpenSocket(streamPortM, streamAddrM, sourceAddrM, isIGMPv3M);
     DropMulticast();
     }
  // Close the socket
  CloseSocket();
  // Do NOT reset stream and source addresses
  //sourceAddrM = strcpyrealloc(sourceAddrM, "");
  //streamAddrM = strcpyrealloc(streamAddrM, "");
  //streamPortM = 0;
  return true;
}

int cIptvProtocolUdp::Read(unsigned char* bufferAddrP, unsigned int bufferLenP)
{
  return cIptvUdpSocket::Read(bufferAddrP, bufferLenP);
}

bool cIptvProtocolUdp::SetSource(const char* locationP, const int parameterP, const int indexP)
{
  debug("cIptvProtocolUdp::%s(%s, %d, %d)", __FUNCTION__, locationP, parameterP, indexP);
  if (!isempty(locationP)) {
     // Drop the multicast group
     if (!isempty(streamAddrM)) {
        OpenSocket(streamPortM, streamAddrM, sourceAddrM, isIGMPv3M);
        DropMulticast();
        }
     // Update stream address and port
     streamAddrM = strcpyrealloc(streamAddrM, locationP);
     // <group address> or <source address>@<group address>
     char *p = strstr(streamAddrM, "@");
     if (p) {
        *p = 0;
        sourceAddrM = strcpyrealloc(sourceAddrM, streamAddrM);
        streamAddrM = strcpyrealloc(streamAddrM, p + 1);
        isIGMPv3M = true;
        }
     else {
        sourceAddrM = strcpyrealloc(sourceAddrM, streamAddrM);
        isIGMPv3M = false;
        }
     streamPortM = parameterP;
     // Join a new multicast group
     if (!isempty(streamAddrM)) {
        OpenSocket(streamPortM, streamAddrM, sourceAddrM, isIGMPv3M);
        JoinMulticast();
        }
     }
  return true;
}

bool cIptvProtocolUdp::SetPid(int pidP, int typeP, bool onP)
{
  //debug("cIptvProtocolUdp::%s(%d, %d, %d)", __FUNCTION__, pidP, typeP, onP);
  return true;
}

cString cIptvProtocolUdp::GetInformation(void)
{
  //debug("cIptvProtocolUdp::%s()", __FUNCTION__);
  if (isIGMPv3M)
     return cString::sprintf("udp://%s@%s:%d", sourceAddrM, streamAddrM, streamPortM);
  return cString::sprintf("udp://%s:%d", streamAddrM, streamPortM);
}
