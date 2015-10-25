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
#include <unistd.h>

#include "dabplus_decoder.h"
#include "fic_decoder.h"
#include "CProcessPCMData.h"
#include "CWAVStreamer.h"
#include "CHTTPServer.h"
#include "CRTKDABServer.h"
#include "CRTKDABClient.h"
#include "CFIC.h"
#include "CJSONAPI.h"

using namespace std;


int main()
{
	// Setup objects
	CWAVStreamer WAVStreamer;
	CHTTPServer HTTPServer(&WAVStreamer);
	CProcessPCMData ProcessPCMData(&WAVStreamer);
	SuperframeFilter DABPlusDecoder(&ProcessPCMData);
	CFIC FIC;
	FICDecoder DABFICDecoder(&FIC);
	CRTKDABServer RTKDABServer(9000, &DABPlusDecoder, &DABFICDecoder);
	CRTKDABClient RTKDABClient("127.0.0.1", 8778);
	CJSON_API JSON_API(9001, &DABFICDecoder, &RTKDABClient);

	WAVStreamer.SetRTKDABClient(&RTKDABClient);
	WAVStreamer.SetFICDecoder(&DABFICDecoder);

	cout << "!!!rtl_dab_streamer - pre-alpha !!!" << endl;

	while (1)
	{
		usleep(100000); // Very ugly.
	}

	return 0;
}
