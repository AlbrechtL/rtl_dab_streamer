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


#ifndef CPROCESSPCMDATA_H_
#define CPROCESSPCMDATA_H_

#include "subchannel_sink.h"
#include "CWAVStreamer.h"

class CProcessPCMDataSink
{
protected:
	CWAVStreamer *WAVStreamer;
public:
	CProcessPCMDataSink(CWAVStreamer* WAVStreamer) : WAVStreamer(WAVStreamer) {}
	virtual ~CProcessPCMDataSink() {};
};


class CProcessPCMData: public SubchannelSinkObserver, public CProcessPCMDataSink
{
public:
	CProcessPCMData(CWAVStreamer *WAVStreamer);
	virtual ~CProcessPCMData();

	void FormatChange(std::string format);
	void StartAudio(int samplerate, int channels, bool float32);
	void PutAudio(const uint8_t *data, size_t len);
	void ProcessPAD(const uint8_t *xpad_data, size_t xpad_len, const uint8_t *fpad_data);
};
#endif /* CPROCESSPCMDATA_H_ */
