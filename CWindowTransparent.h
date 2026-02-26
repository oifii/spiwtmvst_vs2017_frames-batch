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
#if !defined __CWINDOWTRANSPARENT_H__
#define __CWINDOWTRANSPARENT_H__

#include <Windows.h>

/// <summary>
/// Base class for creating windows easily and quickly.</summary>
/// <remarks>
/// It provides a static method that it forwards the window messages towards a 
/// pure virtual window procedure which must be implemented in the derived 
/// classes to process the window messages.</remarks>
class CWindowTransparent 
{
protected:
	HWND _hwnd;
	HINSTANCE _hInstance;
	LPTSTR _lpClassName;
	LPTSTR _lpCaption;
	LPVOID _lParam;
	WNDPROC _wndprocDefault;
	int _x;
	int _y;
	int _xwidth;
	int _yheight;
	BYTE _alpha;
	int _titlebardisplay;
	int _menubardisplay;

	CWindowTransparent();
	virtual VOID SetClassName(LPCTSTR lpszClassName);
	virtual VOID Register(LPWNDCLASSEX lpwndClassEx);
public:
	CWindowTransparent(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CWindowTransparent(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	CWindowTransparent(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, HICON hIconSm,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);

	virtual ~CWindowTransparent();

	HWND GetHWND();
	virtual LPCTSTR GetCaption();
	virtual VOID SetCaption(LPCTSTR lpszCaption);
	virtual LPCTSTR GetClassName();

	virtual HWND Create();
	virtual HWND Create(HWND hWndParent);
	virtual HWND Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
		INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam);

	virtual BOOLEAN IsWindowVisible();
	virtual BOOLEAN Show(int nCmdShow);
	virtual BOOLEAN Show();
	virtual BOOLEAN Hide();
	virtual BOOLEAN Maximize();
	virtual BOOLEAN Minimize();
	virtual BOOLEAN Restore();

	bool msgRouter_detach();
	bool msgRouter_reattach();
	static LRESULT CALLBACK msgRouter(HWND, UINT, WPARAM, LPARAM);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM) = 0;
};

#endif