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
#if !defined __CImageWindow_H__
#define __CImageWindow_H__

#define IMAGEWINDOW_MAXAUDIOBUFFERSIZE	(2*1920*2)	//max 2 channels and 2 screens 1920x1080
#define IMAGEWINDOW_IDTIMER_UPDATEIMAGE			1

class CImageWindow : public CWindowParameter 
{
public:
	//spi, begin
	//2018april24, frames-batch
	string global_ffmpegpath; // = "c:\\video\\ffmpeg.exe";
	string global_outputvideofoldername; // = "c:\\temp\\spiwtmvstf-shuffle";
	string global_outputvideofilenameext; // = "mp4";
	int global_maxnumberofframeperseedimage; // = 200;
	int global_outputvideoframepersecond; // = 10;
	string global_framefilenameprefix; // = "frame_";
	string global_framefilenameext; // = "bmp";

	bool currentlysavingframe; // = false;
	bool skipsaveframes; // = true;
	string global_outputfoldername; // = "frames";
	int global_frameid; // = 0;
	void saveframetobmpfile(HWND hwnd, HDC hdc);
	//spi, end

private:
	FIBITMAP* _dib;
	HDC _dc;
	HBITMAP _bmp;
	BYTE* _buf;
	int _mode;  
	int _WIDTH;	 //display width
	int _HEIGHT; //display height 
	bool _canvas_ready;
	bool _busy_painting;
	//2021nov29, spi, begin
	HGDIOBJ prevHGDIOBJ;// = NULL;
	//because stretch blit displaced into create canvas, optimization now requires duplicates of _dc, _bmp, _buf, and prevHGDIOBJ that are tagged _ref
	HGDIOBJ prevHGDIOBJ_ref;// = NULL;
	HDC _dc_ref;
	HBITMAP _bmp_ref;
	BYTE* _buf_ref;
	int _prev_specmode;
	spilogfile* p_spilogfile;
	//2021nov29, spi, end

	DWORD _timer;

public:
	CImageWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
		FIBITMAP* dib);
	CImageWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		FIBITMAP* dib,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CImageWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		FIBITMAP* dib,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	void Initialize();
	~CImageWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	void CreateCanvas(int numbyteperpixel=3);
	void UpdateDisplay(int numbyteperpixel=3);
	virtual void UpdateParameter(string name);
	void InvalidateDIB(FIBITMAP* dib);
};


#endif