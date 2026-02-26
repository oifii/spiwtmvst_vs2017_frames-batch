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

#include <GL/gl.h>
#include <GL/glu.h>
#include "ChannelBuffersHandler.h"
#include "SpectrumDisplay.h"
#include "AutocorrDisplay.h"
#include "WaveformDisplay.h"
#include "AutocorrHandler.h"
#include <math.h>
#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL
#include "AutoRescaleOw2doc.h"

#include "CTimeframeWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>

//#include <mmsystem.h>

//2021oct01, spi, begin
#include <direct.h>
//#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL
#include "spivorogui.h"
#include "CVoroguiWindow.h"
#include "CVorogsWindow.h"
extern CVorogsWindow* global_pVorogsWindow;
extern vector<string> global_imagefilenames;
//2021oct01, spi, end

using namespace std;


CTimeframeWindow::CTimeframeWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
	string imagefilename, vector<string> imagefilenames)
: CWindowParameter(lpszCaption, lpwndClassEx)
{
	_imagefilename = imagefilename;
	_imagefilenames = imagefilenames;
	Initialize();
}

CTimeframeWindow::CTimeframeWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	string imagefilename, vector<string> imagefilenames,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_imagefilename = imagefilename;
	_imagefilenames = imagefilenames;
	Initialize();
}

CTimeframeWindow::CTimeframeWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	string imagefilename, vector<string> imagefilenames,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_imagefilename = imagefilename;
	_imagefilenames = imagefilenames;
	Initialize();
}

void CTimeframeWindow::Initialize()
{
	//2021oct01, spi, begin
	//modif frames-batch
	//global_ffmpegpath = "c:\\video\\ffmpeg.exe";
	//global_ffmpegpath = "d:\\spibin\\ffmpeg\\ffmpeg.exe";
	global_ffmpegpath = FFMPEG_APP_PATH;
	global_outputvideofoldername = "d:\\temp-spiwtmvstf-vorotrans-anim";
	global_outputvideofilenameext = "mp4";
	global_maxnumberofframeperseedimage = 10; //for debugging
	//global_maxnumberofframeperseedimage = 300; //usual
	global_outputvideoframepersecond = 10;
	global_framefilenameprefix = "frame_";
	global_framefilenameext = "bmp";

	currentlysavingframe = false;
	skipsaveframes = true;
	global_outputfoldername = "frames";
	global_frameid = 0;
	//2021oct01, spi, end

	_audiobuffer_ready = false;
	/*
	_dc=0;
	_bmp=0;
	_buf=NULL;
	*/
	_mode=1;  
	_xwidth=-1; //display width
	_yheight=-1; //display height 
	
	_imageid = -1;

	_canvas_ready=false;
	_busy_painting=false;

	_timer = 0;

	_inverseprobability = 1;
	_thetacounterthreshold = 150;
	_linewidth = 1.0;
	_pointsize = 1.0;
	_polygonfill = 0;

	_bgcolor_r = 0.0;
	_bgcolor_g = 0.0;
	_bgcolor_b = 0.0;
	_bgcolor_a = 0.0;

	_color1_r = 1.0;
	_color1_g = 0.0;
	_color1_b = 0.0;
	_color2_r = 0.0;
	_color2_g = 0.0;
	_color2_b = 1.0;
	_color3_r = 1.0;
	_color3_g = 0.0;
	_color3_b = 1.0;
	_color4_r = 0.0;
	_color4_g = 1.0;
	_color4_b = 0.0;

	_pOW2Doc = NULL;
	_pOW2View = NULL;
	_thetacounter = 0;
	_hDC = NULL; //returned by opengl InitGL()
	_hRC = NULL; //returned by opengl InitGL() 

	unsigned int bufferFrames = FRAMES_PER_BUFFER;
	//Initialize global buffers and objects
	_callbackFilledAudioBuffers = DBG_NEW ChannelBuffersHandler(bufferFrames, NUM_CHANNELS);
	_drawnFromAudioBuffers = DBG_NEW ChannelBuffersHandler(bufferFrames, NUM_CHANNELS);

	//Initialize Display Objects 
	_waveformDisplayL = DBG_NEW WaveformDisplay(bufferFrames, 10, 380, 10, 80, 0, bufferFrames, -1.0, 1.0);
	_autocorrDisplayL = DBG_NEW AutocorrDisplay(bufferFrames, 10, 380, 10, 80, 0, bufferFrames, -8, 8);
	_specDisplayMix = DBG_NEW SpectrumDisplay(bufferFrames, 2 * bufferFrames, "Hamming", "log", -7, 14, -5, 9, 0, SAMPLE_RATE / 2, -50.0, 20.0);

	//Waterfall Objects 
	//Initialize watefall variables
	//waterfallCount = 20; //original
	_waterfallCount = 2; //spi
	_waterfallDisplayYValuesHistory = DBG_NEW float*[_waterfallCount];
	for (int i = 0; i < _waterfallCount; i++)
	{
		_waterfallDisplayYValuesHistory[i] = DBG_NEW float[2 * bufferFrames];
		memset(_waterfallDisplayYValuesHistory[i], -30, 2 * bufferFrames * sizeof(float));
	}
	_waterfallReadIndex = 1;
	_waterfallWriteIndex = 0;


	// Initialize the coolness variables
	_autocorrHandler = DBG_NEW AutocorrHandler(bufferFrames);
	//	trailCount = 10;	//2048 buffersize
	//trailCount = 20; //original
	_trailCount = 2; //spi
	_autocorrBuffer = DBG_NEW SAMPLE[bufferFrames];
	memset(_autocorrBuffer, 0.0, bufferFrames * sizeof(SAMPLE));
	_displayPointsHistory1 = DBG_NEW Point3D*[_trailCount];
	for (int i = 0; i < _trailCount; i++)
	{
		_displayPointsHistory1[i] = DBG_NEW Point3D[bufferFrames];
		memset(_displayPointsHistory1[i], 0, bufferFrames * sizeof(Point3D));
	}
	_displayPointsHistory2 = DBG_NEW Point3D*[_trailCount];
	for (int i = 0; i < _trailCount; i++)
	{
		_displayPointsHistory2[i] = DBG_NEW Point3D[bufferFrames];
		memset(_displayPointsHistory2[i], 0, bufferFrames * sizeof(Point3D));
	}
	_trailReadIndex = 1;
	_trailWriteIndex = 0;
	_autocorrSkip1 = 10; //original
	//autocorrSkip1 = 1; //spi
	_autocorrSkip2 = 80; //original
	//autocorrSkip2 = 2; //spi
	//Set display variables
	_scaleFactor3D = 0.015* 4.0;
	_scaleFactor2D = 2.4*4.0;
	_theta = 0.0;
	_showWindows = false; //original
	//showWindows = true; //spi
	_show3D = true;

	//Global thread locking / management variables --------------
	//pthread_mutex_t g_mutex;
	_ready = false;



	addParameter("tf_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
	addParameter("tf_mode", 0.0, 5.0, _mode);

	addParameter("tf_redrawperiod", 0.050, 1.0, 0.050);

	addParameter("tf_invprob", 1.0, 100.0, _inverseprobability);
	addParameter("tf_thetathres", 1.0, 300.0, _thetacounterthreshold);
	addParameter("tf_linewidth", 1.0, 50.0, _linewidth);
	addParameter("tf_pointsize", 1.0, 50.0, _pointsize);
	addParameter("tf_polygonfill", 0.0, 1.0, _polygonfill, ParameterTypeToggle);

	addParameter("tf_bgcolor_r", 0.0, 1.0, _bgcolor_r);
	addParameter("tf_bgcolor_g", 0.0, 1.0, _bgcolor_g);
	addParameter("tf_bgcolor_b", 0.0, 1.0, _bgcolor_b);
	addParameter("tf_bgcolor_a", 0.0, 1.0, _bgcolor_a);

	addParameter("tf_color1_r", 0.0, 1.0, _color1_r);
	addParameter("tf_color1_g", 0.0, 1.0, _color1_g);
	addParameter("tf_color1_b", 0.0, 1.0, _color1_b);
	addParameter("tf_color2_r", 0.0, 1.0, _color2_r);
	addParameter("tf_color2_g", 0.0, 1.0, _color2_g);
	addParameter("tf_color2_b", 0.0, 1.0, _color2_b);
	addParameter("tf_color3_r", 0.0, 1.0, _color3_r);
	addParameter("tf_color3_g", 0.0, 1.0, _color3_g);
	addParameter("tf_color3_b", 0.0, 1.0, _color3_b);
	addParameter("tf_color4_r", 0.0, 1.0, _color4_r);
	addParameter("tf_color4_g", 0.0, 1.0, _color4_g);
	addParameter("tf_color4_b", 0.0, 1.0, _color4_b);

	/*
	addParameter("tf_bgcolorid_r", 0.0, 255.0, 0.0);
	addParameter("tf_bgcolorid_g", 0.0, 255.0, 0.0);
	addParameter("tf_bgcolorid_b", 0.0, 255.0, 0.0);

	addParameter("tf_idcolorlowest_r", 0.0, 255.0, 64.0);
	addParameter("tf_idcolorhighest_r", 0.0, 255.0, 96.0);
	addParameter("tf_idcolorlowest_g", 0.0, 255.0, 64.0);
	addParameter("tf_idcolorhighest_g", 0.0, 255.0, 96.0);
	addParameter("tf_idcolorlowest_b", 0.0, 255.0, 64.0);
	addParameter("tf_idcolorhighest_b", 0.0, 255.0, 96.0);

	addParameter("tf_mixratio", 0.0, 1.0, 0.5);

	addParameter("tf_att_r", 0.0, 1.0, 1.0);
	addParameter("tf_att_g", 0.0, 1.0, 0.5);
	addParameter("tf_att_b", 0.0, 1.0, 1.0);

	addParameter("tf_invprob", 2.0, 10.0, 2.0);

	addParameter("tf_nx", 1.0, 40.0, 10.0);
	addParameter("tf_ny", 1.0, 40.0, 10.0);
	*/
}

void CTimeframeWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "tf_showwnd")
	{
		float showflag = getParameterValue("tf_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}

	if (name == "tf_redrawperiod")
	{
		if (_hwnd)
		{
			KillTimer(_hwnd, TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY);
			float redrawperiod_s = getParameterValue("tf_redrawperiod");
			SetTimer(_hwnd, TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY, redrawperiod_s * 1000, (TIMERPROC)NULL);
		}
	}
	//some parameters updated in UpdateDisplay()
}

CTimeframeWindow::~CTimeframeWindow()
{
	if (_hwnd)
	{
		KillTimer(_hwnd, TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY);
	}
	while (_busy_painting) Sleep(1);
	/*
	if (_dc) DeleteDC(_dc);
	if (_bmp) DeleteObject(_bmp);
	*/
	if (_pOW2Doc) delete _pOW2Doc;
	if (_pOW2View) delete _pOW2View;

	//CloseGL(hWnd, global_hDC, hRC);
	CloseGL(_hwnd, _hDC, _hRC);

	delete _callbackFilledAudioBuffers;
	delete _drawnFromAudioBuffers;
	delete _specDisplayMix;
	delete _waveformDisplayL;
	delete _autocorrDisplayL;
	delete _autocorrHandler;
	delete[] _autocorrBuffer;
	for (int i = 0; i < _trailCount; i++)
	{
		delete[] _displayPointsHistory1[i];
		delete[] _displayPointsHistory2[i];
	}
	delete[] _displayPointsHistory1;
	delete[] _displayPointsHistory2;
	for (int i = 0; i < _waterfallCount; i++)
	{
		delete[] _waterfallDisplayYValuesHistory[i];
	}
	delete[] _waterfallDisplayYValuesHistory;
}

//2021oct01, spi, begin
//modif frames batch
//leaving function here, but now using saveopenglframetobmpfile()
void CTimeframeWindow::saveframetobmpfile(HWND hwnd, HDC hdc)
{
	currentlysavingframe = true;
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
	filename += "\\frame_";
	//global_frameid++;
	char buf[256];
	sprintf(buf, "%06d", global_frameid);
	filename += buf;
	filename += ".bmp";
	HANDLE hFile = CreateFileA(filename.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
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
	return;

}

//void SaveAsBMP(const char *fileName);
//void SwitchVoronoiDiagram();
void CTimeframeWindow::saveopenglframetobmpfile()
{
	currentlysavingframe = true;

	global_frameid++;

	if (global_frameid <= global_maxnumberofframeperseedimage)
	{
		//1) save frame to disk (as a .bmp)
		string filename = global_outputfoldername;
		filename += "\\frame_";
		char buf[256];
		sprintf(buf, "%06d", global_frameid);
		filename += buf;
		filename += ".bmp";
		SaveAsBMP(filename.c_str());
		currentlysavingframe = false;
	}
	//2024feb22, spi, begin
	//else
	else if (global_pVorogsWindow != NULL)
	//2024feb22, spi, end
	{
		//string base_seedimagefilename = global_txtfilenames[global_imageid].substr(global_txtfilenames[global_imageid].find_last_of("/\\") + 1);
		string base_seedimagefilename = global_imagefilenames[global_pVorogsWindow->GetImageID()].substr(global_imagefilenames[global_pVorogsWindow->GetImageID()].find_last_of("/\\") + 1);
		size_t dotindex = base_seedimagefilename.find_last_of(".");
		string base_seedimagefilename_noext = base_seedimagefilename.substr(0, dotindex);

		////////////////////////////
		//1) convert frames to video
		////////////////////////////
		_mkdir(global_outputvideofoldername.c_str());
		string videocutpath;
		videocutpath = global_outputvideofoldername + "\\" + base_seedimagefilename_noext + "_vorotrans-anim" + "." + global_outputvideofilenameext;
		char bufferfps[64];
		int integerfps = (int)global_outputvideoframepersecond;
		if (integerfps<1) integerfps = 1;
		if (integerfps>60) integerfps = 60;
		sprintf(bufferfps, "%d", integerfps);
		char bufferscale[64];
		//sprintf(bufferscale, "%dx%d", global_xwidth, global_yheight);
		sprintf(bufferscale, "%dx%d", _xwidth, _yheight);

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
		string fullpath = global_outputvideofoldername + "\\" + base_seedimagefilename_noext;
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
		//SwitchVoronoiDiagram();
		PostMessage(global_pVorogsWindow->GetHWND(), WM_KEYDOWN, LOWORD('N'), 0);
	}

	//currentlysavingframe = false;
}

#define GL_BGR		0x80E0
void CTimeframeWindow::SaveAsBMP(const char *fileName)
{
	FILE *file;
	unsigned long imageSize;
	GLbyte *data = NULL;
	GLint viewPort[4];
	GLenum lastBuffer;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	bmfh.bfType = 'MB';
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = 54;
	glGetIntegerv(GL_VIEWPORT, viewPort);
	imageSize = ((viewPort[2] + ((4 - (viewPort[2] % 4)) % 4))*viewPort[3] * 3) + 2;
	bmfh.bfSize = imageSize + sizeof(bmfh) + sizeof(bmih);
	data = (GLbyte*)malloc(imageSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_SWAP_BYTES, 1);
	glGetIntegerv(GL_READ_BUFFER, (GLint*)&lastBuffer);
	//2021sept26, spi, begin
	//glReadBuffer(GL_FRONT);
	glReadBuffer(GL_BACK); //windows 10 fix, frames were saved all black
	//2021sept26, spi, end
	glReadPixels(0, 0, viewPort[2], viewPort[3], GL_BGR, GL_UNSIGNED_BYTE, data);
	//glReadPixels(0, 0, viewPort[2], viewPort[3], GL_RGB, GL_UNSIGNED_BYTE, data);
	data[imageSize - 1] = 0;
	data[imageSize - 2] = 0;
	glReadBuffer(lastBuffer);
	file = fopen(fileName, "wb");
	bmih.biSize = 40;
	bmih.biWidth = viewPort[2];
	bmih.biHeight = viewPort[3];
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = 0;
	bmih.biSizeImage = imageSize;
	//bmih.biXPelsPerMeter = 45089;
	bmih.biXPelsPerMeter = 0;
	//bmih.biYPelsPerMeter = 45089;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	fwrite(&bmfh, sizeof(bmfh), 1, file);
	fwrite(&bmih, sizeof(bmih), 1, file);
	fwrite(data, imageSize, 1, file);
	free(data);
	fclose(file);
}
//2021oct01, spi, end

LRESULT CALLBACK CTimeframeWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			InitGL(hwnd, _hDC, _hRC);
			CreateCanvas();
			SetTimer(hwnd, TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY, 50, (TIMERPROC)NULL);
	}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY:
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
			/*
			if (!_canvas_ready)
			{
				CreateCanvas(); //was not ready in the case of borderless window
				//return;
			}
			*/
			reshapeFunc(LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_KEYDOWN:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_ERASEBKGND:
		{
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CTimeframeWindow::AcquireAudioBuffer(float* audiobuffer, int framesperbuffer, int numchannels)
{
	_audiobuffer_ready = false;
	memcpy(_audiobuffer, audiobuffer, sizeof(float)*framesperbuffer*numchannels);
	_audiobuffer_ready = true;
}

void CTimeframeWindow::CreateCanvas()
{ 
	if (_hwnd == NULL) return;

	_canvas_ready = false;
	while (_busy_painting) Sleep(1);
	/*
	if (_dc) DeleteDC(_dc);
	if (_bmp) DeleteObject(_bmp);
	*/

	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	_xwidth = rcClient.right - 0;
	_yheight = rcClient.bottom - 0;
	_xwidth = _xwidth - _xwidth % 4; //ensure _xwidth is divisible by 4
	if (_xwidth == 0 || _yheight == 0) return;

	/////////////////////////
	//compute voronoi diagram
	/////////////////////////
	CAutoRescaleOW2Doc* pOldOW2Doc=_pOW2Doc;
	COW2View* pOldOW2View=_pOW2View;
	_pOW2Doc = DEBUG_NEW CAutoRescaleOW2Doc(_hwnd); //DBG_NEW CAutoRescaleOW2Doc(_hwnd);
	_pOW2View = DEBUG_NEW COW2View(_hwnd, (COWDocument*)_pOW2Doc); //DBG_NEW COW2View(_hwnd, (COWDocument*)_pOW2Doc);
	_pOW2View->OnCreate(NULL);
	if (_pOW2Doc != NULL && _pOW2View != NULL)
	{
		wstring mywstring = utf8_decode(_imagefilename);
		if (_imageid>-1 && _imageid<_imagefilenames.size())
		{
			mywstring = utf8_decode(_imagefilenames[_imageid]);
		}
		bool bresult = _pOW2Doc->OnOpenDocument(mywstring.c_str());

		if (bresult)
		{
			_pOW2View->OnInitialUpdate();

			InvalidateRect(_hwnd, NULL, false);
			_pOW2Doc->OnAnalysisTextureextraction();

			//global_pOW2View->m_pViewDataSettings->bDrawPointset = TRUE;
			_pOW2View->m_pViewDataSettings->bDrawPointset = FALSE;
			_pOW2View->m_pViewDataSettings->bDrawVoronoiset = TRUE;
			InvalidateRect(_hwnd, NULL, false);

			//spi, avril 2015, begin
			//string fullpath = "c:\\temp\\";
			//_mkdir(fullpath.c_str());
			//string base_filename = global_txtfilenames[global_imageid].substr(global_txtfilenames[global_imageid].find_last_of("/\\") + 1);
			//fullpath = fullpath + base_filename;
			_pOW2Doc->SaveVoronoiDiagramColoredWithPixelsetAsImagefile(L"output.jpg", _pOW2View);
			//spi, avril 2015, end

			if (pOldOW2Doc) delete pOldOW2Doc;
			if (pOldOW2View) delete pOldOW2View;
		}
		else
		{
			if (_pOW2Doc) delete _pOW2Doc;
			if (_pOW2View) delete _pOW2View;
			_pOW2Doc = pOldOW2Doc;
			_pOW2View = pOldOW2View;

		}
	}


	_canvas_ready = true;
}

// update the image display - the interesting bit :)
void CTimeframeWindow::UpdateDisplay()
{
	if (!IsWindowVisible()) return;
	if (!_canvas_ready)
	{
		CreateCanvas(); //was not ready in the case of borderless window
		//return;
	}
	int bcanvas_dirty = false;

	while (!_audiobuffer_ready) Sleep(1);
	memcpy(_buf, _audiobuffer, sizeof(float)*TIMEFRAMEWINDOW_MAXAUDIOBUFFERSIZE);

	_callbackFilledAudioBuffers->acquireAndSeparateNewBuffer((float*)_buf);
	_ready = true;

	_busy_painting = true;

	_mode = (int) (getParameterValue("tf_mode")+0.5f);

	_inverseprobability = (int)(getParameterValue("tf_invprob") + 0.5);
	_thetacounterthreshold = (int)(getParameterValue("tf_thetathres")+0.5);
	_linewidth = getParameterValue("tf_linewidth");
	_pointsize = getParameterValue("tf_pointsize");
	_polygonfill = (int)(getParameterValue("tf_polygonfill")+0.5);

	_bgcolor_r = getParameterValue("tf_bgcolor_r");
	_bgcolor_g = getParameterValue("tf_bgcolor_g");
	_bgcolor_b = getParameterValue("tf_bgcolor_b");
	_bgcolor_a = getParameterValue("tf_bgcolor_a");

	_color1_r = getParameterValue("tf_color1_r");
	_color1_g = getParameterValue("tf_color1_g");
	_color1_b = getParameterValue("tf_color1_b");
	_color2_r = getParameterValue("tf_color2_r");
	_color2_g = getParameterValue("tf_color2_g");
	_color2_b = getParameterValue("tf_color2_b");
	_color3_r = getParameterValue("tf_color3_r");
	_color3_g = getParameterValue("tf_color3_g");
	_color3_b = getParameterValue("tf_color3_b");
	_color4_r = getParameterValue("tf_color4_r");
	_color4_g = getParameterValue("tf_color4_g");
	_color4_b = getParameterValue("tf_color4_b");

	
	// update the display
	HDC dc = GetDC(_hwnd);
	displayFunc(dc);

	//2021oct01, spi, begin
	//modif frames-batch
	if (!skipsaveframes)
	{
		//saveframetobmpfile(_hwnd, dc);
		saveopenglframetobmpfile();
	}
	//2021oct01, spi, end

	ReleaseDC(_hwnd, dc);
	_busy_painting = false;
}

void CTimeframeWindow::InvalidateDIB(int newimageid)
{
	_imageid = newimageid;
	_canvas_ready = false;
}

// Initialize OpenGL
//static void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC)
void CTimeframeWindow::InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC)
{
	/*
	///////////////////////
	//Enable alpha blending
	///////////////////////
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof pfd);
	pfd.nSize = sizeof pfd;
	pfd.nVersion = 1;
	//pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //blaine's
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;

	hDC = GetDC(hWnd);

	int i = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, i, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

}

// Shut down OpenGL
//static void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
void CTimeframeWindow::CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);

	ReleaseDC(hWnd, hDC);
}

//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void CTimeframeWindow::reshapeFunc(GLsizei w, GLsizei h)
{
	// save the new window size
	//g_windowWidth = w; g_windowHeight = h;
	_xwidth = w; _yheight = h;
	// map the view port to the client area
	glViewport(0, 0, w, h);

	if (_show3D)
	{
		// set the matrix mode to project
		glMatrixMode(GL_PROJECTION);
		// load the identity matrix
		glLoadIdentity();
		// create the viewing frustum
		gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 300.0);
		// set the matrix mode to modelview
		glMatrixMode(GL_MODELVIEW);
		// load the identity matrix
		glLoadIdentity();
		// position the view point
		gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		/* //no glutSwapBuffers() function available
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glutSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glutSwapBuffers();
		glMatrixMode(GL_PROJECTION);
		// load the identity matrix
		glLoadIdentity();
		// set to ortho for 2D version
		gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
		// set the matrix mode to modelview
		glMatrixMode(GL_MODELVIEW);
		// load the identity matrix
		glLoadIdentity();
		// position the view point
		gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		*/
	}
}


//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void CTimeframeWindow::displayFunc(HDC hdc)
{
	// ---------------- Wait and Grab New Audio  ---------------------- 

	//Don't do anything until we have new audio data    
	//while( !g_ready ) usleep( 1000 );
	//while (!g_ready) Sleep(10); //spi
	while (!_ready) Sleep(1); //spi

	//Lock while copying so the contents of the callbackFilledAudioBuffers don't change during copy
	//if (0) pthread_mutex_lock(&g_mutex);

	//Copy the latest audio buffers filled by the audio callback function
	_drawnFromAudioBuffers->copyInBuffersFromHandler(_callbackFilledAudioBuffers);
	//mark that we don't have new audio data yet
	_ready = false;

	//if (0) pthread_mutex_unlock(&g_mutex);


	// -------------------- Draw 3D  -------------------------- 	
	if (_show3D)
	{

		// -------------------- Draw the Coolness  -------------------------- 	

		//Reset viewport and projection if we're drawing windows
		if (_showWindows)
		{
			// map the view port to the client area
			glViewport(0, 0, _xwidth, _yheight);
			// set the matrix mode to project
			glMatrixMode(GL_PROJECTION);
			// load the identity matrix
			glLoadIdentity();
			// create the viewing frustum
			gluPerspective(45.0, (GLfloat)_xwidth / (GLfloat)_yheight, 1.0, 300.0);
		}

		// set the matrix mode to modelview
		glMatrixMode(GL_MODELVIEW);
		// load the identity matrix
		glLoadIdentity();
		// position the view point - view rotates around
		gluLookAt(5.0*sin(_theta), 5.0*sin(_theta), 10.0*cos(_theta), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		//theta += MY_PI / (6*43.06); //original
		//theta = rand()*MY_PI / (6*43.06); //spi
		//theta += 0; //spi
		_theta += MY_PI / (6 * 10 * 43.06); //spi
		if (_theta > 1000.0 * MY_PI) _theta = 0.0;
		_thetacounter++;
		if (_thetacounter > _thetacounterthreshold)
		{
			_theta = rand()*MY_PI / (6 * 43.06); //spi
			_thetacounter = 0;
		}
		// clear the color and depth buffers
		glClearColor(_bgcolor_r, _bgcolor_g, _bgcolor_b, _bgcolor_a); 
		//glClearColor(255.0 / 255.0, 0.0, 0.0, 150.0 / 255.0); //spi
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glPushMatrix();

		//copy out Left channel buffer
		_drawnFromAudioBuffers->copyOutChannelBuffer(_autocorrBuffer, 0);

		//update read index and and calculate autocorrelation
		_trailReadIndex = _trailWriteIndex;
		_autocorrHandler->autocorr(_autocorrBuffer, _autocorrBuffer);

		//Calculate the plot point locations for the lines to be drawn, given the just calculated autocorrelation function
		//Spacing 1 (white color)
		for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip1 * 5 - 4; i = i + 2)
		{
			_displayPointsHistory1[_trailWriteIndex][i].x = _scaleFactor3D * _autocorrBuffer[i];
			_displayPointsHistory1[_trailWriteIndex][i].y = -_scaleFactor3D * _autocorrBuffer[i + _autocorrSkip1];
			_displayPointsHistory1[_trailWriteIndex][i].z = _scaleFactor3D * _autocorrBuffer[i + 2 * _autocorrSkip1];

			_displayPointsHistory1[_trailWriteIndex][i + 1].x = _scaleFactor3D * _autocorrBuffer[i + 3 * _autocorrSkip1];
			_displayPointsHistory1[_trailWriteIndex][i + 1].y = -_scaleFactor3D * _autocorrBuffer[i + 4 * _autocorrSkip1];
			_displayPointsHistory1[_trailWriteIndex][i + 1].z = _scaleFactor3D * _autocorrBuffer[i + 5 * _autocorrSkip1];
		}

		//Spacing 2 (blue color)
		for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 5 - 4; i = i + 2)
		{
			_displayPointsHistory2[_trailWriteIndex][i].x = _scaleFactor3D * _autocorrBuffer[i];
			_displayPointsHistory2[_trailWriteIndex][i].y = -_scaleFactor3D * _autocorrBuffer[i + _autocorrSkip2];
			_displayPointsHistory2[_trailWriteIndex][i].z = _scaleFactor3D * _autocorrBuffer[i + 2 * _autocorrSkip2];

			_displayPointsHistory2[_trailWriteIndex][i + 1].x = _scaleFactor3D * _autocorrBuffer[i + 3 * _autocorrSkip2];
			_displayPointsHistory2[_trailWriteIndex][i + 1].y = -_scaleFactor3D * _autocorrBuffer[i + 4 * _autocorrSkip2];
			_displayPointsHistory2[_trailWriteIndex][i + 1].z = _scaleFactor3D * _autocorrBuffer[i + 5 * _autocorrSkip2];
		}

		//Decrement write pointer (displayPointsHistory keeps a circular buffer)
		_trailWriteIndex--;
		if (_trailWriteIndex < 0)
			_trailWriteIndex = _trailCount - 1;

		//Set line width
		//glLineWidth(1.1);	//original 	
		glLineWidth(_linewidth);	//spi 	
		//glLineWidth( 5.5 );	//spi 	
		//glLineWidth( 0.5 );	//spi 	
		//glLineWidth( 20.0 );	//spi 	
		//glLineWidth( 100.0 );	//spi 	
		//glPointSize(50.0f); //spi
		//glPointSize(10.0f); //spi
		//glPointSize(1.0f); //spi
		glPointSize(_pointsize); //spi

		glPushMatrix();

		//spi, begin
		Point3D displaypointshistory1maxamplitude = { 0.0, 0.0, 0.0 };
		Point3D displaypointshistory2maxamplitude = { 0.0, 0.0, 0.0 };
		//spi, end

		int drawIndex;

		//draw from oldest to newest (actually just need to make sure we set the alpha to the lowest on the oldest plot)
		for (int plotNum = _trailCount - 1; plotNum > -1; plotNum--)
		{
			if (_trailReadIndex + plotNum >= _trailCount)
				drawIndex = _trailReadIndex + plotNum - _trailCount;
			else
				drawIndex = _trailReadIndex + plotNum;

			//Set color to blue, alpha geometrically decreasing for older plots
			//	   			glColor4f(0.0 , 136/255.0, 158/255.0, pow(0.8, plotNum)*0.7); //2048 buffersize
			//	   			glColor4f(0.0 , 136/255.0, 158/255.0, pow(0.89, plotNum)*0.7); //1024 buffersize //original
			glColor4f(_color4_r, _color4_g, _color4_b, pow(0.89, plotNum)*0.7); //1024 buffersize //spi

			//Plot the blue timeSkip2 lines - these points were ordered previously when calculating points from the autocorrelation function
			if (_mode == 4) glBegin( GL_LINES ); //original
			if (_mode == 5) glBegin(GL_POINTS); //spi
			for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 5 - 4; i++)
			{
				if (_mode == 4 || _mode == 5) glVertex3f(_displayPointsHistory2[drawIndex][i].x, _displayPointsHistory2[drawIndex][i].y, _displayPointsHistory2[drawIndex][i].z); //original
				//spi, begin
				if (_displayPointsHistory2[drawIndex][i].x>displaypointshistory2maxamplitude.x) displaypointshistory2maxamplitude.x = _displayPointsHistory2[drawIndex][i].x;
				if (_displayPointsHistory2[drawIndex][i].y>displaypointshistory2maxamplitude.y) displaypointshistory2maxamplitude.y = _displayPointsHistory2[drawIndex][i].y;
				if (_displayPointsHistory2[drawIndex][i].z>displaypointshistory2maxamplitude.z) displaypointshistory2maxamplitude.z = _displayPointsHistory2[drawIndex][i].z;
				//spi, end
			}
			if (_mode == 4 || _mode == 5) glEnd();

			//Set color to white, alpha geometrically decreasing for older plots
			//	   			glColor4f(1.0, 1.0, 1.0, pow(0.8, plotNum)*0.8);	//2048 buffersize
			//	   			glColor4f(1.0, 1.0, 1.0, pow(0.88, plotNum)*0.8); //original
			glColor4f(_color3_r, _color3_g, _color3_b, pow(0.88, plotNum)*0.8); //spi

			//Plot the white timeSkip1 lines - these points were ordered previously when calculating points from the autocorrelation function   			
			if (_mode == 4) glBegin(GL_LINES); //original
			if (_mode == 5) glBegin(GL_POINTS); //spi
			for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip1 * 5 - 4; i++)
			{
				if (_mode == 4 || _mode == 5) glVertex3f(_displayPointsHistory1[drawIndex][i].x, _displayPointsHistory1[drawIndex][i].y, _displayPointsHistory1[drawIndex][i].z); //original
				//spi, begin
				if (_displayPointsHistory1[drawIndex][i].x>displaypointshistory1maxamplitude.x) displaypointshistory1maxamplitude.x = _displayPointsHistory1[drawIndex][i].x;
				if (_displayPointsHistory1[drawIndex][i].y>displaypointshistory1maxamplitude.y) displaypointshistory1maxamplitude.y = _displayPointsHistory1[drawIndex][i].y;
				if (_displayPointsHistory1[drawIndex][i].z>displaypointshistory1maxamplitude.z) displaypointshistory1maxamplitude.z = _displayPointsHistory1[drawIndex][i].z;
				//spi, end
			}
			if (_mode == 4 || _mode == 5) glEnd();
		}

		if (_mode == 0 || _mode == 1 || _mode == 2 || _mode == 3)
		{
			//spi, begin
			//draw voronoi (only if pointset present)
			POINTSET* pPS = NULL;
			if (_pOW2Doc && _pOW2Doc->m_pPointset && _pOW2Doc->m_pPointset->m_pPOINTSET)
			{
				pPS = _pOW2Doc->m_pPointset->m_pPOINTSET;
			}
			if (pPS)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				if (_polygonfill == 0)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				//////////////////////
				//draw voronoi regions
				//////////////////////
				int numtrifound, numneighborfound, itriseed;
				int p_arraytri[200];
				int p_arrayneighbor[200];

				// the triangle seed must be an existing triangle index 
				itriseed = 0;
				for (int ivertex = 0; ivertex < (pPS->npts); ivertex++)
				{
					//skip drawing one out of 2 times
					if (_inverseprobability>1)
					{
						//int dodraw = rand() % 2;
						//int dodraw = rand() % 200;
						//int dodraw = rand() % 5;
						int dodraw = RandomInt(1, _inverseprobability);
						if (dodraw != 1) continue;
					}
					//glColor3f(pPS->color_r[ivertex], pPS->color_g[ivertex], pPS->color_b[ivertex]);
					glColor4f(pPS->color_r[ivertex], pPS->color_g[ivertex], pPS->color_b[ivertex], 150. / 255.);
					glBegin(GL_POLYGON);
					//
					//Pour chacun des points qui n'ont aucun triangle adjacents
					//touchant directement la frontiere convexe ...
					//
					float randomz = RandomFloat(0.0, 1.0)*displaypointshistory1maxamplitude.z;
					if (FindAllValidTriSurroundingVertex(pPS,
						ivertex,
						&itriseed,
						&numtrifound,
						p_arraytri,
						&numneighborfound,
						p_arrayneighbor) == TRUE)
					{
						//if all surrounding triangles are valid,		 
						// build voronoi polygon using each adjtri's center 
						//ASSERT(numtrifound<200); //development-time error, if allowed, define p_arraytri[n] with n>200
						for (int j = 0; j < numtrifound; j++)
						{
							//glVertex3d(pPS->ctx[p_arraytri[j]] / 192.0, pPS->cty[p_arraytri[j]] / 108.0, 0.0); //static
							//glVertex3d(pPS->ctx[p_arraytri[j]] * displaypointshistory1maxamplitude.x / 192.0, pPS->cty[p_arraytri[j]] * displaypointshistory1maxamplitude.y / 108.0, displaypointshistory1maxamplitude.z); //dynamic in x, y and z
							//glVertex3d((pPS->ctx[p_arraytri[j]]-pPS->xmax/2.0) * displaypointshistory1maxamplitude.x / 192.0, (pPS->cty[p_arraytri[j]]-pPS->ymax/2.0) * displaypointshistory1maxamplitude.y / 108.0, 0.0); //dynamic in x and y
							//for a background.jpg of 3648x2736
							//glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * scaleFactor3D / 19.20, (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * scaleFactor3D / 10.80 / 2.0, 0.0); //dynamic in x and y
							//for a background.jpg of arbitrary size
							if (_mode == 0) glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * _scaleFactor3D / (20.0*pPS->xmax / 3648.0), (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * _scaleFactor3D / (20.0*pPS->ymax / 2736.0), 0.0); //dynamic in x and y
							else if (_mode == 1) glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * _scaleFactor3D / (20.0*pPS->xmax / 3648.0), (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * _scaleFactor3D / (20.0*pPS->ymax / 2736.0), RandomFloat(0.0, 1.0)*displaypointshistory1maxamplitude.z); //dynamic in x and y
							else if (_mode == 2) glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * _scaleFactor3D / (20.0*pPS->xmax / 3648.0), (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * _scaleFactor3D / (20.0*pPS->ymax / 2736.0), randomz); //dynamic in x and y
							else if (_mode == 3) glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * _scaleFactor3D / (20.0*pPS->xmax / 3648.0), (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * _scaleFactor3D / (20.0*pPS->ymax / 2736.0), displaypointshistory1maxamplitude.z); //dynamic in x and y
						}
					}
					else
					{
					}
					glEnd();
				}
			}
			//spi, end

			//spi, begin
			////////////////////////////////////
			//now, draw isolated voronoi polygon
			////////////////////////////////////
			if (pPS)
			{
				glColor4f(1.0f, 0.0f, 0.0f, 10.f);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				//////////////////////
				//draw voronoi regions
				//////////////////////
				int numtrifound, numneighborfound, itriseed;
				int p_arraytri[200];
				int p_arrayneighbor[200];

				// the triangle seed must be an existing triangle index 
				itriseed = 0;

				//draw from oldest to newest (actually just need to make sure we set the alpha to the lowest on the oldest plot)
				for (int plotNum = _trailCount - 1; plotNum > -1; plotNum--)
				{
					if (_trailReadIndex + plotNum >= _trailCount)
						drawIndex = _trailReadIndex + plotNum - _trailCount;
					else
						drawIndex = _trailReadIndex + plotNum;

					//Set color to blue, alpha geometrically decreasing for older plots
					//glColor4f(0.0 , 136/255.0, 158/255.0, pow(0.8, plotNum)*0.7); //2048 buffersize
					//glColor4f(0.0 , 136/255.0, 158/255.0, pow(0.89, plotNum)*0.7); //1024 buffersize //original
					//glColor4f(136 / 255.0, 0.0, 158 / 255.0, pow(0.89, plotNum)*0.7); //1024 buffersize //spi

					//Plot the blue timeSkip2 lines - these points were ordered previously when calculating points from the autocorrelation function
					//glBegin( GL_LINES ); //original
					//glBegin(GL_POINTS); //spi
					for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 5 - 4; i++)
					{
						//glVertex3f(displayPointsHistory2[drawIndex][i].x, displayPointsHistory2[drawIndex][i].y, displayPointsHistory2[drawIndex][i].z);
						//now, instead of drawing each point ...
						//draw nearest voronoi polygon
						//get nearest pointset object (idvertex, also equals the idstartsegment)
						//idPointsetObject = global_pOW2Doc->GetNearestPointsetObject(displayPointsHistory2[drawIndex][i].x, displayPointsHistory2[drawIndex][i].y, &itriseed);
						int ivertex = _pOW2Doc->GetNearestPointsetObject((_displayPointsHistory2[drawIndex][i].x + 0.5)*pPS->xmax, (_displayPointsHistory2[drawIndex][i].y + 0.5)*pPS->ymax, &itriseed);
						if (ivertex != -1)
						{
							//glColor4f(pPS->color_r[ivertex], pPS->color_g[ivertex], pPS->color_b[ivertex], pow(0.89, plotNum)*0.7);
							//glColor4f(pPS->color_r[ivertex], pPS->color_g[ivertex], pPS->color_b[ivertex], 100./255.);
							glColor4f(pPS->color_r[ivertex], pPS->color_g[ivertex], pPS->color_b[ivertex], 50. / 255.);
							glBegin(GL_POLYGON);
							//
							//Pour chacun des points qui n'ont aucun triangle adjacents
							//touchant directement la frontiere convexe ...
							//

							if (FindAllValidTriSurroundingVertex(pPS,
								ivertex,
								&itriseed,
								&numtrifound,
								p_arraytri,
								&numneighborfound,
								p_arrayneighbor) == TRUE)
							{
								//if all surrounding triangles are valid,		 
								// build voronoi polygon using each adjtri's center 
								//ASSERT(numtrifound<200); //development-time error, if allowed, define p_arraytri[n] with n>200
								for (int j = 0; j < numtrifound; j++)
								{
									//glVertex3d(pPS->ctx[p_arraytri[j]] / 192.0, pPS->cty[p_arraytri[j]] / 108.0, 0.0); //static
									//glVertex3d(pPS->ctx[p_arraytri[j]] * displaypointshistory1maxamplitude.x / 192.0, pPS->cty[p_arraytri[j]] * displaypointshistory1maxamplitude.y / 108.0, displaypointshistory1maxamplitude.z); //dynamic in x, y and z
									//glVertex3d((pPS->ctx[p_arraytri[j]]-pPS->xmax/2.0) * displaypointshistory1maxamplitude.x / 192.0, (pPS->cty[p_arraytri[j]]-pPS->ymax/2.0) * displaypointshistory1maxamplitude.y / 108.0, 0.0); //dynamic in x and y
									//for a background.jpg of 3648x2736
									//glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * scaleFactor3D / 19.20, (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * scaleFactor3D / 10.80 / 2.0, 0.0); //dynamic in x and y
									//glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * scaleFactor3D / 19.20, (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * scaleFactor3D / 10.80 / 2.0, displayPointsHistory2[drawIndex][i].z); //dynamic in x and y
									//for a background.jpg of arbitrary size
									glVertex3d((pPS->ctx[p_arraytri[j]] - pPS->xmax / 2.0) * _scaleFactor3D / (20.0*pPS->xmax / 3648.0), (pPS->cty[p_arraytri[j]] - pPS->ymax / 2.0) * _scaleFactor3D / (20.0*pPS->ymax / 2736.0), _displayPointsHistory2[drawIndex][i].z); //dynamic in x and y

								}
							}
							else
							{
							}
							glEnd();
						}

					}
					//glEnd();

					/*
					//Set color to white, alpha geometrically decreasing for older plots
					//glColor4f(1.0, 1.0, 1.0, pow(0.8, plotNum)*0.8);	//2048 buffersize
					//glColor4f(1.0, 1.0, 1.0, pow(0.88, plotNum)*0.8); //original
					glColor4f(0.0, 0.0, 1.0, pow(0.88, plotNum)*0.8); //spi

					//Plot the white timeSkip1 lines - these points were ordered previously when calculating points from the autocorrelation function
					//glBegin( GL_LINES ); //original
					glBegin(GL_POINTS); //spi
					for (GLint i = 0; i < autocorrHandler->getN() - autocorrSkip1 * 5 - 4; i++)
					{
					glVertex3f(displayPointsHistory1[drawIndex][i].x, displayPointsHistory1[drawIndex][i].y, displayPointsHistory1[drawIndex][i].z);
					}
					glEnd();
					*/
				}

			}
			//spi, end
		}
		else
		{
			//draw from oldest to newest (actually just need to make sure we set the alpha to the lowest on the oldest plot)
			for (int plotNum = _trailCount - 1; plotNum > -1; plotNum--)
			{
				if (_trailReadIndex + plotNum >= _trailCount)
					drawIndex = _trailReadIndex + plotNum - _trailCount;
				else
					drawIndex = _trailReadIndex + plotNum;

				//Set color to blue, alpha geometrically decreasing for older plots
				//glColor4f(0.0 , 136/255.0, 158/255.0, pow(0.8, plotNum)*0.7); //2048 buffersize
				glColor4f(_color2_r, _color2_g, _color2_b, pow(0.89, plotNum)*0.7); //1024 buffersize //original
				//glColor4f(136 / 255.0, 0.0, 158 / 255.0, pow(0.89, plotNum)*0.7); //1024 buffersize //spi

				//Plot the blue timeSkip2 lines - these points were ordered previously when calculating points from the autocorrelation function
				if (_mode == 4) glBegin( GL_LINES ); //original
				if (_mode == 5) glBegin(GL_POINTS); //spi
				for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 5 - 4; i++)
				{
					if(_mode == 4 || _mode == 5) glVertex3f(_displayPointsHistory2[drawIndex][i].x, _displayPointsHistory2[drawIndex][i].y, _displayPointsHistory2[drawIndex][i].z);

				}
				if(_mode == 4 || _mode == 5) glEnd();

				
				//Set color to white, alpha geometrically decreasing for older plots
				//glColor4f(1.0, 1.0, 1.0, pow(0.8, plotNum)*0.8);	//2048 buffersize
				//glColor4f(1.0, 1.0, 1.0, pow(0.88, plotNum)*0.8); //original
				glColor4f(_color1_r, _color1_g, _color1_b, pow(0.88, plotNum)*0.8); //spi

				//Plot the white timeSkip1 lines - these points were ordered previously when calculating points from the autocorrelation function
				if (_mode == 4) glBegin(GL_LINES); //original
				if (_mode == 5) glBegin(GL_POINTS); //spi
				for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip1 * 5 - 4; i++)
				{
					if (_mode == 4 || _mode == 5) glVertex3f(_displayPointsHistory1[drawIndex][i].x, _displayPointsHistory1[drawIndex][i].y, _displayPointsHistory1[drawIndex][i].z);
				}
				if (_mode == 4 || _mode == 5) glEnd();
				
			}


		}

		glPopMatrix();

		glPopMatrix();


		// ---------------------- Coolness end ---------------------------	 	  


		// -------------------- Draw windows --------------------------
		if (_showWindows)
		{

			// ---------------- Autocorrelation window ----------------
			glPushMatrix();

			// map the view port to the autocorr window location
			glViewport(20, 10, 400, 100);
			// set the matrix mode to project
			glMatrixMode(GL_PROJECTION);
			// load the identity matrix
			glLoadIdentity();
			// working in ortho mode for this line plot
			gluOrtho2D(0, 400, 0, 100);
			// set the matrix mode to modelview
			glMatrixMode(GL_MODELVIEW);
			// load the identity matrix
			glLoadIdentity();
			// position the view point
			gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Draw black mostly opaque background
			glColor4f(0.0, 0.0, 0.0, 0.85);

			glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 100.0, 0.0);
			glVertex3f(400.0, 100.0, 0.0);
			glVertex3f(400.0, 0.0, 0.0);
			glEnd();

			//Grab left channel audio from the buffer and generate the display from this buffer
			//  (calculates autocorrelation and scales the output to fit the given space, with specified clipping limits)
			_drawnFromAudioBuffers->copyOutChannelBuffer(_autocorrDisplayL->realValuedInputBuffer, 0);
			_autocorrDisplayL->generateDisplayFromBuffer();

			//Get pointers to the display coordinate values found above
			const float* xVals = _autocorrDisplayL->getDisplayXValuesPointer();
			const float* yVals = _autocorrDisplayL->getDisplayYValuesPointer();

			glColor4f(1.0, 1.0, 1.0, 1.0);

			//Plot the line
			glPushMatrix();
			glBegin(GL_LINE_STRIP);
			for (GLint i = 0; i < _autocorrDisplayL->getPlotLength(); i++)
				glVertex2f(*xVals++, *yVals++);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			// ----------------- Autocorr window  end --------------------	

			// ------------------ Waveform window ------------------------

			glPushMatrix();

			// map the view port to the client area
			glViewport(440, 10, 400, 100);
			// set the matrix mode to project
			glMatrixMode(GL_PROJECTION);
			// load the identity matrix
			glLoadIdentity();
			// working in ortho mode for this line plot
			gluOrtho2D(0, 400, 0, 100);
			// set the matrix mode to modelview
			glMatrixMode(GL_MODELVIEW);
			// load the identity matrix
			glLoadIdentity();
			// position the view point
			gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Draw black mostly opaque background
			glColor4f(0.0, 0.0, 0.0, 0.85);
			glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 100.0, 0.0);
			glVertex3f(400.0, 100.0, 0.0);
			glVertex3f(400.0, 0.0, 0.0);
			glEnd();


			//Grab left channel audio from the buffer and generate the display from this buffer
			//  (scales the waveform to fit the given space, with specified clipping limits)
			_drawnFromAudioBuffers->copyOutChannelBuffer(_waveformDisplayL->realValuedInputBuffer, 0);
			_waveformDisplayL->generateDisplayFromBuffer();

			//Get pointers to the display coordinate values found above			
			xVals = _waveformDisplayL->getDisplayXValuesPointer();
			yVals = _waveformDisplayL->getDisplayYValuesPointer();


			glColor3f(1.0, 1.0, 1.0);

			//Plot the line
			glPushMatrix();
			glBegin(GL_LINE_STRIP);
			for (GLint i = 0; i < _waveformDisplayL->getPlotLength(); i++)
				glVertex2f(*xVals++, *yVals++);
			glEnd();
			glPopMatrix();

			glPopMatrix();

			// --------------- Spectrum window end ---------------------	

			// ---------------- Waterfall window -----------------------	

			glPushMatrix();

			// map the view port to the client area
			glViewport(860, 10, 400, 100);
			// set the matrix mode to project
			glMatrixMode(GL_PROJECTION);
			// load the identity matrix
			glLoadIdentity();
			// working in ortho for this line plot
			gluOrtho2D(0, 400, 0, 100);
			// set the matrix mode to modelview
			glMatrixMode(GL_MODELVIEW);
			// load the identity matrix
			glLoadIdentity();
			// position the view point
			gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// Draw black mostly opaque background
			glColor4f(0.0, 0.0, 0.0, 0.85);
			glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 100.0, 0.0);
			glVertex3f(400.0, 100.0, 0.0);
			glVertex3f(400.0, 0.0, 0.0);
			glEnd();


			glPushMatrix();

			glViewport(870, 13, 380, 88);
			//Switch back to perspective mode, within this window, in order to plot the waterfall with 3D perspective
			glMatrixMode(GL_PROJECTION);
			// load the identity matrix
			glLoadIdentity();
			// create the viewing frustum
			gluPerspective(45.0, (GLfloat)_xwidth / (GLfloat)_yheight, 1.0, 300.0);
			// set the matrix mode to modelview
			glMatrixMode(GL_MODELVIEW);
			// load the identity matrix
			glLoadIdentity();
			// position the view point
			gluLookAt(0.0f, 3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


			//Update the read index and store the last display  values in the circular history buffer
			_waterfallReadIndex = _waterfallWriteIndex;
			_specDisplayMix->copyDisplayYValues(_waterfallDisplayYValuesHistory[_waterfallWriteIndex--]);

			if (_waterfallWriteIndex < 0)
				_waterfallWriteIndex = _waterfallCount - 1;

			//Get a mix of the channels and generate the spectral display from it with the specified parameters
			SAMPLE mixCoeffs[2] = { 0.707, 0.707 };
			_drawnFromAudioBuffers->copyOutMixOfChannelBuffers(_specDisplayMix->realValuedInputBuffer, mixCoeffs);
			_specDisplayMix->generateDisplayFromBuffer();

			//Get pointers to the display coordinate values found above	
			xVals = _specDisplayMix->getDisplayXValuesPointer();
			yVals = _specDisplayMix->getDisplayYValuesPointer();

			//Plot current spectrum
			glBegin(GL_LINE_STRIP);
			for (GLint i = 0; i < _specDisplayMix->getPlotLength(); i++)
			{
				glVertex3f(xVals[i], yVals[i], 0.0);
			}
			glEnd();


			//Plot waterfall - plot from oldest to newest so we can get the darkness of the lines right
			int drawIndex;
			for (int plotNum = _waterfallCount - 1; plotNum >= 0; plotNum--)
			{
				if (_waterfallReadIndex + plotNum >= _waterfallCount)
					drawIndex = _waterfallReadIndex + plotNum - _waterfallCount;
				else
					drawIndex = _waterfallReadIndex + plotNum;

				//Older plots are darker
				glColor3f(1.0 - 2.0*plotNum*(1.0 / _waterfallCount), 1.0 - 2.0*plotNum*(1.0 / _waterfallCount), 1.0 - 2.0*plotNum*(1.0 / _waterfallCount));

				glBegin(GL_LINE_STRIP);
				for (GLint i = 0; i < _specDisplayMix->getPlotLength(); i++)
				{
					glVertex3f(xVals[i], _waterfallDisplayYValuesHistory[drawIndex][i], (plotNum + 1) * -1.0);
				}
				glEnd();
			}

			glPopMatrix();

			glPopMatrix();

			// ---------------- Waterfall window end ----------------------						
		}

		// ------------------ Draw windows end ------------------------

		// flush!
		glFlush();

		/* //not available without glut
		// swap the double buffer
		glutSwapBuffers();
		*/
		SwapBuffers(hdc);

	}
	// ------------------ Draw 3D end ------------------------				  

	// -------------------- Draw 2D --------------------------
	else
	{
		//We're doing alpha fading by drawing an mostly transparent black rectangle over the whole scene between frames.
		//	This really should use single buffering, but since the program should be double buffered to support the 3D
		//	version, we just draw the same thing twice here to put it on both buffers. This seems to work okay.
		for (int buffer = 0; buffer < 2; buffer++)
		{

			//alpha fade what's there
			//		2048 buffer -> 0.15
			glColor4f(0.0, 0.0, 0.0, 0.04);
			glBegin(GL_QUADS);
			glVertex3f(-_xwidth / 2, -_yheight / 2, 0.0);
			glVertex3f(-_xwidth / 2, _yheight / 2, 0.0);
			glVertex3f(_xwidth / 2, _yheight / 2, 0.0);
			glVertex3f(_xwidth / 2, -_yheight / 2, 0.0);
			glEnd();

			// ------------------- Draw 2D Coolness -------------------------	 	  		

			//copy out Left channel buffer and calculate autocorrelation
			_drawnFromAudioBuffers->copyOutChannelBuffer(_autocorrBuffer, 0);
			_autocorrHandler->autocorr(_autocorrBuffer, _autocorrBuffer);

			glPushMatrix();

			//Plot blue lines
			glLineWidth(1.0);
			glColor4f(0.0, 136 / 255.0, 158 / 255.0, 0.4);

			glBegin(GL_LINES);
			for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 3 - 4; i = i + 2)
			{
				glVertex3f(_scaleFactor2D * _autocorrBuffer[i], -_scaleFactor2D * _autocorrBuffer[i + _autocorrSkip2], 0.0);
				glVertex3f(_scaleFactor2D * _autocorrBuffer[i + 2 * _autocorrSkip2], -_scaleFactor2D * _autocorrBuffer[i + 3 * _autocorrSkip2], 0.0);
			}
			glEnd();

			glPopMatrix();


			glPushMatrix();

			//Plot white lines
			glLineWidth(1.0);
			glColor4f(1.0, 1.0, 1.0, 1.0);

			glBegin(GL_LINES);
			for (GLint i = 0; i < _autocorrHandler->getN() - _autocorrSkip2 * 3 - 4; i = i + 2)
			{
				glVertex3f(_scaleFactor2D * _autocorrBuffer[i], -_scaleFactor2D * _autocorrBuffer[i + _autocorrSkip1], 0.0);
				glVertex3f(_scaleFactor2D * _autocorrBuffer[i + 2 * _autocorrSkip1], -_scaleFactor2D * _autocorrBuffer[i + 3 * _autocorrSkip1], 0.0);
			}
			glEnd();

			glPopMatrix();

			// -------------------- 2D Coolness end --------------------------	 	  	  

			// flush!
			glFlush();
			/* //not available without glut
			glutSwapBuffers();
			*/
			SwapBuffers(hdc);
		}

	}

}
