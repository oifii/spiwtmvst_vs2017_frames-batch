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
#if !defined __CSPITextWindow_H__
#define __CSPITextWindow_H__

#include "spiutility.h"
//#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//it is better not to change the following because the code depends on these, instead, copy/paste and modify if need new ones
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPITEXT_EXAMPLE_0	"\"SPI.RUN\" -1 3600 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2.1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_1	"\"COUNTDOWN HH:MM:SS\" -1 10 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2.1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_11	"\"COUNTDOWN HH:MM:SS\" -1 10 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1.1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_2	"\"CLOCK HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2.1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "

//used with voroguiwindow
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_QUOTESTRINGQUOTE		7
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_SPACEXSPACE			16
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_Y						18
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_SPACEFONTHEIGHTSPACE	22
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_HWND					86
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_LT	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"left\" \"top\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_LB	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"left\" \"bottom\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_RT	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"right\" \"top\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_RB	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"right\" \"bottom\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_CC	"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "

#define SPITEXT_EXAMPLE_MONITOR_1_1_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_2_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:2\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_3_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:3\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_4_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:4\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_5_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:5\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_6_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:6\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_7_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:7\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_8_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:8\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_9_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:9\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_10_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:10\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_11_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:11\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_12_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:12\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_13_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:13\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_14_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:14\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_15_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:15\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_1_16_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"1:16\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "

#define SPITEXT_EXAMPLE_MONITOR_2_1_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_2_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:2\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_3_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:3\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_4_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:4\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_5_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:5\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_6_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:6\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_7_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:7\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_8_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:8\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_9_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:9\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_10_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:10\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_11_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:11\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_12_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:12\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_13_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:13\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_14_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:14\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_15_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:15\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_2_16_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"2:16\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "

#define SPITEXT_EXAMPLE_MONITOR_3_1_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:1\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_2_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:2\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_3_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:3\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_4_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:4\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_5_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:5\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_6_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:6\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_7_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:7\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_8_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:8\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_9_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:9\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_10_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:10\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_11_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:11\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_12_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:12\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_13_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:13\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_14_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:14\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_15_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:15\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
#define SPITEXT_EXAMPLE_MONITOR_3_16_16	"\"COUNTDOWN HH:MM:SS\" -1 60 0 0 1920 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"3:16\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "


#define SPITEXT_NOPOSTQUITMESSAGE			1
#define SPITEXT_NOPARAMETERS				2
#define SPITEXT_ABORTCLOSE_NOSHOWINSTEAD	4


#define MAX_LOADSTRING 256

 /*
 #define SPICOUNTERMODE_COUNTUP		0
 #define SPICOUNTERMODE_COUNTDOWN	1
 #define SPICOUNTERMODE_CLOCK		2
 */
#define ID_TIMER_EVERYSECOND	1
#define ID_TIMER_ONCESTART		2
#define ID_TIMER_ONCEKILL		3


#define SPITEXT_DEFAULTSTRING	" "




class CSPITextWindow : public CWindowParameter //public CWindowMultimonitor 
{
public:
	//2021nov29, spi, begin
	DWORD _CREATEWINDOWEX_FLAGS1; // = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT;
	DWORD _CREATEWINDOWEX_FLAGS2; // = WS_POPUP;
	HINSTANCE _hinstance;
	DWORD _flags;
	spilogfile* p_spilogfile;
	//2021nov29, spi, end

	//TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	//TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
	TCHAR szTitle[1024] = { L"spitexttitle" };					// The title bar text
	TCHAR szWindowClass[1024] = { L"spitextclass" };			// the main window class name

	int global_x = 0;
	int global_y = 0;
	//2021nov23, spi, begin
	//string global_spitextstring="some text";
	string global_spitextstring = SPITEXT_DEFAULTSTRING;
	//2021nov23, spi, end
	HFONT global_hFont = NULL; //spi
	int global_fontheight = 480;
	int global_fontwidth = -1; //will be computed within WM_PAINT handler

	int global_starttime_sec = -1; //user specified, -1 for not specified
	int global_endtime_sec = -1; //user specified, -1 for not specified

	int global_timetodisplay_sec; //calculated
	DWORD global_startstamp_ms;
	DWORD global_nowstamp_ms;

	char charbuffer[1024] = { "" };
	char charbuffer_prev[1024] = { "" };

	BYTE global_alpha = 220;

	//global_monitor is either empty "" (global_x and global_y will be considered absolute) 
	//or "1" or "2" or "2.1" or "2.2" or "2.3" or "2.4" specifying monitor (global_x and global_y will be considered relative)
	//global_monitor can also be using ":" instead of "." in which case there will be 16 posisble submonitors instead of 4, i.e. "1:1", "1:2", "1:3", ..., "1:15", "1:16", etc. for specifying monitor (global_x and global_y will be considered relative)
	string global_monitor = "";
	//global_hmonitor is either an empty string "" (global_x and global_y will be considered absolute) or the HMONITOR numeric value embeded into a string i.e. "0x00001E76" specifying a monitor handle (global_x and global_y will be considered relative)
	string global_hmonitor = "";
	//global_hwnd is either an empty string "" (global_x and global_y will be considered absolute) or the HWND numeric value embeded into a string i.e. "0x00001E76" specifying a window handle (global_x and global_y will be considered relative)
	string global_hwnd = "";
	//global_windowclass is either an empty string "" (global_x and global_y will be considered absolute) or the window class string i.e. "windowclassofinterest" (global_x and global_y will be considered relative)
	string global_windowclass = "";
	//global_windowtitle is either an empty string "" (global_x and global_y will be considered absolute) or the window title string i.e. "windowtitleofinterest" (global_x and global_y will be considered relative)
	string global_windowtitle = "";
	//global_horizontaljustification is either an empty string "" (global_x and global_y will be considered) or i.e. "left", "right", "center" (global_x and global_y will be ignored)
	string global_horizontaljustification = "";
	//global_verticaljustification is either an empty string "" (global_x and global_y will be considered) or i.e. "top", "bottom", "center" (global_x and global_y will be ignored)
	string global_verticaljustification = "";

	//global_horizontalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
	//int global_horizontalforcefit = 0;
	float global_horizontalforcefit = 0.0f;
	//global_vecticalforcefit is either 0 for no force fit or 1 for yes force fit (global_fontheight may be adjusted, only considered when global_x and global_y are relative to a monitor or to a window rect)
	//int global_verticalforcefit = 0;
	float global_verticalforcefit = 0.0f;

	int global_fullmonitorssurface = 1; //global_fullmonitorssurface is either 0 for no or 1, the default, for yes do display string over multiple monitors

	//new parameters
	//#include <atlconv.h>
	//string global_fontface="Segoe Script"; //see charmap.exe for fontface (Win+R>charmap)
	string global_fontface = "Arial"; //see charmap.exe for fontface (Win+R>charmap)
	int global_idfontcolor = 0;
	//string global_classname="spicounterclass";
	//string global_title="spicountertitle";
	//string global_begin="begin.ahk";
	string global_begin = "";
	//string global_starting="starting.ahk";
	string global_starting = "";
	//string global_finishing="finishing.ahk";
	string global_finishing = "";
	//string global_end="end.ahk";
	string global_end = "";


	//keying color (to be made transparent color)
	COLORREF global_keyingcolor = RGB(255, 0, 255);


	//these structures initialize themselves automatically with the list of monitors 
	//and the list of all top-level windows on the screens
	MonitorRects _monitors;
	EnumWindowsStruct_spitext _windows;
	//initially presume global_x and global_y coordinate absolute
	POINT myoutputPOINT;

	bool globalxyabsolute = true;
	RECT myoutputRECT;

	FILE* pFILE = NULL;
	//SIZE mySIZE;

	int global_textmode = -1; //defaults to -1 to display straight text
	int global_textformat = -1; //defaults to -1 to display straight text otherwise used by the counter modes

	vector<string> global_countermode;
	vector<string> global_counterformat;

	int global_countermodeCOUNTUP = 0;
	int global_countermodeCOUNTDOWN = 1;
	int global_countermodeCLOCK = 2;

	int global_counterformatHHMMSS = 0;
	int global_counterformatHHMM = 1;
	int global_counterformatMMSS = 2;
	int global_counterformatHH = 3;
	int global_counterformatMM = 4;
	int global_counterformatSS = 5;

public:
	CSPITextWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName, LPCTSTR commandlinearguments = L"", DWORD flags=0, //unicode commandline identical to spitext.exe app
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);	
	CSPITextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName, LPCSTR commandlinearguments = "", DWORD flags = 0, //multibyte commandline identical to spitext.exe app
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	void Initialize();
	~CSPITextWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy() {}
	virtual void UpdateParameter(string name);
	virtual VOID Register(LPWNDCLASSEX lpwndClassEx);
	virtual HWND Create();
	virtual HWND Create(HWND hWndParent);
	virtual HWND Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
		INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam);
	bool justification(SIZE mySIZE);
	void DrawTextXOR(HDC hdc, const char* charbuffer, int charbufferlength);
};


#endif