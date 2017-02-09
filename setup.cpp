#include <iostream>
#include <ftd2xx.h>
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
#include "setup.h"

using namespace std;

bool boolDebug = false;
string *device;
string strDebug = "UI Setup: ";
QString	cmdPlatform = "./xc3sprog/platform.bash",
		cmdJTAG		= "./xc3sprog/jtag-flash.bash",
        cmdUSB		= "./xc3sprog/ftdi-flash.bash",
        cmdModule	= "sudo rmmod ftdi_sio";

UIsetup::UIsetup()
{
	
	procSetup = new QProcess(this);
	
	setupUI(this);

	deviceScan();
	
	connect(btnScan,		SIGNAL(released()), this, SLOT(scan()));
	connect(btnModule,		SIGNAL(released()), this, SLOT(module()));
	connect(btnPlatform,	SIGNAL(released()), this, SLOT(programPlatform()));
	connect(btnJTAG,		SIGNAL(released()), this, SLOT(programJTAG()));
	connect(btnUSB,			SIGNAL(released()), this, SLOT(programUSB()));
	connect(btnClose, 		SIGNAL(released()), this, SLOT(close()));

	connect(procSetup, SIGNAL(readyReadStandardError()),  this, SLOT(procStdErr()));
	connect(procSetup, SIGNAL(readyReadStandardOutput()), this, SLOT(procStdOut()));

	connect(comboDevice, SIGNAL(currentIndexChanged(QString)), this, SLOT(deviceChanged(QString)));

	//connect(grpFlags, SIGNAL(buttonReleased(int)), this, SLOT(flagsChanged(int)));
	
}

UIsetup::~UIsetup()
{
	if (boolDebug) cout << strDebug << "Destroyed UIsetup" << endl;
}

void UIsetup::Device(string *dev)
{
	device = dev;
	if (comboDevice->count() == 0)
	{
		comboDevice->clear();
    	comboDevice->insertItems(0, QStringList() << QString::fromStdString(*dev));
    }
}

void UIsetup::Debug(bool debug)
{
	if (boolDebug) cout << strDebug << ((debug) ? "Enabled Debug" : "Disabled Debug") << endl;
	
	boolDebug = debug;
}

void UIsetup::scan()
{
	if (boolDebug) cout << strDebug << "Scanning for FTDI devices" << endl;

	txtConsole->clear();

	deviceScan();

}

void UIsetup::module()
{
	if (boolDebug) cout << strDebug << "Unloading FTDI Module" << endl;
	
	txtConsole->clear();
	
	procSetup->start(cmdModule);
}

void UIsetup::programPlatform()
{
	if (boolDebug) cout << strDebug << "Loading Hex File to Platform Cable" << endl;
	
	txtConsole->clear();
	
	procSetup->start(cmdPlatform);
}

void UIsetup::programJTAG()
{
	if (boolDebug) cout << strDebug << "Programming via JTAG Interface" << endl;
	
	txtConsole->clear();
	
	procSetup->start(cmdJTAG);
}

void UIsetup::programUSB()
{
	if (boolDebug) cout << strDebug << "Programming via USB Interface" << endl;
	
	txtConsole->clear();
	
	procSetup->start(cmdUSB);
}

void UIsetup::procStdErr()
{
	if (boolDebug) cout << strDebug << "Printing Std Error" << endl;

	// for some reason, xc3sprog is outputting with std err so it looks scary even when it succeeds

	QByteArray buffer = procSetup->readAllStandardError();
	//txtConsole->setTextColor(QColor(255, 0, 0));
	txtConsole->append(buffer);
}

void UIsetup::procStdOut()
{
	if (boolDebug) cout << strDebug << "Printing Std Out" << endl;

	QByteArray buffer = procSetup->readAllStandardOutput();
	//txtConsole->setTextColor(QColor(0, 0, 0));
	txtConsole->append(buffer);
}

void UIsetup::deviceScan()
{
	FT_STATUS ftStatus;
	FT_DEVICE_LIST_INFO_NODE *devInfo;
	DWORD numDevs;

	QStringList strDevices;

	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus != FT_OK || (numDevs < 1)) {
		printf("Error getting device info list\n");
		//return EXIT_FAILURE;
	}	

	cout << strDebug << numDevs << " FTDI devices found" << endl;

	devInfo = (FT_DEVICE_LIST_INFO_NODE*) malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
	ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
	
	if (ftStatus == FT_OK) {
		
		unsigned int j = 0;
		
		for (j = 0; j < numDevs; j++) {
			
			txtConsole->append(
					QString::fromStdString(
						boost::lexical_cast<string>(
							boost::format("Device %d:\n\tFlags:\t0x%x\n\tType:\t0x%x\n\tID:\t0x%x\n\tLocID:\t0x%x\n\tSerial:\t%s\n\tDesc:\t%s\n") 
							              % j % devInfo[j].Flags % devInfo[j].Type % devInfo[j].ID % devInfo[j].LocId % devInfo[j].SerialNumber % devInfo[j].Description)
						)
					);

			cout << boost::format("Device %d:\tFlags=0x%x\tType=0x%x\tID=0x%x\tLocID=0x%x\n \
					\r\t\tSerial=%s\tDesc=%s\n") % 
					j % devInfo[j].Flags % devInfo[j].Type % devInfo[j].ID % devInfo[j].LocId % 
					devInfo[j].SerialNumber % devInfo[j].Description << endl;
			strDevices << devInfo[j].SerialNumber;
		
		}
	}

	comboDevice->clear();
	comboDevice->addItems(strDevices);
	
}

void UIsetup::deviceChanged(QString dev)
{
	if (boolDebug) cout << strDebug << "Changing device to " << dev.toStdString() << endl;

	if (dev.startsWith("NUMATO4"))
		
		btnUSB->setEnabled(true);

	else

		btnUSB->setEnabled(false);

	*device = dev.toStdString();
}
