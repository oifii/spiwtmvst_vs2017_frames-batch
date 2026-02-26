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
#if !defined __CVoroguiWindow_H__
#define __CVoroguiWindow_H__

//2022may21, spi, begin
//comment it out and include this header directly in .cpp files where ever required
//#include "spivorogui.h"
//2022may21, spi, end

#define VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR	1

//for indexed 8 bit using palette
//#define VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL	1
//for r,g,b 24 bit
#define VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_NUMBYTEPERPIXEL	3

#define VOROGUIWINDOW_NUMBEROFBUFFERS_PERCANVASSIZE	2


#define VOROGUIWINDOW_IDTIMER_AUTOSPEC								1
#define VOROGUIWINDOW_IDTIMER_AUTOSPEC24							2
#define VOROGUIWINDOW_IDTIMER_AUTOIMG								3
//2022july25, spi, begin
//not used yet, using time stamped variable instead
//#define VOROGUIWINDOW_IDTIMER_VOROGUI_IDPOINTRECENTLYDISPLAYED	    4
//2022july25, spi, end

class spicanvas
{
public:
	SIZE _size;

	HDC _hdc;
	HBITMAP _hbitmap;
	BYTE* _buf;
	HGDIOBJ _prev_hgdiobj;
	HDC _hdc_ref;
	HBITMAP _hbitmap_ref;
	BYTE* _buf_ref;
	HGDIOBJ _prev_hgdiobj_ref;
	bool _canvas_allocated;
	bool _canvas_rendered;
	bool _canvas_rendered_ref;
	bool _canvas_ready;

	spicanvas()
	{
		_size = {0,0};
		_hdc = 0;
		_hbitmap = 0;
		_buf = 0;
		_prev_hgdiobj = 0;
		_hdc_ref = 0;
		_hbitmap_ref = 0;
		_buf_ref = 0;
		_prev_hgdiobj_ref = 0;
		_canvas_allocated = false;
		_canvas_rendered = false;
		_canvas_rendered_ref = false;
		_canvas_ready = false; //ready means allocated and rendered
	}

	~spicanvas()
	{
		if (_prev_hgdiobj &&_hdc) SelectObject(_hdc, _prev_hgdiobj);
		if (_hbitmap) DeleteObject(_hbitmap);
		if (_hdc) DeleteDC(_hdc);
		if (_prev_hgdiobj_ref &&_hdc_ref) SelectObject(_hdc_ref, _prev_hgdiobj_ref);
		if (_hbitmap_ref) DeleteObject(_hbitmap_ref);
		if (_hdc_ref) DeleteDC(_hdc_ref);
	}
};


class CVoroguiWindow : public CWindowParameter 
{
protected:
	//2022may20, spi, begin 
	//Synth* _pSynth;
	class Synth* _pSynth;
	//2022may20, spi, end
	vector<CWindowParameter*> _windowparameterpointers;
	FIBITMAP* _dib;
	//POINTSET* _pPOINTSET;

	vector<string> _imagefilenames;
	int _imageid;
	FIBITMAP* _nextdib;
	FIBITMAP* _nextquantizeddib;

	//2021nov29, spi, begin
	int _prev_imagewidth;
	int _prev_imageheight;
	bool _prev_displaytextlabelflag;
	HFONT _hfont;
	HFONT _prevhfont;
	//for keeping voronoi diagram drawing
	HDC _dc;
	HBITMAP _bmp;
	BYTE* _buf;
	HGDIOBJ _prevHGDIOBJ;// = NULL;
	//for keeping voronoi control levels drawing
	HDC _dc_ref;
	HBITMAP _bmp_ref;
	BYTE* _buf_ref;
	HGDIOBJ _prevHGDIOBJ_ref;// = NULL;
	int _mode;
	LONG _WIDTH; // int _WIDTH;	 //display width
	LONG _HEIGHT; // int _HEIGHT; //display height 
	bool _canvas_allocated;
	bool _canvas_rendered;
	bool _canvas_rendered_ref;
	bool _canvas_ready;
	bool _busy_painting;
	spilogfile* p_spilogfile;
	spicanvas* p_spicanvas;
	HPEN hpen; // = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hpen_green; // = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HBRUSH hbrush; // = CreateSolidBrush(RGB(50, 50, 50));
	HBRUSH hatchhbrush; //= CreateHatchBrush(HS_DIAGCROSS, RGB(50, 50, 50));
	//2021nov29, spi, end
protected:
	//class VOROGUI* _pVOROGUI;
	spivorogui* _pVOROGUI;
	int vorogui_clustermode;

public:
	CVoroguiWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
	class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY);
	CVoroguiWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CVoroguiWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
	class Synth* pSynth, vector<CWindowParameter*> windowparameterpointers, FIBITMAP* dib, vector<string> imagefilenames, int vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	void Initialize();
	~CVoroguiWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	virtual void UpdateParameter(string name);
	//2021nov29, spi, begin
	void CreateCanvas();
	void UpdateDisplay(HDC hdc);
	//
	void PreCreateCanvas();
	int MatchCanvas(SIZE size);
	int MatchCanvas(int width, int height);
	void CreateCanvas(int width, int height, LONG &width_actual, LONG &height_actual,
		HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
		HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
		HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
		bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
		int numbyteperpixel=3, int createcanvasflag=VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
	void RenderCanvas(int width, int height, LONG &width_actual, LONG &height_actual,
		HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
		HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
		HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
		bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
		int numbyteperpixel = 3, int createcanvasflag = VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
	void RescaleVoroguiIfNeeded(int width, int height, LONG &width_actual, LONG &height_actual,
		HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
		HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
		HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
		bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
		int numbyteperpixel = 3, int createcanvasflag = VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR);
	bool RenderVectorLayer(int width, int height, LONG &width_actual, LONG &height_actual,
		HDC &hdc, HBITMAP &hbitmap, BYTE* &buffer,
		HDC &hdc_ref, HBITMAP &hbitmap_ref, BYTE* &buffer_ref,
		HGDIOBJ &prev_hgdiobj, HGDIOBJ &prev_hgdiobj_ref,
		bool &canvas_allocated, bool &canvas_rendered, bool &canvas_rendered_ref, bool &canvas_ready,
		int numbyteperpixel = 3, int createcanvasflag = VOROGUIWINDOW_CREATECANVAS_CANVASSIZE_DIVISIBLEBYFOUR, bool force_dc_ref_refresh=true, HDC hdc_physical=NULL);
	//2021nov29, spi, end
	int GetImageID();
	void SwitchToNextImage();
	//2022nov18, spi, begin
	//bool IsVorossWindow(); //was not working anyway, created vorogswindow class instead
	#define VOROGUI_INVALIDATEDIB_FORALLCONTROL			1
	#define VOROGUI_INVALIDATEDIB_FORONECONTROL			2
	#define VOROGUI_INVALIDATEDIB_FORONERANDOMCONTROL	4
	#define VOROGUI_INVALIDATEDIB_FOR50PCRANDOMCONTROL	8
	int _invalidatedib_flag;
	void InvalidateDIB(FIBITMAP* dib);
	void InvalidateDIBForOneControl(FIBITMAP* dib);
	void InvalidateDIBForOneRandomControl(FIBITMAP* newdib);
	void InvalidateDIBFor50pcRandomControl(FIBITMAP* newdib);
	//2022nov18, spi, end
	void AutoSpec();
	void AutoSpec24();
	void AutoImg();
	//2024aug31, spi, begin
	//added for CVorossWindow to save a frame on user's request
	//string global_ffmpegpath; // = "c:\\video\\ffmpeg.exe";
	//string global_outputvideofoldername; // = "c:\\temp\\spiwtmvstf-shuffle";
	//string global_outputvideofilenameext; // = "mp4";
	//int global_maxnumberofframeperseedimage; // = 200;
	//int global_outputvideoframepersecond; // = 10;
	string global_framefilenameprefix; // = "frame_";
	string global_framefilenameext; // = "bmp";

	bool currentlysavingframe; // = false;
	//bool skipsaveframes; // = true;
	string global_outputfoldername; // = "frames";
	int global_frameid; // = 0;
	void saveframetobmpfile(HWND hwnd, HDC hdc);
	//2024aug31, spi, end

};


#endif // __CVoroguiWindow_H__