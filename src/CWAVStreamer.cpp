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


#include <stdio.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include "CWAVStreamer.h"
#include "tools.h"

#define TIMEOUT_S 10
#define WAIT_US 100000

CWAVStreamer::CWAVStreamer()
{
	RTKDABClient = NULL;
	DABFICDecoder = NULL;
	CurrentID = 0;
	CurrentFrequency = 0;
}

CWAVStreamer::~CWAVStreamer()
{
}

int CWAVStreamer::Connected(void)
{
	RTKDABClient->OpenDevice();
	RTKDABClient->Start();

	return 0;
}

int CWAVStreamer::Disconnected(void)
{
	RTKDABClient->Stop();
	RTKDABClient->CloseDevice();

	return 0;
}

int CWAVStreamer::HEADResponse(void)
{
	// TODO: Use only strings, no hexadecimal values
    /*
    HTTP/1.0 200 OK
    Content-Length: 0
    Content-type: audio/wav
    Cache-Control: no-cache
    */
    uint8_t HTTP_OK[] =
    {
    0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
    0x0a,0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,
    0x20,0x30,0x0d,0x0a,0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,
    0x3a,0x20,0x61,0x75,0x64,0x69,0x6f,0x2f,0x77,0x61,0x76,0x0d,0x0a,0x43,0x61,0x63,
    0x68,0x65,0x2d,0x43,0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x3a,0x20,0x6e,0x6f,0x2d,0x63,
    0x61,0x63,0x68,0x65,0x0d,0x0a,0x0d,0x0a
    };

    fprintf(stderr, "Send OK\n");

    if(HTTPServer)
    	HTTPServer->Send(HTTP_OK, sizeof(HTTP_OK));

	return 0;
}

int CWAVStreamer::GETResponse(std::vector<std::string> URLPartList)
{
	// TODO: Use only strings, no hexadecimal values
    /*HTTP/1.0 200 OK
    Content-type: audio/wav
    Cache-Control: no-cache*/
	uint8_t StreamStart[] =
    {
    0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
    0x0a,0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,
    'a','u','d','i','o','/','w','a','v',0x0d,0x0a,0x43,0x61,0x63,0x68,0x65,0x2d,0x43,
    0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x3a,0x20,0x6e,0x6f,0x2d,0x63,0x61,0x63,0x68,0x65,
    0x0d,0x0a,0x0d,0x0a
    };

	// TODO: Update the WAV-header
	// 0x18 is sample rate, 0x1c is rate * channels * bytes per sample
	uint8_t WAVHeaderStereo[] =
    {
    0x52,0x49, 0x46,0x46, 0x64,0x19, 0xff,0x7f, 0x57,0x41, 0x56,0x45, 0x66,0x6d, 0x74,0x20,
    0x10,0x00, 0x00,0x00, 0x01,0x00, 0x02,0x00, 0x80,0xbb, 0x00,0x00, 0x80,0xbb, 0x00,0x00,
    0x02,0x00, 0x10,0x00, 0x64,0x61, 0x74,0x61, 0x40,0x19, 0xff,0x7f, 0x00,0x00, 0x00,0x00
    };

	if(URLPartList.size() == 2)
		ProcessURL(URLPartList.at(0), URLPartList.at(1));
	else
		fprintf(stderr, "Wrong URL\n");

    if(HTTPServer)
    {
    	HTTPServer->Send(StreamStart, sizeof(StreamStart));
    	HTTPServer->Send(WAVHeaderStereo, sizeof(WAVHeaderStereo));
    }

	return 0;
}

int CWAVStreamer::SendAudioSamples(const uint8_t* Data, size_t Length)
{
	if(HTTPServer)
		HTTPServer->Send(Data, Length);

	return 0;
}

void CWAVStreamer::SetRTKDABClient(CRTKDABClient *RTKDABClient_)
{
	RTKDABClient = RTKDABClient_;
}

void CWAVStreamer::SetFICDecoder(FICDecoder *DABFICDecoder_)
{
	DABFICDecoder = DABFICDecoder_;
}

void CWAVStreamer::ProcessURL(std::string Channel, std::string SID)
{
	uint32_t Timeout = 0;

	uint16_t iSID = std::stoul(SID, nullptr, 16);

	std::cout << "Channel: " << Channel<< ", SID: " << SID << std::endl;


	if(RTKDABClient && DABFICDecoder)
	{
		auto dab_channels_it = dab_channels.find(Channel);

		// Abort if channel frequency is not found
		if(dab_channels_it == dab_channels.end())
		{
			return;
		}

		// Get the channel frequency
		uint32_t Frequency = dab_channels_it->second;

		std::cout << "Tune to " << Frequency << " kHz" << std::endl;
		RTKDABClient->SetFreq(Frequency);

		if(CurrentFrequency != Frequency)
		{
			// Reset FIC decoder to get the new channel information
			DABFICDecoder->Reset();

			CurrentFrequency = Frequency;
		}

		// *** Wait for the signal and for the channel information ***
		bool foundChannel = false;
		while(!foundChannel)
		{
			// Get the signal status
			int isSignal = RTKDABClient->GetSignalLock();

			if (isSignal == 1) // Signal is available
			{
				// Get available DAB channels
				services_t new_services = DABFICDecoder->GetServices();

				// Find audio service in database
				for (auto &any : new_services) // TODO Maybe to use of find is better
				{
					if(any.sid == iSID) // Found it
					{
						std::cout << "SID detected" << std::endl;

						uint8_t subchid = any.subchannel.subchid;
						uint16_t  StartCU = any.subchannel.StartCU; // CU start address
						uint8_t U_E = any.subchannel.U_E; // UEP or EEP
						uint8_t EEPProtectionLevel = any.subchannel.EEPProtectionLevel;
						uint8_t SubChannelSize = any.subchannel.SubChannelSize;

						// Delete old channel
						// int ParaMode, int ID, int PacketAddr
						//RTKDABClient->DelServiceCom(0,CurrentID,0);

						// int ParaMode, int ID, int StartCU, int U_E, int Index, int EEPIdx, int CUNum, int PacketAddr, int FEC
						RTKDABClient->AddServiceCom(0, subchid, StartCU, U_E, 0, EEPProtectionLevel, SubChannelSize, 0, 2);
						CurrentID = subchid;

						foundChannel = true;
					}
				}
			}

			Timeout += WAIT_US;
			if(Timeout >= TIMEOUT_S * 1000000)
			{
				std::cout << "Signal Timeout " << std::endl;

				break;
			}

			// Wait 100 ms
			usleep(WAIT_US);
		}
	}
}
