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

#include "dabplus_decoder.h"
#include "fic_decoder.h"
#include "CProcessPCMData.h"
#include "CWAVStreamer.h"
#include "CHTTPServer.h"
#include "CRTKDABServer.h"
#include "CRTKDABClient.h"

#include "CFIC.h"

using namespace std;

int main()
{
	CWAVStreamer WAVStreamer;
	CHTTPServer HTTPServer(&WAVStreamer);
	CProcessPCMData ProcessPCMData(&WAVStreamer);
	SuperframeFilter DABPlusDecoder(&ProcessPCMData);
	CFIC FIC;
	FICDecoder DABFICDecoder(&FIC);
	CRTKDABServer RTKDABServer(9000, &DABPlusDecoder, &DABFICDecoder);
	CRTKDABClient RTKDABClient("127.0.0.1", 8778);

	WAVStreamer.SetRTKDABClient(&RTKDABClient);
	WAVStreamer.SetFICDecoder(&DABFICDecoder);

	cout << "!!!rtl_dab_streamer - pre-alpha !!!" << endl;

    char c;

    do
    {
    	c = 0;
        cin >> c;
    	//fgets(&c, 1, stdin);
    	//fprintf(stderr,"test %s\n",&c);
		if (c == 'c')
		{
			// Del WDR 2
			fprintf(stderr,"Send DelServiceCom(0,7,0)\n");
			RTKDABClient.DelServiceCom(0,7,0);

			// int ParaMode, int ID, int StartCU, int U_E, int Index, int EEPIdx, int CUNum, int PacketAddr, int FEC
			// 1 Live at 222064 kHz
			fprintf(stderr,"Send AddServiceCom(0, 1, 0, 1, 0, 2, 72, 0, 2)\n");
			RTKDABClient.AddServiceCom(0, 1, 0, 1, 0, 2, 72, 0, 2);
			fprintf(stderr,"End c\n");
		}

		if (c == 'x')
		{
			// Del 1 Live
			fprintf(stderr,"Send DelServiceCom(0,1,0)\n");
			RTKDABClient.DelServiceCom(0,1,0);

			// int ParaMode, int ID, int StartCU, int U_E, int Index, int EEPIdx, int CUNum, int PacketAddr, int FEC
			// WDR2 at 222064 kHz
			fprintf(stderr,"Send AddServiceCom(0, 7, 360, 1, 0, 2, 72, 0, 2)\n");
			RTKDABClient.AddServiceCom(0, 7, 360, 1, 0, 2, 72, 0, 2);
			fprintf(stderr,"End x\n");
		}

    } while (c != 'q');

	return 0;
}
