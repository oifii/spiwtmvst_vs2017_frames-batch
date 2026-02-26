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
#if !defined __CTextWindow_H__
#define __CTextWindow_H__


class CTextWindow : public CWindowParameter 
{
private:
	int _staticalignment;
	COLORREF _statictextcolor;
	HFONT _hfont;
	FIBITMAP* _dib;

public:
	CTextWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
		int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib);
	CTextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CTextWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		int staticalignment, COLORREF statictextcolor, HFONT hfont, FIBITMAP* dib,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	void Initialize();
	~CTextWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	virtual void UpdateParameter(string name);
	void InvalidateDIB(FIBITMAP* dib);
};


#endif