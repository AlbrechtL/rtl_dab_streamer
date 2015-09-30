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

#ifndef CHTTPCONTENTPROVIDER_H_
#define CHTTPCONTENTPROVIDER_H_

class CHTTPContentProvider;

#include <vector>
#include "CHTTPServer.h"

class CHTTPContentProvider
{
public:
	CHTTPContentProvider();
	virtual ~CHTTPContentProvider();

	virtual int Connected(void) = 0;
	virtual int Disconnected(void) = 0;
	virtual int HEADResponse(void) = 0;
	virtual int GETResponse(std::vector<std::string> URLPartList) = 0;
	virtual int SendAudioSamples(const uint8_t* Data, size_t Length) = 0;

	void SetHTTPServer(CHTTPServer *pHTTPServer);

protected:
	CHTTPServer* HTTPServer;
};

#endif /* CHTTPCONTENTPROVIDER_H_ */
