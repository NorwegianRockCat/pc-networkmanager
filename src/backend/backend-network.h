//===========================================
//  PC-BSD source code
//  Copyright (c) 2015, PC-BSD Software/iXsystems
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef __PCBSD_LIB_UTILS_NETWORK_H
#define __PCBSD_LIB_UTILS_NETWORK_H

//#include "sysadm-global.h"
#include <QString>
#include <QStringList>
#include <QDebug>

//FreeBSD Includes
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_media.h>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h> //from "man getnetent" (network entries)

namespace backend{

//General purpose enumeration of network encryption types
enum NetCrypt {UNKNOWN, NONE, WEP, WPA1, WPA2, WPAE};

//Data structure for network entries ("/etc/networks" entries for example)
struct NetworkEntry{
  QString name;
  QStringList aliases;
  unsigned int netnum;
};

//Data structure for rc.conf settings
struct NetDevSettings{
	QString device; //the device these settings correspond with (usually unset if there was an error)
	QString asDevice; //associated device (Example: wifi device -> wlan<num> device)
	//Wifi settings
	QString wifiCountry, wifiSSID, wifiBSSID, wifiChannel;
	bool wifihost;
	bool wifisecurity; //always recommended for wifi settings - otherwise can't connect to secure access points
	//Addressing
	bool useDHCP;
	QString staticIPv4, staticNetmask, staticIPv6; //assumes DHCP if none are set
	QString staticGateway;
};

//General data class for network devices
// Note: Sources in NetDevice.cpp
class NetDevice{
private: 
	QString name;
public:
	NetDevice(QString devName){ name = devName; }
	QString device(){ return name; } //full device name (wlan0)
	QString devName(); 	//name only (wlan)
	uint devNum(); 		//number only (0)
	QString ipAsString();
	QString ipv6AsString();
	QString netmaskAsString();
	QString desc();
	QString macAsString();
	//QString mediaTypeAsString();
	QString mediaStatusAsString();
	QString gatewayAsString();
	bool isWireless();
	QString getWifiParent();
	bool usesDHCP();
	bool isUp();
	long packetsRx();
	long packetsTx();
	long errorsRx();
	long errorsTx();
	
	//Setting Functions (to make changes - requires root)
	void setUp(bool up); //Turn device on/off (temporary - not saved globally)

	//Generic listing of devices
	static QStringList listNetDevices(); //probe the system for all network devices
};

//General data structure for wifi access points (local or available)
struct NetWifi{
	NetCrypt encryption;
	QString BSSID, SSID;
};

	
//The general-purpose class that any user/app can utilitize
class Network{
public:
	//static QList<NetworkEntry> listNetworkEntries();
	static QStringList readRcConf(); //use this when reading /etc/rc.conf for network stuff - prevents opening the file repeatedly
	static NetDevSettings deviceRCSettings(QString dev); //settings in rc.conf (bootup)
	//static NetDevSettings deviceIfconfigSettings(QString dev); //settings currently running
};

//The class that requires overarching root permissions (usually for changes to system)
class NetworkRoot{
public:
	//static bool saveNetworkEntry(NetworkEntry); //**Not implemented yet**
	static bool saveRCSettings(NetDevSettings);	//rc.conf settings (bootup)
	static bool setIfconfigSettings(NetDevSettings); 	//ifconfig settings (temporary session)
};
	
} //end of namespace

#endif
