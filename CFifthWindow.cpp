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

#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"
#include "CFifthWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>

using namespace std;



CFifthWindow::CFifthWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) 
: CWindowMultimonitor(lpszCaption, lpwndClassEx)
{

}

CFifthWindow::CFifthWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CFifthWindow::CFifthWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CFifthWindow::~CFifthWindow()
{
}

LRESULT CALLBACK CFifthWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CREATE:
		{

		}
		return 0;
	case WM_SIZE:
		{
			CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_KEYDOWN:
		{
			CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
