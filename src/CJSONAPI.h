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
#ifndef CJSONAPI_H_
#define CJSONAPI_H_

#include <anyrpc/anyrpc.h>
#include <anyrpc/method.h>

#include "CRTKDABClient.h"
#include "fic_decoder.h"

class CJSON_API
{
public:
	CJSON_API(int ListenPort, FICDecoder *DABFICDecoder, CRTKDABClient *RTKDABClient);
	virtual ~CJSON_API();

private:
	anyrpc::Server *JSON_RPC_Server;
	anyrpc::MethodManager *MethodManager;
};


/*
 * Base class to implement JSON-RPC function that uses CRTKDABClient
 */
class CFunctionCallbackRTKDAB : public anyrpc::Method
{
public:
	CFunctionCallbackRTKDAB(std::string const& name, std::string const& help, bool deleteOnRemove, CRTKDABClient* RTKDABClient_);

	virtual void Execute(anyrpc::Value& params, anyrpc::Value& result) = 0;

protected:
	CRTKDABClient* RTKDABClient;
};

/*
 * Base class to implement JSON-RPC function that uses FICDecoder
 */
class CFunctionCallbackFIC : public anyrpc::Method
{
public:
	CFunctionCallbackFIC(std::string const& name, std::string const& help, bool deleteOnRemove, FICDecoder* DABFICDecoder_);

	virtual void Execute(anyrpc::Value& params, anyrpc::Value& result) = 0;

protected:
	FICDecoder* DABFICDecoder;
};

/*
 * SetChannel class
 */
class CSetChannel : public CFunctionCallbackRTKDAB
{
public:
	CSetChannel(std::string const& name, std::string const& help, bool deleteOnRemove, CRTKDABClient* RTKDABClient_) :CFunctionCallbackRTKDAB(name, help, deleteOnRemove, RTKDABClient_){};

	void Execute(anyrpc::Value& params, anyrpc::Value& result);
};

/*
 * SetChannel class
 */
class CGetChannelInformation : public CFunctionCallbackFIC
{
public:
	CGetChannelInformation(std::string const& name, std::string const& help, bool deleteOnRemove, FICDecoder* DABFICDecoder_) :CFunctionCallbackFIC(name, help, deleteOnRemove, DABFICDecoder_){};

	void Execute(anyrpc::Value& params, anyrpc::Value& result);
};
#endif /* CJSONAPI_H_ */
