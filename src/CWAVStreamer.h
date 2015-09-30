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


#ifndef CWAVSTREAMER_H_
#define CWAVSTREAMER_H_

#include <iostream>

#include "CHTTPContentProvider.h"
#include "CRTKDABClient.h"
#include "fic_decoder.h"

class CWAVStreamer: public CHTTPContentProvider
{
public:
	CWAVStreamer();
	virtual ~CWAVStreamer();

	int Connected(void);
	int Disconnected(void);
	int HEADResponse(void);
	int GETResponse(std::vector<std::string> URLPartList);
	int SendAudioSamples(const uint8_t* Data, size_t Length);
	void SetRTKDABClient(CRTKDABClient *RTKDABClient_);
	void SetFICDecoder(FICDecoder *DABFICDecoder_);

private:
	CRTKDABClient* RTKDABClient;
	FICDecoder* DABFICDecoder;

	uint16_t CurrentID;
	uint32_t CurrentFrequency;

	void ProcessURL(std::string Channel, std::string SID);
};

#endif /* CWAVSTREAMER_H_ */
