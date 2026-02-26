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

#include "CWindowTransparent.h"
#include <tchar.h>
#include "resource.h"

HWND CWindowTransparent::GetHWND()
{
	return _hwnd;
}

CWindowTransparent::CWindowTransparent() 
{
	this->_hwnd = NULL;
	this->_lpCaption = NULL;
	this->_lpClassName = NULL;

	this->_x = CW_USEDEFAULT;
	this->_y = CW_USEDEFAULT;
	this->_xwidth = CW_USEDEFAULT;
	this->_yheight = CW_USEDEFAULT;

	this->_alpha = 150;
	this->_titlebardisplay=1;
	this->_menubardisplay=1;
}

CWindowTransparent::CWindowTransparent(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) 
{
	this->_hwnd = NULL;
	this->_lpCaption = NULL;
	this->_lpClassName = NULL;

	this->_x = CW_USEDEFAULT;
	this->_y = CW_USEDEFAULT;
	this->_xwidth = CW_USEDEFAULT;
	this->_yheight = CW_USEDEFAULT;

	this->_alpha = 150;
	this->_titlebardisplay = 1;
	this->_menubardisplay = 1;

	this->SetCaption(lpszCaption);
	this->SetClassName(lpwndClassEx->lpszClassName);
	this->_hInstance = lpwndClassEx->hInstance;

	this->Register(lpwndClassEx);
}

CWindowTransparent::CWindowTransparent(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
{
	WNDCLASSEX wnd;

	this->_hwnd = NULL;
	this->_lpCaption = NULL;
	this->_lpClassName = NULL;

	this->_x = x;
	this->_y = y;
	this->_xwidth = xwidth;
	this->_yheight = yheight;

	this->_alpha = alpha;
	this->_titlebardisplay = titlebardisplay;
	this->_menubardisplay = menubardisplay;

	this->SetCaption(lpszCaption);
	this->SetClassName(lpszClassName);
	this->_hInstance = hInstance;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	//wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hIcon = (HICON)LoadImage(NULL, L"background_32x32x16.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	//wnd.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wnd.hIconSm = (HICON)LoadImage(NULL, L"background_16x16x16.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	//wnd.lpszMenuName = NULL;
	if (menubardisplay)
	{
		wnd.lpszMenuName = MAKEINTRESOURCE(IDC_SPIWAVWIN32); //original with menu
	}
	else
	{
		wnd.lpszMenuName = NULL; //no menu
	}

	this->Register(&wnd);
}

CWindowTransparent::CWindowTransparent(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
{
	
	WNDCLASSEX wnd;

	this->_hwnd = NULL;
	this->_lpCaption = NULL;
	this->_lpClassName = NULL;

	this->_x = x;
	this->_y = y;
	this->_xwidth = xwidth;
	this->_yheight = yheight;

	this->_alpha = alpha;
	this->_titlebardisplay = titlebardisplay;
	this->_menubardisplay = menubardisplay;

	this->SetCaption(lpszCaption);
	this->SetClassName(lpszClassName);
	this->_hInstance = hInstance;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.style = style;
	wnd.hbrBackground = hbrBackground;
	wnd.hCursor = hCursor;
	wnd.hIcon = hIcon;
	wnd.hIconSm = hIconSm;
	//wnd.lpszMenuName = lpszMenuName;
	if (menubardisplay)
	{
		wnd.lpszMenuName = lpszMenuName; //original with menu
	}
	else
	{
		wnd.lpszMenuName = NULL; //no menu
	}

	this->Register(&wnd);
}

CWindowTransparent::~CWindowTransparent() 
{
	if (this->_hwnd != NULL) 
	{
		if (0)
		{
			DestroyWindow(this->_hwnd);
			this->_hwnd = NULL;
		}
	}

	if (this->_lpClassName != NULL)
	{
		free((LPVOID)this->_lpClassName);
		this->_lpClassName = NULL;
	}

	if (this->_lpCaption != NULL)
	{
		free((LPVOID)this->_lpCaption);
		this->_lpCaption = NULL;
	}
}

VOID CWindowTransparent::Register(LPWNDCLASSEX lpwndClassEx) 
{

	lpwndClassEx->lpfnWndProc = CWindowTransparent::msgRouter;
	lpwndClassEx->lpszClassName = this->_lpClassName;
	lpwndClassEx->hInstance = this->_hInstance;

	if (!RegisterClassEx(lpwndClassEx)) 
	{
		throw "Could not register the window class.";
	}
}

VOID CWindowTransparent::SetClassName(LPCTSTR lpszClassName) 
{
	int nLen;

	if (lpszClassName != NULL && this->_hwnd == NULL) 
	{
		if (this->_lpClassName != NULL) 
		{
			free((LPVOID)this->_lpClassName);
		}

		nLen = _tcsnlen(lpszClassName, 255) + 1;
		this->_lpClassName = (LPTSTR)calloc(nLen, sizeof(TCHAR));
		_tcscpy_s(this->_lpClassName, nLen, lpszClassName);
	}
	else 
	{
		throw "Invalid class name";
	}
}

LPCTSTR CWindowTransparent::GetCaption() 
{
	return this->_lpCaption == NULL ? TEXT("") : this->_lpCaption;
}

VOID CWindowTransparent::SetCaption(LPCTSTR lpszCaption) 
{
	int nLen;

	if (lpszCaption != NULL && this->_hwnd == NULL) 
	{
		if (this->_lpCaption != NULL) 
		{
			free((LPVOID)this->_lpCaption);
		}

		nLen = _tcsnlen(lpszCaption, 255) + 1;
		this->_lpCaption = (LPTSTR)calloc(nLen , sizeof(TCHAR));
		_tcscpy_s(this->_lpCaption, nLen, lpszCaption);
	}
	else 
	{
		throw "Invalid caption text";
	}
}

LPCTSTR CWindowTransparent::GetClassName() 
{
	return this->_lpClassName == NULL ? TEXT("") : this->_lpClassName;
}

HWND CWindowTransparent::Create() 
{
	/* //spi
	return this->Create(
			(HWND)NULL,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
	*/
	return this->Create((HWND)NULL);

}

HWND CWindowTransparent::Create(HWND hWndParent) 
{
	/*
	return this->Create(
			(HWND)hWndParent,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
	*/

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	if (!this->_titlebardisplay) dwStyle = WS_POPUP | WS_VISIBLE;

	return this->Create(
		(HWND)hWndParent,
		0L,
		dwStyle,
		_x,
		_y,
		_xwidth,
		_yheight,
		(HMENU)NULL,
		NULL
		);

}

HWND CWindowTransparent::Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
	INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam) 
{	
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
		SetLayeredWindowAttributes(this->_hwnd, 0, this->_alpha, LWA_ALPHA);
	}
	return this->_hwnd;
}

BOOLEAN CWindowTransparent::IsWindowVisible()
{
	return (this->_hwnd == NULL) ? FALSE : ::IsWindowVisible(this->_hwnd);
}

BOOLEAN CWindowTransparent::Show(int nCmdShow) 
{
	return (this->_hwnd == NULL) ? FALSE : ShowWindow(this->_hwnd, nCmdShow);
}

BOOLEAN CWindowTransparent::Show() 
{
	return this->Show(SW_SHOW);
}

BOOLEAN CWindowTransparent::Hide() 
{
	return this->Show(SW_HIDE);
}

BOOLEAN CWindowTransparent::Maximize() 
{
	return this->Show(SW_MAXIMIZE | SW_SHOW);
}

BOOLEAN CWindowTransparent::Minimize()
{
	return this->Show(SW_MINIMIZE | SW_SHOW);
}

BOOLEAN CWindowTransparent::Restore()
{
	return this->Show(SW_RESTORE | SW_SHOW);
}

bool CWindowTransparent::msgRouter_detach()
{
	//2021nov29, spi, begin
	CWindowTransparent* pWindowTransparent = NULL;
#ifdef _WIN64
				//for 64bit, SetWindowLong() call to be replaced by SetWindowLongPtr()
	::SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)pWindowTransparent);
#else
	::SetWindowLong(_hwnd, GWL_USERDATA, (long)pWindowTransparent);
#endif
	//2021nov29, spi, end
	return true;
}

//don't call this function unless msgRouter_detach() was call explicitely and now need to reattach _hwnd to its corresponding CWindowTransparent*
//msgRouter() handle attaching newly created CWindowTransparent class pointer to its corresponding HWND
bool CWindowTransparent::msgRouter_reattach()
{
	//2021nov29, spi, begin
	CWindowTransparent* pWindowTransparent = this;
	if (this == NULL) return false;
#ifdef _WIN64
	//for 64bit, SetWindowLong() call to be replaced by SetWindowLongPtr()
	::SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)pWindowTransparent);
#else
	::SetWindowLong(_hwnd, GWL_USERDATA, (long)pWindowTransparent);
#endif
	//2021nov29, spi, end
	return true;
}


LRESULT CALLBACK CWindowTransparent::msgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	CWindowTransparent *wnd = 0;

	if (uMsg == WM_NCCREATE) 
	{
		//2021nov29, spi, begin
#ifdef _WIN64
				//for 64bit, SetWindowLong() call to be replaced by SetWindowLongPtr()
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, ((LONG_PTR)(LPCREATESTRUCT(lParam))->lpCreateParams));
#else
		::SetWindowLong(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
#endif
		//2021nov29, spi, end
	}

	//2021nov29, spi, begin
#ifdef _WIN64
				//for 64bit, SetWindowLong() call to be replaced by SetWindowLongPtr()
	wnd = (CWindowTransparent *)(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
#else
	wnd = (CWindowTransparent *)(::GetWindowLong(hwnd, GWL_USERDATA));
#endif
	//2021nov29, spi, end

	if (wnd)
	{
		return wnd->wndProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}