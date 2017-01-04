#pragma once
#include "afxwin.h"


class CZigbeeMessage
{
public:
	CZigbeeMessage();
	CZigbeeMessage(BYTE *data, int length);

	int CommandCode;
	int PackageLength;
	int InfoType;
	int NodeAddress;
	int ParentAddress;
	int Rssi;
	BYTE ExtendData[100];
	int ExtendDataLength;
};