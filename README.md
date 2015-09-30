RTL DAB Streamer
===================
Turns your Realtek RTL2832 based DVB dongle into a DAB radio receiver.

Description
-----------
RTL DAB Streamer is a small tool to stream DAB radio by using a DVB-T dongle to a client e.g. KODI, VLC or mplayer.

The DVB-T dongle has to be based on the Realtek RTL2832U.
See [http://sdr.osmocom.org/trac/wiki/rtl-sdr](http://sdr.osmocom.org/trac/wiki/rtl-sdr) for more RTL SDR details.

This tool uses RTKDAB_Interface (https://github.com/AlbrechtL/RTKDAB_Interface) as backend.

Usage
-----
At the moment this program is under heavy development. Don't expect a super working application.
Start "RTKDAB_Interface" first.

Default port to connect to it is 2346.

    $ ./rtl_dab_streamer

Streaming
---------
To connect to the server you can use KODI, VLC or mplayer. Just connect to the URL 

    http://IPAddress:Port/Channel/SID

e.g. for "WDR 2" in NRW/Germany:

    http://localhost:2346/11D/0xD392

To get the channels and the SIDs information for your location you can use http://www.fmlist.org.

To use this tool in KODI simply create a *.strm file e.g. "WDR_2.strm"
 
    http://localhost:2346/11D/0xD392

Limitations
--------------
- Server accepts only one client
- Only DAB+ support

References
--------------
- Uses parts of DABlin: https://github.com/Opendigitalradio/dablin
- DAB Specification: https://www.worlddab.org/technology-rollout/standards/technical-specifications-list
  - EN 300 401 "Radio Broadcasting Systems; Digital Audio Broadcasting (DAB) to mobile, portable and fixed receivers"
  - TS 102 563 "Digital Audio Broadcasting (DAB); Transport of Advanced Audio Coding (AAC) audio"
