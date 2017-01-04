#pragma once
#include "ZigbeeMessage.h"
#include "afxwin.h"


class CZigbeeSensor
{
public :
	CZigbeeSensor();
	CZigbeeSensor(CZigbeeMessage msg);

	int NodeAddress;

	CString type;
	CString value;
};