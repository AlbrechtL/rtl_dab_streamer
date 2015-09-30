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


#ifndef CRTKDABSERVER_H_
#define CRTKDABSERVER_H_

#include "dabplus_decoder.h"

#define ANYRPC_THREADING
#include <anyrpc/anyrpc.h>
#include "CMSCCallback.h"
#include "CFICCallback.h"

class CRTKDABServer
{
public:
	CRTKDABServer(int Port, SuperframeFilter *DABPlusDecoder, FICDecoder* DABFICDecoder);
	virtual ~CRTKDABServer();

private:
	anyrpc::Server *MessagePack_Server;
	anyrpc::MethodManager *MethodManager;
};

#endif /* CRTKDABSERVER_H_ */
