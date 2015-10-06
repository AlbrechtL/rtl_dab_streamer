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


#ifndef CRTKDABCLIENT_H_
#define CRTKDABCLIENT_H_

#include <anyrpc/anyrpc.h>
#include <string.h>

class CRTKDABClient
{
public:
	CRTKDABClient(std::string ServerIPAdress, int ServerPort);
	virtual ~CRTKDABClient();

	void OpenDevice(void);
	void CloseDevice(void);

	void Start(void);
	void Stop(void);

	void AddServiceCom(int ParaMode, int ID, int StartCU, int U_E, int Index, int EEPIdx, int CUNum, int PacketAddr, int FEC);
	void DelServiceCom(int ParaMode, int ID, int PacketAddr);
	void SetFreq(int Frequency);

	int GetFreqAndBW(void);
	int GetSignalQuality(void);
	int GetSignalLock(void);
	int GetSignalPresent(void);
	int GetSignalStrength(void);
	int GetTunerRange(void);

private:
	anyrpc::Client* pMessagePackRPC_Client;
};

#endif /* CRTKDABCLIENT_H_ */
