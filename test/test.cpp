// test.cpp : �������̨Ӧ�ó������ڵ㡣

#include <iostream>
#include "stdafx.h"
#include "test.h"
#include "Serial.h"
#include "ZigbeeMessage.h"
#include "ZigbeeSensor.h"
#include "Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define	MAX_PACKAFE__LEN	50

// Ψһ��Ӧ�ó������

using namespace std;

CWinApp theApp;


void connectSerial();
void disconnectSerial();
void receiveFromSerialPort();
void AddZigbeeSensor(CZigbeeMessage msg);
void controlRelaySensor(bool b);

CSerial cSerial;
CZigbeeSensor mSensor;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		connectSerial();

		receiveFromSerialPort();

		int n;
		cin >> n;
		disconnectSerial();
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	return nRetCode;
}


void connectSerial(){
		CString str;
        int nPort = 3;
		boolean b = cSerial.Open(nPort,115200);
		if(b == false){
			cout << "failed while opening COM" << nPort << endl;
		}
		else{
            cout << "open COM" << nPort << " successfully" << endl;
		}
}

void disconnectSerial(){
		boolean b = cSerial.Close();
		if(b == false)
			cout << "failed while closing serial" << endl;
		else{
            cout << "Serial closed successfully" << endl;
		}
}


void receiveFromSerialPort(){
	BYTE temp[MAX_PACKAFE__LEN];
	cout << "Start Read\n";
	int count = 0;
	while(true) {

		int readlen = cSerial.Read(temp,1);
		if(readlen > 0)
		{
			if(temp[0] == 0x7E)	
			{
				CByteArray msg;
				msg.Add(temp[0]);
				for(int i=0;i<8;i++)
				{
					readlen = cSerial.Read(temp,1);
					if(readlen >0)
						msg.Add(temp[0]);
				}
				int size = msg.GetSize();
				CString str = Hex2String(msg.GetData(),size);
				wcout << "[Coor] " << (const wchar_t*) str << endl;
			}else if(temp[0] == 0xAA){
				CByteArray msg;
				/* ͬ��ͷ */
				msg.Add(temp[0]);

				/* ������� */
				readlen = cSerial.Read(temp,2);
				if(readlen >= 2)
				{
						msg.Add(temp[0]);
						msg.Add(temp[1]);
				}
				/* ������ */
				readlen = cSerial.Read(temp,2);
				if(readlen >= 2)
				{
						msg.Add(temp[0]);
						msg.Add(temp[1]);
				}
				int package_len = temp[0] + 256*temp[1];

				/* ��Ϣ���� - RSSI ֵ */
				for(int i=0;i<package_len;i++)
				{
					readlen = cSerial.Read(temp,1);
					if(readlen >0)
							msg.Add(temp[0]);
				}

				/* ��β */
				readlen = cSerial.Read(temp,1);
				if(readlen >0)
						msg.Add(temp[0]);

				int size = msg.GetSize();

				CString str = Hex2String(msg.GetData(),size);
				wcout << "Node: " << (const wchar_t*) str << endl;

				/* ����֡ */
				if(msg.GetData()[size-1] != 0x55)
				{
					cout << "Wrong Frame" << endl;
					continue;
				}
				CZigbeeMessage message(msg.GetData(),msg.GetSize());
				AddZigbeeSensor(message);
				

			}
		}

		if(count % 30 == 0){
			controlRelaySensor(true);
		}
		else if((count+15) % 30 == 0){
			controlRelaySensor(false);
		}
		count++;
	}
}

	
void AddZigbeeSensor(CZigbeeMessage msg){
	int NodeAddress = msg.NodeAddress;
	int InfoType = msg.InfoType;

	/* �ڵ��ϴ���ַ��Ϣ */
	if(InfoType == 0x05)
		return;

	CZigbeeSensor sensor(msg);
	mSensor = sensor;
	wcout << "SensorType: " << (const wchar_t*) mSensor.type << ", Value: " << (const wchar_t*) mSensor.value << endl;

}


void controlRelaySensor(bool b){

	if(mSensor.type != "Relay Sensor"){
		//cout << "Relay Sensor not detected" << endl;
		return;
	}

	BYTE  data[13];
	data[0] = 0xAA;
	data[1] = 0x09;
	data[2] = 0x00;
	data[3] = 0x04;
	data[4] = 0x00;
	data[5] = 0x06;

	memcpy(&data[6], &mSensor.NodeAddress, 2);

	data[8] = 0x01;
	data[9] = 0x01;

	data[10] = 0x08;

	CString value = mSensor.value;

	if(b == true){
		data[11] = 0x02;
		cout << "Relay OPEN" << endl;
	}
	else{
		data[11] = 0x01;
		cout << "Relay CLOSE" << endl;
	}

	data[12] = 0x55;

	CString str = Hex2String(data,13);
	wcout << "SensorType: " << (const wchar_t*) str << endl;
	cSerial.Write(data, 13);
}