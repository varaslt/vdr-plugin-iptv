/*
 * iptv.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: iptv.c,v 1.4 2007/09/15 23:58:23 rahrenbe Exp $
 */

#include <getopt.h>
#include <vdr/plugin.h>
#include "common.h"
#include "config.h"
#include "setup.h"
#include "device.h"

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = trNOOP("Experiment the IPTV");

class cPluginIptv : public cPlugin {
private:
  // Add any member variables or functions you may need here.
  unsigned int deviceCount;
public:
  cPluginIptv(void);
  virtual ~cPluginIptv();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  virtual const char *MainMenuEntry(void) { return NULL; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPluginIptv::cPluginIptv(void)
: deviceCount(1)
{
  //debug("cPluginIptv::cPluginIptv()\n");
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginIptv::~cPluginIptv()
{
  //debug("cPluginIptv::~cPluginIptv()\n");
  // Clean up after yourself!
}

const char *cPluginIptv::CommandLineHelp(void)
{
  debug("cPluginIptv::CommandLineHelp()\n");
  // Return a string that describes all known command line options.
  return cString::sprintf(
         "  -d <number> --devices=<number> number of devices to be created (default: %d)\n",
         deviceCount);
}

bool cPluginIptv::ProcessArgs(int argc, char *argv[])
{
  debug("cPluginIptv::ProcessArgs()\n");
  // Implement command line argument processing here if applicable.
  static const struct option long_options[] = {
       { "devices", required_argument, NULL, 'd' },
       { NULL }
     };

  int c;
  while ((c = getopt_long(argc, argv, "d:", long_options, NULL)) != -1) {
        switch (c) {
          case 'd':
               deviceCount = atoi(optarg);
               break;
          default:
               return false;
          }
        }
  return true;
}

bool cPluginIptv::Initialize(void)
{
  debug("cPluginIptv::Initialize()\n");
  // Initialize any background activities the plugin shall perform.
  return cIptvDevice::Initialize(deviceCount);
}

bool cPluginIptv::Start(void)
{
  debug("cPluginIptv::Start()\n");
  // Start any background activities the plugin shall perform.
  return true;
}

void cPluginIptv::Stop(void)
{
  debug("cPluginIptv::Stop()\n");
  // Stop any background activities the plugin is performing.
}

void cPluginIptv::Housekeeping(void)
{
  //debug("cPluginIptv::Housekeeping()\n");
  // Perform any cleanup or other regular tasks.
}

void cPluginIptv::MainThreadHook(void)
{
  //debug("cPluginIptv::MainThreadHook()\n");
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginIptv::Active(void)
{
  //debug("cPluginIptv::Active()\n");
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPluginIptv::WakeupTime(void)
{
  //debug("cPluginIptv::WakeupTime()\n");
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPluginIptv::MainMenuAction(void)
{
  //debug("cPluginIptv::MainMenuAction()\n");
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPluginIptv::SetupMenu(void)
{
  debug("cPluginIptv::SetupMenu()\n");
  // Return a setup menu in case the plugin supports one.
  return new cIptvPluginSetup();
}

bool cPluginIptv::SetupParse(const char *Name, const char *Value)
{
  debug("cPluginIptv::SetupParse()\n");
  // Parse your own setup parameters and store their values.
  if (!strcasecmp(Name, "BufferSize"))
     IptvConfig.SetBufferSizeMB(atoi(Value));
  else if (!strcasecmp(Name, "BufferPrefill"))
     IptvConfig.SetBufferPrefillRatio(atoi(Value));
  else
     return false;
  return true;
}

bool cPluginIptv::Service(const char *Id, void *Data)
{
  //debug("cPluginIptv::Service()\n");
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginIptv::SVDRPHelpPages(void)
{
  //debug("cPluginIptv::SVDRPHelpPages()\n");
  // Return help text for SVDRP commands this plugin implements
  return NULL;
}

cString cPluginIptv::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  //debug("cPluginIptv::SVDRPCommand()\n");
  // Process SVDRP commands this plugin implements
  return NULL;
}

VDRPLUGINCREATOR(cPluginIptv); // Don't touch this!