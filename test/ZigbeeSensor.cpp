#include "stdafx.h"
#include "ZigbeeSensor.h"

CZigbeeSensor::CZigbeeSensor()
{
	NodeAddress = 0;
	type = "";
	value = "";

}


CZigbeeSensor::CZigbeeSensor(CZigbeeMessage msg)
{
	NodeAddress = msg.NodeAddress;

	int id = msg.ExtendData[1];


	switch(id)
	{
		case 0x01:
		{
			type = "温湿度传感器";
			float temperature = ((float)(msg.ExtendData[2] + 256 * msg.ExtendData[3]))/100;
			CString temperature_str;
			temperature_str.Format(_T("%.2f"), temperature);
			value += _T("温度:"); 
			value += temperature_str;
			
			value += _T(" ");

			float humidity = ((float)(msg.ExtendData[5] + 256 * msg.ExtendData[6]))/100;
			CString humidity_str;
			humidity_str.Format(_T("%.2f"), humidity);
			value += _T("湿度:"); 
			value += humidity_str;
			value +=_T(" %");
			break;
		}
		case 0x03:
		{
			type ="光照传感器";
			float illumination;
			memcpy(&illumination, &msg.ExtendData[2], 4);
			CString illumination_str;
			illumination_str.Format(_T("%.2f"), illumination); 
			value += illumination_str;
			value +=_T(" Lux");
			break;
		}
		case 0x04:
		{
			type = "人体热释传感器";
			int temp = msg.ExtendData[2];
			if(temp == 0x00)
				value = "无人";
			else
				value = "有人";
			break;
		}
		case 0x05:
		{
			type ="烟雾传感器";
			float smoke;
			memcpy(&smoke, &msg.ExtendData[2], 4);
			CString smoke_str;
			smoke_str.Format(_T("%.2f"), smoke); 
			value += smoke_str;
			value +=_T(" PPM");
			break;
		}
		case 0x06:
		{
			type ="超声波感器";
			int distance = msg.ExtendData[2] + 256*msg.ExtendData[3];
			CString distance_str;
			distance_str.Format(_T("%d"), distance);
			value += distance_str;
			value +=_T(" CM");
			break;
		}
		case 0x07:
		{
			type ="酒精传感器";
			float alcohol;
			memcpy(&alcohol, &msg.ExtendData[2], 4);
			CString alcohol_str;
			alcohol_str.Format(_T("%.2f"), alcohol); 
			value += alcohol_str;
			value +=_T(" PPM");
			break;
		}
		case 0x08:
		{
			type = "继电器";
			int temp = msg.ExtendData[2];
			if(temp == 0x01)
				value = "断开";
			else
				value = "闭合";
			break;
		}
		case 0x09:
		{
			type ="三轴加速度传感器";
			int x = (msg.ExtendData[2]/16) + 16*(msg.ExtendData[3]);
			CString x_str;
			x_str.Format(_T("%d"), x);
			value += " x: ";
			value += x_str;

			int y = (msg.ExtendData[4]/16) + 16*(msg.ExtendData[5]);
			CString y_str;
			y_str.Format(_T("%d"), y);
			value += " y: ";
			value += y_str;

			int z = (msg.ExtendData[6]/16) + 16*(msg.ExtendData[7]);
			CString z_str;
			z_str.Format(_T("%d"), z);
			value += " z: ";
			value += z_str;
			break;
		}
		default:
		{
			type ="";
			value ="";
			break;
		}
	}
}