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
#pragma once

#ifndef _SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_H
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_H


#include "resource.h"
#include <map>
#include <string>






//2021dec29, spi, begin
//#define VOROGUISPITEXTWINDOW_VECTORINDEX_CONTROLVALUE	0
//#include "cspitextwindow.h"
//2021dec29, spi, end

#ifndef COMPILING_SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_CPP
extern int global_debugmode;
extern int global_spitextmode;
extern int global_spivoicemode;
extern int global_spirecordmode;
extern int global_cpulocalmode;
extern int global_cpuglobalmode;
extern int global_memlocalmode;
extern int global_memglobalmode;
//class CSPITextWindow* get_voroguispitextwindow_vector();
extern int global_x;
extern int global_y;
extern int global_xwidth;
extern int global_yheight;
#endif
void launchspitextwindow(LPCTSTR text, HWND hwnd, HINSTANCE hinstance = NULL);
void launchspitextwindow(LPCTSTR text, RECT rect, HWND hwnd, HINSTANCE hinstance = NULL);





 //needs administrator priviledge under windows 10
 //#define SPISLIDESHOWWIN32_VS2013_HKEYROOT	L"HKEY_LOCAL_MACHINE"	
 //using root key for current user instead
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_HKEYROOT	HKEY_CURRENT_USER
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_REGSUBKEY	L"SOFTWARE\\audiospi.com\\spiwtmvst_vs2017"	

//2021nov29, spi, begin
//only enable these when debugging these classes
//enabling debug for these will generate a lot of debug txt file
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPISUBRECT_DEBUG		0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPISUBRECT_DEBUG		1
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MONITORRECTS_DEBUG		0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MONITORRECTS_DEBUG	1
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MULTIMONITOR_DEBUG		0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MULTIMONITOR_DEBUG	1

#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPITEXTWINDOW_DEBUG			0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPITEXTWINDOW_DEBUG		1
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPIVOROGUIWINDOW_DEBUG		0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPIVOROGUIWINDOW_DEBUG	1

#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_IMAGEWINDOW_DEBUG	0
//#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_IMAGEWINDOW_DEBUG	1

#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MAX_PHYSICALMEMORYUSAGE_MB		(30*1024)
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MIN_PHYSICALMEMORYNOTTOUSE_MB	2048
#define SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MAX_CPUUSAGE_INPERCENT			80.00f
//2021nov29, spi, end



#endif //SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_H
