/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "stdafx.h"

#ifdef _DEBUG
 //#define new DBG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "spiwindowtransparentmidivoroguispectrumtimeframe.h"
#include "spiutility.h"

#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"
#include "CParameter.h"
#include "CWindowParameter.h"

#include "CSPITextWindow.h"

#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
 //#include <wchar.h>
#include <atlconv.h>

using namespace std;



void CSPITextWindow::Initialize()
{
	//2021nov29, spi, begin
	p_spilogfile = NULL;
	if (SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPITEXTWINDOW_DEBUG)
	{
		p_spilogfile = DBG_NEW spilogfile("cspitextwindow_debug.txt", true);
	}
	//2021nov29, spi, end

	if(_flags & SPITEXT_NOPOSTQUITMESSAGE)
	{
		_CREATEWINDOWEX_FLAGS1 = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT;
		_CREATEWINDOWEX_FLAGS2 = WS_POPUP;
	}
	else
	{
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		if (!this->_titlebardisplay) dwStyle = WS_POPUP | WS_VISIBLE;

		//_CREATEWINDOWEX_FLAGS1 = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT;
		_CREATEWINDOWEX_FLAGS1 = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT;
		_CREATEWINDOWEX_FLAGS2 = dwStyle;
	}

	if (_flags & SPITEXT_NOPARAMETERS)
	{
		//nothing to do
	}
	else
	{
		///////////////////
		//do add parameters
		///////////////////
		addParameter("spitext_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
		/* todo, similar to this
		addParameter("spec_mode", 0.0, 18.0, _specmode);
		addParameter("spec_idcolorpalette", 0.0, 8.0, _idcolorpalette);
		addParameter("spec_bgcolorid", 0.0, 255.0, 0.0);
		addParameter("spec_idcolorch1", 0.0, 255.0, 1.0);
		addParameter("spec_idcolorch2", 0.0, 255.0, 127.0);
		addParameter("spec_idcolorlowest", 0.0, 255.0, 64.0);
		addParameter("spec_idcolorhighest", 0.0, 255.0, 96.0);
		addParameter("spec_nx", 1.0, 40.0, 1.0);
		addParameter("spec_ny", 1.0, 40.0, 1.0);
		*/
	}
	return;
}

void CSPITextWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "spitext_showwnd")
	{
		float showflag = getParameterValue("spitext_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}
	//some parameters updated in UpdateSpectrum()
}

VOID CSPITextWindow::Register(LPWNDCLASSEX lpwndClassEx)
{
	if (lpwndClassEx)
	{
		lpwndClassEx->style = CS_HREDRAW | CS_VREDRAW;
		lpwndClassEx->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	}
	CWindowTransparent::Register(lpwndClassEx);
}

HWND CSPITextWindow::Create()
{
	return this->Create((HWND)NULL);
}

HWND CSPITextWindow::Create(HWND hWndParent)
{
	//hWnd = CreateWindowEx(Flags1, utf8_decode(global_classname).c_str(), szTitle, Flags2, 0, 0, 100, 100, 0, 0, hInstance, 0);
	//return CreateWindowEx(_CREATEWINDOWEX_FLAGS1, szWindowClass, szTitle, _CREATEWINDOWEX_FLAGS2, 0, 0, 100, 100, 0, 0, _hinstance, 0);
	int x = 0;
	int y = 0;
	int nWidth = 100;
	int nHeight = 100; 
	HMENU hMenu = NULL;
	LPVOID lpParam = NULL;
	return Create(hWndParent, _CREATEWINDOWEX_FLAGS1, _CREATEWINDOWEX_FLAGS2, x, y, nWidth, nHeight, hMenu, lpParam);
}

HWND CSPITextWindow::Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
	INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam)
{
	//return CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, _hinstance, lpParam);
	/*
	return CWindowTransparent::Create(hWndParent, dwExStyle, dwStyle,
		x, y, nWidth, nHeight, hMenu, lpParam);
	*/
	//2021nov29, spi, begin
	//in here, copy integrally base class member function CWindowTransparent::Create(hWndParent, dwExStyle, dwStyle, x, y, nWidth, nHeight, hMenu, lpParam)
	//then, modify the parameters when calling SetLayeredWindowAttributes()
	if (this->_hwnd == NULL)
	{
		this->_hwnd = CreateWindowEx(
			dwExStyle,
			this->_lpClassName,
			this->_lpCaption,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			(HWND)hWndParent,
			(HMENU)hMenu,
			this->_hInstance,
			this
		);

		if (this->_hwnd == NULL)
		{
			throw "The window could not be created";
		}

		this->_lParam = lpParam;

		SetWindowLong(this->_hwnd, GWL_EXSTYLE, GetWindowLong(this->_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//SetLayeredWindowAttributes(this->_hwnd, 0, this->_alpha, LWA_ALPHA);
		SetLayeredWindowAttributes(this->_hwnd, global_keyingcolor, this->_alpha, LWA_COLORKEY | LWA_ALPHA);
	}
	return this->_hwnd;
	//2021nov29, spi, end
}

CSPITextWindow::CSPITextWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) 
: CWindowParameter(lpszCaption, lpwndClassEx) //CWindowMultimonitor(lpszCaption, lpwndClassEx)
{
	_hinstance = NULL;
	_flags = 0;
	Initialize();
}

CSPITextWindow::CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName, //CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_hinstance = hInstance;
	_flags = 0;
	Initialize();
}

CSPITextWindow::CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,  //CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_hinstance = hInstance;
	_flags = 0;
	Initialize();
}

CSPITextWindow::CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName, LPCTSTR commandlinearguments/*=L""*/, DWORD flags,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,  //CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
		x, y, xwidth, yheight,
		alpha, titlebardisplay, menubardisplay)
{
	wstring commandlinewstring(commandlinearguments);
	string commandlinestring = utf8_encode(commandlinewstring);
	CSPITextWindow(lpszCaption, hInstance, lpszClassName, commandlinestring.c_str(), flags);
}

CSPITextWindow::CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName, LPCSTR commandlinearguments/*=""*/, DWORD flags,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,  //CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
		x, y, xwidth, yheight,
		alpha, titlebardisplay, menubardisplay)
{
	_hinstance = hInstance;
	_flags = flags; //must be set before the call Initialize()
	Initialize();


	global_startstamp_ms = GetTickCount();

	string commandlinestring(commandlinearguments);
	wstring commandlinewstring;
	if (commandlinestring.empty())
	{
		commandlinestring  = SPITEXT_EXAMPLE_0;
	}
	commandlinestring = "spitext.exe " + commandlinestring;
	commandlinewstring = utf8_decode(commandlinestring);

	LPSTR *szArgList;
	int nArgs;
	szArgList = CommandLineToArgvA((PCHAR)commandlinestring.c_str(), &nArgs);//CommandLineToArgvA(GetCommandLineA(), &nArgs);
	if (NULL == szArgList)
	{
		//wprintf(L"CommandLineToArgvA failed\n");
		return; // FALSE;
	}
	LPWSTR *szArgListW;
	int nArgsW;
	szArgListW = CommandLineToArgvW(commandlinewstring.c_str(), &nArgsW); //CommandLineToArgvW(GetCommandLineW(), &nArgsW);
	if (NULL == szArgListW)
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return; // FALSE;
	}
	if (nArgs > 1)
	{
		global_spitextstring = szArgList[1]; //any text to be displayed or "COUNTUP", "COUNTDOWN", "CLOCK", or a combinaison of "COUNTUP", "COUNTDOWN", "CLOCK" along with "HH", "MM", "SS", 
											 //"HH:MM:SS", "HH:MM", "MM:SS", "COUNTUP HH:MM:SS", "COUNTDOWN MM:SS", "CLOCK HH:MM:SS"
	}
	if (nArgs > 2)
	{
		global_starttime_sec = atoi(szArgList[2]); //-1, 0 or positive integer specifying seconds, used to setup display string when global_spitextstring contains "COUNTUP", "COUNTDOWN", "CLOCK" 
													//otherwise with any other global_spitextstring, global_starttime_sec is used as a delay before initiating global_spitextstring display
	}
	if (nArgs > 3)
	{
		global_endtime_sec = atoi(szArgList[3]); //-1, 0 or positive integer specifying seconds, used to setup display string when global_spitextstring contains "COUNTUP", "COUNTDOWN", "CLOCK"
												//otherwise with any other global_spitextstring, global_starttime_sec is used as a delay before killing global_spitextstring display
	}
	if (nArgs > 4)
	{
		global_x = atoi(szArgList[4]);
	}
	if (nArgs > 5)
	{
		global_y = atoi(szArgList[5]);
	}
	if (nArgs > 6)
	{
		global_fontheight = atoi(szArgList[6]);
	}
	//new parameters
	if (nArgs > 7)
	{
		global_fontface = szArgList[7];
	}
	if (nArgs > 8)
	{
		global_idfontcolor = atoi(szArgList[8]);
	}
	if (nArgs > 9)
	{
		wcscpy(szWindowClass, szArgListW[9]);
	}
	if (nArgs > 10)
	{
		wcscpy(szTitle, szArgListW[10]);
	}
	if (nArgs > 11)
	{
		global_begin = szArgList[11];
	}
	if (nArgs > 12)
	{
		global_starting = szArgList[12];
	}
	if (nArgs > 13)
	{
		global_finishing = szArgList[13];
	}
	if (nArgs > 14)
	{
		global_end = szArgList[14];
	}
	if (nArgs > 15)
	{
		global_alpha = atoi(szArgList[15]);
	}
	if (nArgs > 16)
	{
		global_monitor = szArgList[16]; //global_monitor is either empty "" (global_x and global_y will be considered absolute) or i.e. "1" or "2" or "2.1" or "2.2" or "2.3" or "2.4" etc. specifying monitor (global_x and global_y will be considered relative)
										//global_monitor can also be using ":" instead of "." in which case there will be 16 posisble submonitors instead of 4, i.e. "1:1", "1:2", "1:3", ..., "1:15", "1:16", etc. for specifying monitor (global_x and global_y will be considered relative)
	}
	if (nArgs > 17)
	{
		global_hmonitor = szArgList[17]; //global_hmonitor is either an empty string "" (global_x and global_y will be considered absolute) or the HMONITOR numeric value embeded into a string i.e. "0x00001E76" specifying a monitor handle (global_x and global_y will be considered relative)
	}
	if (nArgs > 18)
	{
		global_hwnd = szArgList[18]; //global_hwnd is either an empty string "" (global_x and global_y will be considered absolute) or the HWND numeric value embeded into a string i.e. "0x00001E76" specifying a window handle (global_x and global_y will be considered relative)
	}
	if (nArgs > 19)
	{
		global_windowclass = szArgList[19]; //global_windowclass is either an empty string "" (global_x and global_y will be considered absolute) or the window class string i.e. "windowclassofinterest" (global_x and global_y will be considered relative)
	}
	if (nArgs > 20)
	{
		global_windowtitle = szArgList[20]; //global_windowtitle is either an empty string "" (global_x and global_y will be considered absolute) or the window title string i.e. "windowtitleofinterest" (global_x and global_y will be considered relative)
	}
	if (nArgs > 21)
	{
		global_horizontaljustification = szArgList[21]; //global_horizontaljustification is either an empty string "" (global_x and global_y will be considered) or i.e. "left", "right", "center" (global_x and global_y will be ignored)
	}
	if (nArgs > 22)
	{
		global_verticaljustification = szArgList[22]; //global_verticaljustification is either an empty string "" (global_y and global_y will be considered) or i.e. "top", "bottom", "center" (global_x and global_y will be ignored)
	}
	if (nArgs > 23)
	{
		//global_horizontalforcefit = atoi(szArgList[23]); //global_horizontalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
		global_horizontalforcefit = atof(szArgList[23]); //global_horizontalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
		if (global_horizontalforcefit < 0.0f) global_horizontalforcefit = 0.0f;
		if (global_horizontalforcefit > 1.0f) global_horizontalforcefit = 1.0f;
	}
	if (nArgs > 24)
	{
		//global_vecticalforcefit = atoi(szArgList[24]); //global_vecticalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
		global_verticalforcefit = atof(szArgList[24]); //global_vecticalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
		if (global_verticalforcefit < 0.0f) global_verticalforcefit = 0.0f;
		if (global_verticalforcefit > 1.0f) global_verticalforcefit = 1.0f;
	}
	if (nArgs > 25)
	{
		global_fullmonitorssurface = atoi(szArgList[25]); //global_fullmonitorssurface is either 0 for no or 1, the default, for yes do display string over multiple monitors
	}

	LocalFree(szArgList);
	LocalFree(szArgListW);

	pFILE = fopen("debug.txt", "w");
	if (pFILE)
	{
		fprintf(pFILE, "global_horizontalforcefit is %f\n", global_horizontalforcefit);
		fprintf(pFILE, "global_verticalforcefit is %f\n", global_verticalforcefit);
		fprintf(pFILE, "\n\n");
	}
	//global_startstamp_ms = GetTickCount();

	//init possible clock modes and formats
	global_countermode.push_back("COUNTUP");
	global_countermode.push_back("COUNTDOWN");
	global_countermode.push_back("CLOCK");
	global_counterformat.push_back("HH:MM:SS");
	global_counterformat.push_back("HH:MM");
	global_counterformat.push_back("MM:SS");
	global_counterformat.push_back("HH");
	global_counterformat.push_back("MM");
	global_counterformat.push_back("SS");

	/*
	int global_countermodeCOUNTUP = 0;
	int global_countermodeCOUNTDOWN = 1;
	int global_countermodeCLOCK = 2;
	*/
	//global_textmode = -1; //default to display straight text
	if ((global_spitextstring.find(global_countermode[global_countermodeCOUNTUP]) != string::npos))
	{
		global_textmode = global_countermodeCOUNTUP;
	}
	else if ((global_spitextstring.find(global_countermode[global_countermodeCOUNTDOWN]) != string::npos))
	{
		global_textmode = global_countermodeCOUNTDOWN;
	}
	else if ((global_spitextstring.find(global_countermode[global_countermodeCLOCK]) != string::npos))
	{
		global_textmode = global_countermodeCLOCK;
	}


	/*
	int global_counterformatHHMMSS = 0;
	int global_counterformatHHMM = 1;
	int global_counterformatMMSS = 2;
	int global_counterformatHH = 3;
	int global_counterformatMM = 4;
	int global_counterformatSS = 5;
	*/
	if (global_textmode > -1)
	{
		//some counter mode, set counter initial time to be displayed
		if (global_starttime_sec > -1)
		{
			global_timetodisplay_sec = global_starttime_sec;
		}
		else
		{
			global_timetodisplay_sec = 0;
		}

		global_textformat = global_counterformatHHMMSS; //default when unspecified by user
		//some counter mode, check if counter format specified
		if ((global_spitextstring.find(global_counterformat[global_counterformatHHMMSS]) != string::npos))
		{
			global_textformat = global_counterformatHHMMSS;
		}
		else if ((global_spitextstring.find(global_counterformat[global_counterformatHHMM]) != string::npos))
		{
			global_textformat = global_counterformatHHMM;
		}
		else if ((global_spitextstring.find(global_counterformat[global_counterformatMMSS]) != string::npos))
		{
			global_textformat = global_counterformatMMSS;
		}
		else if ((global_spitextstring.find(global_counterformat[global_counterformatHH]) != string::npos))
		{
			global_textformat = global_counterformatHH;
		}
		else if ((global_spitextstring.find(global_counterformat[global_counterformatMM]) != string::npos))
		{
			global_textformat = global_counterformatMM;
		}
		else if ((global_spitextstring.find(global_counterformat[global_counterformatSS]) != string::npos))
		{
			global_textformat = global_counterformatSS;
		}
		//2021nov29, spi, begin
		if (global_textmode == global_countermodeCOUNTDOWN)
		{
			if (global_starttime_sec < 0) global_starttime_sec = 0;
			if (global_endtime_sec > global_starttime_sec)
			{
				int temp = global_endtime_sec;
				global_endtime_sec = global_starttime_sec;
				global_starttime_sec = temp;
				global_timetodisplay_sec = global_starttime_sec;
			}
		}
		else
		{
			if ((global_endtime_sec - global_starttime_sec) <= 0) global_endtime_sec = -1; //no ending timer will be set
		}
		//2021nov29, spi, end
	}
	else
	{
		//2021nov23, spi, begin
		//string display mode
		if (global_spitextstring.empty()) global_spitextstring = SPITEXT_DEFAULTSTRING; //force to be at least 1 space so textextent and fontsize can be computed
		//2021nov23, spi, end
		//string display mode, use start and end time as delay and kill time
		if (global_starttime_sec <= 0) global_starttime_sec = 0; //no delay start timer will be set
		if (global_endtime_sec <= 0) global_endtime_sec = -1; //no ending timer will be set
		if ((global_endtime_sec - global_starttime_sec) <= 0) global_endtime_sec = -1; //no ending timer will be set
	}


	//these structures initialize themselves automatically with the list of monitors 
	//and the list of all top-level windows on the screens
	//MonitorRects _monitors; 
	//EnumWindowsStruct_spitext _windows;
	if (pFILE)
	{
		int numberofmonitor = _monitors.rcMonitors.size();
		if (numberofmonitor > 0)
		{
			//idmonitor is valid
			for (int indexmonitor = 0; indexmonitor < numberofmonitor; indexmonitor++)
			{
				RECT myRECT = _monitors.rcMonitors[indexmonitor];
				HMONITOR hmonitor = _monitors.hMonitors[indexmonitor];
				fprintf(pFILE, "detecting monitors id %d (left %d, right %d, top %d, bottom %d)\n", indexmonitor + 1, myRECT.left, myRECT.right, myRECT.top, myRECT.bottom);
				stringstream ss;
				ss << std::hex << hmonitor;
				string hexstring(ss.str());
				fprintf(pFILE, "detecting monitors id %d (hmonitor: dec %d, hex 0x%s)\n", indexmonitor + 1, hmonitor, hexstring.c_str());
			}
			fprintf(pFILE, "combined global rectangle from all monitors (left %d, right %d, top %d, bottom %d)\n", _monitors.rcCombined.left, _monitors.rcCombined.right, _monitors.rcCombined.top, _monitors.rcCombined.bottom);
		}
		else
		{
			fprintf(pFILE, "error, not detecting any monitors!\n");
			fclose(pFILE);
			return; // (FALSE);
		}
	}

#ifdef _DEBUG
	if (pFILE)
	{
		fprintf(pFILE, "\ndetecting the following windows (window classname listing):\n\n");
		if (_windows.hwndstringmap_class.size() > 1)
		{
			std::map<HWND, wstring>::iterator it;
			for (it = _windows.hwndstringmap_class.begin(); it != _windows.hwndstringmap_class.end(); it++)
			{
				HWND myHWND = it->first;
				wstring mywindowclassw = it->second;
				string mywindowclass = "";
				if (!mywindowclassw.empty()) mywindowclass = utf8_encode(mywindowclassw);
				fprintf(pFILE, "hwnd,windowclassname %d,%40s\n", myHWND, mywindowclass.c_str());
			}
		}
		fprintf(pFILE, "\n");
		fprintf(pFILE, "\ndetecting the following windows (window title listing):\n\n");
		if (_windows.hwndstringmap_title.size() > 1)
		{
			std::map<HWND, wstring>::iterator it;
			for (it = _windows.hwndstringmap_title.begin(); it != _windows.hwndstringmap_title.end(); it++)
			{
				HWND myHWND = it->first;
				wstring mywindowtitlew = it->second;
				string mywindowtitle = "";
				if (!mywindowtitlew.empty()) mywindowtitle = utf8_encode(mywindowtitlew);
				fprintf(pFILE, "hwnd,windowtitle %d,%40s\n", myHWND, mywindowtitle.c_str());
			}
		}
		fflush(pFILE);
	}
#endif

	//initially presume global_x and global_y coordinate absolute
	//POINT myoutputPOINT;
	myoutputPOINT.x = global_x;
	myoutputPOINT.y = global_y;
	//bool globalxyabsolute = true;
	//RECT myoutputRECT;

	HMONITOR myHMONITOR = NULL;
	std::map<HMONITOR, RECT>::iterator it;
	if (global_fullmonitorssurface)
	{
		//default to the rectangle made from all monitors combined
		myoutputRECT = _monitors.rcCombined;
	}
	else
	{
		//fit to one monitor, rectrict from displaying string extending over multiple monitors
		//HMONITOR myHMONITOR;
		myHMONITOR = MonitorFromPoint(myoutputPOINT, MONITOR_DEFAULTTONEAREST);
		//std::map<HMONITOR, RECT>::iterator it;
		it = _monitors.hmapMonitors.find(myHMONITOR);
		if (it != _monitors.hmapMonitors.end())
		{
			myoutputRECT = (*it).second;
		}
		else
		{
			assert(false);
			return; // (FALSE);
		}
	}
	if (!global_monitor.empty())
	{
		int idmonitor = atoi(global_monitor.substr(0, 1).c_str());
		int idsubmonitor = 0;
		int numsubmonitors = 1;
		if (idmonitor == 0) idmonitor = 1;
		if (idmonitor > 0 && idmonitor < 10)
		{
			if (idmonitor <= _monitors.rcMonitors.size())
			{
				//idmonitor is valid
				globalxyabsolute = false;
				myoutputRECT = _monitors.rcMonitors[idmonitor - 1];
			}
			if ((global_monitor.size() > 2) && ((global_monitor.substr(1, 1) == ".") || (global_monitor.substr(1, 1) == ":")))
			{
				if (global_monitor.substr(1, 1) == ".")
				{
					idsubmonitor = atoi(global_monitor.substr(2, 1).c_str());
					if (idsubmonitor < 1) idsubmonitor = 1;
					if (idsubmonitor > 4) idsubmonitor = 4;
					numsubmonitors = 4;
				}
				else if (global_monitor.substr(1, 1) == ":")
				{
					idsubmonitor = atoi(global_monitor.substr(2, 1).c_str());
					if (idsubmonitor == 1 && (global_monitor.size() > 3))
					{
						//use second digit if present and valid
						int seconddigit = atoi(global_monitor.substr(3, 1).c_str());
						if (seconddigit > -1 && seconddigit < 7) idsubmonitor = 10 + seconddigit;
					}
					if (idsubmonitor < 1) idsubmonitor = 1;
					if (idsubmonitor > 16) idsubmonitor = 16;
					numsubmonitors = 16;
				}
				if (idsubmonitor > 0 && idsubmonitor < 17)
				{
					//idsubmonitor is valid
					//divbyfour=true;
					//now, let's divide myoutputRECT by 4 same size rectangles
					//based on https://stackoverflow.com/questions/6190019/split-a-rectangle-into-equal-sized-rectangles
					RECT original = myoutputRECT;
					//int numsubmonitors = 1;
					//if (global_monitor.substr(1,1)==".") numsubmonitors = 4;
					//else if (global_monitor.substr(1,1)==":") numsubmonitors = 16;
					int columns = ceil(sqrt((float)numsubmonitors));
					int fullrows = numsubmonitors / columns;
					int orphans = numsubmonitors % columns;   // how many 'odd-sized' ones on our bottom row.
					//calculate output width and height
					int width = (original.right - original.left) / columns; //original.width/ columns;
					int height = (original.bottom - original.top) / (orphans == 0 ? fullrows : (fullrows + 1)); //original.height / (orphans == 0 ? fullrows : (fullrows+1)); // reduce height if there are orphans
					//calculate output rectangles
					RECT output[16];
					assert(numsubmonitors <= 16);
					int i = -1;
					//to be similar to displayfusion software installed on remotedroide
					//i.e. second monitor's submonitors are specified like this
					//2.1 2.3
					//2.2 2.4
					for (int x = 0; x < columns; ++x) //for (int y = 0; y < fullrows; ++y)
					{
						for (int y = 0; y < fullrows; ++y) //for (int x = 0; x < columns; ++x)
						{
							i++;
							//submonitor "X.1"
							output[i].left = original.left + x * width; //output.push_back(CRect(x * width, y * height, width, height));
							//submonitor "X.2"
							output[i].top = original.top + y * height; //output.push_back(CRect(x * width, y * height, width, height));
							//submonitor "X.3"
							output[i].right = output[i].left + width; //output.push_back(CRect(x * width, y * height, width, height));
							//submonitor "X.4"
							output[i].bottom = output[i].top + height; //output.push_back(CRect(x * width, y * height, width, height));
						}
					}
					myoutputRECT = output[idsubmonitor - 1];
				}
			}
		}
		if (pFILE)
		{
			fprintf(pFILE, "\nmonitor specified by %s\n", global_monitor.c_str());
			fprintf(pFILE, "idmonitor is %d, idsubmonitor is %d, number of submonitors is %d\n", idmonitor, idsubmonitor, numsubmonitors);
			fprintf(pFILE, "output rect is now, left %d right %d top %d bottom %d\n", myoutputRECT.left, myoutputRECT.right, myoutputRECT.top, myoutputRECT.bottom);
		}
	}
	else if (!global_hmonitor.empty())
	{
		//todo: for hex string, use strtol https://en.cppreference.com/w/c/string/byte/strtol
		HMONITOR suppliedhmonitor;
		if (global_hmonitor.size() > 3 && global_hmonitor.substr(0, 2) == "0x")
		{
			global_hmonitor = global_hmonitor.substr(2);
			suppliedhmonitor = (HMONITOR)strtol(global_hmonitor.c_str(), NULL, 16);
		}
		else
		{
			suppliedhmonitor = (HMONITOR)atol(global_hmonitor.c_str());
		}
		it = _monitors.hmapMonitors.find(suppliedhmonitor);
		if (it != _monitors.hmapMonitors.end())
		{
			myHMONITOR = (*it).first;
			myoutputRECT = (*it).second;
		}
		else
		{
			assert(false);
			if (pFILE)
			{
				fprintf(pFILE, "error global_hmonitor not found");
				fclose(pFILE);
			}
			return; // (FALSE);
		}
		globalxyabsolute = false;
	}
	else if (!global_hwnd.empty())
	{
		//todo: for hex string, use strtol https://en.cppreference.com/w/c/string/byte/strtol
		HWND suppliedhwnd;
		if (global_hwnd.size() > 3 && global_hwnd.substr(0, 2) == "0x")
		{
			global_hwnd = global_hwnd.substr(2);
			suppliedhwnd = (HWND)strtol(global_hwnd.c_str(), NULL, 16);
		}
		else
		{
			suppliedhwnd = (HWND)atol(global_hwnd.c_str());
		}

		BOOL bresult = GetWindowRect(suppliedhwnd, &myoutputRECT);
		if (!bresult)
		{
			assert(false);
			if (pFILE)
			{
				fprintf(pFILE, "error global_hwnd not found");
				fclose(pFILE);
			}
			return; // (FALSE);
		}
		myHMONITOR = MonitorFromRect(&myoutputRECT, MONITOR_DEFAULTTONEAREST);
		globalxyabsolute = false;
	}
	else if (!global_windowclass.empty())
	{
		BOOL bresult = false;
		HWND suppliedhwnd;
		if (!global_windowtitle.empty())
		{
			suppliedhwnd = (HWND)FindWindowA(global_windowclass.c_str(), global_windowtitle.c_str());
		}
		else
		{
			suppliedhwnd = (HWND)FindWindowA(global_windowclass.c_str(), NULL);
		}
		if (suppliedhwnd) bresult = GetWindowRect(suppliedhwnd, &myoutputRECT);
		if (!bresult)
		{
			assert(false);
			if (pFILE)
			{
				fprintf(pFILE, "error global_windowclass and window title not found");
				fclose(pFILE);
			}
			return; // (FALSE);
		}
		myHMONITOR = MonitorFromRect(&myoutputRECT, MONITOR_DEFAULTTONEAREST);
		globalxyabsolute = false;
	}
	else if (!global_windowtitle.empty())
	{
		BOOL bresult = false;
		HWND suppliedhwnd = FindWindowA(NULL, global_windowtitle.c_str());
		if (suppliedhwnd) bresult = GetWindowRect(suppliedhwnd, &myoutputRECT);
		if (!bresult)
		{
			assert(false);
			if (pFILE)
			{
				fprintf(pFILE, "error global_windowtitle not found");
				fclose(pFILE);
			}
			return; // (FALSE);
		}
		myHMONITOR = MonitorFromRect(&myoutputRECT, MONITOR_DEFAULTTONEAREST);
		globalxyabsolute = false;
	}
	//2021dec29, spi, begin
	else if(global_x<0 && global_y<0 && global_fontheight<0)
	{
		//while we are still in the constructor, handle the specific case 
		//when called from launchspitextwindow() in spiwtmvst app
		//x, y, x + xwidth, y + yheight //in window client coordinates
		global_fontheight = yheight;
		//now, convert to absolute coordinates, add these client coordinate to spitext window's monitor top left origin absolute coordinate
		/* _hwnd is not created yet!!!
		RECT mywindowRECT;
		//
		BOOL bresult = GetWindowRect(_hwnd, &mywindowRECT); //in screen coordinates that are relative to the upper-left corner of the screen
		if (!bresult)
		{
			assert(false);
			if (pFILE)
			{
				fprintf(pFILE, "error spitext window's RECT not found");
				fclose(pFILE);
			}
			return; // (FALSE);
		}
		HMONITOR hmon = MonitorFromRect(&mywindowRECT, MONITOR_DEFAULTTONEAREST);
		RECT mymonitorRECT = _monitors.FindMonitorRect(hmon); //absolute coordinates
		//int dx = mymonitorRECT.left; //mywindowRECT.left-mymonitorRECT.left;
		//int dy = mymonitorRECT.top; //mywindowRECT.top - mymonitorRECT.top;
		//OffsetRect(&myoutputRECT, dx, dy);
		//
		myoutputRECT = mywindowRECT; //{ x, y, x + xwidth, y + yheight }; //in window client coordinates
		*/
		globalxyabsolute = false;
		global_x = x;
		global_y = y;
	}
	//2021dec29, spi, end

	/*
	//moved into InitInstance()
	if( (global_textmode==-1) && (global_starttime_sec>0) )
	{
		//todo, launch a timer to wait for global_starttime_sec before display;
	}
	if( (global_textmode==-1) && (global_endtime_sec>0) && ((global_endtime_sec-global_starttime_sec)>0) )
	{
		//todo, launch a timer to wait for global_timetodisplay_sec before killing app;
	}
	*/

}

CSPITextWindow::~CSPITextWindow()
{
	DeleteObject(global_hFont);
	if (pFILE) fclose(pFILE);
	if (p_spilogfile) delete p_spilogfile;
}

LRESULT CALLBACK CSPITextWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			/*
			DWORD Flags1 = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT;
			DWORD Flags2 = WS_POPUP;
			//todo, make sure _hwnd is created with proper flags

			hWnd = CreateWindowEx(Flags1, szWindowClass, szTitle, Flags2, 0, 0, 100, 100, 0, 0, hInstance, 0);
			//hWnd = CreateWindowEx(Flags1, utf8_decode(global_classname).c_str(), szTitle, Flags2, 0, 0, 100, 100, 0, 0, hInstance, 0);
			if (!hWnd)
			{
				return FALSE;
			}
			*/
			//global_hFont=CreateFontW(global_fontheight,0,0,0,FW_NORMAL,0,0,0,0,0,0,2,0,L"SYSTEM_FIXED_FONT");
			//global_hFont=CreateFontW(global_fontheight,0,0,0,FW_BOLD,0,0,0,0,0,0,2,0,L"Segoe Script");
			//global_hFont=CreateFontA(global_fontheight,0,0,0,FW_BOLD,0,0,0,0,0,0,2,0,global_fontface.c_str());
			global_hFont = CreateFontA(global_fontheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, global_fontface.c_str());


			SIZE mySIZE;
			HDC myHDC = GetDC(hwnd);
			HGDIOBJ prevHGDIOBJ = SelectObject(myHDC, global_hFont);

			if (global_textmode < 0)
			{
				//GetTextExtentPoint32A(myHDC, "88:88:88", strlen("88:88:88"), &mySIZE);
				GetTextExtentPoint32A(myHDC, global_spitextstring.c_str(), global_spitextstring.length(), &mySIZE);
			}
			else
			{
				/*
				//if global_spitextstring specifies a counter mode and a counter format, fetch it properly
				if(global_textformat<0) GetTextExtentPoint32A(myHDC, "88:88:88", strlen("88:88:88"), &mySIZE);
				else if(global_textformat==global_counterformatHHMMSS) GetTextExtentPoint32A(myHDC, "88:88:88", strlen("88:88:88"), &mySIZE);
				else if(global_textformat==global_counterformatHHMM) GetTextExtentPoint32A(myHDC, "88:88", strlen("88:88"), &mySIZE);
				else if(global_textformat==global_counterformatMMSS) GetTextExtentPoint32A(myHDC, "88:88", strlen("88:88"), &mySIZE);
				else if(global_textformat==global_counterformatHH) GetTextExtentPoint32A(myHDC, "88", strlen("88"), &mySIZE);
				else if(global_textformat==global_counterformatMM) GetTextExtentPoint32A(myHDC, "88", strlen("88"), &mySIZE);
				else if(global_textformat==global_counterformatSS) GetTextExtentPoint32A(myHDC, "88", strlen("88"), &mySIZE);
				//GetTextExtentPoint32A(myHDC, global_spitextstring.c_str(), global_spitextstring.length(), &mySIZE);
				*/
				//if global_spitextstring specifies a counter mode and a counter format, fetch it properly
				global_spitextstring = "88:88:88";
				if (global_textformat < 0) global_spitextstring = "88:88:88";
				else if (global_textformat == global_counterformatHHMMSS) global_spitextstring = "88:88:88";
				else if (global_textformat == global_counterformatHHMM) global_spitextstring = "88:88";
				else if (global_textformat == global_counterformatMMSS) global_spitextstring = "88:88";
				else if (global_textformat == global_counterformatHH) global_spitextstring = "88";
				else if (global_textformat == global_counterformatMM) global_spitextstring = "88";
				else if (global_textformat == global_counterformatSS) global_spitextstring = "88";
				GetTextExtentPoint32A(myHDC, global_spitextstring.c_str(), global_spitextstring.length(), &mySIZE);
			}

			if (globalxyabsolute == false)
			{
				//relative coordinates
				global_x = myoutputRECT.left + global_x;
				global_y = myoutputRECT.top + global_y;
				if (pFILE)
				{
					if (global_x > myoutputRECT.right) fprintf(pFILE, "\nwarning, global_x is considered relative and exceeds targeted monitor or window rect\n");
					if (global_y > myoutputRECT.bottom) fprintf(pFILE, "\nwarning, global_y is considered relative and exceeds targeted monitor or window rect\n");
				}
			}
			else
			{
				//absolute coordinates
				myoutputPOINT.x = global_x;
				myoutputPOINT.y = global_y;
				if (global_fullmonitorssurface)
				{
					bool found = false;
					found = PtInRect(&(_monitors.rcCombined), myoutputPOINT);
					if (!found)
					{
						assert(false);
						if (pFILE)
						{
							fprintf(pFILE, "\nwarning, global_x and global_y are considered absolute but do not point inside the combined rect of all monitors\n");
						}
					}
				}
				else
				{
					bool found = false;
					for (int i = 0; i < _monitors.rcMonitors.size(); ++i)
					{
						if (PtInRect(&(_monitors.rcMonitors[i]), myoutputPOINT))
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						assert(false);
						if (pFILE)
						{
							fprintf(pFILE, "\nwarning, global_x and global_y are considered absolute but do not point inside any monitor rects\n");
						}
					}
				}
			}

			if (!global_horizontaljustification.empty() || !global_verticaljustification.empty())
			{
				justification(mySIZE);
			}
			//if(!globalxyabsolute && (global_horizontalforcefit || global_verticalforcefit) && (global_spitextstring.length()>0) && (mySIZE.cx>0))
			if ((!globalxyabsolute) && ((global_horizontalforcefit > 0.0f) || (global_verticalforcefit > 0.0f)) && (global_spitextstring.length() > 0) && (mySIZE.cx > 0))
			{
				if (pFILE)
				{
					fprintf(pFILE, "\n");
					if ((global_horizontalforcefit > 0.0f)) fprintf(pFILE, "before horizontal force fit\n");
					if ((global_verticalforcefit > 0.0f)) fprintf(pFILE, "before vertical force fit\n");
					fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
					fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
				}
				POINT topleftPOINT;
				topleftPOINT.x = global_x;
				topleftPOINT.y = global_y;
				POINT bottomrightPOINT;
				bottomrightPOINT.x = global_x + mySIZE.cx;
				bottomrightPOINT.y = global_y + mySIZE.cy;
				if (!PtInRect(&myoutputRECT, topleftPOINT) || !PtInRect(&myoutputRECT, bottomrightPOINT))
				{
					//if(global_horizontalforcefit)
					if (global_horizontalforcefit > 0.0f)
					{
						if (global_x < myoutputRECT.left) global_x = myoutputRECT.left;
						if (global_x > myoutputRECT.right) global_x = myoutputRECT.right;
					}
					//if(global_verticalforcefit)
					if (global_verticalforcefit > 0.0f)
					{
						if (global_y < myoutputRECT.top) global_y = myoutputRECT.top;
						if (global_y > myoutputRECT.bottom) global_y = myoutputRECT.bottom;
					}

					int maxwidth = myoutputRECT.right - myoutputRECT.left;
					assert(maxwidth > 0);
					int maxheight = myoutputRECT.bottom - myoutputRECT.top;
					assert(maxheight > 0);

					//2020sept23, spi, begin
					//global_horizontalforcefit and global_verticalforcefit can now be between 0.0 and 1.0, specifying a percentage (a scale) when greater than 0
					int newmaxwidth = maxwidth;
					int newmaxheight = maxheight;
					if (global_horizontalforcefit > 0.0f)
					{
						newmaxwidth = maxwidth * global_horizontalforcefit;
						if (global_x < (myoutputRECT.left + (maxwidth - newmaxwidth) / 2)) global_x = myoutputRECT.left + (maxwidth - newmaxwidth) / 2;
						if (global_x > (myoutputRECT.right - (maxwidth - newmaxwidth) / 2)) global_x = myoutputRECT.right - (maxwidth - newmaxwidth) / 2;
					}
					//if(global_verticalforcefit)
					if (global_verticalforcefit > 0.0f)
					{
						newmaxheight = maxheight * global_verticalforcefit;
						if (global_y < (myoutputRECT.top + (maxheight - newmaxheight) / 2)) global_y = myoutputRECT.top + (maxheight - newmaxheight) / 2;
						if (global_y > (myoutputRECT.bottom - (maxheight - newmaxheight) / 2)) global_y = myoutputRECT.bottom - (maxheight - newmaxheight) / 2;
					}
					//2020sept23, spi, end

					//regle de 3
					//global_fontheight -> mySIZE.cx/global_spitextstring.length()
					//newfontheight -> maxwidth/global_spitextstring.length()
					//int newfontheight = maxwidth * global_fontheight / mySIZE.cx;
					int newfontheight = newmaxwidth * global_fontheight / mySIZE.cx;
					//if( (global_verticalforcefit>0.0f) && newfontheight>maxheight) newfontheight = maxheight;
					//if( (global_verticalforcefit>0.0f) && newfontheight>maxheight) newfontheight = maxheight*global_verticalforcefit;
					if ((global_verticalforcefit > 0.0f) && newfontheight > newmaxheight) newfontheight = maxheight;
					//create new font using newfontheight
					HFONT newhfont = CreateFontA(newfontheight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, global_fontface.c_str());
					if (newhfont)
					{
						//deselect current font
						SelectObject(myHDC, prevHGDIOBJ);
						//delete current font
						DeleteObject(global_hFont);
						//select new font
						global_hFont = newhfont;
						global_fontheight = newfontheight;
						prevHGDIOBJ = SelectObject(myHDC, global_hFont);
						//recompute new string extent size
						GetTextExtentPoint32A(myHDC, global_spitextstring.c_str(), global_spitextstring.length(), &mySIZE);
						//if enabled, justify once again
						if (!global_horizontaljustification.empty() || !global_verticaljustification.empty())
						{
							justification(mySIZE);
						}
					}
					else
					{
						if (pFILE)
						{
							fprintf(pFILE, "\nwarning, create font of new size %d did not succeed\n", newfontheight);
							fprintf(pFILE, "warning, force fit may not be successful\n");
						}
					}
				}

				if (pFILE)
				{
					fprintf(pFILE, "\n");
					if ((global_horizontalforcefit > 0.0f)) fprintf(pFILE, "after horizontal force fit\n");
					if ((global_verticalforcefit > 0.0f)) fprintf(pFILE, "after vertical force fit\n");
					fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
					fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
				}
			}

			if (pFILE)
			{
				fprintf(pFILE, "\ndisplaying string %s at:\n", global_spitextstring.c_str());
				fprintf(pFILE, "\nglobal_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
				fflush(pFILE);
			}
			SetWindowPos(hwnd, NULL, global_x, global_y, mySIZE.cx, mySIZE.cy, SWP_NOZORDER);
			SelectObject(myHDC, prevHGDIOBJ);
			ReleaseDC(hwnd, myHDC);
			/*
			HRGN GGG = CreateRectRgn(0, 0, 1920, 1200);
			InvertRgn(GetDC(hWnd), GGG);
			SetWindowRgn(hWnd, GGG, false);
			*/
			//COLORREF RRR = RGB(255, 0, 255);
			//COLORREF global_keyingcolor = RGB(255, 0, 255);

			//2020sept22, spi, begin
			//SetLayeredWindowAttributes(hWnd, global_keyingcolor, (BYTE)0, LWA_COLORKEY);
			//SetLayeredWindowAttributes(hWnd, global_keyingcolor, (BYTE)global_alpha, LWA_COLORKEY);
			SetLayeredWindowAttributes(hwnd, global_keyingcolor, (BYTE)global_alpha, LWA_COLORKEY | LWA_ALPHA);
			if (pFILE)
			{
				fprintf(pFILE, "\n\nSetLayeredWindowAttributes() with global_alpha %d\n", global_alpha);
			}
			//2020sept22, spi, end
			/*
			SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(hWnd, 0, global_alpha, LWA_ALPHA);
			*/
			ShowWindow(hwnd, SW_SHOWNA);
			UpdateWindow(hwnd);
			/*
			DeleteObject(GGG);
			*/

			//SetTimer(hWnd, 1, 1000, NULL);

			//moved into InitInstance()
			if ((global_textmode > -1)) //|| ( (global_starttime_sec>=0)||(global_endtime_sec>=0) ) )
			{
				//when in counter modes
				//set timer to update display or to keep track of start time and kill time
				SetTimer(hwnd, ID_TIMER_EVERYSECOND, 1000, NULL); //every seconds timer
			}
			if ((global_textmode < 0) && (global_starttime_sec > 0))
			{
				//todo, launch a timer to wait for global_starttime_sec before display;
				//set timer to start to display
				SetTimer(hwnd, ID_TIMER_ONCESTART, global_starttime_sec * 1000, NULL);
				if (pFILE)
				{
					fprintf(pFILE, "SetTimer() ID_TIMER_ONCESTART with global_starttime_sec %d\n", global_starttime_sec);
				}
			}
			if ((global_textmode < 0) && (global_endtime_sec > 0) && ((global_endtime_sec - global_starttime_sec) > 0))
			{
				//todo, launch a timer to wait for global_timetodisplay_sec before killing app;
				//set timer to kill display
				//SetTimer(hWnd, ID_TIMER_ONCEKILL, (global_endtime_sec-global_starttime_sec)*1000, NULL); 
				SetTimer(hwnd, ID_TIMER_ONCEKILL, global_endtime_sec * 1000, NULL);
				if (pFILE)
				{
					fprintf(pFILE, "SetTimer() ID_TIMER_ONCEKILL with global_endtime_sec %d\n", global_endtime_sec);
				}
			}

			//return TRUE;
		}
		return 0;
	case WM_SIZE:
		{
			CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_ERASEBKGND:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		//FillRect((HDC)wParam, &rect, CreateSolidBrush(RGB(0, 0, 0)));
		//FillRect((HDC)wParam, &rect, CreateSolidBrush(RGB(255, 0, 255))); //keying color
		//2021nov20, spi, begin
		//FillRect((HDC)wParam, &rect, CreateSolidBrush(global_keyingcolor)); //keying color
		int result = FillRect((HDC)wParam, &rect, CreateSolidBrush(global_keyingcolor)); //keying color
		return result;
		//2021nov29, spi, end
	}
	break;

	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIMER_EVERYSECOND:
		case ID_TIMER_ONCESTART:
		case ID_TIMER_ONCEKILL:

			///////////////////
			//update time stamp
			///////////////////
			global_nowstamp_ms = GetTickCount();

			////////////////
			//for all modes
			///////////////
			//1) check for start condition
			if (wParam == ID_TIMER_ONCESTART)
			{
				//InvalidateRect(hwnd, NULL, FALSE);
				KillTimer(hwnd, ID_TIMER_ONCESTART);
			}
			//2) check for end condition
			else if (wParam == ID_TIMER_ONCEKILL)
			{
				int nShowCmd = false;
				//ShellExecuteA(NULL, "open", "c:\\temp\\batch.bat", "", NULL, nShowCmd);
				//ShellExecuteA(NULL, "open", "finishing.ahk", "", NULL, nShowCmd);
				if (!global_finishing.empty()) ShellExecuteA(NULL, "open", global_finishing.c_str(), "", NULL, nShowCmd);
				KillTimer(hwnd, ID_TIMER_ONCEKILL);
				//PostMessage(hwnd, WM_DESTROY, 0, 0);
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}

			////////////////////////
			//text string mode only
			///////////////////////
			if (global_textmode < 0 && ((wParam == ID_TIMER_ONCESTART) || (wParam == ID_TIMER_ONCEKILL)))
			{
				//nothing to do in here evrery second
				//only redraw when start and kill
				InvalidateRect(hwnd, NULL, FALSE);
				//2021dec29, spi, begin
				return 0;
				//2021dec29, spi, end
			}

			////////////////////
			//counter modes only
			////////////////////
			if (global_textmode == global_countermodeCLOCK)
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			else if (global_textmode == global_countermodeCOUNTUP)
			{
				//1) calculate time to display
				if (global_starttime_sec < 0) global_starttime_sec = 0;
				int elapsed_sec = (global_nowstamp_ms - global_startstamp_ms) / 1000;
				global_timetodisplay_sec = global_starttime_sec + elapsed_sec;
				InvalidateRect(hwnd, NULL, FALSE);

				//2) check for end condition
				if (global_endtime_sec > -1 && ((global_endtime_sec - global_starttime_sec) - elapsed_sec) < 1)
				{
					int nShowCmd = false;
					//ShellExecuteA(NULL, "open", "c:\\temp\\batch.bat", "", NULL, nShowCmd);
					//ShellExecuteA(NULL, "open", "finishing.ahk", "", NULL, nShowCmd);
					if (!global_finishing.empty()) ShellExecuteA(NULL, "open", global_finishing.c_str(), "", NULL, nShowCmd);
					KillTimer(hwnd, ID_TIMER_ONCEKILL);
					//PostMessage(hwnd, WM_DESTROY, 0, 0);
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
			}
			else if (global_textmode == global_countermodeCOUNTDOWN)
			{
				//calculate time to display
				if (global_starttime_sec < 0) global_starttime_sec = 0;
				int elapsed_sec = (global_nowstamp_ms - global_startstamp_ms) / 1000;
				global_timetodisplay_sec = global_starttime_sec - elapsed_sec;
				InvalidateRect(hwnd, NULL, FALSE);

				//2) check for end condition
				if(global_endtime_sec>-1 && ((global_starttime_sec-global_endtime_sec)-elapsed_sec)<1)
				{
					int nShowCmd = false;
					//ShellExecuteA(NULL, "open", "c:\\temp\\batch.bat", "", NULL, nShowCmd);
					//ShellExecuteA(NULL, "open", "finishing.ahk", "", NULL, nShowCmd);
					if (!global_finishing.empty()) ShellExecuteA(NULL, "open", global_finishing.c_str(), "", NULL, nShowCmd);
					KillTimer(hwnd, ID_TIMER_ONCEKILL);
					//PostMessage(hwnd, WM_DESTROY, 0, 0);
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				}
			}
			else
			{
				assert(false);
			}
			return 0;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		//SaveDC(hdc);
		//int nOldDrawingMode = SetROP2(hdc, R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.

		//int iOldBkMode = SetBkMode(hdc, TRANSPARENT);
		COLORREF crOldTextColor = SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
		HGDIOBJ hOldFont = (HFONT)SelectObject(hdc, global_hFont);


		if (global_textmode < 0)
		{
			global_nowstamp_ms = GetTickCount();
			int elapsed_sec = (global_nowstamp_ms - global_startstamp_ms) / 1000;
			if (global_starttime_sec > 0 && (global_starttime_sec - elapsed_sec) > 0)
			{
				//string text mode
				//sprintf(charbuffer, "%s", "delayed"); //empty display
				sprintf(charbuffer, "%s", ""); //empty display
			}
			else
			{
				//string text mode
				sprintf(charbuffer, "%s", global_spitextstring.c_str());
			}
		}
		else
		{
			SYSTEMTIME st;
			//GetSystemTime(&st);
			GetLocalTime(&st);
			int hh = st.wHour;
			int mm = st.wMinute;
			int ss = st.wSecond;
			if (global_textmode == global_countermodeCLOCK)
			{
				//sprintf(charbuffer, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
				//sprintf(charbuffer, "%02d.%02d:%02d", st.wHour, st.wMinute, st.wSecond);
				//sprintf(charbuffer, mytimeformat.c_str(), st.wHour, st.wMinute, st.wSecond);
			}
			else if (global_textmode == global_countermodeCOUNTUP)
			{
				hh = global_timetodisplay_sec / 3600;
				mm = (global_timetodisplay_sec % 3600) / 60;
				ss = global_timetodisplay_sec % 60;
				//sprintf(charbuffer, "%02d:%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, "%02d.%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, mytimeformat.c_str(), hh, mm, ss);
			}
			else if (global_textmode == global_countermodeCOUNTDOWN)
			{
				hh = global_timetodisplay_sec / 3600;
				mm = (global_timetodisplay_sec % 3600) / 60;
				ss = global_timetodisplay_sec % 60;
				//sprintf(charbuffer, "%02d:%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, "%02d.%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, mytimeformat.c_str(), hh, mm, ss);

			}
			else
			{
				//ERROR
				assert(false);
				hh = 99;
				mm = 99;
				ss = 99;
				//sprintf(charbuffer, "%02d:%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, "%02d.%02d:%02d", hh, mm, ss);
				//sprintf(charbuffer, mytimeformat.c_str(), hh, mm, ss);
			}
			string mytimeformat = "%02d.%02d:%02d";
			if (global_textformat == global_counterformatHHMMSS)
			{
				mytimeformat = "%02d.%02d:%02d";
				sprintf(charbuffer, mytimeformat.c_str(), hh, mm, ss);
			}
			else if (global_textformat == global_counterformatHHMM)
			{
				mytimeformat = "%02d.%02d";
				sprintf(charbuffer, mytimeformat.c_str(), hh, mm);
			}
			else if (global_textformat == global_counterformatMMSS)
			{
				mytimeformat = "%02d:%02d";
				sprintf(charbuffer, mytimeformat.c_str(), mm, ss);
			}
			else if (global_textformat == global_counterformatHH ||
				global_textformat == global_counterformatMM ||
				global_textformat == global_counterformatSS)
			{
				mytimeformat = "%02d";
				if (global_textformat == global_counterformatHH) sprintf(charbuffer, mytimeformat.c_str(), hh);
				else if (global_textformat == global_counterformatMM) sprintf(charbuffer, mytimeformat.c_str(), mm);
				else if (global_textformat == global_counterformatSS) sprintf(charbuffer, mytimeformat.c_str(), ss);
			}

		}

		//TextOutA(hdc, 50, 50, charbuffer, charbufferlength);
		int charbufferlength = strlen(charbuffer);
		//2021nov23, spi, begin
		//if(strcmp(charbuffer_prev, "")) DrawTextXOR(hdc, charbuffer_prev, strlen(charbuffer));
		if (strcmp(charbuffer_prev, "")) DrawTextXOR(hdc, charbuffer_prev, strlen(charbuffer_prev)); //when charbuffer_prev not empty, erase before drawing
		//2021nov23, spi, end
		DrawTextXOR(hdc, charbuffer, charbufferlength);

		SetTextColor(hdc, crOldTextColor);
		//SetBkMode(hdc, iOldBkMode);
		SelectObject(hdc, hOldFont);

		//SetROP2(hdc, nOldDrawingMode); 
		//RestoreDC(hdc, -1);

		EndPaint(hwnd, &ps);
		//2021nov20, spi, begin
		return 0;
		//2021nov20, spi, end
	}
	break;

	case WM_KEYDOWN:
	{
		CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;

	//2021nov23, spi, begin
	case WM_SETTEXT:
	{
		////////////////////////
		//text string mode only
		///////////////////////
		if (global_textmode < 0)
		{

			//erase previous text
			//global_spitextstring = "";
			//InvalidateRect(hWnd, NULL, FALSE);
			//Sleep(100);

			//get new text string
			WCHAR myWCHAR[1024];
			WCHAR myCHAR[1024];
			if (lParam != NULL)
			{
				//wsprintf(myWCHAR, L"%s", (WCHAR*)lParam);
				//wcscpy(myWCHAR, (WCHAR*)lParam);
				wstring mywstring((WCHAR*)lParam); //careful lParam is a non-null terminated string
				string mystring((CHAR*)lParam); //careful lParam is a non-null terminated string
				//global_spitextstring = utf8_encode(mywstring);
				global_spitextstring = utf8_encode(mywstring);
				if (global_spitextstring.empty()) global_spitextstring = SPITEXT_DEFAULTSTRING;
				//redraw
				//InvalidateRect(hWnd, NULL, FALSE);
				InvalidateRect(hwnd, NULL, TRUE);
				//2021nov20, spi, begin
				return true; //when text is set 
				//2021nov29, spi, end
			}
		}
		//2021nov20, spi, begin
		return false; //when text not set 
		//2021nov29, spi, end
	}
	break;
	//2021nov23, spi, end
	//2021nov29, spi, begin
	case WM_CLOSE:
		{
			if (_flags & SPITEXT_ABORTCLOSE_NOSHOWINSTEAD)
			{
				ShowWindow(hwnd, SW_HIDE); //SW_SHOWNA
				return 0;
			}
			if (p_spilogfile)
			{
				stringstream ss;
				ss << "CSPITextWindow processing WM_CLOSE and will now kill all timers, detach itself from msgRouter(), and call DestroyWindow()" << endl;
				p_spilogfile->writeline(ss.str());
			}
			KillTimer(hwnd, ID_TIMER_EVERYSECOND);
			KillTimer(hwnd, ID_TIMER_ONCESTART);
			KillTimer(hwnd, ID_TIMER_ONCEKILL);
			if (pFILE)
			{
				fclose(pFILE);
				pFILE = NULL;
			}
			//destructs only this CSPITextWindow 
			//1) untie base class message router
			this->msgRouter_detach();
			//2) erase and destroy CSPITextWindow's hwnd
			//WPARAM wparam = ID_TIMER_ONCEKILL;
			//LPARAM lparam = 0;
			//SendMessage(hwnd, WM_TIMER, wparam, lparam);
			//3) delete this class safely
			//delete this;
			DestroyWindow(hwnd);
			//return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_DESTROY:
		if (p_spilogfile)
		{
			stringstream ss;
			ss << "CSPITextWindow processing WM_DESTROY and is now to be killing all timers etc." << endl;
			p_spilogfile->writeline(ss.str());
		}
		//DeleteObject(global_hFont); //moved into destructor, we still need it to erase here below
		KillTimer(hwnd, ID_TIMER_EVERYSECOND);
		KillTimer(hwnd, ID_TIMER_ONCESTART);
		KillTimer(hwnd, ID_TIMER_ONCEKILL);
		//if (!global_end.empty()) ShellExecuteA(NULL, "open", global_end.c_str(), "", NULL, 0);
		if (pFILE)
		{
			fclose(pFILE);
			pFILE = NULL;
		}
		if (_flags & SPITEXT_NOPOSTQUITMESSAGE)
		{
			if (p_spilogfile)
			{
				stringstream ss;
				ss << "CSPITextWindow is not calling PostQuitMessage(0), instead ..." << endl;
				p_spilogfile->writeline(ss.str());
			}

			//0) force erase display now 
			//erasing works perfectly like that but let's leave screen dirty while debugging/finding how to delete this class and its hwnd properly
			global_textmode = -1; //force textmode
			global_spitextstring = ""; //force empty empty text string
			InvalidateRect(hwnd, NULL, FALSE); //InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
			
			//destructs only this CSPITextWindow 
			//1) untie base class message router
			//this->msgRouter_detach();
			//2) erase and destroy CSPITextWindow's hwnd
			//WPARAM wparam = ID_TIMER_ONCEKILL;
			//LPARAM lparam = 0;
			//SendMessage(hwnd, WM_TIMER, wparam, lparam);
			//3) delete this class safely
			delete this;
			//DestroyWindow(hwnd);
			//return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
			//return 1;
			return 0;
		}
		else
		{
			//destroys the whole application
			PostQuitMessage(0);
			return 0;
		}
		return 0;
	case WM_NCDESTROY:
		if (p_spilogfile)
		{
			stringstream ss;
			ss << "CSPITextWindow processing WM_NCDESTROY and is now to be deleting the this pointer" << endl;
			p_spilogfile->writeline(ss.str());
		}
		//delete this;
		return 0;
	//2021nov29, spi, end
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	//2021nov29, spi, begin
	return 0;
	//2021nov29, spi, end
}

bool CSPITextWindow::justification(SIZE mySIZE)
{
	bool didjustify = false;

	//horizontal justification
	if (pFILE && !global_horizontaljustification.empty())
	{
		fprintf(pFILE, "\nbefore horizontal justification\n");
		fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
		fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
	}
	if (global_horizontaljustification == "left")
	{
	}
	else if (global_horizontaljustification == "right")
	{
		if ((globalxyabsolute != 0) && (global_fullmonitorssurface == 0))
		{
			global_x = global_x - mySIZE.cx;
		}
		else
		{
			global_x = myoutputRECT.right - mySIZE.cx;
		}
		didjustify = true;
	}
	else if (global_horizontaljustification == "center")
	{
		if ((globalxyabsolute != 0) && (global_fullmonitorssurface == 0)) global_x = global_x - mySIZE.cx / 2;
		else global_x = myoutputRECT.left + (myoutputRECT.right - myoutputRECT.left) / 2 - mySIZE.cx / 2;
		didjustify = true;
	}
	if (pFILE && !global_horizontaljustification.empty())
	{
		fprintf(pFILE, "\nafter horizontal justification\n");
		fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
		fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
	}
	//vertical justification
	if (pFILE && !global_verticaljustification.empty())
	{
		fprintf(pFILE, "\nbefore vertical justification\n");
		fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
		fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
	}
	if (global_verticaljustification == "top")
	{
	}
	else if (global_verticaljustification == "bottom")
	{
		if ((globalxyabsolute != 0) && (global_fullmonitorssurface == 0)) global_y = global_y - mySIZE.cy;
		else global_y = myoutputRECT.bottom - mySIZE.cy;
		didjustify = true;
	}
	else if (global_verticaljustification == "center")
	{
		if ((globalxyabsolute != 0) && (global_fullmonitorssurface == 0)) global_y = global_y - mySIZE.cy / 2;
		else global_y = myoutputRECT.top + (myoutputRECT.bottom - myoutputRECT.top) / 2 - mySIZE.cy / 2;
		didjustify = true;
	}
	if (pFILE && !global_verticaljustification.empty())
	{
		fprintf(pFILE, "\nafter vertical justification\n");
		fprintf(pFILE, "global_x, global_y, mySIZE.cx, mySIZE.cy are %d %d %d %d\n", global_x, global_y, mySIZE.cx, mySIZE.cy);
		fprintf(pFILE, "global_fontheight is %d\n", global_fontheight);
	}
	return didjustify;
}

void CSPITextWindow::DrawTextXOR(HDC hdc, const char* charbuffer, int charbufferlength)
{

	//2021nov23, spi, begin
	if (charbuffer == NULL || charbufferlength < 1) return;
	//2021nov23, spi, end

	HDC myMemHDC = CreateCompatibleDC(hdc);
	HFONT hOldFont_memhdc = (HFONT)SelectObject(myMemHDC, global_hFont);
	SIZE mySIZE2;
	GetTextExtentPoint32A(myMemHDC, charbuffer, charbufferlength, &mySIZE2);

	HBITMAP myHBITMAP = CreateCompatibleBitmap(hdc, mySIZE2.cx, mySIZE2.cy);
	HGDIOBJ prevHBITMAP = SelectObject(myMemHDC, myHBITMAP);
	//COLORREF crOldBkColor = SetBkColor(myMemHDC, RGB(0xFF, 0xFF, 0xFF));
	//COLORREF crOldBkColor = SetBkColor(myMemHDC, RGB(0x00, 0x00, 0xFF));
	//COLORREF crOldBkColor = SetBkColor(myMemHDC, RGB(0x00, 0x00, 0x00));
	//COLORREF crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0xFF, 0xFF, 0xFF)); //not visible
	COLORREF crOldTextColor_memhdc;
	if (global_idfontcolor == 0)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0xFF, 0x00, 0xFF)); //white
	}
	else if (global_idfontcolor == 1)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0x00, 0xFF, 0xFF)); //blue
	}
	else if (global_idfontcolor == 2)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0xFF, 0xFF, 0x00)); //red
	}
	else if (global_idfontcolor == 3)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0x00, 0xFF, 0x00)); //black
	}
	else if (global_idfontcolor == 4)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0xFF, 0x00, 0x00)); //yellow
	}
	else if (global_idfontcolor == 5)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0xA0, 0x00, 0x20)); //green lime
	}
	else if (global_idfontcolor == 6)
	{
		crOldTextColor_memhdc = SetTextColor(myMemHDC, RGB(0x00, 0x00, 0x00)); //green
	}

	//int nOldDrawingMode_memhdc = SetROP2(myMemHDC, R2_NOTXORPEN); //XOR mode, always have to erase what's drawn.
	//int iOldBkMode_memhdc = SetBkMode(myMemHDC, TRANSPARENT);
	//HFONT hOldFont_memhdc=(HFONT)SelectObject(myMemHDC,global_hFont);
	//TextOutA(myMemHDC, 1, 1, "test string", 11);
	TextOutA(myMemHDC, 0, 0, charbuffer, charbufferlength);
	strcpy(charbuffer_prev, charbuffer);
	//Rectangle(myMemHDC, 0, 0, 1000, 800);
	//BitBlt(hdc, 0, 0, 1000, 800, myMemHDC, 0, 0, SRCCOPY); 
	BitBlt(hdc, 0, 0, mySIZE2.cx, mySIZE2.cy, myMemHDC, 0, 0, 0x00990066); //XOR mode, always have to erase what's drawn.
	//BitBlt(hdc, global_x, global_y, mySIZE2.cx, mySIZE2.cy, myMemHDC, 0, 0, 0x00990066); //XOR mode, always have to erase what's drawn.
	SelectObject(myMemHDC, prevHBITMAP);
	DeleteDC(myMemHDC);
	DeleteObject(myHBITMAP);
	//DeleteDC(myMemHDC2);
	return;
}
