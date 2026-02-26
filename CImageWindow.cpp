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

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
 //#define new DBG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "spiwindowtransparentmidivoroguispectrumtimeframe.h"
#include "spiutility.h"

#include "FreeImage.h"
#include "defs.h"
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"

#include "CParameter.h"
#include "CWindowParameter.h"

#include "CImageWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>

//#include <mmsystem.h>

//spi, begin
//2018apr24, frames-batch
#include <direct.h>
#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL
#include "spivorogui.h"
#include "CVoroguiWindow.h"
#include "CVorogsWindow.h"
extern CVorogsWindow* global_pVorogsWindow;
extern vector<string> global_imagefilenames;
//extern int global_xwidth;
//extern int global_yheight;

//spi, end
using namespace std;



CImageWindow::CImageWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
	FIBITMAP* dib)
: CWindowParameter(lpszCaption, lpwndClassEx)
{
	_dib = dib;
	Initialize();
}

CImageWindow::CImageWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	FIBITMAP* dib,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_dib = dib;
	Initialize();
}

CImageWindow::CImageWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	FIBITMAP* dib,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_dib = dib;
	Initialize();
}

void CImageWindow::Initialize()
{
	//2021nov29, spi, begin
	p_spilogfile = NULL;
	if (SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_IMAGEWINDOW_DEBUG)
	{
		p_spilogfile = DBG_NEW spilogfile("cimagewindow_debug.txt", true);
	}
	//2021nov29, spi, end

	//spi, begin
	//2018april24, frames-batch
	//global_ffmpegpath = "c:\\video\\ffmpeg.exe";
	//global_ffmpegpath = "d:\\spibin\\ffmpeg\\ffmpeg.exe";
	global_ffmpegpath = FFMPEG_APP_PATH;
	//global_outputvideofoldername = "c:\\temp\\spiwtmvstf-shuffle";
	global_outputvideofoldername = "d:\\temp-spiwtmvstf-shuffle";
	global_outputvideofilenameext = "mp4";
	global_maxnumberofframeperseedimage = 200;
	global_outputvideoframepersecond = 10;
	global_framefilenameprefix = "frame_";
	global_framefilenameext = "bmp";

	currentlysavingframe = false;
	skipsaveframes = true;
	global_outputfoldername = "frames";
	global_frameid = 0;
	//spi, end

	_dc = 0;
	_bmp=0;
	//2021nov29, spi, begin
	prevHGDIOBJ = NULL;
	_prev_specmode = 0;
	prevHGDIOBJ_ref = NULL;
	_dc_ref = 0;
	_bmp_ref =0;
	_buf_ref =0;
	//2021nov29, spi, end
	_buf=NULL;
	_mode=1; //spi, was 0
	_WIDTH=-1; //display width
	_HEIGHT=-1; //display height 

	_canvas_ready=false;
	_busy_painting=false;

	_timer = 0;

	addParameter("img_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
	addParameter("img_mode", 0.0, 4.0, _mode);

	addParameter("img_redrawperiod", 0.050, 1.0, 0.050);

	addParameter("img_bgcolorid_r", 0.0, 255.0, 0.0);
	addParameter("img_bgcolorid_g", 0.0, 255.0, 0.0);
	addParameter("img_bgcolorid_b", 0.0, 255.0, 0.0);

	addParameter("img_idcolorlowest_r", 0.0, 255.0, 64.0);
	addParameter("img_idcolorhighest_r", 0.0, 255.0, 96.0);
	addParameter("img_idcolorlowest_g", 0.0, 255.0, 64.0);
	addParameter("img_idcolorhighest_g", 0.0, 255.0, 96.0);
	addParameter("img_idcolorlowest_b", 0.0, 255.0, 64.0);
	addParameter("img_idcolorhighest_b", 0.0, 255.0, 96.0);

	addParameter("img_mixratio", 0.0, 1.0, 0.5);

	addParameter("img_att_r", 0.0, 1.0, 1.0);
	addParameter("img_att_g", 0.0, 1.0, 0.5);
	addParameter("img_att_b", 0.0, 1.0, 1.0);

	addParameter("img_invprob", 2.0, 10.0, 2.0);

	//addParameter("img_nx", 1.0, 40.0, 4.0); //spi, was 10.0
	//addParameter("img_ny", 1.0, 40.0, 5.0); //spi, was 10.0
	addParameter("img_nx", 1.0, 40.0, 10.0); //spi, was causing an error with 50.0!!!
	addParameter("img_ny", 1.0, 40.0, 10.0); //spi, was 10.0
	//addParameter("img_nx", 1.0, 40.0, 1.0); //spi, was 10.0
	//addParameter("img_ny", 1.0, 40.0, 20.0); //spi, was 10.0
}

void CImageWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "img_showwnd")
	{
		float showflag = getParameterValue("img_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}

	if (name == "img_redrawperiod")
	{
		if (_hwnd)
		{
			KillTimer(_hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE);
			float redrawperiod_s = getParameterValue("img_redrawperiod");
			SetTimer(_hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE, redrawperiod_s * 1000, (TIMERPROC)NULL);
		}
	}
	//some parameters updated in UpdateDisplay()
}

//spi, begin
//2018apr24, frames-batch modif
void CImageWindow::saveframetobmpfile(HWND hwnd, HDC hdc)
{
	currentlysavingframe = true;

	global_frameid++;

	if (global_frameid <= global_maxnumberofframeperseedimage)
	{
		///////////////////////////////////
		//1) save frame to disk (as a .bmp)
		///////////////////////////////////
		//HDC hdcScreen;
		HDC hdcWindow;
		HDC hdcMemDC = NULL;
		HBITMAP hbmScreen = NULL;
		BITMAP bmpScreen;

		// Retrieve the handle to a display device context for the client 
		// area of the window. 
		//hdcScreen = GetDC(NULL);
		hdcWindow = hdc; //hdcWindow = GetDC(g_hWnd);

		// Create a compatible DC which is used in a BitBlt from the window DC
		hdcMemDC = CreateCompatibleDC(hdcWindow);
		if (!hdcMemDC)
		{
			currentlysavingframe = false;
			return;
		}
		RECT rcClient;
		GetClientRect(hwnd, &rcClient); //GetClientRect(g_hWnd, &rcClient);
		hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top);
		if (!hbmScreen)
		{
			DeleteObject(hdcMemDC);
			currentlysavingframe = false;
			return;
		}

		HBITMAP oldhbitmap = (HBITMAP)SelectObject(hdcMemDC, hbmScreen);

		if (!BitBlt(hdcMemDC,
			0, 0,
			rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
			hdcWindow,
			0, 0,
			SRCCOPY))
		{
			SelectObject(hdcMemDC, oldhbitmap);
			DeleteObject(hbmScreen);
			DeleteObject(hdcMemDC);
			currentlysavingframe = false;
			return;
		}
		GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

		BITMAPFILEHEADER   bmfHeader;
		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bmpScreen.bmWidth;
		bi.biHeight = bmpScreen.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 *
			bmpScreen.bmHeight;

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
		char *lpbitmap = (char *)GlobalLock(hDIB);

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		GetDIBits(hdcWindow, hbmScreen, 0,
			(UINT)bmpScreen.bmHeight,
			lpbitmap,
			(BITMAPINFO *)&bi, DIB_RGB_COLORS);

		// A file is created, this is where we will save the screen capture.
		string filename = global_outputfoldername;
		filename += "\\"; // "\\frame_";
		filename += global_framefilenameprefix;
		//global_frameid++;
		char buf[256];
		sprintf(buf, "%06d", global_frameid);
		filename += buf;
		filename += "."; //".bmp";
		filename += global_framefilenameext;
		HANDLE hFile = CreateFileA(filename.c_str(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		// Add the size of the headers to the size of the bitmap to get the total file size
		DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER)+
			sizeof(BITMAPINFOHEADER);

		//Offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+
			(DWORD)sizeof(BITMAPINFOHEADER);

		//Size of the file
		bmfHeader.bfSize = dwSizeofDIB;

		//bfType must always be BM for Bitmaps
		bmfHeader.bfType = 0x4D42; //BM   

		DWORD dwBytesWritten = 0;
		WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

		//Unlock and Free the DIB from the heap
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);

		//Close the handle for the file that was created
		CloseHandle(hFile);

		//Clean up
		/*
		done:
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		//ReleaseDC(NULL,hdcScreen);
		//ReleaseDC(g_hWnd,hdcWindow);
		*/
		SelectObject(hdcMemDC, oldhbitmap);
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		currentlysavingframe = false;
	}
	//2024feb22, spi, begin
	//else
	else if(global_pVorogsWindow!=NULL)
	//2024feb22, spi, end
	{
		string base_seedimagefilename = global_imagefilenames[global_pVorogsWindow->GetImageID()].substr(global_imagefilenames[global_pVorogsWindow->GetImageID()].find_last_of("/\\") + 1);
		size_t dotindex = base_seedimagefilename.find_last_of(".");
		string base_seedimagefilename_noext = base_seedimagefilename.substr(0, dotindex);

		////////////////////////////
		//1) convert frames to video
		////////////////////////////
		_mkdir(global_outputvideofoldername.c_str());

		char nxbyny[64];
		int nx = (int) getParameterValue("img_nx");
		int ny = (int) getParameterValue("img_ny");
		sprintf(nxbyny, "(nx%d-ny%d)", nx, ny);

		string videocutpath;
		videocutpath = global_outputvideofoldername + "\\" + base_seedimagefilename_noext + "_img-shuffle" + nxbyny + "." + global_outputvideofilenameext;
		char bufferfps[64];
		int integerfps = (int)global_outputvideoframepersecond;
		if (integerfps<1) integerfps = 1;
		if (integerfps>60) integerfps = 60;
		sprintf(bufferfps, "%d", integerfps);
		char bufferscale[64];
		//sprintf(bufferscale, "%dx%d", global_xwidth, global_yheight);
		sprintf(bufferscale, "%dx%d", _WIDTH, _HEIGHT);
		
		char absolutepath[_MAX_PATH];
		_fullpath(absolutepath, global_outputfoldername.c_str(), _MAX_PATH);

		string systemcommand;
		systemcommand = global_ffmpegpath + " -r " + bufferfps + " -s " + bufferscale + " -start_number 1 -i " + absolutepath + "\\" + global_framefilenameprefix + "%6d." + global_framefilenameext \
			+ " -vcodec libx264 -crf 25 -pix_fmt yuv420p " + videocutpath;
		//debug begin
		FILE* pFILE1 = fopen("commands.txt", "w");
		if (pFILE1)
		{
			fprintf(pFILE1, "%s\n", systemcommand.c_str());
			fclose(pFILE1);
		}
		//debug end
		system(systemcommand.c_str());

		////////////////////////////////////
		//2) copy .bmp frames to .jpg images
		////////////////////////////////////
		string fullpath = global_outputvideofoldername + "\\" + base_seedimagefilename_noext + "_img-shuffle" + nxbyny;
		_mkdir(fullpath.c_str());
		for (int i = 1; i < global_frameid; i++)
		{
			string bmp_filepath = global_outputfoldername;
			bmp_filepath += "\\";
			string filename = "frame_";
			char buf[256];
			sprintf(buf, "%06d", i);
			filename += buf;
			bmp_filepath += filename;
			bmp_filepath += ".bmp";
			//FIBITMAP* myFIBITMAP = FreeImage_Load(FIF_BMP, bmp_filepath.c_str(), 0);
			FIBITMAP* myFIBITMAP = FreeImage_Load(FIF_BMP, bmp_filepath.c_str(), BMP_DEFAULT);
			if (myFIBITMAP)
			{
				string jpg_filepath = fullpath;
				jpg_filepath += "\\";
				jpg_filepath += filename;
				jpg_filepath += ".jpg";
				FIBITMAP* my24bitFIBITMAP = FreeImage_ConvertTo24Bits(myFIBITMAP);
				if (my24bitFIBITMAP)
				{
					BOOL bresult = FreeImage_Save(FIF_JPEG, my24bitFIBITMAP, jpg_filepath.c_str(), JPEG_QUALITYGOOD);
					FreeImage_Unload(my24bitFIBITMAP);
				}
				FreeImage_Unload(myFIBITMAP);
			}
			///////////////////////
			//3) delete .bmp frames
			///////////////////////
			DeleteFileA(bmp_filepath.c_str());
		}

		/////////////////////////
		//4) reset global_frameid
		/////////////////////////
		global_frameid = 0;
		currentlysavingframe = false;

		///////////////////////////
		//5) select next seed image
		///////////////////////////
		//PostMessage(global_pVoroguiWindow->GetHWND(), WM_CHAR, LOWORD('N'), 0);
		PostMessage(global_pVorogsWindow->GetHWND(), WM_KEYDOWN, LOWORD('N'), 0);
		
	}
	return;

}
//spi, end

CImageWindow::~CImageWindow()
{
	if (_hwnd)
	{
		KillTimer(_hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE);
	}
	while (_busy_painting) Sleep(1);
	if (prevHGDIOBJ) SelectObject(_dc, prevHGDIOBJ); //unselect hbitmap
	if (_dc) DeleteDC(_dc);
	if (_bmp) DeleteObject(_bmp);
	//2021nov29, spi, begin
	//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
	if (prevHGDIOBJ_ref) SelectObject(_dc_ref, prevHGDIOBJ_ref); //unselect hbitmap
	if (_dc_ref) DeleteDC(_dc_ref);
	if (_bmp_ref) DeleteObject(_bmp_ref);
	if (p_spilogfile) delete p_spilogfile;
	//2021nov29, spi, end
}

LRESULT CALLBACK CImageWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			CreateCanvas();
			SetTimer(hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE, 50, (TIMERPROC)NULL);
	}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case IMAGEWINDOW_IDTIMER_UPDATEIMAGE:
			// process the 50-ms timer 
			UpdateDisplay();
			return 0;
			/*
		case SPECTRUMWINDOW_IDTIMER_UPDATEPARAMETER:
			// process the 1 second timer 
			UpdateParameters();
			return 0;
			*/
		}
	case WM_SIZE:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
			CreateCanvas();
		}
		return 0;
	case WM_KEYDOWN:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CImageWindow::CreateCanvas(int numbyteperpixel/*=3*/)
{ 
	if (_hwnd == NULL) return;

	_canvas_ready = false;
	while (_busy_painting) Sleep(1);
	//2021nov29, spi, begin
	/*
	if (_dc) DeleteDC(_dc);
	if (_bmp) DeleteObject(_bmp);
	*/
	if (prevHGDIOBJ) SelectObject(_dc, prevHGDIOBJ); //unselect hbitmap
	if (_dc)
	{
		DeleteDC(_dc);
		_dc = 0;
	}
	if (_bmp)
	{
		DeleteObject(_bmp);
		_bmp = 0;
	}
	//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
	if (prevHGDIOBJ_ref) SelectObject(_dc_ref, prevHGDIOBJ_ref); //unselect hbitmap
	if (_dc_ref)
	{
		DeleteDC(_dc_ref);
		_dc_ref = 0;
	}
	if (_bmp_ref)
	{
		DeleteObject(_bmp_ref);
		_bmp_ref = 0;
	}
	//2021nov29, spi, end

	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	_WIDTH = rcClient.right - 0;
	_HEIGHT = rcClient.bottom - 0;
	//2021nov29, spi, begin
	/*
	_WIDTH = _WIDTH - _WIDTH % 4; //ensure _WIDTH is divisible by 4
	if (_WIDTH == 0 || _HEIGHT == 0) return;
	*/
	if ( (_WIDTH <= (_WIDTH % 4)) || (_HEIGHT <= 0) ) return;
	if (_WIDTH % 4)
	{
		_WIDTH = _WIDTH - (_WIDTH % 4); //ensure _WIDTH is divisible by 4
	}
	if (_WIDTH <= 0) return;
	//2021nov29, spi, end

	// create bitmap to draw spectrum in (24 bit for easy updating)
	BYTE data[2000] = { 0 };
	BITMAPINFOHEADER *bh = (BITMAPINFOHEADER*)data;
	RGBQUAD *pal = (RGBQUAD*)(data + sizeof(*bh));
	int a;
	bh->biSize = sizeof(*bh);
	bh->biWidth = _WIDTH;
	bh->biHeight = _HEIGHT; // upside down (line 0=bottom)
	bh->biSizeImage = _WIDTH * _HEIGHT * numbyteperpixel; // 3;
	bh->biPlanes = 1;
	bh->biCompression = BI_RGB;
	bh->biBitCount = 8 * numbyteperpixel; // 24;
	bh->biClrUsed = bh->biClrImportant = 0;

	// create the bitmap
	_bmp = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, (void**)&_buf, NULL, 0);
	//2021nov29, spi, begin
	//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
	_bmp_ref = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, (void**)&_buf_ref, NULL, 0);
	HDC dc = 0; // GetDC(_hwnd);
	if (dc)
	{
		_dc = CreateCompatibleDC(dc);
	}
	else
	{
		_dc = CreateCompatibleDC(0);
	}
	//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
	if (dc)
	{
		_dc_ref = CreateCompatibleDC(dc);
	}
	else
	{
		_dc_ref = CreateCompatibleDC(0);
	}

	//HBITMAP oldhbitmap = NULL; 
	//HBITMAP oldhbitmap_ref = NULL;
	if (_bmp && _dc && _bmp_ref && _dc_ref) //if (_bmp && _dc)
	{
		prevHGDIOBJ = SelectObject(_dc, _bmp);
		//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		prevHGDIOBJ_ref = SelectObject(_dc_ref, _bmp_ref);
		//if(0) //not for now while debugging
		if (_dib)
		{
			//now optimized, this was slow, because was done on each update no matter for which drawing mode
			int dib_width = FreeImage_GetWidth(_dib);
			int dib_height = FreeImage_GetHeight(_dib);
			if (dib_width>0 && dib_height>0)
			{
				BYTE* pBYTE = FreeImage_GetBits(_dib);
				BITMAPINFO* pBITMAPINFO = FreeImage_GetInfo(_dib);
				if (pBYTE && pBITMAPINFO)
				{
					//todo, could we avoid stretch blitting to _dc (are we certain it is always fully written on each update()?)
					SetStretchBltMode(_dc, COLORONCOLOR);
					StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
						0, 0, dib_width, dib_height,
						pBYTE, pBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
					//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
					SetStretchBltMode(_dc_ref, COLORONCOLOR);
					StretchDIBits(_dc_ref, 0, 0, _WIDTH, _HEIGHT,
						0, 0, dib_width, dib_height,
						pBYTE, pBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
				}
			}
		}
		else
		{
			//do nothing for now
		}
		//prevHGDIOBJ = SelectObject(_dc, _bmp); //moved before stretchdibits
		_canvas_ready = true;
		if(dc) ReleaseDC(_hwnd, dc);
		return;
	}
	else
	{
		//error, return safely
		if (_dc)
		{
			//_bmp is 0
			//SelectObject(_dc, prevHGDIOBJ); //unselect hbitmap
			DeleteDC(_dc);
			_dc = 0;
		}
		if (_bmp)
		{
			DeleteObject(_bmp);
			_bmp = 0;
		}
		if (dc) ReleaseDC(_hwnd, dc);
		//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		if (_dc_ref)
		{
			//_bmp is 0
			//SelectObject(_dc, prevHGDIOBJ); //unselect hbitmap
			DeleteDC(_dc_ref);
			_dc_ref = 0;
		}
		if (_bmp_ref)
		{
			DeleteObject(_bmp_ref);
			_bmp_ref = 0;
		}
		_canvas_ready = false;
		if (dc) ReleaseDC(_hwnd, dc);
		return;
	}
	if (dc) ReleaseDC(_hwnd, dc);
	_canvas_ready = false;
	return;
	//2021nov29, spi, end
}

// update the image display - the interesting bit :)
void CImageWindow::UpdateDisplay(int numbyteperpixel/*=3*/)
{
	if (!IsWindowVisible()) return;
	if (!_canvas_ready)
	{
		CreateCanvas(); //was not ready in the case of borderless window
		return;
	}
	int bcanvas_dirty = false;

	_busy_painting = true;

	_mode = (int) (getParameterValue("img_mode")+0.5f);

	int bgcolorid_r = (int)getParameterValue("img_bgcolorid_r");
	int bgcolorid_g = (int)getParameterValue("img_bgcolorid_g");
	int bgcolorid_b = (int)getParameterValue("img_bgcolorid_b");

	float fcolorlowest_r = getParameterValue("img_idcolorlowest_r");
	float fcolorhighest_r = getParameterValue("img_idcolorhighest_r");
	int idcolorlowest_r = 0;
	int idcolorhighest_r = 0;
	if (fcolorlowest_r <= fcolorhighest_r)
	{
		idcolorlowest_r = (int)fcolorlowest_r;
		idcolorhighest_r = (int)fcolorhighest_r;
	}
	else
	{
		idcolorlowest_r = (int)fcolorhighest_r;
		idcolorhighest_r = (int)fcolorlowest_r;
	}
	float fcolorlowest_g = getParameterValue("img_idcolorlowest_g");
	float fcolorhighest_g = getParameterValue("img_idcolorhighest_g");
	int idcolorlowest_g = 0;
	int idcolorhighest_g = 0;
	if (fcolorlowest_g <= fcolorhighest_g)
	{
		idcolorlowest_g = (int)fcolorlowest_g;
		idcolorhighest_g = (int)fcolorhighest_g;
	}
	else
	{
		idcolorlowest_g = (int)fcolorhighest_g;
		idcolorhighest_g = (int)fcolorlowest_g;
	}
	float fcolorlowest_b = getParameterValue("img_idcolorlowest_b");
	float fcolorhighest_b = getParameterValue("img_idcolorhighest_b");
	int idcolorlowest_b = 0;
	int idcolorhighest_b = 0;
	if (fcolorlowest_b <= fcolorhighest_b)
	{
		idcolorlowest_b = (int)fcolorlowest_b;
		idcolorhighest_b = (int)fcolorhighest_b;
	}
	else
	{
		idcolorlowest_b = (int)fcolorhighest_b;
		idcolorhighest_b = (int)fcolorlowest_b;
	}

	float fmixratio = getParameterValue("img_mixratio");

	float fattenuation_r = getParameterValue("img_att_r");
	float fattenuation_g = getParameterValue("img_att_g");
	float fattenuation_b = getParameterValue("img_att_b");

	int inverseprobability = (int)(getParameterValue("img_invprob")+0.5);

	int nx = (int)getParameterValue("img_nx");
	int ny = (int)getParameterValue("img_ny");

	if (p_spilogfile)
	{
		stringstream ss;
		ss << "mode = " << _mode << endl;
		ss << "nx = " << nx << endl;
		ss << "ny = " << ny << endl;
		p_spilogfile->writeline(ss.str());
	}
	if (bcanvas_dirty)
	{
		_busy_painting = false;
		CreateCanvas();
		return;
	}

	if (!_dib)
	{
		//no image
		_busy_painting = false;
		if (_hwnd) KillTimer(_hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE);
		return;
	}

	//2021nov29, spi, begin
	//HBITMAP oldhbitmap = (HBITMAP)SelectObject(_dc, _bmp);
	//memset(_buf, 0, _WIDTH*_HEIGHT * 3);
	//2021nov29, spi, end

	if (_mode == 0)
	{
		//2021nov29, spi, begin
		/* moved into createcanvas()
		SetStretchBltMode(_dc, COLORONCOLOR);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
			0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
			FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);

		int nicell = nx;
		int njcell = ny;
		int cellwidth = _WIDTH / nicell;
		int cellheight = _HEIGHT / njcell;
		for (int icell = 0; icell<nicell; icell++)
		{
			for (int jcell = 0; jcell<njcell; jcell++)
			{
				int ioffset = icell*cellwidth;
				int joffset = jcell*cellheight;

				int random_integer1 = RandomInt(0, 1);

				int cellcolor_r = 0;
				int cellcolor_g = 0;
				int cellcolor_b = 0;
				if (random_integer1 == 0)
				{
					cellcolor_r = RandomInt(idcolorlowest_r, idcolorhighest_r);
					cellcolor_g = RandomInt(idcolorlowest_g, idcolorhighest_g);
					cellcolor_b = RandomInt(idcolorlowest_b, idcolorhighest_b);
				}
				else
				{
					cellcolor_r = bgcolorid_r;
					cellcolor_g = bgcolorid_g;
					cellcolor_b = bgcolorid_b;
				}

				int random_integer2 = RandomInt(0, 1);
				if (random_integer2 == 1)
				{
					for (int i = ioffset; i<(ioffset + cellwidth); i++)
					{
						for (int j = joffset; j<(joffset + cellheight); j++)
						{
							//set pixel
							_buf[j*_WIDTH * 3 + i * 3] = cellcolor_b; //B
							_buf[j*_WIDTH * 3 + i * 3 + 1] = cellcolor_g; //G
							_buf[j*_WIDTH * 3 + i * 3 + 2] = cellcolor_r; //R
						}
					}
				}
			}
		}
		*/
		bytebuffer_cellset_setpixel(_buf, _WIDTH, _HEIGHT, 3, 
									{ (BYTE)bgcolorid_b, (BYTE)bgcolorid_g, (BYTE)bgcolorid_r, (BYTE)0 }, 
									nx, ny, 2, 2, //pixels and cells inverse probabilities of 2 and 2, means 1/2 and 1/2, meaning 50% and 50% chances
									{ (BYTE)idcolorlowest_b, (BYTE)idcolorlowest_g, (BYTE)idcolorlowest_r, (BYTE)0 }, 
									{ (BYTE)idcolorhighest_b, (BYTE)idcolorhighest_g, (BYTE)idcolorhighest_r, (BYTE)0 }, 
									1);
		//2021nov29, spi, end
	}
	else if (_mode==1)
	{
		//2021nov29, spi, begin
		/*	//moved into createcanvas() for speed optimization especially with large dib,
			//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		SetStretchBltMode(_dc, COLORONCOLOR);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
			0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
			FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);
		*/
		//2021nov29, spi, end
		/*
		int nicell = nx;
		int njcell = ny;
		int cellwidth = _WIDTH / nicell;
		int cellheight = _HEIGHT / njcell;
		for (int ii = 0; ii < (fmixratio*nx*ny); ii++)
		{
			//src
			int icell = RandomInt(0, nicell-1);
			int jcell = RandomInt(0, njcell-1);
			int ioffset = icell*cellwidth;
			int joffset = jcell*cellheight;
			//dst
			int iicell = RandomInt(0, nicell-1);
			int jjcell = RandomInt(0, njcell-1);
			int iioffset = iicell*cellwidth;
			int jjoffset = jjcell*cellheight;
			//copy pixels
			for (int i = ioffset; i<(ioffset + cellwidth); i++)
			{
				for (int j = joffset; j<(joffset + cellheight); j++)
				{
					//get pixel
					//2021nov29, spi, begin
					//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
					//BYTE b = _buf[j*_WIDTH * 3 + i * 3]; //B
					//BYTE g = _buf[j*_WIDTH * 3 + i * 3 + 1]; //G
					//BYTE r = _buf[j*_WIDTH * 3 + i * 3 + 2]; //R					
					BYTE b = _buf_ref[j*_WIDTH * 3 + i * 3]; //B
					BYTE g = _buf_ref[j*_WIDTH * 3 + i * 3 + 1]; //G
					BYTE r = _buf_ref[j*_WIDTH * 3 + i * 3 + 2]; //R
					//2021nov29, spi, end
					//set pixel
					int iii = i - ioffset + iioffset;
					int jjj = j - joffset + jjoffset;
					_buf[jjj*_WIDTH * 3 + iii * 3] = b; //B
					_buf[jjj*_WIDTH * 3 + iii * 3 + 1] = g; //G
					_buf[jjj*_WIDTH * 3 + iii * 3 + 2] = r; //R
				}
			}
		}
		*/
		bytebuffer_cellset_getsetpixel(_buf_ref, _buf, _WIDTH, _HEIGHT, 3,
										{ (BYTE)bgcolorid_b, (BYTE)bgcolorid_g, (BYTE)bgcolorid_r, (BYTE)0 },
										nx, ny, 1, 1, //pixels and cells inverse probabilities of 1 and 1, means 1/1 and 1/1, meaning 100% and 100% chances to get _buf_ref color
										{ (BYTE)idcolorlowest_b, (BYTE)idcolorlowest_g, (BYTE)idcolorlowest_r, (BYTE)0 },
										{ (BYTE)idcolorhighest_b, (BYTE)idcolorhighest_g, (BYTE)idcolorhighest_r, (BYTE)0 },
										1,
										fmixratio, 1.0, 1.0, 1.0); // no attenuation
	}
	else if (_mode == 2)
	{
		//2021nov29, spi, begin
		/*	//moved into createcanvas() for speed optimization especially with large dib,
			//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		SetStretchBltMode(_dc, COLORONCOLOR);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
			0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
			FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);
		*/
		//2021nov29, spi, end
		/*
		int nicell = nx;
		int njcell = ny;
		int cellwidth = _WIDTH / nicell;
		int cellheight = _HEIGHT / njcell;
		for (int ii = 0; ii < (fmixratio*nx*ny); ii++)
		{
			//src
			int icell = RandomInt(0, nicell - 1);
			int jcell = RandomInt(0, njcell - 1);
			int ioffset = icell*cellwidth;
			int joffset = jcell*cellheight;
			//dst
			int iicell = RandomInt(0, nicell - 1);
			int jjcell = RandomInt(0, njcell - 1);
			int iioffset = iicell*cellwidth;
			int jjoffset = jjcell*cellheight;
			//copy pixels
			for (int i = ioffset; i<(ioffset + cellwidth); i++)
			{
				for (int j = joffset; j<(joffset + cellheight); j++)
				{
					//get pixel
					//2021nov29, spi, begin
					//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
					//BYTE b = _buf[j*_WIDTH * 3 + i * 3]; //B
					//BYTE g = _buf[j*_WIDTH * 3 + i * 3 + 1]; //G
					//BYTE r = _buf[j*_WIDTH * 3 + i * 3 + 2]; //R					
					BYTE b = _buf_ref[j*_WIDTH * 3 + i * 3]; //B
					BYTE g = _buf_ref[j*_WIDTH * 3 + i * 3 + 1]; //G
					BYTE r = _buf_ref[j*_WIDTH * 3 + i * 3 + 2]; //R
					//2021nov29, spi, end
					//set pixel (with attenuation)
					int iii = i - ioffset + iioffset;
					int jjj = j - joffset + jjoffset;
					_buf[jjj*_WIDTH * 3 + iii * 3] = b*fattenuation_b; //B
					_buf[jjj*_WIDTH * 3 + iii * 3 + 1] = g*fattenuation_g; //G
					_buf[jjj*_WIDTH * 3 + iii * 3 + 2] = r*fattenuation_r; //R
				}
			}
		}
		*/
		bytebuffer_cellset_getsetpixel(_buf_ref, _buf, _WIDTH, _HEIGHT, 3,
										{ (BYTE)bgcolorid_b, (BYTE)bgcolorid_g, (BYTE)bgcolorid_r, (BYTE)0 },
										nx, ny, 1, 1, //pixels and cells inverse probabilities of 1 and 1, means 1/1 and 1/1, meaning 100% and 100% chances to get _buf_ref color
										{ (BYTE)idcolorlowest_b, (BYTE)idcolorlowest_g, (BYTE)idcolorlowest_r, (BYTE)0 },
										{ (BYTE)idcolorhighest_b, (BYTE)idcolorhighest_g, (BYTE)idcolorhighest_r, (BYTE)0 },
										1,
										fmixratio, fattenuation_b, fattenuation_g, fattenuation_r); //some attenuation
	}
	else if (_mode == 3)
	{
		//2021nov29, spi, begin
		/*	//moved into createcanvas() for speed optimization especially with large dib,
			//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		SetStretchBltMode(_dc, COLORONCOLOR);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
			0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
			FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);
		*/
		//2021nov29, spi, end

		/*
		int nicell = nx;
		int njcell = ny;
		int cellwidth = _WIDTH / nicell;
		int cellheight = _HEIGHT / njcell;
		for (int ii = 0; ii < (fmixratio*nx*ny); ii++)
		{
			//src
			int icell = RandomInt(0, nicell - 1);
			int jcell = RandomInt(0, njcell - 1);
			int ioffset = icell*cellwidth;
			int joffset = jcell*cellheight;
			//dst
			int iicell = RandomInt(0, nicell - 1);
			int jjcell = RandomInt(0, njcell - 1);
			int iioffset = iicell*cellwidth;
			int jjoffset = jjcell*cellheight;
			//copy pixels
			for (int i = ioffset; i<(ioffset + cellwidth); i++)
			{
				for (int j = joffset; j<(joffset + cellheight); j++)
				{
					//get pixel
					//2021nov29, spi, begin
					//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref					
					//BYTE b = _buf[j*_WIDTH * 3 + i * 3]; //B
					//BYTE g = _buf[j*_WIDTH * 3 + i * 3 + 1]; //G
					//BYTE r = _buf[j*_WIDTH * 3 + i * 3 + 2]; //R										
					BYTE b = _buf_ref[j*_WIDTH * 3 + i * 3]; //B
					BYTE g = _buf_ref[j*_WIDTH * 3 + i * 3 + 1]; //G
					BYTE r = _buf_ref[j*_WIDTH * 3 + i * 3 + 2]; //R
					//2021nov29, spi, end
					int dice = RandomInt(0,inverseprobability);
					if (dice == 0)
					{
						//set pixel (random)
						int iii = i - ioffset + iioffset;
						int jjj = j - joffset + jjoffset;
						_buf[jjj*_WIDTH * 3 + iii * 3] = RandomInt(0,255); //B
						_buf[jjj*_WIDTH * 3 + iii * 3 + 1] = RandomInt(0, 255); //G
						_buf[jjj*_WIDTH * 3 + iii * 3 + 2] = RandomInt(0, 255); //R
					}
					else
					{
						//set pixel (identical)
						int iii = i - ioffset + iioffset;
						int jjj = j - joffset + jjoffset;
						_buf[jjj*_WIDTH * 3 + iii * 3] = b; //B
						_buf[jjj*_WIDTH * 3 + iii * 3 + 1] = g; //G
						_buf[jjj*_WIDTH * 3 + iii * 3 + 2] = r; //R
					}
				}
			}
		}
		*/
		bytebuffer_cellset_getsetpixel(_buf_ref, _buf, _WIDTH, _HEIGHT, 3,
										{ (BYTE)bgcolorid_b, (BYTE)bgcolorid_g, (BYTE)bgcolorid_r, (BYTE)0 },
										nx, ny, 1, inverseprobability, //pixels and cells inverse probabilities of inverseprobability and 1, means 1/inverseprobability and 1/1, meaning 50% when inverseprobability=2 for pixel color and 100% chances for cell
										{ (BYTE)idcolorlowest_b, (BYTE)idcolorlowest_g, (BYTE)idcolorlowest_r, (BYTE)0 },
										{ (BYTE)idcolorhighest_b, (BYTE)idcolorhighest_g, (BYTE)idcolorhighest_r, (BYTE)0 },
										1,
										fmixratio, 1.0, 1.0, 1.0); //no attenuation

	}
	else if (_mode == 4)
	{
		//2021nov29, spi, begin
		/*	//moved into createcanvas() for speed optimization especially with large dib,
			//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		SetStretchBltMode(_dc, COLORONCOLOR);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT,
			0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
			FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);
		*/
		//2021nov29, spi, end

		int nicell = nx;
		int njcell = ny;
		//int nicell = 16;
		//int njcell = 16;
		int cellwidth = _WIDTH / nicell;
		int cellheight = _HEIGHT / njcell;

		//cellwidth has got to be a multiple of 4 because it will be stretchblit later
		if (!(cellwidth % 4 == 0)) cellwidth=cellwidth-cellwidth%4;

		//src
		int icell = RandomInt(0, nicell - 1);
		int jcell = RandomInt(0, njcell - 1);
		int ioffset = icell*cellwidth;
		int joffset = jcell*cellheight;
		//dst (first cell)
		int iicell = 0;
		int jjcell = 0;
		int iioffset = iicell*cellwidth;
		int jjoffset = jjcell*cellheight;
		//copy pixels
		for (int i = ioffset; i<(ioffset + cellwidth); i++)
		{
			for (int j = joffset; j<(joffset + cellheight); j++)
			{
				//get pixel
				//2021nov29, spi, begin
				//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
				
				//BYTE b = _buf[j*_WIDTH * 3 + i * 3]; //B
				//BYTE g = _buf[j*_WIDTH * 3 + i * 3 + 1]; //G
				//BYTE r = _buf[j*_WIDTH * 3 + i * 3 + 2]; //R
				
				BYTE b = _buf_ref[j*_WIDTH * 3 + i * 3]; //B
				BYTE g = _buf_ref[j*_WIDTH * 3 + i * 3 + 1]; //G
				BYTE r = _buf_ref[j*_WIDTH * 3 + i * 3 + 2]; //R
				//2021nov29, spi, end

				//set pixel (identical)
				int iii = i - ioffset + iioffset;
				int jjj = j - joffset + jjoffset;
				
				//_buf[jjj*_WIDTH * 3 + iii * 3] = b; //B
				//_buf[jjj*_WIDTH * 3 + iii * 3 + 1] = g; //G
				//_buf[jjj*_WIDTH * 3 + iii * 3 + 2] = r; //R
				
				_buf[jjj*cellwidth * 3 + iii * 3] = b; //B
				_buf[jjj*cellwidth * 3 + iii * 3 + 1] = g; //G
				_buf[jjj*cellwidth * 3 + iii * 3 + 2] = r; //R
			}
		}
		//BYTE data[2000] = { 0 };
		//BITMAPINFOHEADER *bh = (BITMAPINFOHEADER*)data;
		//RGBQUAD *pal = (RGBQUAD*)(data + sizeof(*bh));
		//bh->biSize = sizeof(*bh);
		//bh->biWidth = cellwidth;
		//bh->biHeight = cellheight; // upside down (line 0=bottom)
		//bh->biSizeImage = cellwidth*cellheight * 3;
		//bh->biPlanes = 1;
		//bh->biCompression = BI_RGB;
		//bh->biBitCount = 24;
		//bh->biClrUsed = bh->biClrImportant = 0;
		//create bitmap info header for this first cell
		BITMAPINFO myBITMAPINFO;
		myBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		myBITMAPINFO.bmiHeader.biWidth = cellwidth;
		myBITMAPINFO.bmiHeader.biHeight = cellheight;
		myBITMAPINFO.bmiHeader.biSizeImage = cellwidth*cellheight * 3;
		myBITMAPINFO.bmiHeader.biPlanes = 1;
		myBITMAPINFO.bmiHeader.biCompression = BI_RGB;
		myBITMAPINFO.bmiHeader.biBitCount = 3 * 8; // 24;
		myBITMAPINFO.bmiHeader.biClrUsed = 0;
		myBITMAPINFO.bmiHeader.biClrImportant = 0;
		myBITMAPINFO.bmiHeader.biXPelsPerMeter = 0;
		myBITMAPINFO.bmiHeader.biYPelsPerMeter = 0;

		//stretchblit this first cell onto the whole memory _dc
		//2021nov29, spi, begin
		//SetStretchBltMode(_dc, COLORONCOLOR);
		//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
		//StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT, 0, 0, cellwidth, cellheight,
		//	//(const void*)&_buf[0], (BITMAPINFO*)bh, DIB_RGB_COLORS, SRCCOPY);
		//	(const void*)&_buf[0], (BITMAPINFO*)&myBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
		StretchDIBits(_dc, 0, 0, _WIDTH, _HEIGHT, 0, 0, cellwidth, cellheight,
			(const void*)&_buf[0], (BITMAPINFO*)&myBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
		//2021nov29, spi, end
	}

	// update the display
	HDC dc = GetDC(_hwnd);
	BitBlt(dc, 0, 0, _WIDTH, _HEIGHT, _dc, 0, 0, SRCCOPY); 
	//2021nov29, spi, begin
	//if (oldhbitmap) SelectObject(_dc, oldhbitmap);
	//2021nov29, spi, end

	//spi, begin
	//2018apr24, frames-batch modif
	if (!skipsaveframes)
	{
		saveframetobmpfile(_hwnd, dc);
	}
	//spi, end

	ReleaseDC(_hwnd, dc);
	_busy_painting = false;
}

void CImageWindow::InvalidateDIB(FIBITMAP* newdib)
{
	_dib = newdib;
	_canvas_ready = false;
}