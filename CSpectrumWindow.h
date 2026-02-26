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
#if !defined __CSpectrumWindow_H__
#define __CSpectrumWindow_H__

//2021nov29, spi, begin
//#define SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE	(2*1920*2)	//max 2 channels and 2 screens 1920x1080
//#define SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE	(2*2048*2)	//max 2 channels * max framesperbuffer (should be greater than width of a 4K screen or greater than width of 2 screens of 1920x1080)
#define SPECTRUMWINDOW_MAXNUMCHANNELS		2
//#define SPECTRUMWINDOW_MAXFRAMEBUFFER		(2*2048)
//2 * FRAMES_PER_BUFFER
#define SPECTRUMWINDOW_MAXFRAMESPERBUFFER	(2*FRAMES_PER_BUFFER)
//2 * max num channels * max framesperbuffer * 2 (for 4K displays, has to be greater than width of a 4K screen or greater than width of 2 screens of 1920x1080)
#define SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE	(2*SPECTRUMWINDOW_MAXNUMCHANNELS*SPECTRUMWINDOW_MAXFRAMESPERBUFFER)	
//2021nov29, spi, end
#define SPECTRUMWINDOW_IDTIMER_UPDATESPECTRUM	1
#define SPECTRUMWINDOW_IDTIMER_UPDATEPARAMETER	12

class CSpectrumWindow : public CWindowParameter 
{
public:
	float _audiobuffer[SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE]; 

private:
	bool _audiobuffer_ready;
	RGBQUAD* _dibpalette;
	float _buf[SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE];
	float _fftbuf[SPECTRUMWINDOW_MAXAUDIOBUFFERSIZE];
	HDC _specdc;
	HBITMAP _specbmp;
	//2021nov29, spi, begin
	HGDIOBJ prevHGDIOBJ;// = NULL;
	int _prev_specmode;
	//2021nov29, spi, end
	BYTE* _specbuf;
	int _specmode; //spectrum mode 
	int _specpos; //marker pos for 2nd mode
	int _SPECWIDTH;	 //display width
	int _SPECHEIGHT; //display height 
	int _idcolorpalette;
	int _bands;
	bool _canvas_ready;
	bool _busy_painting;

	DWORD _timer;
	//2021nov29, spi, begin
	int _framesperbuffer_lastacquired; //generally does not change, but in case it does
	int _numchannels_lastacquired; //generally does not change, but in case it does
	//2021nov29, spi, end
public:
	CSpectrumWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
		RGBQUAD* dibpalette);
	CSpectrumWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		RGBQUAD* dibpalette,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CSpectrumWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		RGBQUAD* dibpalette,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	void Initialize();
	~CSpectrumWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	void AcquireAudioBuffer(float* audiobuffer, int framesperbuffer, int numchannels);
	void CreateCanvas();
	//void CALLBACK UpdateSpectrum(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	//2021nov29, spi, begin
	int getv(int &x, int &c, int &framesperbuffer, int &numchannels);
	//2021nov29, spi, end
	void UpdateSpectrum();
	virtual void UpdateParameter(string name);
	void InvalidateDIBPalette(RGBQUAD* newdibpalette);
};


#endif