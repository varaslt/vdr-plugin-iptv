/*
 * common.h: IPTV plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: common.h,v 1.2 2007/09/29 16:23:40 rahrenbe Exp $
 */

#ifndef __IPTV_COMMON_H
#define __IPTV_COMMON_H

#include <vdr/tools.h>

#ifdef DEBUG
#define debug(x...) dsyslog("IPTV: " x);
#define error(x...) esyslog("IPTV: " x);
#else
#define debug(x...) ;
#define error(x...) esyslog("IPTV: " x);
#endif

#endif // __IPTV_COMMON_H

