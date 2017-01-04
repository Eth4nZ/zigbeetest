#include "stdafx.h"
#include "ZigbeeMessage.h"
#include "Util.h"

CZigbeeMessage::CZigbeeMessage()
{
	
}

CZigbeeMessage::CZigbeeMessage(BYTE * data, int length)
{
	CommandCode = data[1] + 256*data[2];
//	TRACE("Command Code = %d\n", CommandCode);
	
	PackageLength = data[3] + 256*data[4];
//	TRACE("PackageLength = %d\n", PackageLength);

	InfoType = (int)data[5];
//	TRACE("InfoType = %d\n", InfoType);

	NodeAddress = data[6] + 256*data[7];
//	TRACE("NodeAddress = %x\n", NodeAddress);

	switch(InfoType)
	{
		case 0x05:
		{
			ParentAddress = data[8] + 256*data[9];
//			TRACE("ParentAddress = %x\n", ParentAddress);
			break;
		}
		case 0x06:
		{
			ExtendDataLength = PackageLength - 4;
			memcpy(ExtendData, &data[8], ExtendDataLength);
			CString str = Hex2String(ExtendData,ExtendDataLength);
//			TRACE(str);
//			TRACE("\n");
			break;
		}
		default:
		{
			break;
		}
	}
}
