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
#include "spiwavsetlib.h"
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"

#include "CParameter.h"
#include "CWindowParameter.h"

#include "CTextWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>

using namespace std;



CTextWindow::CTextWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
	int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib)
: CWindowParameter(lpszCaption, lpwndClassEx)
{
	_staticalignment = staticalignment;
	_statictextcolor = statictextcolor;
	_hfont = hfont;
	_dib = dib;

	Initialize();
}

CTextWindow::CTextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_staticalignment = staticalignment;
	_statictextcolor = statictextcolor;
	_hfont = hfont;
	_dib = dib;

	Initialize();
}

CTextWindow::CTextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowParameter(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{
	_staticalignment = staticalignment;
	_statictextcolor = statictextcolor;
	_hfont = hfont;
	_dib = dib;

	Initialize();
}

void CTextWindow::Initialize()
{
	addParameter("txt_showwnd", 0.0, 1.0, 1.0, ParameterTypeToggle);
	addParameter("txt_colorr", 0.0, 255.0, GetRValue(_statictextcolor), ParameterTypeContinuous);
	addParameter("txt_colorg", 0.0, 255.0, GetGValue(_statictextcolor), ParameterTypeContinuous);
	addParameter("txt_colorb", 0.0, 255.0, GetBValue(_statictextcolor), ParameterTypeContinuous);
}

void CTextWindow::UpdateParameter(string name)
{
	//update member variable
	if (name == "txt_showwnd")
	{
		float showflag = getParameterValue("txt_showwnd");
		if (showflag >= 0.5) Show(SW_SHOWNOACTIVATE);
		else Show(false);
	}
	else if (name == "txt_colorr")
	{
		int r = GetRValue(_statictextcolor);
		int g = GetGValue(_statictextcolor);
		int b = GetBValue(_statictextcolor);
		int red = (int)getParameterValue("txt_colorr");
		_statictextcolor = RGB(red, g, b);
	}
	else if (name == "txt_colorg")
	{
		int r = GetRValue(_statictextcolor);
		int g = GetGValue(_statictextcolor);
		int b = GetBValue(_statictextcolor);
		int green = (int)getParameterValue("txt_colorg");
		_statictextcolor = RGB(r, green, b);
	}
	else if (name == "txt_colorb")
	{
		int r = GetRValue(_statictextcolor);
		int g = GetGValue(_statictextcolor);
		int b = GetBValue(_statictextcolor);
		int blue = (int)getParameterValue("txt_colorb");
		_statictextcolor = RGB(r, g, blue);
	}
}

CTextWindow::~CTextWindow()
{
	WavSetLib_Terminate();
}

LRESULT CALLBACK CTextWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			HWND hStatic = CreateWindowEx(WS_EX_TRANSPARENT, L"STATIC", L"", WS_CHILD | WS_VISIBLE | _staticalignment,
				0, 100, 100, 100, hwnd, (HMENU)IDC_MAIN_STATIC, GetModuleHandle(NULL), NULL);
			if (hStatic == NULL)
				MessageBox(hwnd, L"Could not create static text.", L"Error", MB_OK | MB_ICONERROR);
			SendMessage(hStatic, WM_SETFONT, (WPARAM)_hfont, MAKELPARAM(FALSE, 0));
		}
		return 0;
	case WM_SIZE:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);

			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			HWND hStatic = GetDlgItem(hwnd, IDC_MAIN_STATIC);
			int staticwidth = rcClient.right - 0;
			int staticheight = rcClient.bottom - 0;
			HDC hdc = GetDC(hwnd);
			HFONT hOldFont = (HFONT)SelectObject(hdc, _hfont);
			TEXTMETRIC myTEXTMETRIC;
			GetTextMetrics(hdc, &myTEXTMETRIC);
			int fontwidth = myTEXTMETRIC.tmAveCharWidth;
			int fontheight = myTEXTMETRIC.tmHeight;
			WavSetLib_Initialize(hwnd, IDC_MAIN_STATIC, staticwidth, staticheight, fontwidth, fontheight, _staticalignment);
			SetWindowPos(hStatic, NULL, 0, 0, staticwidth, staticheight, SWP_NOZORDER);
			SelectObject(hdc, hOldFont);
			ReleaseDC(hwnd, hdc);
		}
		return 0;
	case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			//SetTextColor((HDC)wParam, RGB(0xFF, 0xFF, 0xFF)); 
			SetTextColor((HDC)wParam, _statictextcolor);
			return (INT_PTR)::GetStockObject(NULL_PEN);
		}
		return 0;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			if (_dib)
			{
				RECT rcClient;
				GetClientRect(hwnd, &rcClient);
				int imagewidth = rcClient.right - 0;
				int imageheight = rcClient.bottom - 0;
				SetStretchBltMode(hdc, COLORONCOLOR);
				StretchDIBits(hdc, 0, 0, imagewidth, imageheight,
					0, 0, FreeImage_GetWidth(_dib), FreeImage_GetHeight(_dib),
					FreeImage_GetBits(_dib), FreeImage_GetInfo(_dib), DIB_RGB_COLORS, SRCCOPY);
			}
			EndPaint(hwnd, &ps);
		}
		return 0;
	case WM_KEYDOWN:
		{
			CWindowParameter::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
		}
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CTextWindow::InvalidateDIB(FIBITMAP* dib)
{
	_dib = dib;
	RedrawWindow(_hwnd, NULL, NULL, RDW_INVALIDATE);
}