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

#include "defs.h"
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"

#include "CParameter.h"
#include "CWindowParameter.h"

#include "CSpectrum24bitWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>

//#include <mmsystem.h>

//fourier.h is the addon from audio programming book to the rfftw library,
//see audio programming book page 536 for details. in short, fourier.cpp
//wraps the rfftw by providing 2 functions: fft() and ifft().
//fourier.h also depends on libsndfile so it makes rfftw.lib depends on
//libsndfile for compiling (no need for linking it if you don't use it
//elsewhere).
//fft() function can only be called always with the same sample size N,
//this because within fft() implementation rfftw_create_plan() is called
//only once (the first time fft() is called).
#include <fourier.h> //in rfftw.lib (static library)

using namespace std;



CSpectrum24bitWindow::CSpectrum24bitWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
	RGBQUAD* dibpalette)
: CWindowParameter(lpszCaption, lpwndClassEx)
{
	_dibpalette = dibpalette;
	Initialize();
}

CSpectrum24bitWindow::CSpectrum24bitWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	RGBQUAD* dibpalette,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_dibpalette = dibpalette;
	Initialize();
}

CSpectrum24bitWindow::CSpectrum24bitWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	RGBQUAD* dibpalette,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_dibpalette = dibpalette;
	Initialize();
}

void CSpectrum24bitWindow::Initialize()
{
	_audiobuffer_ready = false;

	_specdc=0;
	_specbmp=0;
	//2021nov29, spi, begin
	prevHGDIOBJ = NULL;
	_prev_specmode = 0;
	//2021nov29, spi, end
	_specbuf=NULL;
	_specmode=4; //spectrum mode 
	_specpos=0; //marker pos for 2nd mode
	_SPECWIDTH=-1; //display width
	_SPECHEIGHT=-1; //display height 
	_idcolorpalette = 8;
	//_bands = 28;
	//_bands = 80;
	//_bands = 12;
	_bands = 20;

	_canvas_ready=false;
	_busy_painting=false;

	_timer = 0;

	//2021nov29, spi, begin
	_framesperbuffer_lastacquired = 0;
	_numchannels_lastacquired = 0;
	//2021nov29, spi, end

	addParameter("spec24_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
	//2021nov29, spi, begin
	addParameter("spec24_mode", 0.0, 18.0, _specmode); //todo, mode 0, 1 and 2
	//addParameter("spec24_mode", 3.0, 18.0, _specmode);
	//2021nov29, spi, end
	//addParameter("spec24_idcolorpalette", 0.0, 8.0, _idcolorpalette);
	addParameter("spec24_bgcolorid_r", 0.0, 255.0, 0.0);
	addParameter("spec24_bgcolorid_g", 0.0, 255.0, 0.0);
	addParameter("spec24_bgcolorid_b", 0.0, 255.0, 0.0);
	addParameter("spec24_idcolorch1_r", 0.0, 255.0, 255.0);
	addParameter("spec24_idcolorch1_g", 0.0, 255.0, 0.0);
	addParameter("spec24_idcolorch1_b", 0.0, 255.0, 0.0);
	addParameter("spec24_idcolorch2_r", 0.0, 255.0, 0.0);
	addParameter("spec24_idcolorch2_g", 0.0, 255.0, 0.0);
	addParameter("spec24_idcolorch2_b", 0.0, 255.0, 255.0);
	addParameter("spec24_idcolorlowest_r", 0.0, 255.0, 64.0);
	addParameter("spec24_idcolorhighest_r", 0.0, 255.0, 96.0);
	addParameter("spec24_idcolorlowest_g", 0.0, 255.0, 64.0);
	addParameter("spec24_idcolorhighest_g", 0.0, 255.0, 96.0);
	addParameter("spec24_idcolorlowest_b", 0.0, 255.0, 64.0);
	addParameter("spec24_idcolorhighest_b", 0.0, 255.0, 96.0);
	addParameter("spec24_nx", 1.0, 40.0, 1.0);
	addParameter("spec24_ny", 1.0, 40.0, 1.0);
}

void CSpectrum24bitWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "spec24_showwnd")
	{
		float showflag = getParameterValue("spec24_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}
	//some parameters updated in UpdateSpectrum()
}

CSpectrum24bitWindow::~CSpectrum24bitWindow()
{
	if (_hwnd)
	{
		KillTimer(_hwnd, SPECTRUM24BITWINDOW_IDTIMER_UPDATESPECTRUM);
		//KillTimer(_hwnd, SPECTRUMWINDOW_IDTIMER_UPDATEPARAMETER);
	}
	while (_busy_painting) Sleep(1);
	//2021nov29, spi, begin
	/*
	if (_specdc) DeleteDC(_specdc);
	if (_specbmp) DeleteObject(_specbmp);
	*/
	if (_specdc)
	{
		if (prevHGDIOBJ) SelectObject(_specdc, prevHGDIOBJ); //unselect hbitmap
		DeleteDC(_specdc);
		_specdc = NULL;
	}
	if (_specbmp)
	{
		DeleteObject(_specbmp); //delete hbitmap
		_specbmp = NULL;
	}
	//2021nov29, spi, end
}

LRESULT CALLBACK CSpectrum24bitWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			CreateCanvas();
			//_timer = timeSetEvent(25, 25, (LPTIMECALLBACK)&(CSpectrum24bitWindow::UpdateSpectrum), 0, TIME_PERIODIC);
			SetTimer(hwnd, SPECTRUM24BITWINDOW_IDTIMER_UPDATESPECTRUM, 25, (TIMERPROC)NULL);
			//SetTimer(hwnd, SPECTRUMWINDOW_IDTIMER_UPDATEPARAMETER, 1000, (TIMERPROC)NULL);
	}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case SPECTRUM24BITWINDOW_IDTIMER_UPDATESPECTRUM:
			// process the 25-ms timer 
			UpdateSpectrum();
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

void CSpectrum24bitWindow::AcquireAudioBuffer(float* audiobuffer, int framesperbuffer, int numchannels)
{
	_audiobuffer_ready = false;
	//2021nov29, spi, begin
	//memcpy(_audiobuffer, audiobuffer, sizeof(float)*framesperbuffer*numchannels);
	//_audiobuffer_ready = true;
	if ((framesperbuffer > 0) && (numchannels > 0))
	{
		//if ((framesperbuffer*numchannels) < SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE)
		if ((framesperbuffer*numchannels) <= SPECTRUM24BITWINDOW_MAXAUDIOBUFFERSIZE)
		{
			memcpy(_audiobuffer, audiobuffer, sizeof(float)*framesperbuffer*numchannels);
			_framesperbuffer_lastacquired = framesperbuffer;
			_numchannels_lastacquired = numchannels;
			_audiobuffer_ready = true;
		}
		else
		{
			//truncates audio signal and returns safely
			memcpy(_audiobuffer, audiobuffer, sizeof(float)*SPECTRUM24BITWINDOW_MAXAUDIOBUFFERSIZE);
			_framesperbuffer_lastacquired = SPECTRUM24BITWINDOW_MAXAUDIOBUFFERSIZE / numchannels;
			_numchannels_lastacquired = numchannels;
			_audiobuffer_ready = true;
		}
	}
	//2021nov29, spi, end
}

void CSpectrum24bitWindow::CreateCanvas()
{ 
	if (_hwnd == NULL) return;

	_canvas_ready = false;
	while (_busy_painting) Sleep(1);
	//2021nov29, spi, begin
	/*
	if (_specdc) DeleteDC(_specdc);
	if (_specbmp) DeleteObject(_specbmp);
	*/
	if (_specdc)
	{
		if (prevHGDIOBJ) SelectObject(_specdc, prevHGDIOBJ); //unselect hbitmap
		DeleteDC(_specdc);
		_specdc = NULL;
	}
	if (_specbmp)
	{
		DeleteObject(_specbmp); //delete hbitmap
		_specbmp = NULL;
	}
	//2021nov29, spi, end

	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	_SPECWIDTH = rcClient.right - 0;
	_SPECHEIGHT = rcClient.bottom - 0;
	_SPECWIDTH = _SPECWIDTH - _SPECWIDTH % 4; //ensure 	_SPECWIDTH is divisible by 4
	if (_SPECWIDTH == 0 || _SPECHEIGHT == 0) return;

	// create bitmap to draw spectrum in (8 bit for easy updating)
	//2021nov29, spi, begin
	//BYTE data[2000] = { 0 };
	BYTE data[4000] = { 0 }; //2000 was safe, 4000 is even safer, size must always be greater than size of BITMAPINFOHEADER + (size of RGBQUAD times 256)
	//2021nov29, spi, end
	BITMAPINFOHEADER *bh = (BITMAPINFOHEADER*)data;
	RGBQUAD *pal = (RGBQUAD*)(data + sizeof(*bh));
	int a;
	bh->biSize = sizeof(*bh);
	bh->biWidth = _SPECWIDTH;
	bh->biHeight = _SPECHEIGHT; // upside down (line 0=bottom)
	bh->biSizeImage = _SPECWIDTH*_SPECHEIGHT * 3;
	bh->biPlanes = 1;
	bh->biCompression = BI_RGB;
	bh->biBitCount = 24;
	bh->biClrUsed = bh->biClrImportant = 0;

	/*
	// setup palette
	if (_idcolorpalette == 0)
	{
		//original palette, green shifting to red
		for (a = 1; a<128; a++) {
			pal[a].rgbGreen = 256 - 2 * a;
			pal[a].rgbRed = 2 * a;
		}
		for (a = 0; a<32; a++) {
			pal[128 + a].rgbBlue = 8 * a;
			pal[128 + 32 + a].rgbBlue = 255;
			pal[128 + 32 + a].rgbRed = 8 * a;
			pal[128 + 64 + a].rgbRed = 255;
			pal[128 + 64 + a].rgbBlue = 8 * (31 - a);
			pal[128 + 64 + a].rgbGreen = 8 * a;
			pal[128 + 96 + a].rgbRed = 255;
			pal[128 + 96 + a].rgbGreen = 255;
			pal[128 + 96 + a].rgbBlue = 8 * a;
		}
	}
	else if (_idcolorpalette == 1)
	{
		//altered palette, red shifting to green
		for (a = 1; a<128; a++) {
			pal[a].rgbRed = 256 - 2 * a;
			pal[a].rgbGreen = 2 * a;
		}
		for (a = 0; a<32; a++) {
			pal[128 + a].rgbBlue = 8 * a;
			pal[128 + 32 + a].rgbBlue = 255;
			pal[128 + 32 + a].rgbGreen = 8 * a;
			pal[128 + 64 + a].rgbGreen = 255;
			pal[128 + 64 + a].rgbBlue = 8 * (31 - a);
			pal[128 + 64 + a].rgbRed = 8 * a;
			pal[128 + 96 + a].rgbGreen = 255;
			pal[128 + 96 + a].rgbRed = 255;
			pal[128 + 96 + a].rgbBlue = 8 * a;
		}
	}
	else if (_idcolorpalette == 2)
	{
		//altered palette, blue shifting to green
		for (a = 1; a<128; a++) {
			pal[a].rgbBlue = 256 - 2 * a;
			pal[a].rgbGreen = 2 * a;
		}
		for (a = 0; a<32; a++) {
			pal[128 + a].rgbBlue = 8 * a;
			pal[128 + 32 + a].rgbRed = 255;
			pal[128 + 32 + a].rgbGreen = 8 * a;
			pal[128 + 64 + a].rgbGreen = 255;
			pal[128 + 64 + a].rgbRed = 8 * (31 - a);
			pal[128 + 64 + a].rgbBlue = 8 * a;
			pal[128 + 96 + a].rgbGreen = 255;
			pal[128 + 96 + a].rgbBlue = 255;
			pal[128 + 96 + a].rgbRed = 8 * a;
		}
	}
	else if (_idcolorpalette == 3)
	{
		//altered palette, black shifting to white - grascale
		for (a = 1; a<256; a++) {
			pal[a].rgbRed = a;
			pal[a].rgbBlue = a;
			pal[a].rgbGreen = a;
		}
	}
	else if (_idcolorpalette == 4)
	{
		//altered palette, pink
		for (a = 1; a<256; a++) {
			pal[a].rgbRed = 255;
			pal[a].rgbBlue = 255;
			pal[a].rgbGreen = a;
		}
	}
	else if (_idcolorpalette == 5)
	{
		//altered palette, yellow
		for (a = 1; a<256; a++) {
			pal[a].rgbRed = 255;
			pal[a].rgbBlue = a;
			pal[a].rgbGreen = 255;
		}
	}
	else if (_idcolorpalette == 6)
	{
		//altered palette, cyan
		for (a = 1; a<256; a++) {
			pal[a].rgbRed = a;
			pal[a].rgbBlue = 255;
			pal[a].rgbGreen = 255;
		}
	}
	else if (_idcolorpalette == 7)
	{
		//altered palette, lite green
		for (a = 1; a<256; a++) {
			pal[a].rgbRed = a;
			pal[a].rgbBlue = 127;
			pal[a].rgbGreen = 255;
		}
	}
	else if (_idcolorpalette == 8)
	{
		if (_dibpalette)
		{
			//altered palette, color quantized background image
			for (a = 1; a<256; a++) {
				pal[a].rgbRed = _dibpalette[a].rgbRed;
				pal[a].rgbBlue = _dibpalette[a].rgbBlue;
				pal[a].rgbGreen = _dibpalette[a].rgbGreen;
			}
		}
		else
		{
			//altered palette, black shifting to white - grascale
			for (a = 1; a<256; a++) {
				pal[a].rgbRed = a;
				pal[a].rgbBlue = a;
				pal[a].rgbGreen = a;
			}
		}
	}
	*/

	// create the bitmap
	_specbmp = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, (void**)&_specbuf, NULL, 0);
	_specdc = CreateCompatibleDC(0);
	//2021nov29, spi, begin
	//SelectObject(_specdc, _specbmp);
	prevHGDIOBJ = SelectObject(_specdc, _specbmp);
	//2021nov29, spi, end
	_canvas_ready = true;
}

int CSpectrum24bitWindow::getv(int &x, int &c, int &framesperbuffer, int &numchannels)
{
	/*
	int v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2; // invert and scale to fit display //2021nov29, spi, was //int v = (1 - _buf[x*NUM_CHANNELS + c])*_SPECHEIGHT / 2; // invert and scale to fit display
	if (v<0) v = 0;
	else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
	*/
	int v = 0;
	if (framesperbuffer < _SPECWIDTH)
	{
		int x_inframes = (int)((float)x * ((float)framesperbuffer) / ((float)_SPECWIDTH));
		v = (1 - _buf[x_inframes*numchannels + c])*_SPECHEIGHT / 2;
	}
	else
	{
		v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2;
	}
	if (v < 0) v = 0;
	else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
	return v;
}

// update the spectrum display - the interesting bit :)
//void CALLBACK CSpectrum24bitWindow::UpdateSpectrum(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
void CSpectrum24bitWindow::UpdateSpectrum()
{
	if (!IsWindowVisible()) return;
	if (!_canvas_ready)
	{
		CreateCanvas(); //was not ready in the case of borderless window
		return;
	}
	int bcanvas_dirty = false;

	while (!_audiobuffer_ready) Sleep(1);
	//2021nov29, spi, begin
	//memcpy(_buf, _audiobuffer, sizeof(float)*SPECTRUM24BITWINDOW_MAXAUDIOBUFFERSIZE);
	memcpy(_buf, _audiobuffer, sizeof(float)*_framesperbuffer_lastacquired*_numchannels_lastacquired);
	int framesperbuffer = _framesperbuffer_lastacquired;
	int numchannels = _numchannels_lastacquired;
	//_bands = 28;
	//_bands = 80;
	//_bands = 12;
	if (1)
	{
		_bands = _SPECWIDTH / 16; //proportional, 120 bands for 2K display, 240 bands for 4K display, etc.
	}
	else
	{
		_bands = 1920 / 16; //constant, 16 pixels per band, 120 bands on any display
	}
	if (_bands < 4) _bands = 4;
	if (_bands > (framesperbuffer - 2)) _bands = (framesperbuffer - 2);
	//2021nov29, spi, end

	_busy_painting = true;

	_specmode = (int)(getParameterValue("spec24_mode")+0.5);
	/*
	int idcolorpalette = (int)getParameterValue("spec24_idcolorpalette");
	if (idcolorpalette != _idcolorpalette)
	{
		_idcolorpalette = idcolorpalette;
		bcanvas_dirty = true;
	}
	*/
	int bgcolorid_r = (int)(getParameterValue("spec24_bgcolorid_r") + 0.5);
	int bgcolorid_g = (int)(getParameterValue("spec24_bgcolorid_g") + 0.5);
	int bgcolorid_b = (int)(getParameterValue("spec24_bgcolorid_b") + 0.5);

	int idcolorch0_r = (int)(getParameterValue("spec24_idcolorch1_r") + 0.5);
	int idcolorch0_g = (int)(getParameterValue("spec24_idcolorch1_g") + 0.5);
	int idcolorch0_b = (int)(getParameterValue("spec24_idcolorch1_b") + 0.5);
	int idcolorch1_r = (int)(getParameterValue("spec24_idcolorch2_r") + 0.5);
	int idcolorch1_g = (int)(getParameterValue("spec24_idcolorch2_g") + 0.5);
	int idcolorch1_b = (int)(getParameterValue("spec24_idcolorch2_b") + 0.5);

	float fcolorlowest_r = getParameterValue("spec24_idcolorlowest_r");
	float fcolorhighest_r = getParameterValue("spec24_idcolorhighest_r");
	int idcolorlowest_r = 0;
	int idcolorhighest_r = 255;
	if (fcolorlowest_r<=fcolorhighest_r)
	{
		idcolorlowest_r = (int)(fcolorlowest_r + 0.5);
		idcolorhighest_r = (int)(fcolorhighest_r + 0.5);
	}
	else
	{
		idcolorlowest_r = (int)(fcolorhighest_r + 0.5);
		idcolorhighest_r = (int)(fcolorlowest_r + 0.5);
	}
	float fcolorlowest_g = getParameterValue("spec24_idcolorlowest_g");
	float fcolorhighest_g = getParameterValue("spec24_idcolorhighest_g");
	int idcolorlowest_g = 0;
	int idcolorhighest_g = 255;
	if (fcolorlowest_g <= fcolorhighest_g)
	{
		idcolorlowest_g = (int)(fcolorlowest_g + 0.5);
		idcolorhighest_g = (int)(fcolorhighest_g + 0.5);
	}
	else
	{
		idcolorlowest_g = (int)(fcolorhighest_g + 0.5);
		idcolorhighest_g = (int)(fcolorlowest_g + 0.5);
	}
	float fcolorlowest_b = getParameterValue("spec24_idcolorlowest_b");
	float fcolorhighest_b = getParameterValue("spec24_idcolorhighest_b");
	int idcolorlowest_b = 0;
	int idcolorhighest_b = 255;
	if (fcolorlowest_b <= fcolorhighest_b)
	{
		idcolorlowest_b = (int)(fcolorlowest_b + 0.5);
		idcolorhighest_b = (int)(fcolorhighest_b + 0.5);
	}
	else
	{
		idcolorlowest_b = (int)(fcolorhighest_b + 0.5);
		idcolorhighest_b = (int)(fcolorlowest_b + 0.5);
	}

	//2021nov29, spi, begin
	RgbColor bgcolorid_RgbColor(bgcolorid_r, bgcolorid_g, bgcolorid_b);
	HsvColor bgcolorid_HsvColor = RgbToHsv(bgcolorid_RgbColor);
	RgbColor idcolorch0_RgbColor(idcolorch0_r, idcolorch0_g, idcolorch0_b);
	HsvColor idcolorch0_HsvColor = RgbToHsv(idcolorch0_RgbColor);
	RgbColor idcolorch1_RgbColor(idcolorch1_r, idcolorch1_g, idcolorch1_b);
	HsvColor idcolorch1_HsvColor = RgbToHsv(idcolorch1_RgbColor);
	RgbColor idcolorlowest_RgbColor(idcolorlowest_r, idcolorlowest_g, idcolorlowest_b);
	HsvColor idcolorlowest_HsvColor = RgbToHsv(idcolorlowest_RgbColor);
	RgbColor idcolorhighest_RgbColor(idcolorhighest_r, idcolorhighest_g, idcolorhighest_b);
	HsvColor idcolorhighest_HsvColor = RgbToHsv(idcolorhighest_RgbColor);
	//2021nov29, spi, end

	int nx = (int)(getParameterValue("spec24_nx") + 0.5);
	int ny = (int)(getParameterValue("spec24_ny") + 0.5);

	if (bcanvas_dirty)
	{
		_busy_painting = false;
		CreateCanvas();
		return;
	}


	HDC dc;
	int x, y, y1;

	if (_specmode == 3 || _specmode == 4 || _specmode == 5 || _specmode == 6 ||
		_specmode == 7 || _specmode == 8 || _specmode == 9 || _specmode == 10 ||
		_specmode == 11 || _specmode == 12 || _specmode == 13 || _specmode == 14 ||
		_specmode == 15 || _specmode == 16 || _specmode == 17 || _specmode == 18)
	{ // waveform and filled waveform
		int c;
		//float *buf;

		//black background
		if (_specmode == 3 || _specmode == 4 || _specmode == 5 || _specmode == 6)
		{
			/*
			//memset(_specbuf, 0, _SPECWIDTH*_SPECHEIGHT);
			memset(_specbuf, bgcolorid, _SPECWIDTH*_SPECHEIGHT);
			*/
			for (int i = 0; i<_SPECWIDTH; i++)
			{
				for (int j = 0; j<_SPECHEIGHT; j++)
				{
					//specbuf[j*SPECWIDTH+i]=random_integer;
					_specbuf[j*_SPECWIDTH * 3 + i * 3] = bgcolorid_b; //B
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = bgcolorid_g; //G
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = bgcolorid_r; //R
				}
			}

		}
		//noisy background
		else if (_specmode == 7 || _specmode == 8 || _specmode == 9 || _specmode == 10)
		{
			for (int i = 0; i<_SPECWIDTH; i++)
			{
				for (int j = 0; j<_SPECHEIGHT; j++)
				{
					_specbuf[j*_SPECWIDTH*3 + i*3] = RandomInt(idcolorlowest_b, idcolorhighest_b); //B
					_specbuf[j*_SPECWIDTH*3 + i*3+1] = RandomInt(idcolorlowest_g, idcolorhighest_g); //G
					_specbuf[j*_SPECWIDTH*3 + i*3+2] = RandomInt(idcolorlowest_r, idcolorhighest_r); //R
				}
			}
		}
		//solid slightly or radically shifting background
		else if (_specmode == 11 || _specmode == 12 || _specmode == 13 || _specmode == 14)
		{
			/*
			int random_integer;
			//int lowest=1, highest=127;
			//int lowest=1, highest=255; //radically shifting
			//int lowest = 64, highest = 96; //slightly shifting
			int lowest = idcolorlowest;
			int highest = idcolorhighest;
			int range = (highest - lowest) + 1;
			random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
			memset(_specbuf, random_integer, _SPECWIDTH*_SPECHEIGHT);
			*/
			int R = RandomInt(idcolorlowest_r, idcolorhighest_r);
			int G = RandomInt(idcolorlowest_g, idcolorhighest_g);
			int B = RandomInt(idcolorlowest_b, idcolorhighest_b);
			for (int i = 0; i<_SPECWIDTH; i++)
			{
				for (int j = 0; j<_SPECHEIGHT; j++)
				{
					//specbuf[j*SPECWIDTH+i]=random_integer;
					_specbuf[j*_SPECWIDTH * 3 + i * 3] = B; //B
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = G; //G
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = R; //R
				}
			}

		}
		//solid background divided into cells
		else //_specmode==15 || _specmode==16 || _specmode==17 || _specmode==18
		{
			/*
			int random_integer;
			//int lowest=1, highest=127;
			//int lowest = 1, highest = 255;
			//int lowest=64, highest=96; //good
			int lowest = idcolorlowest;
			int highest = idcolorhighest;
			int range = (highest - lowest) + 1;
			random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
			memset(_specbuf, random_integer, _SPECWIDTH*_SPECHEIGHT);
			*/
			int nicell = nx;
			int njcell = ny;
			int cellwidth = _SPECWIDTH / nicell;
			int cellheight = _SPECHEIGHT / njcell;
			for (int icell = 0; icell<nicell; icell++)
			{
				for (int jcell = 0; jcell<njcell; jcell++)
				{
					int ioffset = icell*cellwidth;
					int joffset = jcell*cellheight;

					int random_integer2 = RandomInt(0, 1);

					int cellcolor_r = 0;
					int cellcolor_g = 0;
					int cellcolor_b = 0;
					if (random_integer2 == 0)
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

					for (int i = ioffset; i<(ioffset + cellwidth); i++)
					{
						for (int j = joffset; j<(joffset + cellheight); j++)
						{
							_specbuf[j*_SPECWIDTH*3 + i*3] = cellcolor_b; //B
							_specbuf[j*_SPECWIDTH*3 + i*3+1] = cellcolor_g; //G
							_specbuf[j*_SPECWIDTH*3 + i*3+2] = cellcolor_r; //R
						}
					}

				}

			}

		}

		//global allocation from now on
		//buf=(float*)alloca(NUM_CHANNELS*SPECWIDTH*sizeof(float)); // allocate buffer for data

		//global_err = Pa_ReadStream( global_stream, buf, FRAMES_PER_BUFFER );
		/*
		global_err = Pa_ReadStream(global_stream, buf, NUM_CHANNELS*SPECWIDTH);
		if (global_err != paNoError)
		{
			return;
		}
		*/

		//2021nov29, spi, begin
		/*
		if (framesperbuffer < _SPECWIDTH)  //if (_SPECWIDTH > framesperbuffer) //2021nov29, spi, was //if (_SPECWIDTH > FRAMES_PER_BUFFER)
		{
			//don't draw audio signal, just the background

			// update the display
			dc = GetDC(_hwnd);
			BitBlt(dc, 0, 0, _SPECWIDTH, _SPECHEIGHT, _specdc, 0, 0, SRCCOPY);
			ReleaseDC(_hwnd, dc);

			_busy_painting = false;
			return;
		}
		*/
		//2021nov29, spi, end

		//under waveform filled down to bottom
		if (_specmode == 6 || _specmode == 10 || _specmode == 14 || _specmode == 18)
		{
			for (c = 0; c<numchannels; c++)
			{
				for (x = 0; x<_SPECWIDTH; x++)
				{
					/*
					int v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2; // invert and scale to fit display
					if (v<0) v = 0;
					else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
					*/
					//2021nov29, spi, begin
					// invert and scale to fit display
					int v = getv(x, c, framesperbuffer, numchannels);
					//2021nov29, spi, end

					//under waveform filled down to bottom
					y = v;
					while (--y >= 0)
					{
						_specbuf[y*_SPECWIDTH*3 + x*3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH*3 + x*3+1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH*3 + x*3+2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
				}
			}
		}
		//waveform filled towards center
		else if (_specmode == 5 || _specmode == 9 || _specmode == 13 || _specmode == 17)
		{
			for (c = 0; c<numchannels; c++)
			{
				for (x = 0; x<_SPECWIDTH; x++)
				{
					/*
					int v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2; // invert and scale to fit display
					if (v<0) v = 0;
					else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
					*/
					//2021nov29, spi, begin
					// invert and scale to fit display
					int v = getv(x, c, framesperbuffer, numchannels);
					//2021nov29, spi, end

					//waveform filled towards center
					y = v;
					if (y>(_SPECHEIGHT / 2))
					while (--y >= (_SPECHEIGHT / 2))
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
					else if (y<(_SPECHEIGHT / 2))
					while (++y <= (_SPECHEIGHT / 2))
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
					else
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
				}
			}
		}
		//waveform filled towards opposite
		else if (_specmode == 4 || _specmode == 8 || _specmode == 12 || _specmode == 16)
		{
			for (c = 0; c<numchannels; c++)
			{
				for (x = 0; x<_SPECWIDTH; x++)
				{
					/*
					int v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2; // invert and scale to fit display
					if (v<0) v = 0;
					else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
					*/
					//2021nov29, spi, begin
					// invert and scale to fit display
					int v = getv(x, c, framesperbuffer, numchannels);
					//2021nov29, spi, end

					//waveform filled towards opposite
					y = v;
					if (y>(_SPECHEIGHT / 2))
					while (--y >= (_SPECHEIGHT / 2 - (v - (_SPECHEIGHT / 2))))
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
					else if (y<(_SPECHEIGHT / 2))
					while (++y <= (_SPECHEIGHT / 2 + ((_SPECHEIGHT / 2) - v)))
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
					else
					{
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					}
				}
			}
		}
		//waveform (original)
		else if (_specmode == 3 || _specmode == 7 || _specmode == 11 || _specmode == 15)
		{
			for (c = 0; c<numchannels; c++)
			{
				for (x = 0; x<_SPECWIDTH; x++)
				{
					/*
					int v = (1 - _buf[x*numchannels + c])*_SPECHEIGHT / 2; // invert and scale to fit display
					if (v<0) v = 0;
					else if (v >= _SPECHEIGHT) v = _SPECHEIGHT - 1;
					*/
					//2021nov29, spi, begin
					// invert and scale to fit display
					int v = getv(x, c, framesperbuffer, numchannels);
					//2021nov29, spi, end

					if (!x) y = v;
					do
					{ // draw line from previous sample...
						if (y<v) y++;
						else if (y>v) y--;
						_specbuf[y*_SPECWIDTH * 3 + x * 3] = c & 1 ? idcolorch1_b : idcolorch0_b; //B
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 1] = c & 1 ? idcolorch1_g : idcolorch0_g; //G
						_specbuf[y*_SPECWIDTH * 3 + x * 3 + 2] = c & 1 ? idcolorch1_r : idcolorch0_r; //R
					} while (y != v);
				}
			}
		}
	}
	else
	{
		//2021nov29, spi, begin
		//int numberofsamples = FRAMES_PER_BUFFER; //original
		int numberofsamples = framesperbuffer; 
		//if (_SPECWIDTH < numberofsamples)
		if(1) 
		//2021nov29, spi, end
		{
			fft(_buf, _fftbuf, numberofsamples);
			for (int i = 0; i<numberofsamples; i++)
			{
				_fftbuf[i] = abs(_fftbuf[i]);
			}

			if (!_specmode)
			{ 
				// "normal" FFT
				for (int i = 0; i<_SPECWIDTH; i++)
				{
					for (int j = 0; j<_SPECHEIGHT; j++)
					{
						//specbuf[j*SPECWIDTH+i]=random_integer;
						_specbuf[j*_SPECWIDTH * 3 + i * 3] = bgcolorid_b; //B
						_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = bgcolorid_g; //G
						_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = bgcolorid_r; //R
					}
				}
				/**/ //todo
				for (x = 0; x<_SPECWIDTH / 2; x++)
				{
					/*
#if 1
					y = sqrt(_fftbuf[x + 1]) * 3 * _SPECHEIGHT - 4; // scale it (sqrt to make low values more visible)
#else
					y = _fftbuf[x + 1] * 10 * _SPECHEIGHT; // scale it (linearly)
#endif
					*/
					//2021nov29, spi, begin
					int x_infft = 0;
					if (numberofsamples < _SPECWIDTH)
					{
						x_infft = 1 + (int)((float)x *  ((float)numberofsamples) / ((float)_SPECWIDTH));
					}
					else
					{
						x_infft = 1 + x;
					}
					/*
					#if 1
						y = sqrt(_fftbuf[x + 1]) * 3 * _SPECHEIGHT - 4; // scale it (sqrt to make low values more visible)
					#else
						y = _fftbuf[x + 1] * 10 * _SPECHEIGHT; // scale it (linearly)
					#endif
					*/
					if (1)
					{
						y = sqrt(_fftbuf[x_infft]) * 3 * _SPECHEIGHT - 4; // scale it (sqrt to make low values more visible)
					}
					else
					{
						y = _fftbuf[x_infft] * 10 * _SPECHEIGHT; // scale it (linearly)
					}
					//2021nov29, spi, end

					if (y>_SPECHEIGHT) y = _SPECHEIGHT; // cap it
					if (x && (y1 = (y + y1) / 2)) // interpolate from previous to make the display smoother
						//while (--y1>=0) specbuf[y1*SPECWIDTH+x*2-1]=y1+1;
					while (--y1 >= 0) 
					{
						//= (127 * y1 / _SPECHEIGHT) + 1;
						int vmin = 127; //idcolorch0_HsvColor.v; //mid level, to be way above background intensity which is typically 0
						int vmax = 255;
						int vrange = vmax - vmin; //vmax - idcolorch0_HsvColor.v;
						if (vrange < 10) vrange = 10;
						if (vrange > 127) vrange = 127;
						vmin = vmax - vrange;
						HsvColor myHsvColor(idcolorch0_HsvColor.h, idcolorch0_HsvColor.s, (vrange * y1 / _SPECHEIGHT) + vmin);
						RgbColor myRgbColor = HsvToRgb(myHsvColor);
						_specbuf[y1*_SPECWIDTH * 3 + x * 2 * 3 - 1 * 3] = myRgbColor.b; //= idcolorch0_b; //B  //= (127 * y1 / _SPECHEIGHT) + 1;
						_specbuf[y1*_SPECWIDTH * 3 + x * 2 * 3 - 1 * 3 + 1] = myRgbColor.g; // = idcolorch0_g; //G  //= (127 * y1 / _SPECHEIGHT) + 1;
						_specbuf[y1*_SPECWIDTH * 3 + x * 2 * 3 - 1 * 3 + 2] = myRgbColor.r; // = idcolorch0_r; //R  //= (127 * y1 / _SPECHEIGHT) + 1;
					}
					y1 = y;
					//while (--y>=0) specbuf[y*SPECWIDTH+x*2]=y+1; // draw level
					while (--y >= 0)
					{
						//= (127 * y1 / _SPECHEIGHT) + 1;
						int vmin = 127; //idcolorch0_HsvColor.v; //mid level, to be way above background intensity which is typically 0
						int vmax = 255;
						int vrange = vmax - vmin; //vmax - idcolorch0_HsvColor.v;
						if (vrange < 10) vrange = 10;
						if (vrange > 127) vrange = 127;
						vmin = vmax - vrange;
						HsvColor myHsvColor(idcolorch0_HsvColor.h, idcolorch0_HsvColor.s, (vrange * y / _SPECHEIGHT) + vmin);
						RgbColor myRgbColor = HsvToRgb(myHsvColor);
						_specbuf[y*_SPECWIDTH*3 + x * 2*3] = myRgbColor.b; //= idcolorch0_b; //B  //= (127 * y / _SPECHEIGHT) + 1; // draw level
						_specbuf[y*_SPECWIDTH * 3 + x * 2 * 3 + 1] = myRgbColor.g; //= idcolorch0_g; //G  //= (127 * y / _SPECHEIGHT) + 1; // draw level
						_specbuf[y*_SPECWIDTH * 3 + x * 2 * 3 + 2] = myRgbColor.r; //= idcolorch0_r; //R  //= (127 * y / _SPECHEIGHT) + 1; // draw level
					}
				}
				/**/
			}
			else if (_specmode == 1)
			{ 
				// logarithmic, acumulate & average bins
				int b0 = 0;
				for (int i = 0; i<_SPECWIDTH; i++)
				{
					for (int j = 0; j<_SPECHEIGHT; j++)
					{
						//specbuf[j*SPECWIDTH+i]=random_integer;
						_specbuf[j*_SPECWIDTH * 3 + i * 3] = bgcolorid_b; //B
						_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = bgcolorid_g; //G
						_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = bgcolorid_r; //R
					}
				}
				//#define BANDS 28
				//#define BANDS 80
				//#define BANDS 12
				//2021nov29, spi, begin
				if (_bands > 2 && _bands < _SPECWIDTH)
				{
					//for each band (each band or each bin)
					for (x = 0; x < _bands; x++)
					{
						float peak = 0;
						int b1 = pow(2, x*10.0 / (_bands - 1));
						//2021nov29, spi, begin
						//if (b1 > 1023) b1 = 1023;
						if (b1 > (numberofsamples - 1)) b1 = (numberofsamples - 1);
						//2021nov29, spi, end
						if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin
						//from b0 (bin start) to b1-1 (bin end)
						for (; b0 < b1; b0++)
						{
							//find bin peak
							//2021nov29, spi, begin
							int b_infft = 0;
							if (numberofsamples < _SPECWIDTH)
							{
								b_infft = 1 + (int)((float)b0 * ((float)numberofsamples) / ((float)_SPECWIDTH));
							}
							else
							{
								b_infft = 1 + b0;
							}
							//if (peak < _fftbuf[1 + b0]) peak = _fftbuf[1 + b0];
							if (peak < _fftbuf[b_infft]) peak = _fftbuf[b_infft];
							//2021nov29, spi, end
						}
						y = sqrt(peak) * 3 * _SPECHEIGHT - 4; // scale it (sqrt to make low values more visible)
						if (y > _SPECHEIGHT) y = _SPECHEIGHT; // cap it
						while (--y >= 0)
						{
							//todo, drawing has to be different for r,g,b
							//memset(_specbuf + y * _SPECWIDTH + x * (_SPECWIDTH / _bands), (127 * y / _SPECHEIGHT) + 1, _SPECWIDTH / _bands - 2); // draw bar
							//int j_min = y;
							//int j_max = y;
							int j = y;
							int i_min = x * (_SPECWIDTH / _bands);
							int i_max = i_min + _SPECWIDTH / _bands - 2;
							//int color = (127 * y / _SPECHEIGHT) + 1; //gray level
							//= (127 * y1 / _SPECHEIGHT) + 1;
							int vmin = 127; //idcolorch0_HsvColor.v; //mid level, to be way above background intensity which is typically 0
							int vmax = 255;
							int vrange = vmax - vmin; //vmax - idcolorch0_HsvColor.v;
							if (vrange < 10) vrange = 10;
							if (vrange > 127) vrange = 127;
							vmin = vmax - vrange;
							HsvColor myHsvColor(idcolorch0_HsvColor.h, idcolorch0_HsvColor.s, (vrange * y / _SPECHEIGHT) + vmin);
							RgbColor myRgbColor = HsvToRgb(myHsvColor);
							for (int i = i_min; i < i_max; i++)
							{
								//for (int j = j_min; j < j_max; j++)
								//{
									_specbuf[j*_SPECWIDTH * 3 + i * 3] = myRgbColor.b; //= idcolorch0_b; //B  //= color; //B // idcolorch0_b; //c & 1 ? idcolorch1_b : idcolorch0_b; //B
									_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = myRgbColor.g; //= idcolorch0_g; //G  //= color; //G // idcolorch0_g; //c & 1 ? idcolorch1_g : idcolorch0_g; //G
									_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = myRgbColor.r; //= idcolorch0_r; //R  //= color; //R // idcolorch0_r; //c & 1 ? idcolorch1_r : idcolorch0_r; //R
								//}
							}
						}
					}
				}
				//2021nov29, spi, end
			}
			else
			{ 
				// "3D", frequencies axis is now vertical and a marker is drawn
				//2021nov29, spi, begin
				if (_prev_specmode != _specmode)
				{
					for (int i = 0; i < _SPECWIDTH; i++)
					{
						for (int j = 0; j < _SPECHEIGHT; j++)
						{
							//specbuf[j*SPECWIDTH+i]=random_integer;
							_specbuf[j*_SPECWIDTH * 3 + i * 3] = bgcolorid_b; //B
							_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = bgcolorid_g; //G
							_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = bgcolorid_r; //R
						}
					}
				}
				//2021nov29, spi, end
				for (x = 0; x < _SPECHEIGHT; x++)
				{
					//2021nov29, spi, begin
					//y = sqrt(_fftbuf[x + 1]) * 3 * 127; // scale it (sqrt to make low values more visible)
					int x_infft = 0;
					if (numberofsamples < _SPECHEIGHT)
					{
						x_infft = 1 + (int)((float)x * ((float)numberofsamples) / ((float)_SPECHEIGHT));
					}
					else
					{
						x_infft = 1 + x;
					}
					y = sqrt(_fftbuf[x_infft]) * 3 * 127; // scale it (sqrt to make low values more visible)
					//2021nov29, spi, end
					if (y > 127) y = 127; // cap it
					//todo
					//_specbuf[x*_SPECWIDTH + _specpos] = 128 + y; // plot it
					int i = _specpos;
					int j = x;
					int color = 0;
					if(y>0) color = 128 + y; //gray level
					/*
					RgbColor blueRgbColor = RgbColor(0,0,y);
					HsvColor blueHsvColor = RgbToHsv(blueRgbColor);
					HsvColor myHsvColor(blueHsvColor.h, blueHsvColor.s, blueHsvColor.v); //HsvColor myHsvColor(whiteHsvColor.h, whiteHsvColor.s, 128 + y);
					RgbColor myRgbColor = HsvToRgb(myHsvColor);
					*/
					_specbuf[j*_SPECWIDTH * 3 + i * 3] = color; // = myRgbColor.b;  //= color; //B // idcolorch0_b; //c & 1 ? idcolorch1_b : idcolorch0_b; //B
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = 0; //= myRgbColor.g;  //= color; //G // idcolorch0_g; //c & 1 ? idcolorch1_g : idcolorch0_g; //G
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = 0; // = myRgbColor.r;  //= color; //R // idcolorch0_r; //c & 1 ? idcolorch1_r : idcolorch0_r; //R
				}
				// move marker onto next position
				_specpos = (_specpos + 1) % _SPECWIDTH;
				for (x = 0; x < _SPECHEIGHT; x++)
				{
					//todo
					//_specbuf[x*_SPECWIDTH + _specpos] = 255; //draws marker
					int i = _specpos;
					int j = x;
					int color = 255; //white
					_specbuf[j*_SPECWIDTH * 3 + i * 3] = color; //B // idcolorch0_b; //c & 1 ? idcolorch1_b : idcolorch0_b; //B
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 1] = color; //G // idcolorch0_g; //c & 1 ? idcolorch1_g : idcolorch0_g; //G
					_specbuf[j*_SPECWIDTH * 3 + i * 3 + 2] = color; //R // idcolorch0_r; //c & 1 ? idcolorch1_r : idcolorch0_r; //R
				}

			}
		}
		else
		{
			memset(_specbuf, 0, _SPECWIDTH*_SPECHEIGHT*3);
		}
	}

	// update the display
	dc = GetDC(_hwnd);
	BitBlt(dc, 0, 0, _SPECWIDTH, _SPECHEIGHT, _specdc, 0, 0, SRCCOPY);
	ReleaseDC(_hwnd, dc);
	//2021nov29, spi, begin
	_prev_specmode = _specmode;
	//2021nov29, spi, end
	_busy_painting = false;
}

void CSpectrum24bitWindow::InvalidateDIBPalette(RGBQUAD* newdibpalette)
{
	/*
	_dibpalette = newdibpalette;
	_canvas_ready = false;
	*/
}