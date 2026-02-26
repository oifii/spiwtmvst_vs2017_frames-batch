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
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"
#include "CParameter.h"
#include "CWindowParameter.h"
#include "Tonic.h"
using namespace Tonic;

//#include "c_pointset.h"
#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL

#include "spivorogui.h"
#include "CVoroguiWindow.h"
#include "CVorossWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>
#include "defs.h"
#include "CSpectrumWindow.h"
#include "CSpectrum24bitWindow.h"
#include "CImageWindow.h"
#include "CTextWindow.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "ChannelBuffersHandler.h"
#include "SpectrumDisplay.h"
#include "AutocorrDisplay.h"
#include "WaveformDisplay.h"
#include "AutocorrHandler.h"
#include <math.h>
#include "AutoRescaleOw2doc.h"

#include "CTimeframeWindow.h"

using namespace std;



CVoroguiWindow::CVoroguiWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int voroguiclustermode)
: CWindowParameter(lpszCaption, lpwndClassEx)
{
	_pSynth = pSynth;
	_windowparameterpointers = windowparameterpointers;
	_dib = dib;
	_imagefilenames = imagefilenames;
	vorogui_clustermode = voroguiclustermode;
	Initialize();
}

CVoroguiWindow::CVoroguiWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int voroguiclustermode,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_pSynth = pSynth;
	_windowparameterpointers = windowparameterpointers;
	_dib = dib;
	_imagefilenames = imagefilenames;
	vorogui_clustermode = voroguiclustermode;
	Initialize();
}

CVoroguiWindow::CVoroguiWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int voroguiclustermode,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_pSynth = pSynth;
	_windowparameterpointers = windowparameterpointers;
	_dib = dib;
	_imagefilenames = imagefilenames;
	vorogui_clustermode = voroguiclustermode;
	Initialize();
}

void CVoroguiWindow::Initialize()
{
	//2021nov29, spi, begin
	p_spilogfile = NULL;
	if (SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPIVOROGUIWINDOW_DEBUG)
	{
		p_spilogfile = DBG_NEW spilogfile("cvoroguiwindow_debug.txt", true);
	}

	//2024aug31, spi, begin
	//added for CVorossWindow to save a frame on user's request
	/*
	//global_ffmpegpath = "c:\\video\\ffmpeg.exe";
	//global_ffmpegpath = "d:\\spibin\\ffmpeg\\ffmpeg.exe";
	global_ffmpegpath = FFMPEG_APP_PATH;
	//global_outputvideofoldername = "c:\\temp\\spiwtmvstf-shuffle";
	global_outputvideofoldername = "d:\\temp-spiwtmvstf-shuffle";
	global_outputvideofilenameext = "mp4";
	global_maxnumberofframeperseedimage = 200;
	global_outputvideoframepersecond = 10;
	*/
	global_framefilenameprefix = "frame_";
	global_framefilenameext = "bmp";

	currentlysavingframe = false;
	//skipsaveframes = true;
	global_outputfoldername = "frames";
	global_frameid = 0;
	//2024aug31, spi, end


	_hfont = 0;
	_prevhfont = 0;
	_dc=0;
	_bmp=0;
	_buf=0;
	_mode=0;
	_prevHGDIOBJ = NULL;
	_dc_ref = 0;
	_bmp_ref = 0;
	_buf_ref = 0;
	_prevHGDIOBJ_ref = NULL;
	_WIDTH=-1; //actual canvas display width
	_HEIGHT=-1; //actual canvas display height 
	_canvas_allocated = false;
	_canvas_rendered = false;
	_canvas_rendered_ref = false;
	_canvas_ready = false;
	_busy_painting = false;
	p_spicanvas = NULL;
	hpen = NULL;
	hpen_green = NULL;
	hbrush = NULL;
	hatchhbrush = NULL;
	//2021nov29, spi, end

	/* //code for vorogswindow's initialization
	if (!IsVorossWindow())
	{
		//2021dec29, spi, begin
		//addParameter("voro_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
		//2021dec29, spi, end

		//2022may21, spi, begin
		//adding new vorogui parameters
		//adding new vorogui toggle parameters
		addParameter("voro_spitext", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_spivoice", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_spirecord", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_cpulocal", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_cpuglobal", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_memlocal", 0.0, 1.0, 0.0, ParameterTypeToggle);
		addParameter("voro_memglobal", 0.0, 1.0, 0.0, ParameterTypeToggle);
		//adding new vorogui continuous parameters
		addParameter("voro_mode", 0.0, numberofvoroguiclustermodes - 1.0f, 1.0, ParameterTypeContinuous); //voro_voroguimode
		//2022may21, spi, end

		CSpectrumWindow* pSpectrumWindow = NULL;
		for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
		{
			pSpectrumWindow = dynamic_cast<CSpectrumWindow*>(*it);
			if (pSpectrumWindow) break;
		}
		if (pSpectrumWindow)
		{
			addParameter("voro_autospeconoff", 0.0, 1.0, 0.0, ParameterTypeToggle);
			addParameter("voro_autospecperiod", 3.0, 30.0, 3.0, ParameterTypeContinuous);
			addParameter("voro_autospecmode", 0.0, 5.0, 0.0, ParameterTypeContinuous);
		}

		CSpectrum24bitWindow* pSpectrum24bitWindow = NULL;
		for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
		{
			pSpectrum24bitWindow = dynamic_cast<CSpectrum24bitWindow*>(*it);
			if (pSpectrum24bitWindow) break;
		}
		if (pSpectrum24bitWindow)
		{
			addParameter("voro_autospec24onoff", 0.0, 1.0, 0.0, ParameterTypeToggle);
			addParameter("voro_autospec24period", 3.0, 30.0, 3.0, ParameterTypeContinuous);
		}

		CImageWindow* pImageWindow = NULL;
		for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
		{
			pImageWindow = dynamic_cast<CImageWindow*>(*it);
			if (pImageWindow) break;
		}
		if (pImageWindow)
		{
			addParameter("voro_autoimgonoff", 0.0, 1.0, 0.0, ParameterTypeToggle);
			addParameter("voro_autoimgperiod", 3.0, 30.0, 3.0, ParameterTypeContinuous);
		}
	}
	*/

	/*
	if (!IsVorossWindow())
	{
		///////////////////////////////////////////////////////////////////////
		//derived class will mostlikely need this section to be in Initialize() 
		///////////////////////////////////////////////////////////////////////
		_windowparameterpointers.push_back(this);
		_pVOROGUI = (spivorogui*)DEBUG_NEW spivorogui(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode); //_pVOROGUI = VOROGUI_Init(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode);
		if (_pVOROGUI) _pVOROGUI->readfromdisk();

		_imageid = 0;
		_nextdib = NULL;
		_nextquantizeddib = NULL;
		//2021nov29, spi, begin
		_prev_imagewidth = -1;
		_prev_imageheight = -1;
		_prev_displaytextlabelflag = false;
		//2021nov29, spi, end

		//2022nov18, spi, begin
		_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORALLCONTROL;
		//2022nov18, spi, end
	}
	*/
}

void CVoroguiWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "voro_showwnd")
	{
		float showflag = getParameterValue("voro_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}
	//2022may21, spi, begin
	//adding new vorogui parameters
	//adding new vorogui toggle parameters
	else if (name == "voro_spitext")
	{
		float mode = getParameterValue("voro_spitext");
		if (mode >= 0.5)
		{
			global_spitextmode = 1; //on
		}
		else
		{
			global_spitextmode = 0; //off
		}
	}
	else if (name == "voro_spivoice")
	{
		float mode = getParameterValue("voro_spivoice");
		if (mode >= 0.5)
		{
			global_spivoicemode = 1; //on
		}
		else
		{
			global_spivoicemode = 0; //off
		}
	}
	else if (name == "voro_spirecord")
	{
		float mode = getParameterValue("voro_spirecord");
		if (mode >= 0.5)
		{
			global_spirecordmode = 1; //on
		}
		else
		{
			global_spirecordmode = 0; //off
		}
	}
	else if (name == "voro_cpulocal")
	{
		float mode = getParameterValue("voro_cpulocal");
		if (mode >= 0.5)
		{
			global_cpulocalmode = 1; //on
		}
		else
		{
			global_cpulocalmode = 0; //off
		}
	}
	else if (name == "voro_cpuglobal")
	{
		float mode = getParameterValue("voro_cpuglobal");
		if (mode >= 0.5)
		{
			global_cpuglobalmode = 1; //on
		}
		else
		{
			global_cpuglobalmode = 0; //off
		}
	}
	else if (name == "voro_memlocal")
	{
		float mode = getParameterValue("voro_memlocal");
		if (mode >= 0.5)
		{
			global_memlocalmode = 1; //on
		}
		else
		{
			global_memlocalmode = 0; //off
		}
	}
	else if (name == "voro_memglobal")
	{
		float mode = getParameterValue("voro_memglobal");
		if (mode >= 0.5)
		{
			global_memglobalmode = 1; //on
		}
		else
		{
			global_memglobalmode = 0; //off
		}
	}
	//adding new vorogui continuous parameters
	//addParameter("voro_voroguimode", 0.0, numberofvoroguiclustermodes-1.0f, 1.0, ParameterTypeContinuous);
	else if (name == "voro_mode") //voro_voroguimode
	{
		float mode = getParameterValue("voro_mode");
		//call setclustermode(int clustermode)
		//refresh vorogui window
	}
	//2022may21, spi, end
	else if (name == "voro_autospeconoff")
	{
		float onoffflag = getParameterValue("voro_autospeconoff");
		if (onoffflag >= 0.5)
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC);
				float period_s = getParameterValue("voro_autospecperiod");
				SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC, period_s*1000, (TIMERPROC)NULL);
			}
		}
		else
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC);
			}
		}
	}
	else if (name == "voro_autospecperiod")
	{
		float period_s = getParameterValue("voro_autospecperiod");
		if (_hwnd)
		{
			KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC);
			SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC, period_s*1000, (TIMERPROC)NULL);
		}
	}
	else if (name == "voro_autospec24onoff")
	{
		float onoffflag = getParameterValue("voro_autospec24onoff");
		if (onoffflag >= 0.5)
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24);
				float period_s = getParameterValue("voro_autospec24period");
				SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24, period_s * 1000, (TIMERPROC)NULL);
			}
		}
		else
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24);
			}
		}
	}
	else if (name == "voro_autospec24period")
	{
		float period_s = getParameterValue("voro_autospec24period");
		if (_hwnd)
		{
			KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24);
			SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24, period_s * 1000, (TIMERPROC)NULL);
		}
	}
	else if (name == "voro_autoimgonoff")
	{
		float onoffflag = getParameterValue("voro_autoimgonoff");
		if (onoffflag >= 0.5)
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG);
				float period_s = getParameterValue("voro_autoimgperiod");
				SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG, period_s * 1000, (TIMERPROC)NULL);
			}
		}
		else
		{
			if (_hwnd)
			{
				KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG);
			}
		}
	}
	else if (name == "voro_autoimgperiod")
	{
		float period_s = getParameterValue("voro_autoimgperiod");
		if (_hwnd)
		{
			KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG);
			SetTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG, period_s * 1000, (TIMERPROC)NULL);
		}
	}
}

CVoroguiWindow::~CVoroguiWindow()
{
	if (_pVOROGUI)
	{
		delete _pVOROGUI; //VOROGUI_Terminate(_pVOROGUI);
		_pVOROGUI = NULL;
	}
	//if (_pPOINTSET) VOROGUI_DestroyPointset(_pVOROGUI, _pPOINTSET);
	if (_hwnd) KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC);
	if (_hwnd) KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOSPEC24);
	//2022july25, spi, begin
	if (_hwnd) KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_AUTOIMG); //bugfix
	//not used yet, using time stamped variable instead
	//if (_hwnd) KillTimer(_hwnd, VOROGUIWINDOW_IDTIMER_VOROGUI_IDPOINTRECENTLYDISPLAYED); //new timer
	//2022july25, spi, end
	if (_nextdib) FreeImage_Unload(_nextdib);
	if (_nextquantizeddib) FreeImage_Unload(_nextquantizeddib);
	//2021nov29, spi, begin
	if (_hfont)
	{
		SelectObject(_dc, _prevhfont);
		DeleteObject(_hfont);
	}
	if (_prevHGDIOBJ &&_dc) SelectObject(_dc, _prevHGDIOBJ);
	if (_bmp) DeleteObject(_bmp);
	if (_dc) DeleteDC(_dc);
	if (_prevHGDIOBJ_ref && _dc_ref) SelectObject(_dc_ref, _prevHGDIOBJ_ref);
	if (_bmp_ref) DeleteObject(_bmp_ref);
	if(_dc_ref) DeleteDC(_dc_ref);
	if (p_spilogfile) delete p_spilogfile;
	if (p_spicanvas) delete[] p_spicanvas;
	//2021nov29, spi, end
}

//allocates canvas (if needed) and renders first layer 
void CVoroguiWindow::CreateCanvas()
{
	//createcanvas's job is to allocate 2 mem dcs, 2 mem bmps, and 2 mem bufs for each canvas size
	//at the minimum, i.e remotedroide has two 4k monitors and one 2k monitor, therefore that make 2 unique canvas sizes, 
	//but since windows are displayed either full screen, 4 per screen, 16 per screen or 64 per screen, that makes a total
	//of 5 different canvas sizes.
	//so, for each canvas size, createcanvas's job is to allocate 5*2 mem dcs and to allocate 5*2 mem bmps, and to associate 5*2 mem bufs 
	//2 because VOROGUIWINDOW_NUMBEROFBUFFERS_PERCANVASSIZE is 2
	//5 because MonitorRects' canvassizesvector; //unique canvas sizes, i.e. on remotedroide, this vector would contain 5 unique canvas sizes: 3840x2160, 1920x1080, 960x540, 480x270, and 240x135

	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	int width = rcClient.right - 0;
	int height = _HEIGHT = rcClient.bottom - 0;

	int const numberofcanvas = _monitors.canvassizesvector.size();
	bool samesizecanvasfound = false;
	int index = -1;
	if (p_spicanvas)
	{
		SIZE size = {width, height};
		index = MatchCanvas(size);
		if (index > -1 && index < numberofcanvas)
		{
			samesizecanvasfound = true;
		}
	}
	if(!samesizecanvasfound || (p_spicanvas && samesizecanvasfound && !(p_spicanvas[index]._canvas_allocated)))
	{
		if (_canvas_allocated && _prev_imagewidth == width && _prev_imageheight == height)
		{
			//just render
		}
		else
		{
			//reallocate and render
			CreateCanvas(width, height, _WIDTH, _HEIGHT,
				_dc, _bmp, _buf,
				_dc_ref, _bmp_ref, _buf_ref,
				_prevHGDIOBJ, _prevHGDIOBJ_ref,
				_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
				VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
		}
		RenderCanvas(width, height, _WIDTH, _HEIGHT,
			_dc, _bmp, _buf,
			_dc_ref, _bmp_ref, _buf_ref,
			_prevHGDIOBJ, _prevHGDIOBJ_ref,
			_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
			VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
	}
	else
	{
		//use precreated/preallocated canvas
		if (p_spicanvas[index]._canvas_ready)
		{
			//nothing else to do
		}
		else
		{
			if (!(p_spicanvas[index]._canvas_rendered))
			{
				//only render first layer
				int i = index;
				RenderCanvas(width, height, (p_spicanvas[i]._size.cx), (p_spicanvas[i]._size.cy),
								(p_spicanvas[i]._hdc), (p_spicanvas[i]._hbitmap), (p_spicanvas[i]._buf),
								(p_spicanvas[i]._hdc_ref), (p_spicanvas[i]._hbitmap_ref), (p_spicanvas[i]._buf_ref),
								(p_spicanvas[i]._prev_hgdiobj), (p_spicanvas[i]._prev_hgdiobj_ref),
								(p_spicanvas[i]._canvas_allocated), (p_spicanvas[i]._canvas_rendered), (p_spicanvas[i]._canvas_rendered_ref), (p_spicanvas[i]._canvas_ready),
								VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);

			}
		}
	}
}

//allocate all required canvas but does not render
void CVoroguiWindow::PreCreateCanvas()
{
	//createcanvas's job is to allocate 2 mem dcs, 2 mem bmps, and 2 mem bufs for each canvas size
	//at the minimum, i.e remotedroide has two 4k monitors and one 2k monitor, therefore that make 2 unique canvas sizes, 
	//but since windows are displayed either full screen, 4 per screen, 16 per screen or 64 per screen, that makes a total
	//of 5 different canvas sizes.
	//so, for each canvas size, createcanvas's job is to allocate 5*2 mem dcs and to allocate 5*2 mem bmps, and to associate 5*2 mem bufs 
	//2 because VOROGUIWINDOW_NUMBEROFBUFFERS_PERCANVASSIZE is 2
	//5 because MonitorRects' canvassizesvector; //unique canvas sizes, i.e. on remotedroide, this vector would contain 5 unique canvas sizes: 3840x2160, 1920x1080, 960x540, 480x270, and 240x135
	if (1)
	{
		int const numberofcanvas = _monitors.canvassizesvector.size();
		p_spicanvas = DBG_NEW spicanvas[numberofcanvas];
		if (p_spicanvas)
		{
			for (int i = 0; i < numberofcanvas; i++)
			{
				/*
				RECT rcClient;
				GetClientRect(_hwnd, &rcClient);
				int width = rcClient.right - 0;
				int height = _HEIGHT = rcClient.bottom - 0;
				*/
				int width = _monitors.canvassizesvector[i].cx;//CreateCanvas() will return actual width in p_spicanvas[i]._size.cx;
				int height = _monitors.canvassizesvector[i].cy;//CreateCanvas() will return actual width in p_spicanvas[i]._size.cy;
				/*
				CreateCanvas(width, height, _WIDTH, _HEIGHT,
								_dc, _bmp, _buf,
								_dc_ref, _bmp_ref, _buf_ref,
								VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
				*/
				CreateCanvas(width, height, (p_spicanvas[i]._size.cx), (p_spicanvas[i]._size.cy),
								(p_spicanvas[i]._hdc), (p_spicanvas[i]._hbitmap), (p_spicanvas[i]._buf),
								(p_spicanvas[i]._hdc_ref), (p_spicanvas[i]._hbitmap_ref), (p_spicanvas[i]._buf_ref),
								(p_spicanvas[i]._prev_hgdiobj), (p_spicanvas[i]._prev_hgdiobj_ref),
								(p_spicanvas[i]._canvas_allocated), (p_spicanvas[i]._canvas_rendered), (p_spicanvas[i]._canvas_rendered_ref), (p_spicanvas[i]._canvas_ready),
								VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
			}
		}
	}
}

int CVoroguiWindow::MatchCanvas(int width, int height)
{
	int const numberofcanvas = _monitors.canvassizesvector.size();
	int index = _monitors.MatchCanvasSize(width, height);
	//valid index>-1 && index < numberofcanvas
	return index;
}

int CVoroguiWindow::MatchCanvas(SIZE size)
{
	LONG width = size.cx; 
	LONG height = size.cy;
	return MatchCanvas(width, height);
}

//allocates canvas but does not render
void CVoroguiWindow::CreateCanvas(int width, int height, LONG &width_actual, LONG &height_actual, 
									HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer, 
									HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref, 
									HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
									bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
									int numbyteperpixel/*=3*/, int createcanvasflag/*=VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR*/)
{
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "CreateCanvas() ..." << endl;
		p_spilogfile->writeline(ss.str());
	}
	//2021dec28, spi, begin
	//if (_hwnd == NULL) return;
	//2021dec28, spi, end

	canvas_ready = false;
	while (_busy_painting) Sleep(1);
	//2021nov29, spi, begin
	if (prev_hgdiobj)
	{
		SelectObject(hdc, prev_hgdiobj); //unselect hbitmap
		prev_hgdiobj = NULL;
	}
	if (hdc)
	{
		DeleteDC(hdc);
		hdc = 0;
	}
	if (hbitmap)
	{
		DeleteObject(hbitmap);
		hbitmap = 0;
	}
	//because stretch blit displaced into create canvas, optimization now requires duplicates of hdc, hbitmap, _buf, and prevHGDIOBJ that are tagged _ref
	if (prev_hgdiobj_ref)
	{
		SelectObject(hdc_ref, prev_hgdiobj_ref); //unselect hbitmap
		prev_hgdiobj_ref = NULL;
	}
	if (hdc_ref)
	{
		DeleteDC(hdc_ref);
		hdc_ref = 0;
	}
	if (hbitmap_ref)
	{
		DeleteObject(hbitmap_ref);
		hbitmap_ref = 0;
	}
	canvas_allocated = false;
	//2021nov29, spi, end

	/*
	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	_WIDTH = rcClient.right - 0;
	_HEIGHT = rcClient.bottom - 0;
	*/
	width_actual = width;
	height_actual = height;
	//2021nov29, spi, begin
	if (createcanvasflag == VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR)
	{
		if ((width_actual <= (width_actual % 4)) || (height_actual <= 0))
		{
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "CreateCanvas() done." << endl;
				p_spilogfile->writeline(ss.str());
			}
			return;
		}
		if (width_actual % 4)
		{
			width_actual = width_actual - (width_actual % 4); //ensure _WIDTH is divisible by 4
		}
		if (width_actual <= 0)
		{
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "CreateCanvas() done." << endl;
				p_spilogfile->writeline(ss.str());
			}
			return;
		}
	}
	else
	{
		if (width_actual == 0 || height_actual == 0)
		{
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "CreateCanvas() done." << endl;
				p_spilogfile->writeline(ss.str());
			}
			return;
		}
	}
	//_hFont = CreateFontW(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"Arial");
	//_hFont = CreateFontW(_HEIGHT / 50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"Arial");
	//2021nov29, spi, end

	// create bitmap to draw spectrum in (24 bit for easy updating)
	BYTE data[2000] = { 0 };
	BITMAPINFOHEADER *bh = (BITMAPINFOHEADER*)data;
	RGBQUAD *pal = (RGBQUAD*)(data + sizeof(*bh));
	int a;
	bh->biSize = sizeof(*bh);
	bh->biWidth = width_actual;
	bh->biHeight = height_actual; // upside down (line 0=bottom)
	bh->biSizeImage = width_actual * height_actual * numbyteperpixel;
	bh->biPlanes = 1;
	bh->biCompression = BI_RGB;
	bh->biBitCount = 8* numbyteperpixel; //24 = 3 * 8 bit 
	bh->biClrUsed = bh->biClrImportant = 0;

	// create the bitmap
	hbitmap = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
	//2021nov29, spi, begin
	//because stretch blit displaced into create canvas, optimization now requires duplicates of hdc, hbitmap, buffer, and prevHGDIOBJ that are tagged _ref
	hbitmap_ref = CreateDIBSection(0, (BITMAPINFO*)bh, DIB_RGB_COLORS, (void**)&buffer_ref, NULL, 0);
	HDC dc = 0; // if(_hwnd) dc=GetDC(_hwnd);
	if (dc)
	{
		hdc = CreateCompatibleDC(dc);
	}
	else
	{
		hdc = CreateCompatibleDC(0);
	}
	//because stretch blit displaced into create canvas, optimization now requires duplicates of hdc, hbitmap, buffer, and prevHGDIOBJ that are tagged _ref
	if (dc)
	{
		hdc_ref = CreateCompatibleDC(dc);
	}
	else
	{
		hdc_ref = CreateCompatibleDC(0);
	}

	if (hbitmap && hdc && hbitmap_ref && hdc_ref) //if (hbitmap && hdc)
	{
		canvas_allocated = true;
	}

	if(canvas_allocated)
	{
		//2021dec29, spi, begin
		/* 
		RenderCanvas(width, height, width_actual, height_actual,
						hdc, hbitmap, buffer,
						hdc_ref, hbitmap_ref, buffer_ref,
						prev_hgdiobj, prev_hgdiobj_ref,
						canvas_allocated, canvas_rendered, canvas_rendered_ref, canvas_ready,
						numbyteperpixel, createcanvasflag);
		*/
		//2021dec29, spi, end
		if (dc && _hwnd) ReleaseDC(_hwnd, dc);
		canvas_allocated = true;
		canvas_rendered = false;
		canvas_rendered_ref = false;
		canvas_ready = false;
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "CreateCanvas() allocated canvas of width and height " << width_actual << " and " << height_actual << endl;
			ss << "CreateCanvas() done." << endl;
			p_spilogfile->writeline(ss.str());
		}
		return;
	}
	else
	{
		//error, return safely
		if (hdc)
		{
			//hbitmap is 0
			//SelectObject(hdc, prevHGDIOBJ); //unselect hbitmap
			DeleteDC(hdc);
			hdc = 0;
		}
		if (hbitmap)
		{
			DeleteObject(hbitmap);
			hbitmap = 0;
		}
		//if (dc) ReleaseDC(_hwnd, dc);
		//because stretch blit displaced into create canvas, optimization now requires duplicates of hdc, hbitmap, buffer, and prevHGDIOBJ that are tagged _ref
		if (hdc_ref)
		{
			//hbitmap is 0
			//SelectObject(hdc, prevHGDIOBJ); //unselect hbitmap
			DeleteDC(hdc_ref);
			hdc_ref = 0;
		}
		if (hbitmap_ref)
		{
			DeleteObject(hbitmap_ref);
			hbitmap_ref = 0;
		}
		//_canvas_ready = false;
		canvas_allocated = false;
		canvas_rendered = false;
		canvas_rendered_ref = false;
		canvas_ready = false;
		if (dc && _hwnd) ReleaseDC(_hwnd, dc);
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "CreateCanvas() did not allocate canvas" << endl;
			ss << "CreateCanvas() done." << endl;
			p_spilogfile->writeline(ss.str());
		}
		return;
	}
	if (dc && _hwnd) ReleaseDC(_hwnd, dc);
	//_canvas_ready = false;
	canvas_allocated = false;
	canvas_rendered = false;
	canvas_rendered_ref = false;
	canvas_ready = false;
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "CreateCanvas() did not allocate canvas" << endl;
		ss << "CreateCanvas() done." << endl;
		p_spilogfile->writeline(ss.str());
	}
	return;
	//2021nov29, spi, end
}

void CVoroguiWindow::RescaleVoroguiIfNeeded(int imagewidth, int imageheight, LONG &width_actual, LONG &height_actual,
	HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
	HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
	HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
	bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
	int numbyteperpixel/*=3*/, int createcanvasflag/*=VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR*/)
{
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RescaleVoroguiIfNeeded() ..." << endl;
		p_spilogfile->writeline(ss.str());
	}

	BITMAP myDDB = {};
	if(hbitmap) //if (_bmp)
	{
		//GetObject(_bmp, sizeof(BITMAP), &myDDB);
		GetObject(hbitmap, sizeof(BITMAP), &myDDB);  
		//if ((myDDB.bmWidth == _prev_imagewidth) && (myDDB.bmHeight == _prev_imageheight)
		//	&& (myDDB.bmWidth == imagewidth) && (myDDB.bmHeight == imageheight))
		if(0)
		{
			//bcanvas_dirty = false;
			canvas_rendered = true; //meaning not dirty
			canvas_ready = true; //meaning not dirty
		}
		else
		{
			if (_pVOROGUI && _pVOROGUI->needsrescale())
			{
				POINTSET* _pPOINTSET = _pVOROGUI->m_pPOINTSET;
				//invalidate canvas
				//bcanvas_dirty = true;
				//2021dec29, spi, begin
				/*
				canvas_rendered = false; //meaning dirty
				canvas_rendered_ref = false;
				canvas_ready = false; //meaning dirty
				*/
				//2021dec29, spi, end
				//rescale pointset
				if (1)
				{
					double x_scale = 1.0;
					double y_scale = 1.0;
					if (_pVOROGUI->m_pPOINTSET->xmax > VOROGUI_X_RESOLUTION)
					{
						x_scale = (float)_pVOROGUI->vorogui_xwidth / _pPOINTSET->xmax;
					}
					if (_pPOINTSET->ymax > VOROGUI_Y_RESOLUTION)
					{
						y_scale = (float)_pVOROGUI->vorogui_yheight / _pPOINTSET->ymax;
					}
					if (x_scale != 1.0 || y_scale != 1.0)
					{
						ScalePointset(_pPOINTSET, x_scale, y_scale);
						//2022may16, spi, begin
						/*
						BuildTriangleNetwork(_pPOINTSET);
						ComputeAllTriangleCenters(_pPOINTSET);
						*/
						_pVOROGUI->BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
						//2022may16, spi, end
					}
					if (p_spilogfile && global_debugmode)
					{
						stringstream ss;
						ss << "RescaleVoroguiIfNeeded() did a poinset rescale of x_scale and y_scale " << x_scale << " and " << y_scale << endl;
						p_spilogfile->writeline(ss.str());
					}
				}
			}
		}
	}
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RescaleVoroguiIfNeeded() done." << endl;
		p_spilogfile->writeline(ss.str());
	}
	return;
}

void CVoroguiWindow::RenderCanvas(int width, int height, LONG &width_actual, LONG &height_actual,
									HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
									HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
									HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
									bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
									int numbyteperpixel/*=3*/, int createcanvasflag/*=VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR*/)
{
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RenderCanvas() ..." << endl;
		p_spilogfile->writeline(ss.str());
	}

	if(prev_hgdiobj==NULL) prev_hgdiobj = SelectObject(hdc, hbitmap);
	//because stretch blit displaced into create canvas, optimization now requires duplicates of hdc, hbitmap, buffer, and prevHGDIOBJ that are tagged _ref
	if(prev_hgdiobj_ref==NULL) prev_hgdiobj_ref = SelectObject(hdc_ref, hbitmap_ref);

	/*
	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	int imagewidth = rcClient.right - 0;
	int imageheight = rcClient.bottom - 0;
	*/
	
	if (_pVOROGUI == NULL || _pVOROGUI->m_pPOINTSET == NULL || (width_actual == 0) || (height_actual == 0))
	{
		if (global_debugmode)
		{
			//memset();
		}
		canvas_rendered = false;
		canvas_rendered_ref = false;
		canvas_ready = false;
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "RenderCanvas() abort" << endl;
			ss << "RenderCanvas() done." << endl;
			p_spilogfile->writeline(ss.str());
		}
		return; //(imagewidth == 0) || (imageheight == 0)) return;
	}

	//if(0) //not for now while debugging
	if (_dib)
	{
		//now optimized, this was slow, because was done on each update no matter for which drawing mode
		int dib_width = FreeImage_GetWidth(_dib);
		int dib_height = FreeImage_GetHeight(_dib);
		if (dib_width > 0 && dib_height > 0)
		{
			BYTE* pBYTE = FreeImage_GetBits(_dib);
			BITMAPINFO* pBITMAPINFO = FreeImage_GetInfo(_dib);
			if (pBYTE && pBITMAPINFO)
			{
				//draw background image
				SetStretchBltMode(hdc, COLORONCOLOR); //do it on every WM_PAINT, AlphaBlend uses it
				StretchDIBits(hdc, 0, 0, width_actual, height_actual, //imagewidth, imageheight,
					0, 0, dib_width, dib_height,
					pBYTE, pBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
			}
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "RenderCanvas() rendered background dib onto width and height " << width_actual << " and " << height_actual << endl;
				p_spilogfile->writeline(ss.str());
			}
		}
	}
	if (1)
	{
		_pVOROGUI->drawvorogui(hdc, POINTSET_ALL, _dib, true, false, false); //no matter if _dib null or valid
		//BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }; //255, 127, 64, 32
		//BOOL result = AlphaBlend(hdc, 0, 0, imagewidth, imageheight, hdc, 0, 0, imagewidth, imageheight, blend);
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "RenderCanvas() rendered vorogui of width and height " << _pVOROGUI->vorogui_xwidth << " and " << _pVOROGUI->vorogui_yheight << endl;
			p_spilogfile->writeline(ss.str());
		}
	}
	if (0)
	{
		prev_hgdiobj = SelectObject(hdc, prev_hgdiobj); //unselect hbitmap
		prev_hgdiobj_ref = SelectObject(hdc_ref, prev_hgdiobj_ref); //unselect hbitmap_ref
	}
	//_canvas_ready = true;
	canvas_rendered = true;
	canvas_rendered_ref = false;
	canvas_ready = true;
	//if (dc) ReleaseDC(_hwnd, dc);
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RenderCanvas() done." << endl;
		p_spilogfile->writeline(ss.str());
	}
	return;
}

bool CVoroguiWindow::RenderVectorLayer(int width, int height, LONG &width_actual, LONG &height_actual,
	HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
	HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
	HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
	bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
	int numbyteperpixel/*=3*/, int createcanvasflag/*=VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR*/, bool force_dc_ref_refresh/*=true*/, HDC hdc_physical/*=NULL*/)
{
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RenderVectorLayer() ..." << endl;
		p_spilogfile->writeline(ss.str());
	}

	bool dc_ref_refreshed = false;

	HDC dc = hdc_physical; //supplied physical dc
	bool mustreleasedc = false;
	if (dc == NULL) 
	{
		dc = GetDC(_hwnd); //acquired physical dc
		if(dc) mustreleasedc = true;
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "RenderVectorLayer() acquired dc" << endl;
			p_spilogfile->writeline(ss.str());
		}
	}
	else
	{
		dc = hdc_physical; //physical
	}
	if (_mode == 0 && dc)
	{
		//default vorogui mode, voronoi diagram with control levels
		//since voronoi diagram is already drawn in createcanvas(),
		//in here ...
		/*
		//on dc, for drawing using pen and brush
		HPEN hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HPEN hpen_green = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		HBRUSH hbrush = CreateSolidBrush(RGB(50, 50, 50));
		//HBRUSH hbrush = CreateSolidBrush(RGB(100, 100, 100));
		HBRUSH hatchhbrush = CreateHatchBrush(HS_DIAGCROSS, RGB(50, 50, 50));
		*/
		HBRUSH nullhbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HPEN oldhpen = (HPEN)SelectObject(dc, hpen);
		//HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, hbrush); //original
		HBRUSH oldhbrush = (HBRUSH)SelectObject(dc, nullhbrush); //for debuging
		//2022dec25, spi, begin
		int iOldMixMode = SetROP2(dc, R2_MERGEPEN); //original
		//int iOldMixMode = SetROP2(dc, R2_XORPEN); //spi test
		//2022dec25, spi, end
		//on dc, for drawing text
		COLORREF crOldBkColor = SetBkColor(dc, RGB(0x00, 0x00, 0x00)); //spi
		COLORREF crOldTextColor = SetTextColor(dc, RGB(0xFF, 0xFF, 0xFF));
		int iOldBkMode = SetBkMode(dc, TRANSPARENT);
		//HFONT hFont = CreateFontW(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
		/*
		if (_hfont)
		{
			SelectObject(hdc, _prevhfont);
			DeleteObject(_hfont);
		}
		_hfont = CreateFontW(height_actual / 50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"Arial");
		_prevhfont = (HFONT)SelectObject(dc, _hfont);
		*/
		HFONT hfont = CreateFontW(height_actual / 50, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"Arial");
		HFONT prevhfont = (HFONT)SelectObject(dc, hfont);
		//on hdc, for drawing using pen and brush
		HPEN oldhpen_dc = (HPEN)SelectObject(hdc, hpen);
		HBRUSH oldhbrush_dc = (HBRUSH)SelectObject(hdc, hbrush); //original
		//2022dec25, spi, begin
		int iOldMixMode_dc = SetROP2(hdc, R2_MERGEPEN); //original
		//int iOldMixMode_dc = SetROP2(hdc, R2_XORPEN); //spi, test
		//2022dec25, spi, end
		int iOldBkMode_dc = SetBkMode(hdc, TRANSPARENT);
		//on dc_ref, for drawing using pen and brush
		HPEN oldhpen_dc_ref = (HPEN)SelectObject(hdc_ref, hpen);
		HBRUSH oldhbrush_dc_ref = (HBRUSH)SelectObject(hdc_ref, hbrush); //original
		//2022dec25, spi, begin
		int iOldMixMode_dc_ref = SetROP2(hdc_ref, R2_MERGEPEN); //original
		//int iOldMixMode_dc_ref = SetROP2(hdc_ref, R2_XORPEN); //spi, test
		//2022dec25, spi, end
		int iOldBkMode_dc_ref = SetBkMode(hdc_ref, TRANSPARENT); //0; // SetBkMode(_dc_ref, TRANSPARENT);

		//1) need to redraw _dc_ref?
		if (hdc && hdc_ref && (force_dc_ref_refresh == true || (_pVOROGUI->vorogui_idpointdirty == POINTSET_ALL)))
		{
			//on _dc_ref, bitblt _dc (the voronoi diagram) 
			//ensure bitmap are selected
			if(prev_hgdiobj==NULL) prev_hgdiobj = SelectObject(hdc, hbitmap);
			//2022nov15, spi, begin
				//else SelectObject(hdc, hbitmap); //new line
			//2022nov15, spi, end
			if(prev_hgdiobj_ref==NULL) prev_hgdiobj_ref = SelectObject(hdc_ref, hbitmap_ref);
			//2022nov15, spi, begin
				//else SelectObject(hdc_ref, hbitmap_ref); //new line
			//2022nov15, spi, end
			//BitBlt(hdc_ref, 0, 0, _WIDTH, _HEIGHT, hdc, 0, 0, SRCCOPY);
			BitBlt(hdc_ref, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
			dc_ref_refreshed = true;
			_pVOROGUI->vorogui_idpointdirty = POINTSET_ALL;
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "RenderVectorLayer() bitblit hdc onto hdc_ref using width and height " << width << " and " << height << endl;
				p_spilogfile->writeline(ss.str());
			}
		}
		//2) redraw _dc_ref if needed
		if (hdc_ref && (dc_ref_refreshed == true || (_pVOROGUI->vorogui_idpointdirty != POINTSET_NONE)))
		{
			//on _dc_ref, only draw one or all control levels (without without control labels)
			//ensure pen and brush are selected
			//2022nov15, spi, begin
			//oldhpen_dc_ref = (HPEN)SelectObject(hdc_ref, hpen); //original
			//oldhbrush_dc_ref = (HBRUSH)SelectObject(hdc_ref, hbrush); //original
			//SelectObject(hdc_ref, hbitmap_ref); //new line
			//2022nov15, spi, end
			FIBITMAP* mydib = _dib;
			if (_pVOROGUI->vorogui_idpointdirty == POINTSET_ALL) mydib = NULL;
			_pVOROGUI->drawvorogui(hdc_ref, _pVOROGUI->vorogui_idpointdirty, mydib, true, true, false); //POINTSET_NONE
			dc_ref_refreshed = true;
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "RenderVectorLayer() update vorogui onto hdc_ref using width and height " << _pVOROGUI->vorogui_xwidth << " and " << _pVOROGUI->vorogui_yheight << endl;
				p_spilogfile->writeline(ss.str());
			}
		}

		//3.1) on dc, bitblt _dc_ref
		if (dc && hdc_ref)
		{
			//ensure bitmap is selected
			if (prev_hgdiobj_ref == NULL) prev_hgdiobj_ref = SelectObject(hdc_ref, hbitmap_ref);
			//2022nov15, spi, begin
				//else SelectObject(hdc_ref, hbitmap_ref); //new line
			//2022nov15, spi, end
			//BitBlt(dc, 0, 0, _WIDTH, _HEIGHT, hdc_ref, 0, 0, SRCCOPY);
			BitBlt(dc, 0, 0, width, height, hdc_ref, 0, 0, SRCCOPY);
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "RenderVectorLayer() bitblit hdc_ref onto physical dc using width and height " << width << " and " << height << endl;
				p_spilogfile->writeline(ss.str());
			}
		}

		//3.2) on dc, draw all control labels (without any other drawing)
		if (dc_ref_refreshed || (_prev_displaytextlabelflag != _pVOROGUI->vorogui_displaytextlabelflag))
		{
			//draw all control labels (depending on the display flag)
			//VOROGUI_DrawPointset(_pVOROGUI, _pPOINTSET, dc, _pVOROGUI->vorogui_idpointdirty, NULL, false, false, _pVOROGUI->vorogui_displaytextlabelflag); //POINTSET_NONE
			_pVOROGUI->drawvorogui(dc, POINTSET_ALL, NULL, false, false, _pVOROGUI->vorogui_displaytextlabelflag); //POINTSET_NONE
			if (p_spilogfile && global_debugmode)
			{
				stringstream ss;
				ss << "RenderVectorLayer() update vorogui labels onto physical dc using width and height " << _pVOROGUI->vorogui_xwidth << " and " << _pVOROGUI->vorogui_yheight << endl;
				p_spilogfile->writeline(ss.str());
			}
		}

		//2022jan04, spi, begin
		if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
		{
			HPEN hpen_prev = (HPEN)SelectObject(dc, hpen_green);
			bool bresult = _pVOROGUI->DrawTriangleset(NULL, NULL, POINTSET_ALL, POINTSET_DRAW_TRIANGLESET_AS_3VERTEX, dc);
			(HPEN)SelectObject(dc, hpen_prev);
			bresult = _pVOROGUI->DrawPointset(NULL, NULL, POINTSET_ALL, POINTSET_DRAW_POINTSET_AS_ELLIPSE, dc);
		}
		//2022jan04, spi, end

		_pVOROGUI->vorogui_idpointdirty = POINTSET_NONE; // = POINTSET_ALL;
		_prev_displaytextlabelflag = _pVOROGUI->vorogui_displaytextlabelflag;

		//for dc
		SelectObject(dc, oldhbrush);
		SelectObject(dc, oldhpen);
		SetROP2(dc, iOldMixMode);
		SetBkColor(dc, crOldBkColor);
		SetTextColor(dc, crOldTextColor);
		SetBkMode(dc, iOldBkMode);
		SelectObject(dc, prevhfont);
		DeleteObject(hfont);
		//for hdc
		SelectObject(hdc, oldhbrush_dc);
		SelectObject(hdc, oldhpen_dc);
		SetROP2(hdc, iOldMixMode_dc);
		SetBkMode(hdc, iOldBkMode_dc);
		//for hdc_ref
		SelectObject(hdc_ref, oldhbrush_dc_ref);
		SelectObject(hdc_ref, oldhpen_dc_ref);
		if (iOldMixMode_dc_ref) SetROP2(hdc_ref, iOldMixMode_dc_ref);
		if (iOldBkMode_dc_ref) SetBkMode(hdc_ref, iOldBkMode_dc_ref);
		if (0)
		{
			//unselect the bitmap
			_prevHGDIOBJ = SelectObject(hdc, _prevHGDIOBJ);
			_prevHGDIOBJ_ref = SelectObject(hdc_ref, _prevHGDIOBJ_ref);
		}
		canvas_rendered = true;
		canvas_rendered_ref = true;
		canvas_ready = true;
	}
	else
	{
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "RenderVectorLayer() warning! either dc is null or _mode unsupported" << endl;
			p_spilogfile->writeline(ss.str());
		}
	}
	if (dc && mustreleasedc) ReleaseDC(_hwnd, dc);
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "RenderVectorLayer() done." << endl;
		p_spilogfile->writeline(ss.str());
	}
	return dc_ref_refreshed;
}

// update the image display - the interesting bit :)
void CVoroguiWindow::UpdateDisplay(HDC hdc)
{
	if (!IsWindowVisible()) return;
	int bcanvas_dirty = false;
	_busy_painting = true;
	if (p_spilogfile && global_debugmode)
	{		
		stringstream ss;
		ss << "UpdateDisplay() ..." << endl;
		p_spilogfile->writeline(ss.str());	
	}
	RECT rcClient;
	GetClientRect(_hwnd, &rcClient);
	int imagewidth = rcClient.right - 0;
	int imageheight = rcClient.bottom - 0;
	if (_pVOROGUI == NULL || _pVOROGUI->m_pPOINTSET == NULL || (imagewidth <= 0) || (imageheight <= 0))
	{
		//abort, because no size
		_busy_painting = false;
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "UpdateDisplay() done." << endl;
			p_spilogfile->writeline(ss.str());
		}
		return;
	}

	if (p_spicanvas == NULL)
	{
		//first time only
		_busy_painting = false;
		PreCreateCanvas(); //does not render
		//return;
		_busy_painting = true;
	}

	//match proper size canavas
	bool validprecreated_samesizecanvas = false;
	int index = MatchCanvas(imagewidth, imageheight);
	int const numberofcanvas = _monitors.canvassizesvector.size();
	if (index > -1 && index < numberofcanvas)
	{
		//matching canvas found
		if (p_spicanvas)
		{
			validprecreated_samesizecanvas = true;
			if (!(p_spicanvas[index]._canvas_ready))
			{
				if (!(p_spicanvas[index]._canvas_allocated))
				{
					if (0)
					{
						//abort, because did not call PreCreateCanvas() or something else is wrong
						_busy_painting = false;
						if (p_spilogfile && global_debugmode)
						{
							stringstream ss;
							ss << "UpdateDisplay() done." << endl;
							p_spilogfile->writeline(ss.str());
						}
						return;
					}
					validprecreated_samesizecanvas = false;
				}
				else
				{
					if (!(p_spicanvas[index]._canvas_rendered))
					{
						//will render it below
						bcanvas_dirty = true;
					}
					else
					{
						//all good to bitblt
						bcanvas_dirty = false;
					}
				}
			}
		}
	}
	else
	{
		//use _hwnd's canvas, it should be already created by now
	}


	if (validprecreated_samesizecanvas)
	{
		int width = imagewidth;
		int height = imageheight;
		int i = index;
		RescaleVoroguiIfNeeded(width, height, (p_spicanvas[i]._size.cx), (p_spicanvas[i]._size.cy),
								(p_spicanvas[i]._hdc), (p_spicanvas[i]._hbitmap), (p_spicanvas[i]._buf),
								(p_spicanvas[i]._hdc_ref), (p_spicanvas[i]._hbitmap_ref), (p_spicanvas[i]._buf_ref),
								(p_spicanvas[i]._prev_hgdiobj), (p_spicanvas[i]._prev_hgdiobj_ref),
								(p_spicanvas[i]._canvas_allocated), (p_spicanvas[i]._canvas_rendered), (p_spicanvas[i]._canvas_rendered_ref), (p_spicanvas[i]._canvas_ready),
								VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
		bcanvas_dirty = !(p_spicanvas[i]._canvas_rendered);
		//_canvas_ready = p_spicanvas[i]._canvas_ready;
	}
	else
	{
		int width = imagewidth;
		int height = imageheight;
		RescaleVoroguiIfNeeded(width, height, _WIDTH, _HEIGHT,
			_dc, _bmp, _buf,
			_dc_ref, _bmp_ref, _buf_ref,
			_prevHGDIOBJ, _prevHGDIOBJ_ref,
			_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
			VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
		bcanvas_dirty = !_canvas_rendered;
		//_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
		//_canvas_ready = _canvas_allocated & _canvas_rendered;
	}

	bool force_dc_ref_refresh = false;
	//if (bcanvas_dirty || !_canvas_ready)
	//if (!_canvas_ready)
	if(bcanvas_dirty)
	{
		_busy_painting = false;
		CreateCanvas(); //create hdc (hdc will have background dib, vorogui polygons with dib inside each polygon)
		//return; //don't return since for VoroguiWindow there is no timer that keeps calling UpdateDisplay()
		bcanvas_dirty = false; //since CreateCanvas() does the first layer rendering
		force_dc_ref_refresh = true;
		_busy_painting = true; //we are not done painting yet
	}
	if (bcanvas_dirty)
	{
		if (validprecreated_samesizecanvas)
		{
			if (!(p_spicanvas[index]._canvas_rendered))
			{
				//only render first layer
				int width = imagewidth;
				int height = imageheight;
				int i = index;
				RenderCanvas(width, height, (p_spicanvas[i]._size.cx), (p_spicanvas[i]._size.cy),
							(p_spicanvas[i]._hdc), (p_spicanvas[i]._hbitmap), (p_spicanvas[i]._buf),
							(p_spicanvas[i]._hdc_ref), (p_spicanvas[i]._hbitmap_ref), (p_spicanvas[i]._buf_ref),
							(p_spicanvas[i]._prev_hgdiobj), (p_spicanvas[i]._prev_hgdiobj_ref),
							(p_spicanvas[i]._canvas_allocated), (p_spicanvas[i]._canvas_rendered), (p_spicanvas[i]._canvas_rendered_ref), (p_spicanvas[i]._canvas_ready),
							VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);

			}
		}
		else
		{
			//only render first layer
			int width = imagewidth;
			int height = imageheight;
			RenderCanvas(width, height, _WIDTH, _HEIGHT,
						_dc, _bmp, _buf,
						_dc_ref, _bmp_ref, _buf_ref,
						_prevHGDIOBJ, _prevHGDIOBJ_ref,
						_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
						VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
		}
	}
	//_mode = (int)(getParameterValue("img_mode") + 0.5f);
	
	/*
	if (!_dib)
	{
		//no image
		_busy_painting = false;
		if (_hwnd) KillTimer(_hwnd, IMAGEWINDOW_IDTIMER_UPDATEIMAGE);
		return;
	}
	*/

	//update the display
	//finally, the last layers
	bool _dc_ref_refreshed = false;
	if (!validprecreated_samesizecanvas)
	{
		int width = imagewidth;
		int height = imageheight;
		_dc_ref_refreshed = RenderVectorLayer(width, height, _WIDTH, _HEIGHT,
												_dc, _bmp, _buf,
												_dc_ref, _bmp_ref, _buf_ref,
												_prevHGDIOBJ, _prevHGDIOBJ_ref,
												_canvas_allocated, _canvas_rendered, _canvas_rendered_ref, _canvas_ready,
												VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR, force_dc_ref_refresh, hdc/*=NULL*/);
		_prev_imagewidth = imagewidth;
		_prev_imageheight = imageheight;
	}
	else
	{

		int width = imagewidth;
		int height = imageheight;
		int i = index;
		_dc_ref_refreshed = RenderVectorLayer(width, height, (p_spicanvas[i]._size.cx), (p_spicanvas[i]._size.cy),
												(p_spicanvas[i]._hdc), (p_spicanvas[i]._hbitmap), (p_spicanvas[i]._buf),
												(p_spicanvas[i]._hdc_ref), (p_spicanvas[i]._hbitmap_ref), (p_spicanvas[i]._buf_ref),
												(p_spicanvas[i]._prev_hgdiobj), (p_spicanvas[i]._prev_hgdiobj_ref),
												(p_spicanvas[i]._canvas_allocated), (p_spicanvas[i]._canvas_rendered), (p_spicanvas[i]._canvas_rendered_ref), (p_spicanvas[i]._canvas_ready),
												VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL, VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR, force_dc_ref_refresh, hdc/*=NULL*/);
		//carefull now, todo, revise how to use these _prev variables from now on 
		//_prev_imagewidth = imagewidth;
		//_prev_imageheight = imageheight;
	}

	//if (oldhbitmap) SelectObject(_dc, oldhbitmap);
	/*
	if (!skipsaveframes)
	{
		saveframetobmpfile(_hwnd, dc);
	}
	*/

	//carefull now, todo, revise how to use these _prev variables from now on 
	//_prev_imagewidth = imagewidth;
	//_prev_imageheight = imageheight;
	//if(dc) ReleaseDC(_hwnd, dc);
	_busy_painting = false;
	if (p_spilogfile && global_debugmode)
	{
		stringstream ss;
		ss << "UpdateDisplay() done." << endl;
		p_spilogfile->writeline(ss.str());
	}
	return;
}

//2024aug31, spi, begin
//added for CVorossWindow to save a frame on user's request
void CVoroguiWindow::saveframetobmpfile(HWND hwnd, HDC hdc)
{
	currentlysavingframe = true;

	global_frameid++;

	/*
	if (global_frameid <= global_maxnumberofframeperseedimage)
	*/
	if(1)
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
	}
	//2024feb22, spi, begin
	//else
	/*
	else if (global_pVorogsWindow != NULL)
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
		int nx = (int)getParameterValue("img_nx");
		int ny = (int)getParameterValue("img_ny");
		sprintf(nxbyny, "(nx%d-ny%d)", nx, ny);

		string videocutpath;
		videocutpath = global_outputvideofoldername + "\\" + base_seedimagefilename_noext + "_img-shuffle" + nxbyny + "." + global_outputvideofilenameext;
		char bufferfps[64];
		int integerfps = (int)global_outputvideoframepersecond;
		if (integerfps < 1) integerfps = 1;
		if (integerfps > 60) integerfps = 60;
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

	*/
	return;

}
//2024aug31, spi, end


LRESULT CALLBACK CVoroguiWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			//PreCreateCanvas(); //too early
			//on dc, for drawing using pen and brush
			hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			hpen_green = CreatePen(PS_SOLID, 1, RGB(0,255,255)); //was green RGB(0, 255, 0));
			hbrush = CreateSolidBrush(RGB(50, 50, 50));
			hatchhbrush = CreateHatchBrush(HS_DIAGCROSS, RGB(50, 50, 50));
		}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case VOROGUIWINDOW_IDTIMER_AUTOSPEC:
			// process  timer 
			AutoSpec();
			return 0;
		case VOROGUIWINDOW_IDTIMER_AUTOSPEC24:
			// process  timer 
			AutoSpec24();
			return 0;
		case VOROGUIWINDOW_IDTIMER_AUTOIMG:
			// process  timer 
			AutoImg();
			return 0;
		}
	case WM_SIZE:
		{
			//2021nov29, spi, begin
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			if (_pVOROGUI)
			{
				_pVOROGUI->vorogui_xwidth = width;
				_pVOROGUI->vorogui_yheight = height;
			}
			//2021nov29, spi, end
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_KEYDOWN:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);

			if (wParam == 'S')
			{
				//spi, begin
				//2018apr24, frames-batch
				//VOROGUI_WriteToDisk(_pPOINTSET);
				//toggle skipsaveframes
				CImageWindow* pImageWindow = NULL;
				for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
				{
					pImageWindow = dynamic_cast<CImageWindow*>(*it);
					if (pImageWindow) break;
				}
				if (pImageWindow)
				{
					if (pImageWindow->skipsaveframes)
					{
						pImageWindow->skipsaveframes = false;
					}
					else
					{
						pImageWindow->skipsaveframes = true;
					}
				}
				//spi, end
				//2021oct01, spi, begin
				CTimeframeWindow* pTimeframeWindow = NULL;
				for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
				{
					pTimeframeWindow = dynamic_cast<CTimeframeWindow*>(*it);
					if (pTimeframeWindow) break;
				}
				if (pTimeframeWindow)
				{
					if (pTimeframeWindow->skipsaveframes)
					{
						pTimeframeWindow->skipsaveframes = false;
					}
					else
					{
						pTimeframeWindow->skipsaveframes = true;
					}
				}
				//2021oct01, spi, end
			}
			else if (wParam == 'N')
			{
				SwitchToNextImage();
			}
			else if (wParam == 'W')
			{
				_pVOROGUI->writetodisk();
			}
			//2024aug31, spi, begin
			//added for CVorossWindow to save a frame on user's request
			else if (wParam == '1')
			{
				HDC hdc = GetDC(hwnd);
				this->saveframetobmpfile(hwnd, hdc);
				ReleaseDC(hwnd, hdc);
			}
			//2024aug31, spi, end
	}
		return 0;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			UpdateDisplay(hdc);
			EndPaint(hwnd, &ps);
		}
		return 0;
	//2022july25, spi, begin
	case WM_MOUSEMOVE:
		{
			_pVOROGUI->onmousemove(hwnd, wParam, lParam);
		}
		return 0;
	//2022july25, spi, begin
	case WM_LBUTTONDOWN:
		{
			_pVOROGUI->onlbuttondown(hwnd, wParam, lParam);
		}
		return 0;
	case WM_LBUTTONUP:
		{
			_pVOROGUI->onlbuttonup(hwnd, wParam, lParam);
		}
		return 0;
	case WM_RBUTTONUP:
		{
			_pVOROGUI->onrbuttonup(hwnd, wParam, lParam);
		}
		return 0;
	case WM_RBUTTONDBLCLK:
		{
		}
		return 0;
	case WM_DESTROY:
		//on dc, for drawing using pen and brush
		if (hpen) DeleteObject(hpen);
		if (hpen_green) DeleteObject(hpen_green);
		if (hbrush) DeleteObject(hbrush);
		if (hatchhbrush) DeleteObject(hatchhbrush);
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

int CVoroguiWindow::GetImageID()
{
	return _imageid;
}

/*
bool CVoroguiWindow::IsVorossWindow()
{
	bool bisvorosswindow = false; //i.e. when it is a CVorossWindow
	CVorossWindow* pVorossWindow = NULL;
	pVorossWindow = dynamic_cast<CVorossWindow*>(this);
	if (pVorossWindow) bisvorosswindow = true;
	return bisvorosswindow;
}
*/

void CVoroguiWindow::SwitchToNextImage()
{
	//if at least one image in the folder
	if (_imagefilenames.size() > 0)
	{
		//2022nov18, spi, begin
		int prev_imageid = _imageid;
		//2022nov18, spi, end

		//2022nov18, spi, begin
		/*
		//spi, begin
		//2018apr24, frames-batch modif
		_imageid++;
		//spi, end
		//spi, begin
		//2018april24, frames-batch
		//reset image id if end of file
		//if (_imageid >= _imagefilenames.size()) _imageid = 0;
		if (_imageid >= _imagefilenames.size()) //stoping saving frames if end of file
		{
			_imageid = 0;
			//1) trigger stop saving batch frames
			CImageWindow* pImageWindow = NULL;
			for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
			{
				pImageWindow = dynamic_cast<CImageWindow*>(*it);
				if (pImageWindow) break;
			}
			if (pImageWindow)
			{
				pImageWindow->skipsaveframes = true; //stopping batch frames saving mode
			}
			CTimeframeWindow* pTimeframeWindow = NULL;
			for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
			{
				pTimeframeWindow = dynamic_cast<CTimeframeWindow*>(*it);
				if (pTimeframeWindow) break;
			}
			if (pTimeframeWindow)
			{
				pTimeframeWindow->skipsaveframes = true; //stopping batch frames saving mode
			}
			//2) return, to abort switching image
			//return;
		}
		//spi, end
		*/
		_imageid++;
		CImageWindow* pImageWindow = NULL;
		for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
		{
			pImageWindow = dynamic_cast<CImageWindow*>(*it);
			if (pImageWindow) break;
		}
		CTimeframeWindow* pTimeframeWindow = NULL;
		for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
		{
			pTimeframeWindow = dynamic_cast<CTimeframeWindow*>(*it);
			if (pTimeframeWindow) break;
		}
		
		/*
		bool bisvorosswindow = IsVorossWindow(); //i.e. when it is a CVorossWindow
		*/

		/*
		CVorossWindow* pVorossWindow = NULL;
		pVorossWindow = dynamic_cast<CVorossWindow*>(this);
		if (pVorossWindow) bisvorosswindow=true;
		*/

		if ((pImageWindow && (pImageWindow->skipsaveframes == true)) && (pTimeframeWindow && (pTimeframeWindow->skipsaveframes == true))
			//2022dec22, spi, begin
			|| ((pImageWindow==NULL) && (pTimeframeWindow==NULL))
			//2022dec22, spi, end
			)
		{
			//not saving frames
			if (_imagefilenames.size() > 1)
			{
				/*
				do {
					_imageid = RandomInt(0, _imagefilenames.size() - 1);
				} while (_imageid == prev_imageid);
				*/
				SHORT keyState = GetKeyState(VK_SPACE); //GetAsyncKeyState()
				bool isToggled = keyState & 1;
				bool isDown = keyState & 0x8000;
				if (isDown == true)
				{
					do {
						_imageid = RandomInt(0, _imagefilenames.size() - 1);
					} while (_imageid == prev_imageid);
				}
				else
				{
					SHORT keyState = GetKeyState(VK_BACK); //GetAsyncKeyState()
					bool isToggled = keyState & 1;
					bool isDown = keyState & 0x8000;
					if (isDown == true)
					{
						//_imageid = prev_imageid;
						_imageid--;
						_imageid--;
						if (_imageid < 0) _imageid = _imagefilenames.size() - 1;
					}
				}
				if (_imageid >= _imagefilenames.size()) _imageid = 0;
			}
		}
		else if ((pImageWindow && (pImageWindow->skipsaveframes == false)) || (pTimeframeWindow && (pTimeframeWindow->skipsaveframes == false)))
		{
			//saving frames
			if (_imageid >= _imagefilenames.size())
			{
				_imageid = 0;
				if(pImageWindow) pImageWindow->skipsaveframes = true; //stopping batch frames saving mode
				if(pTimeframeWindow) pTimeframeWindow->skipsaveframes = true; //stopping batch frames saving mode
			}
		}
		//2022nov18, spi, end

		FIBITMAP* nextdib = FreeImage_Load(FreeImage_GetFIFFromFilename(_imagefilenames[_imageid].c_str()), _imagefilenames[_imageid].c_str());
		if (nextdib)
		{
			if (_nextdib) FreeImage_Unload(_nextdib);
			_nextdib = nextdib;
			_dib = NULL;

			/*
			if (!bisvorosswindow)
			{
				///////////////////////////////////////////////////////
				//when cVoroguiWindow, switch dib for all other windows
				///////////////////////////////////////////////////////

				FIBITMAP* nextquantizeddib = FreeImage_ColorQuantizeEx(_nextdib);
				if (nextquantizeddib)
				{
					if (_nextquantizeddib) FreeImage_Unload(_nextquantizeddib);
					_nextquantizeddib = nextquantizeddib;
					//if next image successfully quantized, invalidate spectrum window's color palette
					CSpectrumWindow* pSpectrumWindow = NULL;
					for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
					{
						pSpectrumWindow = dynamic_cast<CSpectrumWindow*>(*it);
						if (pSpectrumWindow) break;
					}
					if (pSpectrumWindow)
					{
						pSpectrumWindow->InvalidateDIBPalette(FreeImage_GetPalette(_nextquantizeddib));
					}

				}

				//change text window's dib
				CTextWindow* pTextWindow = NULL;
				for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
				{
					pTextWindow = dynamic_cast<CTextWindow*>(*it);
					if (pTextWindow) break;
				}
				if (pTextWindow)
				{
					pTextWindow->InvalidateDIB(_nextdib);
				}

				//change image window's dib
				CImageWindow* pImageWindow = NULL;
				for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
				{
					pImageWindow = dynamic_cast<CImageWindow*>(*it);
					if (pImageWindow) break;
				}
				if (pImageWindow)
				{
					pImageWindow->InvalidateDIB(_nextdib);
				}

				//change timeframe window's image reference id
				CTimeframeWindow* pTimeframeWindow = NULL;
				for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
				{
					pTimeframeWindow = dynamic_cast<CTimeframeWindow*>(*it);
					if (pTimeframeWindow) break;
				}
				if (pTimeframeWindow)
				{
					pTimeframeWindow->InvalidateDIB(_imageid);
				}
			}
			*/

			//2022nov18, spi, begin
			//if (0)
			//if(1)
			if(_invalidatedib_flag == VOROGUI_INVALIDATEDIB_FORALLCONTROL)
			{
				//change vorogui window's image reference id (all vorogui controls also use the same)
				this->InvalidateDIB(_nextdib);
			}
			else if (_invalidatedib_flag == VOROGUI_INVALIDATEDIB_FORONECONTROL)
			{
				//change vorogui window's image reference id (without redrawing all vorogui controls yet,
				//redrawing only one vorogui control at a time)
				this->InvalidateDIBForOneControl(_nextdib);
			}
			else if (_invalidatedib_flag == VOROGUI_INVALIDATEDIB_FORONERANDOMCONTROL)
			{
				//change vorogui window's image reference id (without redrawing all vorogui controls yet,
				//redrawing only one vorogui control at a time)
				this->InvalidateDIBForOneRandomControl(_nextdib);
			}
			else if (_invalidatedib_flag == VOROGUI_INVALIDATEDIB_FOR50PCRANDOMCONTROL)
			{
				//change vorogui window's image reference id (without redrawing all vorogui controls yet,
				//redrawing only one vorogui control at a time)
				this->InvalidateDIBFor50pcRandomControl(_nextdib);
			}
			//2022nov18, spi, end

			//spi, begin
			//2018apr24, frames-batch modif
			//_imageid++;
			//spi, end
			RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);
		}
	}
}

//2022nov18, spi, begin
//when passing null as newdib, will simply not change the _dib
void CVoroguiWindow::InvalidateDIB(FIBITMAP* newdib)
{
	if(newdib) _dib = newdib;
	//_canvas_ready = false;
	int const numberofcanvas = _monitors.canvassizesvector.size();
	if (p_spicanvas)
	{
		for (int i = 0; i < numberofcanvas; i++)
		{
			p_spicanvas[i]._canvas_rendered = false;
			p_spicanvas[i]._canvas_rendered_ref = false;
			p_spicanvas[i]._canvas_ready = false;
		}
	}
}

void CVoroguiWindow::InvalidateDIBForOneControl(FIBITMAP* newdib)
{
	//1) pass along new dib
	if (newdib) _dib = newdib;
	//2) just provoque redrawing of one vorogui control (picked randomly)
	int idpoint = _pVOROGUI->vorogui_idpointrecentlydisplayed;
	if(idpoint!=-1) _pVOROGUI->vorogui_idpointdirty = idpoint;
}

void CVoroguiWindow::InvalidateDIBForOneRandomControl(FIBITMAP* newdib)
{
	//1) pass along new dib
	if (newdib) _dib = newdib;
	//2) just provoque redrawing of one vorogui control (picked randomly)
	int lowest = _pVOROGUI->vorogui_numberofframepoints; //id of first non-frame point
	int highest = _pVOROGUI->GetNumberOfPoints()-1; //-1 to get the id of the last point
	int idpoint = RandomInt(lowest,highest);
	_pVOROGUI->vorogui_idpointdirty = idpoint;
}

void CVoroguiWindow::InvalidateDIBFor50pcRandomControl(FIBITMAP* newdib)
{
	/*//todo
	//1) pass along new dib
	if(newdib) _dib = newdib;
	//2) just provoque redrawing of one vorogui control (picked randomly)
	int lowest = _pVOROGUI->vorogui_numberofframepoints; //id of first non-frame point
	int highest = _pVOROGUI->GetNumberOfPoints() - 1; //-1 to get the id of the last point
	int idpoint = RandomInt(lowest, highest);
	_pVOROGUI->vorogui_idpointdirty = idpoint;
	*/
}

//2022nov18, spi, end

void CVoroguiWindow::AutoSpec()
{
	CSpectrumWindow* pSpectrumWindow = NULL;
	for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
	{
		pSpectrumWindow = dynamic_cast<CSpectrumWindow*>(*it);
		if (pSpectrumWindow) break;
	}
	if (pSpectrumWindow)
	{
		int autospecmode = (int)getParameterValue("voro_autospecmode");
		switch (autospecmode)
		{
		case 0:
		{
				  pSpectrumWindow->setParameter("spec_idcolorpalette", 8.0);

				  int i = rand() % 9 + 1;
				  switch (i)
				  {
				  case 1:
					  pSpectrumWindow->setParameter("spec_mode", pSpectrumWindow->getParameterValueRandom("spec_mode"));
					  break;
				  case 2:
					  //pSpectrumWindow->setParameter("spec_idcolorpalette", pSpectrumWindow->getParameterValueRandom("spec_idcolorpalette"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  break;
				  case 8:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  break;
				  case 9:
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		case 1:
		{
				  int i = rand() % 9 + 1;
				  switch (i)
				  {
				  case 1:
					  pSpectrumWindow->setParameter("spec_mode", pSpectrumWindow->getParameterValueRandom("spec_mode"));
					  break;
				  case 2:
					  pSpectrumWindow->setParameter("spec_idcolorpalette", pSpectrumWindow->getParameterValueRandom("spec_idcolorpalette"));
					  break;
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  break;
				  case 8:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  break;
				  case 9:
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		case 2:
		{
				  int i = rand() % 9 + 1;
				  switch (i)
				  {
				  case 1:
				  {
							//solid background divided into cells
							int lowest = 15;
							int highest = 18;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_mode", (float)random_integer);
							break;
				  }
				  case 2:
				  {
							int lowest = 0;
							int highest = 2;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_idcolorpalette", (float)random_integer);
							break;
				  }
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 8:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  case 9:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		case 3:
		{
				  int i = rand() % 9 + 1;
				  switch (i)
				  {
				  case 1:
				  {
							//solid background divided into cells
							int lowest = 15;
							int highest = 18;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_mode", (float)random_integer);
							break;
				  }
				  case 2:
				  {
							int lowest = 2;
							int highest = 3;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_idcolorpalette", (float)random_integer);
							break;
				  }
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  break;
				  case 8:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  break;
				  case 9:
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		case 4:
		{
				  int i = rand() % 9 + 1;
				  switch (i)
				  {
				  case 1:
				  {
							//solid background divided into cells
							int lowest = 15;
							int highest = 18;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_mode", (float)random_integer);
							break;
				  }
				  case 2:
				  {
							//gray scale only
							int lowest = 3;
							int highest = 3;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_idcolorpalette", (float)random_integer);
							break;
				  }
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  break;
				  case 8:
					  pSpectrumWindow->setParameter("spec_nx", pSpectrumWindow->getParameterValueRandom("spec_nx"));
					  break;
				  case 9:
					  pSpectrumWindow->setParameter("spec_ny", pSpectrumWindow->getParameterValueRandom("spec_ny"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		case 5:
		{
				  int i = rand() % 7 + 1;
				  switch (i)
				  {
				  case 1:
				  {
							//solid slightly or radically shifting background
							int lowest = 11;
							int highest = 14;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_mode", (float)random_integer);
							break;
				  }
				  case 2:
				  {
							int lowest = 0;
							int highest = 3;
							int range = (highest - lowest) + 1;
							int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
							pSpectrumWindow->setParameter("spec_idcolorpalette", (float)random_integer);
							break;
				  }
				  case 3:
					  pSpectrumWindow->setParameter("spec_bgcolorid", pSpectrumWindow->getParameterValueRandom("spec_bgcolorid"));
					  break;
				  case 4:
					  pSpectrumWindow->setParameter("spec_idcolorch1", pSpectrumWindow->getParameterValueRandom("spec_idcolorch1"));
					  break;
				  case 5:
					  pSpectrumWindow->setParameter("spec_idcolorch2", pSpectrumWindow->getParameterValueRandom("spec_idcolorch2"));
					  break;
				  case 6:
					  pSpectrumWindow->setParameter("spec_idcolorlowest", pSpectrumWindow->getParameterValueRandom("spec_idcolorlowest"));
					  break;
				  case 7:
					  pSpectrumWindow->setParameter("spec_idcolorhighest", pSpectrumWindow->getParameterValueRandom("spec_idcolorhighest"));
					  break;
				  default:
					  assert(false);
					  break;
				  }
				  break;
		}
		default:
			assert(false);
			break;
		}
		_pVOROGUI->updatecontrolsfromparameters();
		RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);
	}
	return;
}

void CVoroguiWindow::AutoSpec24()
{
	CSpectrum24bitWindow* pSpectrum24bitWindow = NULL;
	for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
	{
		pSpectrum24bitWindow = dynamic_cast<CSpectrum24bitWindow*>(*it);
		if (pSpectrum24bitWindow) break;
	}
	if (pSpectrum24bitWindow)
	{
		int i = rand() % 9 + 1;
		switch (i)
		{
		case 1:
			pSpectrum24bitWindow->setParameter("spec24_mode", pSpectrum24bitWindow->getParameterValueRandom("spec24_mode"));
			break;
		case 2:
			//pSpectrum24bitWindow->setParameter("spec_idcolorpalette", pSpectrum24bitWindow->getParameterValueRandom("spec_idcolorpalette"));
			pSpectrum24bitWindow->setParameter("spec24_nx", pSpectrum24bitWindow->getParameterValueRandom("spec24_nx"));
			pSpectrum24bitWindow->setParameter("spec24_ny", pSpectrum24bitWindow->getParameterValueRandom("spec24_ny"));
			break;
		case 3:
			pSpectrum24bitWindow->setParameter("spec24_bgcolorid_r", pSpectrum24bitWindow->getParameterValueRandom("spec24_bgcolorid_r"));
			pSpectrum24bitWindow->setParameter("spec24_bgcolorid_g", pSpectrum24bitWindow->getParameterValueRandom("spec24_bgcolorid_g"));
			pSpectrum24bitWindow->setParameter("spec24_bgcolorid_b", pSpectrum24bitWindow->getParameterValueRandom("spec24_bgcolorid_b"));
			break;
		case 4:
			pSpectrum24bitWindow->setParameter("spec24_idcolorch1_r", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch1_r"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorch1_g", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch1_g"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorch1_b", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch1_b"));
			break;
		case 5:
			pSpectrum24bitWindow->setParameter("spec24_idcolorch2_r", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch2_r"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorch2_g", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch2_g"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorch2_b", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorch2_b"));
			break;
		case 6:
			pSpectrum24bitWindow->setParameter("spec24_idcolorlowest_r", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorlowest_r"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorlowest_g", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorlowest_g"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorlowest_b", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorlowest_b"));
			break;
		case 7:
			pSpectrum24bitWindow->setParameter("spec24_idcolorhighest_r", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorhighest_r"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorhighest_g", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorhighest_g"));
			pSpectrum24bitWindow->setParameter("spec24_idcolorhighest_b", pSpectrum24bitWindow->getParameterValueRandom("spec24_idcolorhighest_b"));
			break;
		case 8:
			pSpectrum24bitWindow->setParameter("spec24_nx", pSpectrum24bitWindow->getParameterValueRandom("spec24_nx"));
			break;
		case 9:
			pSpectrum24bitWindow->setParameter("spec24_ny", pSpectrum24bitWindow->getParameterValueRandom("spec24_ny"));
			break;
		default:
			assert(false);
			break;
		}


		_pVOROGUI->updatecontrolsfromparameters();
		RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);
	}
}

void CVoroguiWindow::AutoImg()
{
	CImageWindow* pImageWindow = NULL;
	for (vector<CWindowParameter*>::iterator it = _windowparameterpointers.begin(); it != _windowparameterpointers.end(); it++)
	{
		pImageWindow = dynamic_cast<CImageWindow*>(*it);
		if (pImageWindow) break;
	}
	if (pImageWindow)
	{
		int i = rand() % 9 + 1;
		switch (i)
		{
		case 1:
			pImageWindow->setParameter("img_mode", pImageWindow->getParameterValueRandom("img_mode"));
			break;
		case 2:
			pImageWindow->setParameter("img_nx", pImageWindow->getParameterValueRandom("img_nx"));
			pImageWindow->setParameter("img_ny", pImageWindow->getParameterValueRandom("img_ny"));
			break;
		case 3:
			pImageWindow->setParameter("img_bgcolorid_r", pImageWindow->getParameterValueRandom("img_bgcolorid_r"));
			pImageWindow->setParameter("img_bgcolorid_g", pImageWindow->getParameterValueRandom("img_bgcolorid_g"));
			pImageWindow->setParameter("img_bgcolorid_b", pImageWindow->getParameterValueRandom("img_bgcolorid_b"));
			break;
		case 4:
			pImageWindow->setParameter("img_nx", pImageWindow->getParameterValueRandom("img_nx"));
			pImageWindow->setParameter("img_ny", pImageWindow->getParameterValueRandom("img_ny"));
			break;
		case 5:
			pImageWindow->setParameter("img_mode", pImageWindow->getParameterValueRandom("img_mode"));
			break;
		case 6:
			pImageWindow->setParameter("img_idcolorlowest_r", pImageWindow->getParameterValueRandom("img_idcolorlowest_r"));
			pImageWindow->setParameter("img_idcolorlowest_g", pImageWindow->getParameterValueRandom("img_idcolorlowest_g"));
			pImageWindow->setParameter("img_idcolorlowest_b", pImageWindow->getParameterValueRandom("img_idcolorlowest_b"));
			break;
		case 7:
			pImageWindow->setParameter("img_idcolorhighest_r", pImageWindow->getParameterValueRandom("img_idcolorhighest_r"));
			pImageWindow->setParameter("img_idcolorhighest_g", pImageWindow->getParameterValueRandom("img_idcolorhighest_g"));
			pImageWindow->setParameter("img_idcolorhighest_b", pImageWindow->getParameterValueRandom("img_idcolorhighest_b"));
			break;
		case 8:
			pImageWindow->setParameter("img_nx", pImageWindow->getParameterValueRandom("img_nx"));
			break;
		case 9:
			pImageWindow->setParameter("img_ny", pImageWindow->getParameterValueRandom("img_ny"));
			break;
		default:
			assert(false);
			break;
		}


		_pVOROGUI->updatecontrolsfromparameters();
		RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);
	}
}