/*
    rtl_dab_streamer - Streams the DAB audio samples vie HTTP
    Copyright (c) 2015 Albrecht Lohofener <albrechtloh@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>
#include "CRTKDABClient.h"

CRTKDABClient::CRTKDABClient(std::string ServerIPAdress, int ServerPort)
{
	pMessagePackRPC_Client = new anyrpc::MessagePackTcpClient;

	pMessagePackRPC_Client->SetServer(ServerIPAdress.c_str(), ServerPort);
	pMessagePackRPC_Client->SetTimeout(1000);
}

CRTKDABClient::~CRTKDABClient()
{
}

void CRTKDABClient::AddServiceCom(int ParaMode, int ID, int StartCU, int U_E, int Index, int EEPIdx, int CUNum, int PacketAddr, int FEC)
{
	anyrpc::Value params;
	anyrpc::Value result;
	bool success;

	params.SetArray();
	params[0] = ParaMode;
	params[1] = ID;
	params[2] = StartCU;
	params[3] = U_E;
	params[4] = Index;
	params[5] = EEPIdx;
	params[6] = CUNum;
	params[7] = PacketAddr;
	params[8] = FEC;

	success = pMessagePackRPC_Client->Call("AddServiceCom", params, result);

	std::cout << "AddServiceCom: success: " <<  success << " result: " << result << std::endl;
}

void CRTKDABClient::DelServiceCom(int ParaMode, int ID, int PacketAddr)
{
	anyrpc::Value params;
	anyrpc::Value result;
	bool success;

	params.SetArray();
	params[0] = ParaMode;
	params[1] = ID;
	params[2] = PacketAddr;

	success = pMessagePackRPC_Client->Call("DelServiceCom", params, result);

	std::cout << "DelServiceCom: success: " <<  success << " result: " << result << std::endl;
}

void CRTKDABClient::SetFreq(int Frequency)
{
	anyrpc::Value params;
	anyrpc::Value result;
	bool success;

	params = Frequency;

	success = pMessagePackRPC_Client->Call("SetFreq", params, result);

	std::cout << "SetFreq: success: " <<  success << " result: " << result << std::endl;

}
