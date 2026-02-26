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
#if !defined __CVorossWindow_H__
#define __CVorossWindow_H__


//class CVorossWindow : public CWindowParameter 
class CVorossWindow : public CVoroguiWindow
{
protected:
	int _initialnumberofadditional_ss;
	int _totalnumberofadditional_ss;
public:
	/*
	CVorossWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CVorossWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CVorossWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	*/
	CVorossWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
		class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY);
	CVorossWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CVorossWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);

	void Initialize(bool bfirsttime=true);
	~CVorossWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	virtual void UpdateParameter(string name);

	bool add_ss(int numberof_ss, bool init=true);
	bool remove_ss(int numberof_ss, int index=-1);
};


#endif