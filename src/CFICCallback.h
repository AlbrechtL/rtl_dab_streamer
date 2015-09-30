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

#ifndef CFICCALLBACK_H_
#define CFICCALLBACK_H_

#include <anyrpc/anyrpc.h>
#include <anyrpc/method.h>
#include "fic_decoder.h"

class CFICCallback: public anyrpc::Method
{
public:
	CFICCallback(std::string const& name, std::string const& help, bool deleteOnRemove, FICDecoder* DABFICDecoder_);
	virtual ~CFICCallback();

	void Execute(anyrpc::Value& params, anyrpc::Value& result);

private:
	FICDecoder* DABFICDecoder;
};

#endif /* CFICCALLBACK_H_ */
