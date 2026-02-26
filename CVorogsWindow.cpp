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
#include "CVorogsWindow.h"
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

/*
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
*/


CVorogsWindow::CVorogsWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx, class Synth* pSynth, 
	vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int voroguiclustermode)
: CVoroguiWindow(lpszCaption, lpwndClassEx, pSynth, 
	windowparameterpointers, dib, imagefilenames, voroguiclustermode)
{
	Initialize();
}

CVorogsWindow::CVorogsWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CVoroguiWindow(lpszCaption, hInstance, lpszClassName,
		pSynth, windowparameterpointers, dib, imagefilenames, vorogui_clustermode,
		x, y, xwidth, yheight,
		alpha, titlebardisplay, menubardisplay)
{
	Initialize();
}

CVorogsWindow::CVorogsWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int voroguiclustermode,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CVoroguiWindow(lpszCaption, hInstance, lpszClassName,
		style, hIcon, hCursor, hbrBackground,
		lpszMenuName, hIconSm,
		pSynth, windowparameterpointers, dib, imagefilenames, voroguiclustermode,
		x, y, xwidth, yheight,
		alpha, titlebardisplay, menubardisplay)
{
	Initialize();
}

void CVorogsWindow::Initialize(bool bfirsttime/*=true*/)
{
	if (bfirsttime)
	{
		_windowparameterpointers.push_back(this);
		/*
		_windowparameterpointers.push_back(this);
		_totalnumberofadditional_ss = 0;
		//_initialnumberofadditional_ss = 100;
		_initialnumberofadditional_ss = 0;
		
		add_ss(_initialnumberofadditional_ss, false);
		*/
		
		/*
		addParameter("black_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
		*/


		if (1)
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

	}
	
	///////////////////////////////////////////////////////////////////////
	//derived class will mostlikely need this section to be in Initialize() 
	///////////////////////////////////////////////////////////////////////
	/*
	if (!bfirsttime && _pVOROGUI)
	{
		delete _pVOROGUI; 
		_pVOROGUI = NULL;
	}
	*/
	//spi's DBG_NEW vs ms' DEBUG_NEW
	_pVOROGUI = (spivorogui*)DEBUG_NEW spivorogui(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode); //_pVOROGUI = VOROGUI_Init(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode);
	//_pVOROGUI = (spivorogui*)DBG_NEW spivorogui(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode); //_pVOROGUI = VOROGUI_Init(_pSynth, _windowparameterpointers, _xwidth, _yheight, vorogui_clustermode);
	if (_pVOROGUI) _pVOROGUI->readfromdisk();
	if (bfirsttime)
	{
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

		//overrides
		//_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORONERANDOMCONTROL;
		
		//2022nov25, spi, begin
		//RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE); //added on the 2022nov25, but seems to have no effect
		//2022nov25, spi, end

	}

	/*
	if (1)
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

/*
bool CVorogsWindow::add_ss(int numberof_ss, bool init//=true)
{
	int nextindex = _totalnumberofadditional_ss;
	if (numberof_ss > 0)
	{
		char buffer[1024];
		string name;
		for (int i = 0; i < numberof_ss; i++)
		{
			sprintf(buffer, "ss%i", nextindex + i);
			name = buffer;
			addParameter(name, 0.0, 1.0, 1.0, ParameterTypeToggle);
		}

		//init all frame points and all control points
		if (init)
		{
			Initialize(false);
		}
		_totalnumberofadditional_ss += numberof_ss;
		return true;
	}
	return false;
}

bool CVorogsWindow::remove_ss(int numberof_ss, int index//=-1)
{
	//index input parameter usage not implemented yet

	if (numberof_ss > _totalnumberofadditional_ss) numberof_ss = _totalnumberofadditional_ss;

	if (numberof_ss > 0)
	{
		//iteratively remove random param
		char buffer[1024];
		string name;
		for (int i = 0; i < numberof_ss; i++)
		{
			//int index = RandomInt(0, _totalnumberofadditional_ss - 1);
			int index = _totalnumberofadditional_ss - 1; //remove the very last ones, so control name remain continuous
			sprintf(buffer, "ss%i", index);
			name = buffer;
			removeParameter(name);
			_totalnumberofadditional_ss -= 1;
		}
		//init all frame points and all control points
		Initialize(false);
		return true;
	}
	return false;
}
*/



void CVorogsWindow::UpdateParameter(string name)
{
	/*
	//update member variable
	if (name == "black_showwnd")
	{
		float showflag = getParameterValue("black_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}
	//some parameters updated in UpdateSpectrum()
	*/
}

CVorogsWindow::~CVorogsWindow()
{
}

LRESULT CALLBACK CVorogsWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

	switch (uMsg)
	{
	case WM_CREATE:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			/*
			case IDM_ABOUT:
				//2021nov29, spi, begin
#ifdef _WIN64
				//for 64bit, GetWindowLong() call to be replaced by GetWindowLongPtr()
				DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
#else
				DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
#endif
				//2021nov29, spi, end
				break;
			*/
			case IDM_EXIT:
				DestroyWindow(hwnd);
				break;
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			break;
		}
		return 0;
	case WM_SIZE:
		{
			//CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			//break;
			return 0;
		}
	case WM_KEYDOWN:
		{
			if (wParam == 'N')
			{
				SwitchToNextImage();
			}
			else
			{
				CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			}
			/*
			if (wParam == 'Z')
			{
				add_ss(_initialnumberofadditional_ss);
				_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORALLCONTROL;
				//InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == 'X')
			{
				remove_ss(_initialnumberofadditional_ss);
				_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORALLCONTROL;
				//InvalidateRect(hwnd, NULL, true);
			}
			*/
			//CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
			
		}
		return 0;
	case WM_PAINT:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_MOUSEMOVE:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			/*
			//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			if (!(wParam&MK_SHIFT) && !(wParam&MK_CONTROL))
			{
				if (_pVOROGUI)
				{
					//get nearest pointset object 
					int idpoint = _pVOROGUI->getnearestpointsetobject(xPos + 0.0, yPos + 0.0, &(_pVOROGUI->vorogui_itriseed)); //+0.0 to ensure proper conversion to double
					if (idpoint >= _pVOROGUI->vorogui_numberofframepoints)
					{
						_pVOROGUI->vorogui_idpointrecentlydisplayed = idpoint;
					}
				}
			}
			else
			{
				//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			}
			*/
		}
		return 0;
	case WM_LBUTTONDOWN:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			/*
			//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			if (!(wParam&MK_SHIFT) && !(wParam&MK_CONTROL))
			{
			}
			else
			{
				//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			}
			*/
		}
		return 0;
	case WM_LBUTTONUP:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			/*
			//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			if (!(wParam&MK_SHIFT) && !(wParam&MK_CONTROL))
			{
				if (_pVOROGUI)
				{
					//get nearest pointset object 
					int idpoint = _pVOROGUI->getnearestpointsetobject(xPos + 0.0, yPos + 0.0, &(_pVOROGUI->vorogui_itriseed)); //+0.0 to ensure proper conversion to double
					if (idpoint >= _pVOROGUI->vorogui_numberofframepoints)
					{
						_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORONECONTROL;
					}
					else
					{
						_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORALLCONTROL;
					}
					PostMessage(hwnd, WM_KEYDOWN, (WPARAM)'N', (LPARAM)0);
				}
			}
			else
			{
				//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			}
			*/
			//2024oct19, spi, begin
			if (wParam&MK_CONTROL)
			{
				InvalidateDIB(NULL);
				RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);

			}
			//2024oct19, spi, end

		}
		return 0;
	case WM_RBUTTONUP:
		{
			CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			/*
			//CVoroguiWindow::wndProc(hwnd, uMsg, wParam, lParam);
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			if (!(wParam&MK_SHIFT) && !(wParam&MK_CONTROL))
			{
				if (_pVOROGUI)
				{
					//get nearest pointset object 
					int idpoint = _pVOROGUI->getnearestpointsetobject(xPos + 0.0, yPos + 0.0, &(_pVOROGUI->vorogui_itriseed)); //+0.0 to ensure proper conversion to double
					if (idpoint >= _pVOROGUI->vorogui_numberofframepoints)
					{
						_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FORONERANDOMCONTROL;
					}
					else
					{
						_invalidatedib_flag = VOROGUI_INVALIDATEDIB_FOR50PCRANDOMCONTROL;
					}
					PostMessage(hwnd, WM_KEYDOWN, (WPARAM)'N', (LPARAM)0);
				}
			}
			*/
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0; //spi
}


void CVorogsWindow::SwitchToNextImage()
{
	//baseclass behavior
	CVoroguiWindow::SwitchToNextImage();

	//additional behavior
	if (1)
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

}