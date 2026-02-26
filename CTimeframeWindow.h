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
#if !defined __CTimeframeWindow_H__
#define __CTimeframeWindow_H__

#define TIMEFRAMEWINDOW_MAXAUDIOBUFFERSIZE				(2*1920*2)	//max 2 channels and 2 screens 1920x1080
#define TIMEFRAMEWINDOW_IDTIMER_UPDATEDISPLAY			1


struct Point3D {

	GLfloat x;
	GLfloat y;
	GLfloat z;

};


class CTimeframeWindow : public CWindowParameter 
{
public:
	//2021oct01, spi, begin
	//modif frames-batch
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
	void saveopenglframetobmpfile();
	void SaveAsBMP(const char *fileName);
	//2021oct01, spi, end

	float _audiobuffer[TIMEFRAMEWINDOW_MAXAUDIOBUFFERSIZE];

private:
	bool _audiobuffer_ready;
	float _buf[TIMEFRAMEWINDOW_MAXAUDIOBUFFERSIZE];
	/*
	FIBITMAP* _dib;
	HDC _dc;
	HBITMAP _bmp;
	BYTE* _buf;
	*/
	int _mode;  
	int _xwidth;	 //display width
	int _yheight; //display height 
	
	bool _canvas_ready;
	bool _busy_painting;
	
	string _imagefilename;
	vector<string> _imagefilenames;
	int _imageid;

	DWORD _timer;

	int _inverseprobability;
	int _thetacounterthreshold;
	float _linewidth;
	float _pointsize;
	int _polygonfill;

	float _bgcolor_r;
	float _bgcolor_g;
	float _bgcolor_b;
	float _bgcolor_a;

	float _color1_r;
	float _color1_g;
	float _color1_b;
	float _color2_r;
	float _color2_g;
	float _color2_b;
	float _color3_r;
	float _color3_g;
	float _color3_b;
	float _color4_r;
	float _color4_g;
	float _color4_b;

	CAutoRescaleOW2Doc* _pOW2Doc;// = NULL;
	COW2View* _pOW2View;// = NULL;
	int _thetacounter;// = 0;
	HDC _hDC; //returned by opengl InitGL()
	//static HGLRC hRC; //returned by opengl InitGL()
	HGLRC _hRC; //returned by opengl InitGL()

	//Global buffers -------------------------
	ChannelBuffersHandler* _callbackFilledAudioBuffers;
	ChannelBuffersHandler* _drawnFromAudioBuffers;

	//Waterfall Objects ------------------------
	SpectrumDisplay* _specDisplayMix;
	float** _waterfallDisplayYValuesHistory;
	int _waterfallCount;
	int _waterfallReadIndex;
	int _waterfallWriteIndex;

	//Other Display Objects ------------------------
	AutocorrDisplay* _autocorrDisplayL;
	WaveformDisplay* _waveformDisplayL;

	AutocorrHandler* _autocorrHandler;
	SAMPLE* _autocorrBuffer;
	int _trailCount;
	int _trailReadIndex;
	int _trailWriteIndex;
	int _autocorrSkip1;
	int _autocorrSkip2;
	GLfloat _scaleFactor3D;
	GLfloat _scaleFactor2D;

	Point3D** _displayPointsHistory1;
	Point3D** _displayPointsHistory2;

	float _theta;
	bool _showWindows;
	bool _show3D;

	//Global thread locking / management variables --------------
	//pthread_mutex_t g_mutex;
	GLboolean _ready;// = false;

public:
	CTimeframeWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx,
		string imagefilename, vector<string> imagefilenames);
	CTimeframeWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		string imagefilename, vector<string> imagefilenames,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CTimeframeWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		string imagefilename, vector<string> imagefilenames,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha=150, int titlebardisplay=1, int menubardisplay=1);
	void Initialize();
	~CTimeframeWindow();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy(){}
	void AcquireAudioBuffer(float* audiobuffer, int framesperbuffer, int numchannels);
	void CreateCanvas();
	void UpdateDisplay();
	virtual void UpdateParameter(string name);
	void InvalidateDIB(int imageid);
protected:
	void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC);
	void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC);
	void reshapeFunc(GLsizei width, GLsizei height);
	void displayFunc(HDC hdc);
};


#endif