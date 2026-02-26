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

#ifndef _VOROGUI_H
#define _VOROGUI_H

//2021nov29, spi, begin
//major revision


#define VOROGUI_INITIAL_NUMBEROFELEMENTS		1000
#define VOROGUI_NUMBEROFSTATSPERELEMENT			0
//2022nov18, spi, begin
//#define VOROGUI_ADDITIONAL_NUMBEROFELEMENTS		500
#define VOROGUI_ADDITIONAL_NUMBEROFELEMENTS		1000
//2022nov18, spi, end


#define VOROGUI_X_BORDER 2.0
#define VOROGUI_Y_BORDER 2.0
#define VOROGUI_X_RESOLUTION 1.0
#define VOROGUI_Y_RESOLUTION 1.0

#define VOROGUI_X_RIGHTMARGIN 80.0

//2022may14, spi, begin
//#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_X	40
//#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_X	20
#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_X	100
//#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_Y	20
#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_Y	100
//2022may14, spi, end
//#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_X	400
//#define VOROGUI_FRAMEEDGE_NUMBEROFPOINT_Y	200


//exclusive
#define VOROGUI_DEBUG_CONTROLS						2
#define VOROGUI_DEBUG_VORONOIDIAGRAM				4
//exclusive
#define VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED						0
#define VOROGUI_DISPLAY_CONTROLS_CLUSTERED							1
//exclusive
#define VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY					2
#define VOROGUI_DISPLAY_CONTROLS_GRIDLIKE							4
#define VOROGUI_DISPLAY_CONTROLS_GRIDLIKE_RANDOMIZED				8
#define VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL					16
#define VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL_RANDOMIZED		32
#define VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL				64
#define VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL_RANDOMIZED		128



#ifndef COMPILING_SPIVOROGUI_CPP
extern int vorogui_clustermodes[];
//2022may21, spi, begin
extern int numberofvoroguiclustermodes;
//extern const int numberofvoroguiclustermodes;
//2022may21, spi, end
#endif

//defined in oifiilib
//#define POINTSET_ALL	-1
//#define POINTSET_NONE	-2


class spivorogui : public COWPointset
{	
private:
	class Synth* _pSynth; //global_pSynth;
	//vector<CWindowParameter*> global_windowparameterpointers;
	vector<CWindowParameter*>* p_windowparameterpointers;
public:
	int vorogui_xwidth;
	int vorogui_yheight;

	//vorogui globals
	int vorogui_numberofframepoints;
	int vorogui_numberofpoints;
	int vorogui_itriseed;
	bool vorogui_displaytextlabelflag;
	int vorogui_idpointtobemoved;

	//2021nov29, spi, begin
	int vorogui_clustermode;
	int vorogui_idpointdirty;
	int vorogui_prevclustermode;
	int vorogui_clustermodesindex;
	//2021nov29, spi, end

	//2022july25, spi, begin
	DWORD prev_mousemove_nowstamp_ms;
	DWORD vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms;
	DWORD vorogui_mousepointerspeedbelowthreshold_starttime_ms;
	float prev_fmousepointerspeed;
	int vorogui_idpointtobedisplayed; // = -1 when nothing new, that is when point is the same as before therefore no new point label to be displayed
	int vorogui_idpointrecentlydisplayed; // = -1 when no point label is recently displayed
	DWORD vorogui_idpointrecentlydisplayed_timestamp_ms;
	int prev_xPos;
	int prev_yPos;
	spilogfile* p_spilogfile;
	//2022july25, spi, end

	//2022nov18, spi, begin
	bool vorogui_display_controls_levels;
	bool vorogui_display_controls_polygons;
	//2022nov18, spi, end

public:
	spivorogui()
	{
		Init();
	}
	//VOROGUI* VOROGUI_Init(class Synth* pSynth, vector<CWindowParameter*> &windowparameterpointers, int xwidth, int yheight, int voroguiclustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY);
	spivorogui(class Synth* pSynth, vector<CWindowParameter*> &windowparameterpointers, int xwidth, int yheight, int voroguiclustermode/* = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY*/)
		: COWPointset(VOROGUI_INITIAL_NUMBEROFELEMENTS, VOROGUI_NUMBEROFSTATSPERELEMENT) //1000 and 0 as arbitrary initial POINTSET size, will be reallocated as needed
	{
		Init();
		_pSynth = pSynth; //global_pSynth = pSynth;
		p_windowparameterpointers = &windowparameterpointers;
		vorogui_xwidth = xwidth;
		vorogui_yheight = yheight;
		vorogui_clustermode = voroguiclustermode;
		//createpointset(true); 
		initvoroguiframe(false); //add frame points and control (parameter) points 
		return; //return pVOROGUI;
	}
	//void VOROGUI_Terminate(VOROGUI* pVOROGUI);
	~spivorogui()
	{
		//VOROGUI doesn't own global_pSynth, so no need to delete it
		//VOROGUI doesn't own p_windowparameterpointers, so no need to delete it
		//2022july25, spi, begin
		if (p_spilogfile) delete p_spilogfile;
		//2022july25, spi, end
	}
public:
	void Init()
	{
		_pSynth = NULL;  //global_pSynth = NULL;

		vorogui_xwidth = 1920;
		vorogui_yheight = 1080;

		//vorogui globals
		vorogui_numberofframepoints = 0;
		vorogui_numberofpoints = 0; //excluding frame points
		vorogui_itriseed = 0;
		vorogui_displaytextlabelflag = false;
		vorogui_idpointtobemoved = -1;

		//2021nov29, spi, begin
		vorogui_clustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY;
		vorogui_idpointdirty = POINTSET_ALL;
		vorogui_prevclustermode = vorogui_clustermode;
		vorogui_clustermodesindex = -1;
		//2021nov29, spi, end

		//2022july25, spi, begin
		prev_mousemove_nowstamp_ms = 0;
		vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = 0;
		vorogui_mousepointerspeedbelowthreshold_starttime_ms = 0;
		prev_fmousepointerspeed = FLT_MAX;
		vorogui_idpointtobedisplayed = -1; //when nothing new, that is when point is the same as before therefore no new point label to be displayed
		vorogui_idpointrecentlydisplayed = -1; //when no point label is recently displayed
		vorogui_idpointrecentlydisplayed_timestamp_ms = 0;
		prev_xPos = -1;
		prev_yPos = -1;
		p_spilogfile = NULL;
		if (SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_SPIVOROGUIWINDOW_DEBUG)
		{
			p_spilogfile = DBG_NEW spilogfile("cvorogui_debug.txt", true);
		}
		//2022july25, spi, end

		//2024fev04, spi, begin
		vorogui_display_controls_levels = true; // false; // true; //2024fev04, spi, toggled to true
		vorogui_display_controls_polygons = false; // true;
		//2024fev04, spi, end

	}
	void getparameter(int index, float &min, float &max, float &value, string &name);
	void setparameter(int index, float ratio);
	int getclustermode();
	int setclustermode(int clustermode);
	int setclustermode_toggletonext();
	string getfilename();
	//
	//POINTSET* VOROGUI_CreatePointset(VOROGUI* pVOROGUI, bool buildtinflag = true);
	//POINTSET* createpointset(bool buildtinflag);
	bool initvoroguiframe(bool buildtinflag);
	//void VOROGUI_DestroyPointset(VOROGUI* pVOROGUI, POINTSET* pPOINTSET);
	//bool destroypointset(POINTSET* pPOINTSET);
	//bool destroypointset(COWPoinset* pPOINTSET);
	//bool replacepointset(POINTSET* pPOINTSET);
	//bool replacepointset(COWPoinset* pPOINTSET);
	//POINTSET* VOROGUI_ReadFromDisk(VOROGUI* pVOROGUI);
	bool readfromdisk();
	//void VOROGUI_WriteToDisk(VOROGUI* pVOROGUI, POINTSET* pPOINTSET);
	bool writetodisk();
	//
	//bool VOROGUI_NeedsRescale(VOROGUI* pVOROGUI, POINTSET* pPOINTSET);
	bool needsrescale();
	//void VOROGUI_DrawPointset(VOROGUI* pVOROGUI, POINTSET* pPOINTSET, HDC hdc, int idpoint=POINTSET_ALL, FIBITMAP* dib=NULL, bool bVoroPolygons=true, bool bWithControls=true, bool bWithControlLabels=true);
	//2022nov15, spi, begin
	//bool drawvorogui(HDC hdc, int idpoint = POINTSET_ALL, FIBITMAP* dib = NULL, bool bVoroPolygons = true, bool bWithControls = true, bool bWithControlLabels = true);
	bool drawvorogui(HDC &hdc, int idpoint = POINTSET_ALL, FIBITMAP* dib = NULL, bool bVoroPolygons = true, bool bWithControls = true, bool bWithControlLabels = true);
	//2022nov15, spi, end
	//void VOROGUI_OnLButtonDown(VOROGUI* pVOROGUI, POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
	bool onlbuttondown(HWND hwnd, WPARAM wParam, LPARAM lParam);
	//void VOROGUI_OnLButtonUp(VOROGUI* pVOROGUI, POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
	bool onlbuttonup(HWND hwnd, WPARAM wParam, LPARAM lParam);
	//void VOROGUI_OnRButtonUp(VOROGUI* pVOROGUI, POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
	bool onrbuttonup(HWND hwnd, WPARAM wParam, LPARAM lParam);
	//void VOROGUI_UpdateControlsFromParameters(VOROGUI* pVOROGUI, POINTSET* pPOINTSET);
	bool updatecontrolsfromparameters();
	bool getvoronoipolygon(int ivertex, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon, int maxnumpointoutputpolygon);
	bool getpolygonminmax(POINT* pPointInputPolygon, int numpointinputpolygon, long* p_xmin, long* p_xmax, long* p_ymin, long* p_ymax, int* p_idpointymin);
	bool slicepolygon(POINT* pPointInputPolygon, int numpointinputpolygon, double dfValue, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon);
	//2022may18, spi, begin
	//int spivorogui::getnearestpointsetobject(double dfX, double dfY, int* p_itriseed=NULL);
	int getnearestpointsetobject(double dfX, double dfY, int* p_itriseed = NULL);
	int getnearestpointsetobject_getparameter(double dfX, double dfY, int* p_itriseed, double* p_dfRatio, wstring* p_mywstring, RECT* p_myRECT);
	int getnearestpointsetobject_setparameter(double dfX, double dfY, int* p_itriseed, double* p_dfRatio, wstring* p_mywstring, RECT* p_myRECT);
	//2022may18, spi, end
	//2022july25, spi, begin
	bool onmousemove(HWND hwnd, WPARAM wParam, LPARAM lParam);
	//2022july25, spi, end
};






#endif