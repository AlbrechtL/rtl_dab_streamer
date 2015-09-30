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


#include "CProcessPCMData.h"

CProcessPCMData::CProcessPCMData(CWAVStreamer *WAVStreamer):CProcessPCMDataSink(WAVStreamer)
{

}

CProcessPCMData::~CProcessPCMData()
{

}

void CProcessPCMData::FormatChange(std::string format)
{

}

void CProcessPCMData::StartAudio(int samplerate, int channels, bool float32)
{

}

void CProcessPCMData::PutAudio(const uint8_t *data, size_t len)
{
	if(data)
		WAVStreamer->SendAudioSamples(data,len);
	else
		fprintf(stderr, "*** Audio samples error ***\n");
}

void CProcessPCMData::ProcessPAD(const uint8_t *xpad_data, size_t xpad_len, const uint8_t *fpad_data)
{

}
