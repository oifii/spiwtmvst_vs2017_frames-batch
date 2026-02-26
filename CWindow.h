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
#if !defined __CWINDOW_H__
#define __CWINDOW_H__

#include <Windows.h>

/// <summary>
/// Base class for creating windows easily and quickly.</summary>
/// <remarks>
/// It provides a static method that it forwards the window messages towards a 
/// pure virtual window procedure which must be implemented in the derived 
/// classes to process the window messages.</remarks>
class CWindow 
{
protected:
	HWND _hwnd;
	HINSTANCE _hInstance;
	LPTSTR _lpClassName;
	LPTSTR _lpCaption;
	LPVOID _lParam;
	WNDPROC _wndprocDefault;

	CWindow();
	virtual VOID SetClassName(LPCTSTR lpszClassName);
	virtual VOID Register(LPWNDCLASSEX lpwndClassEx);
public:
	CWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName);
	CWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, HICON hIconSm);

	~CWindow();

	virtual LPCTSTR GetCaption();
	virtual VOID SetCaption(LPCTSTR lpszCaption);
	virtual LPCTSTR GetClassName();

	virtual HWND Create();
	virtual HWND Create(HWND hWndParent);
	virtual HWND Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
		INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam);

	virtual BOOLEAN Show(int nCmdShow);
	virtual BOOLEAN Show();
	virtual BOOLEAN Hide();
	virtual BOOLEAN Maximize();
	virtual BOOLEAN Minimize();
	virtual BOOLEAN Restore();

	static LRESULT CALLBACK msgRouter(HWND, UINT, WPARAM, LPARAM);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM) = 0;
};

#endif