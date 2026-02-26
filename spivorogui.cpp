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

#include "spiwindowtransparentmidivoroguispectrumtimeframe.h" //2021nov29, spi
#include "spiutility.h"

#include "Tonic.h" //spi
using namespace Tonic;

//#include "c_pointset.h"
#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL

//2021nov29, spi, begin
#include "FreeImage.h"
//2021nov29, spi, end
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h" 
#include "CParameter.h"
#include "CWindowParameter.h"

#define COMPILING_SPIVOROGUI_CPP		1
#include "spivorogui.h"

#include <assert.h>

//2021nov29, spi, begin
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include "CSPITextWindow.h"
//2021nov29, spi, end
using namespace std;




/*
#include "ControlSwitcherExpSynth.h"
#include "BandlimitedOscillatorExpSynth.h"
#include "BufferPlayerExpSynth.h"
#include "CompressorExpSynth.h"
#include "FMDroneExpSynth.h"
#include "FilteredNoiseSynth.h"
#include "StepSequencerBufferPlayerExpSynth.h"
#include "StepSequencerBufferPlayerEffectExpSynth.h"
#include "StepSequencerExpSynth.h"
#include "DelayExpSynth.h"
#include "FilterExpSynth.h"
#include "EventsExpSynth.h"
#include "EventsExpBufferPlayerSynth.h"
#include "LFNoiseTestSynth.h"
#include "ReverbTestSynth.h"
#include "SimpleStepSeqSynth.h"
#include "SimpleStepSequencerBufferPlayerSynth.h"
#include "SineSumSynth.h"
#include "XYSpeedSynth.h"
#include "StereoDelayTestSynth.h"
#include "CompressorDuckingTestSynth.h"
*/

#include <Windowsx.h>

#include <typeinfo>    // for 'typeid'

#include <assert.h>

#include <fstream>
#include <iostream>
//using namespace std;


int vorogui_clustermodes[] = {
	VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_GRIDLIKE,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_GRIDLIKE_RANDOMIZED,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL_RANDOMIZED,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL,
	VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL_RANDOMIZED
};
//2022may21, spi, begin
//const int numberofvoroguiclustermodes = sizeof(vorogui_clustermodes)/sizeof(vorogui_clustermodes[0]);
int numberofvoroguiclustermodes = sizeof(vorogui_clustermodes) / sizeof(vorogui_clustermodes[0]);
//2022may21, spi, end


//2021nov29, spi, begin
//replaced by (and localized with) vorogui_xwidth and vorogui_yheight
//extern int global_xwidth;
//extern int global_yheight;
//2021nov29, spi, end

/*
extern class ControlSwitcherExpSynth* global_pSynth;
*/
//int global_xwidth;
//int global_yheight;
//class Synth* global_pSynth;
//vector<CWindowParameter*> global_windowparameterpointers;

//extern class BandlimitedOscillatorExpSynth* global_pSynth;
//extern class BufferPlayerExpSynth* global_pSynth;
//extern class CompressorExpSynth* global_pSynth;
//extern class CompressorDuckingTestSynth* global_pSynth;
//extern class FMDroneExpSynth* global_pSynth;
//extern class FilteredNoiseSynth* global_pSynth;
//extern class StepSequencerBufferPlayerExpSynth* global_pSynth;
//extern class StepSequencerBufferPlayerEffectExpSynth* global_pSynth;
//extern class StepSequencerExpSynth* global_pSynth;
//extern class DelayExpSynth* global_pSynth;
//extern class FilterExpSynth* global_pSynth;
//extern class EventsExpSynth* global_pSynth;
//extern class EventsExpBufferPlayerSynth* global_pSynth;
//extern class LFNoiseTestSynth* global_pSynth;
//extern class ReverbTestSynth* global_pSynth;
//extern class SimpleStepSeqSynth* global_pSynth;
//extern class SineSumSynth* global_pSynth;
//extern class SimpleStepSequencerBufferPlayerSynth* global_pSynth;
//extern class XYSpeedSynth* global_pSynth;
//extern class StereoDelayTestSynth* global_pSynth;

/*
//vorogui globals
int vorogui_numberofframepoints = 0;
int vorogui_numberofpoints = 0;
int vorogui_itriseed = 0;
bool vorogui_displaytextlabelflag = false;
int vorogui_idpointtobemoved = -1;
*/

/*
//instead, call RandomFloat(float a, float b);
float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}
*/

void spivorogui::getparameter(int index, float &min, float &max, float &value, string &name)
{
	if (index < 0 || index >= vorogui_numberofpoints)
	{
		assert(false);
		return;
	}

	int icount=0;
	if (_pSynth)
	{
		vector<ControlParameter> params = _pSynth->getParameters();  //global_pSynth->getParameters();
		if (index < params.size())
		{
			min = params[index].getMin();
			max = params[index].getMax();
			value = params[index].getValue();
			name = params[index].getName();
			return;
		}
		icount = params.size();
	}
	for (vector<CWindowParameter*>::iterator it = p_windowparameterpointers->begin(); it != p_windowparameterpointers->end(); ++it)
	{
		vector<CParameter> parameters = (*it)->getParameters();
		if ((index-icount)<parameters.size())
		{
			min = parameters[index - icount].getMin();
			max = parameters[index - icount].getMax();
			value = parameters[index - icount].getValue();
			name = parameters[index - icount].getName();
			return;
		}
		icount += parameters.size();
	}
	assert(false);
	return;
}

void spivorogui::setparameter(int index, float ratio)
{
	float min = 0.0;
	float max = 0.0;
	float value = 0.0;
	string name = "";

	if (index < 0 || index >= vorogui_numberofpoints)
	{
		assert(false);
		return;
	}
	if (ratio<0.0 || ratio>1.0)
	{
		assert(false);
		return;
	}

	int icount = 0;
	if (_pSynth)
	{
		vector<ControlParameter> params = _pSynth->getParameters();  //global_pSynth->getParameters();
		if (index < params.size())
		{
			min = params[index].getMin();
			max = params[index].getMax();
			value = params[index].getValue();
			name = params[index].getName();
			_pSynth->setParameter(name, ratio*(max - min) + min);  //global_pSynth->setParameter(name, ratio*(max - min) + min);
			return;
		}
		icount = params.size();
	}
	for (vector<CWindowParameter*>::iterator it = p_windowparameterpointers->begin(); it != p_windowparameterpointers->end(); ++it)
	{
		vector<CParameter> parameters = (*it)->getParameters();
		if ((index - icount)<parameters.size())
		{
			min = parameters[index - icount].getMin();
			max = parameters[index - icount].getMax();
			value = parameters[index - icount].getValue();
			name = parameters[index - icount].getName();
			(*it)->setParameter(name, ratio*(max - min) + min);
			return;
		}
		icount += parameters.size();
	}
	assert(false);
	return;
}


int spivorogui::getclustermode()
{
	return vorogui_clustermode;
}

int spivorogui::setclustermode(int clustermode)
{
	vorogui_prevclustermode = vorogui_clustermode;
	vorogui_clustermode = clustermode;
	return vorogui_clustermode;
}

int spivorogui::setclustermode_toggletonext()
{
	if (vorogui_prevclustermode < 0) vorogui_prevclustermode = vorogui_clustermode;
	vorogui_clustermodesindex++;
	if (vorogui_clustermodesindex < 0) vorogui_clustermodesindex = 0;
	if (vorogui_clustermodesindex > numberofvoroguiclustermodes) vorogui_clustermodesindex = 0;
	if (vorogui_clustermodesindex == numberofvoroguiclustermodes)
	{
		vorogui_clustermodesindex = -1;
		vorogui_clustermode = vorogui_prevclustermode;
	}
	else
	{
		vorogui_clustermode = vorogui_clustermodes[vorogui_clustermodesindex];
	}
	return vorogui_clustermode;
}


//2022may20, spi, begin
//todo, to be moved to spiutility.h eventually
RECT getcenteredsubrect(const RECT* pRECT, int divider)
{
	RECT rect_centeredsubrect = {};
	if (pRECT && divider>0)
	{
		int rect_width = pRECT->right - pRECT->left;
		int rect_height = pRECT->bottom - pRECT->top;
		RECT rect_centeredsubrect;
		int divider = 4; //with 4, both x and y offsets are quarters of the width, therefore rect_cluster_centeredsubrect's area will be 4 time smaller
		bool bresult = CopyRect(&rect_centeredsubrect, pRECT);
		if (bresult)
		{
			bresult = InflateRect(&rect_centeredsubrect, -rect_width / divider, -rect_height / divider);
		}
		if (bresult==false)
		{
			//on InflateRect() error
			return *pRECT;
		}
	}
	return rect_centeredsubrect;
}
//2022may20, spi, end


bool spivorogui::initvoroguiframe(bool buildtinflag)
{
	POINTSET* pPOINTSET = m_pPOINTSET;

	///////////////////////////////////
	//find out total number of elements
	///////////////////////////////////
	int nx = VOROGUI_FRAMEEDGE_NUMBEROFPOINT_X; //=20;
	int ny = VOROGUI_FRAMEEDGE_NUMBEROFPOINT_Y; //=20;
	int nframethickness = 1; // =1; //only 1 or 2 for now
	if (nx>1 && ny>1)
	{
		//2022may14, spi, begin
		vorogui_numberofframepoints = (2 * nx + 2 * ny) * nframethickness;
		//vorogui_numberofframepoints = nx + (ny-1) + (nx-1) + (ny-1);
		//2022may14, spi, end
		vorogui_numberofpoints = 0;
		if (_pSynth)
		{
			vector<ControlParameter> params = _pSynth->getParameters(); //global_pSynth->getParameters();
			vorogui_numberofpoints = params.size();
		}
		for (vector<CWindowParameter*>::iterator it = p_windowparameterpointers->begin(); it != p_windowparameterpointers->end(); ++it)
		{
			vector<CParameter> parameters = (*it)->getParameters();
			vorogui_numberofpoints += parameters.size();
		}
		//pPOINTSET = NewPointset(vorogui_numberofpoints + vorogui_numberofframepoints); //already allocated
		/////////////////////
		//add frame of points
		/////////////////////
		pPOINTSET->npts = 0;
		float fxstep = (vorogui_xwidth - 1.0) / float(nx);
		float fystep = (vorogui_yheight - 1.0) / float(ny);
		float fminrandom = 1.0/20.0;
		float fmaxrandom = 2.0/20.0;
		//line along (dfXmin,dfYmin) to (dfXmax, dfYmin)
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nframethickness; k++)
			{
				float fdx = k * RandomFloat(fxstep*fminrandom, fxstep*fmaxrandom);
				float fdy = k * RandomFloat(fystep*fminrandom, fystep*fmaxrandom);
				MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
				pPOINTSET->px[pPOINTSET->npts] = 0.0 + i * fxstep + fdx;
				pPOINTSET->py[pPOINTSET->npts] = 0.0 + fdy;
				pPOINTSET->controlratio[pPOINTSET->npts] = -1.0;
				pPOINTSET->npts++;
			}
		}
		//2022may14, spi, begin
		//line along (dfXmax, dfYmin) to (dfXmax,dfYmax)
		for (int j = 0; j < ny; j++)
		//for (int j = 1; j < ny; j++) //skip (dfXmax, dfYmin), because already entered
		//2022may14, spi, end
		{
			for (int k = 0; k < nframethickness; k++)
			{
				float fdx = k * RandomFloat(fxstep*fminrandom, fxstep*fmaxrandom);
				float fdy = k * RandomFloat(fystep*fminrandom, fystep*fmaxrandom);
				MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
				pPOINTSET->px[pPOINTSET->npts] = 0.0 + (vorogui_xwidth - 1.0) - fdx;
				pPOINTSET->py[pPOINTSET->npts] = 0.0 + j * fystep + fdy;
				pPOINTSET->controlratio[pPOINTSET->npts] = -1.0;
				pPOINTSET->npts++;
			}
		}
		//2022may14, spi, begin
		//line along (dfXmax,dfYmax) to (dfXmin, dfYmax)
		for (int i = 0; i < nx; i++)
		//for (int i = 1; i < nx; i++) //skip (dfXmax,dfYmax), because already entered
		//2022may14, spi, end
		{
			for (int k = 0; k < nframethickness; k++)
			{
				float fdx = k * RandomFloat(fxstep*fminrandom, fxstep*fmaxrandom);
				float fdy = k * RandomFloat(fystep*fminrandom, fystep*fmaxrandom);
				MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
				pPOINTSET->px[pPOINTSET->npts] = 0.0 + (vorogui_xwidth - 1.0) - i * fxstep - fdx;
				pPOINTSET->py[pPOINTSET->npts] = 0.0 + (vorogui_yheight - 1.0) - fdy;
				pPOINTSET->controlratio[pPOINTSET->npts] = -1.0;
				pPOINTSET->npts++;
			}
		}
		//2022may14, spi, begin
		//line along (dfXmin, dfYmax) to(dfXmin,dfYmin)
		for (int j = 0; j < ny; j++)
		//for (int j = 1; j < (ny-1); j++) //skip (dfXmin, dfYmax), because already entered AND skip (dfXmin,dfYmin), because already entered
		//2022may14, spi, end
		{
			for (int k = 0; k < nframethickness; k++)
			{
				float fdx = k * RandomFloat(fxstep*fminrandom, fxstep*fmaxrandom);
				float fdy = k * RandomFloat(fystep*fminrandom, fystep*fmaxrandom);
				MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
				pPOINTSET->px[pPOINTSET->npts] = 0.0 + fdx;
				pPOINTSET->py[pPOINTSET->npts] = 0.0 + (vorogui_yheight - 1.0) - j * fystep - fdy;
				pPOINTSET->controlratio[pPOINTSET->npts] = -1.0;
				pPOINTSET->npts++;
			}
		}
	}

	////////////
	//add points
	////////////
	//2021nov29, spi, begin
	//int ncluster = p_windowparameterpointers->size() + 1; //+1 for the global_pSynth
	int ncluster = p_windowparameterpointers->size(); 
	if (_pSynth)
	{
		ncluster++; //+1 for the global_pSynth
	}
	if ( (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_CLUSTERED) && ncluster > 1)
	{
		//here we separate clusters of parameters, one vorogui cluster for each different opened window
		RECT voroguirect = { 0, 0, vorogui_xwidth, vorogui_yheight };
		int clustergridtypeflag = SPISUBRECT_GRIDLIKE;
		if (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL)
		{
			clustergridtypeflag = SPISUBRECT_LISTLIKE_VERTICAL;
		}
		else if(vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL)
		{
			clustergridtypeflag = SPISUBRECT_LISTLIKE_HORIZONTAL;
		}
		spisubrect myvorogui_spisubrect(voroguirect, ncluster, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, clustergridtypeflag);
		//int index_cluster = -1;
		int i = vorogui_numberofframepoints;
		//for (vector<CWindowParameter*>::iterator it = p_windowparameterpointers->begin(); it != p_windowparameterpointers->end(); ++it)
		vector<CWindowParameter*>::iterator it; 
		for (int index_cluster=0; index_cluster < ncluster; index_cluster++) 
		{
			int iclustersize = 0;
			if (index_cluster==0 && _pSynth!=NULL)
			{
				vector<ControlParameter> params = _pSynth->getParameters(); //global_pSynth->getParameters();
				iclustersize = params.size();
			}
			else if ((index_cluster==0 && _pSynth==NULL) || (index_cluster==1 && _pSynth!=NULL) )
			{
				it = p_windowparameterpointers->begin();
				vector<CParameter> parameters = (*it)->getParameters();
				iclustersize = parameters.size();
			}
			else
			{
				it++;
				vector<CParameter> parameters = (*it)->getParameters();
				iclustersize = parameters.size();
			}
			RECT rect_cluster = myvorogui_spisubrect.getrect(index_cluster);
			//2022may20, spi, begin
			//this modif
			int rect_cluster_width = rect_cluster.right - rect_cluster.left;
			int rect_cluster_height = rect_cluster.bottom - rect_cluster.top;
			RECT rect_cluster_centeredsubrect;
			int divider = 4; //with 4, both x and y offsets are quarters of the width, therefore rect_cluster_centeredsubrect's area will be 4 time smaller
			bool bresult = CopyRect(&rect_cluster_centeredsubrect, &rect_cluster);
			if (bresult)
			{
				bresult = InflateRect(&rect_cluster_centeredsubrect, -rect_cluster_width / divider, -rect_cluster_height / divider);
			}
			if (bresult)
			{
				bresult = true; //OffsetRect(&rect_cluster_centeredsubrect, rect_cluster_width / 4, rect_cluster_height / 4);
			}
			RECT rect_cluster_tobeused;
			if (bresult)
			{
				bresult = CopyRect(&rect_cluster_tobeused, &rect_cluster_centeredsubrect);
			}
			else
			{
				bresult = CopyRect(&rect_cluster_tobeused, &rect_cluster);
			}
			RECT rect_cluster_subrect_randomrange_tobeused = {};
			//2022may20, spi, end
			for (int iii = 0; iii < iclustersize; iii++)
			{
				//i is the pointset's point offset
				//i = i + iii;
				//2022may20, spi, begin
				/*
				int rectclusterright = rect_cluster.right - VOROGUI_X_BORDER - VOROGUI_X_RIGHTMARGIN; //leave a right margin so vorogui control name tags do not exceeed cluster's rect
				if( rectclusterright < (rect_cluster.left + VOROGUI_X_RIGHTMARGIN)) rectclusterright = rect_cluster.right - VOROGUI_X_BORDER;
				if (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY)
				{
					MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
					pPOINTSET->px[i + iii] = 0.0 + RandomFloat(rect_cluster.left + VOROGUI_X_BORDER, rectclusterright); // rect_cluster.right - VOROGUI_X_BORDER);
					pPOINTSET->py[i + iii] = 0.0 + RandomFloat(rect_cluster.top + VOROGUI_Y_BORDER, rect_cluster.bottom - VOROGUI_Y_BORDER);
				*/
				int rectclusterright = rect_cluster_tobeused.right - VOROGUI_X_BORDER - VOROGUI_X_RIGHTMARGIN; //leave a right margin so vorogui control name tags do not exceeed cluster's rect
				if (rectclusterright < (rect_cluster_tobeused.left + VOROGUI_X_RIGHTMARGIN)) rectclusterright = rect_cluster_tobeused.right - VOROGUI_X_BORDER;
				if (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY)
				{
					MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
					pPOINTSET->px[i + iii] = 0.0 + RandomFloat(rect_cluster_tobeused.left + VOROGUI_X_BORDER, rectclusterright); // rect_cluster.right - VOROGUI_X_BORDER);
					pPOINTSET->py[i + iii] = 0.0 + RandomFloat(rect_cluster_tobeused.top + VOROGUI_Y_BORDER, rect_cluster_tobeused.bottom - VOROGUI_Y_BORDER);
				//2022may20, spi, end
					pPOINTSET->controlratio[i + iii] = 0.0;
					pPOINTSET->npts++;
					//verify if new point colliding
					if ((i + iii - vorogui_numberofframepoints) > 1)
					{
						for (int ii = vorogui_numberofframepoints; ii < (i + iii - 1); ii++)
						{
							//if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i])<0.0000001)
							//if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i]) < 1.0)
							if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i + iii]) < VOROGUI_X_RESOLUTION) //2021nov29, spi
							{
								//if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i])<0.0000001)
								//if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i]) < 1.0)
								if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i + iii]) < VOROGUI_Y_RESOLUTION) //2021nov29, spi
								{
									//collision, therefore
									//reject newly added point
									iii = iii - 1;
									pPOINTSET->npts--;
									break;
								}
							}
						}
					}
				}
				else if( (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_GRIDLIKE) || (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_GRIDLIKE_RANDOMIZED))
				{
					if (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_GRIDLIKE)
					{
						if ((vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL))
						{
							int controlsgridtypeflag = SPISUBRECT_LISTLIKE_VERTICAL;
							//2022may20, spi, begin
							//spisubrect myvorogui_clusters_spisubrect(rect_cluster, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							spisubrect myvorogui_clusters_spisubrect(rect_cluster_tobeused, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							//2022may20, spi, end
							POINT point_control = myvorogui_clusters_spisubrect.getrectcenter(iii);
							MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
							pPOINTSET->px[i + iii] = 0.0 + ((double)point_control.x);
							pPOINTSET->py[i + iii] = 0.0 + ((double)point_control.y);
							pPOINTSET->controlratio[i + iii] = 0.0;
							pPOINTSET->npts++;
							if ((vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL_RANDOMIZED))
							{
								RECT rect_control = myvorogui_clusters_spisubrect.getrect(iii);
								RECT rect_control_centeredsubrect = getcenteredsubrect(&rect_control, 4);
								pPOINTSET->px[i + iii] += RandomFloat(0, rect_control_centeredsubrect.right- rect_control_centeredsubrect.left);
								pPOINTSET->py[i + iii] += RandomFloat(0, rect_control_centeredsubrect.bottom - rect_control_centeredsubrect.top);
							}
						}
						else if ((vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL))
						{
							int controlsgridtypeflag = SPISUBRECT_LISTLIKE_HORIZONTAL;
							//2022may20, spi, begin
							//spisubrect myvorogui_clusters_spisubrect(rect_cluster, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							spisubrect myvorogui_clusters_spisubrect(rect_cluster_tobeused, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							//2022may20, spi, end
							POINT point_control = myvorogui_clusters_spisubrect.getrectcenter(iii);
							MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
							pPOINTSET->px[i + iii] = 0.0 + ((double)point_control.x);
							pPOINTSET->py[i + iii] = 0.0 + ((double)point_control.y);
							pPOINTSET->controlratio[i + iii] = 0.0;
							pPOINTSET->npts++;
							if ((vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL_RANDOMIZED))
							{
								RECT rect_control = myvorogui_clusters_spisubrect.getrect(iii);
								RECT rect_control_centeredsubrect = getcenteredsubrect(&rect_control, 4);
								pPOINTSET->px[i + iii] += RandomFloat(0, rect_control_centeredsubrect.right - rect_control_centeredsubrect.left);
								pPOINTSET->py[i + iii] += RandomFloat(0, rect_control_centeredsubrect.bottom - rect_control_centeredsubrect.top);
							}
						}
						else if ((vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_GRIDLIKE))
						{
							int controlsgridtypeflag = SPISUBRECT_GRIDLIKE;
							//2022may20, spi, begin
							//spisubrect myvorogui_clusters_spisubrect(rect_cluster, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							spisubrect myvorogui_clusters_spisubrect(rect_cluster_tobeused, iclustersize, SPISUBRECT_HORIZONTALFIRST, SPISUBRECT_WRAPAROUND, SPISUBRECT_ORPHANSALLOWED, controlsgridtypeflag);
							//2022may20, spi, end
							POINT point_control = myvorogui_clusters_spisubrect.getrectcenter(iii);
							MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
							pPOINTSET->px[i + iii] = 0.0 + ((double)point_control.x);
							pPOINTSET->py[i + iii] = 0.0 + ((double)point_control.y);
							pPOINTSET->controlratio[i + iii] = 0.0;
							pPOINTSET->npts++;
							if (vorogui_clustermode & VOROGUI_DISPLAY_CONTROLS_GRIDLIKE_RANDOMIZED)
							{
								RECT rect_control = myvorogui_clusters_spisubrect.getrect(iii);
								RECT rect_control_centeredsubrect = getcenteredsubrect(&rect_control, 4);
								pPOINTSET->px[i + iii] += RandomFloat(0, rect_control_centeredsubrect.right - rect_control_centeredsubrect.left);
								pPOINTSET->py[i + iii] += RandomFloat(0, rect_control_centeredsubrect.bottom - rect_control_centeredsubrect.top);
							}
						}
					}
				}
			}
			i = i + iclustersize; //prepare pointset's point offset for next cluster
		}
	}
	else
	{
	//2021nov29, spi, end
		//here we don't separate parameters, all opened windows have their parameters initially mixed randomly all together
		for (int i = vorogui_numberofframepoints; i < (vorogui_numberofframepoints + vorogui_numberofpoints); i++)
		{
			MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
			//pPOINTSET->px[i] = 0.0 + RandomFloat(2.0, global_xwidth - 2.0);
			pPOINTSET->px[i] = 0.0 + RandomFloat(VOROGUI_X_BORDER, vorogui_xwidth - VOROGUI_X_BORDER); //2021nov29, spi, total border is therefore 2*VOROGUI_X_BORDER
			//pPOINTSET->py[i] = 0.0 + RandomFloat(2.0, global_yheight - 2.0);
			pPOINTSET->py[i] = 0.0 + RandomFloat(VOROGUI_Y_BORDER, vorogui_yheight - VOROGUI_Y_BORDER); //2021nov29, spi, total border is therefore 2*VOROGUI_Y_BORDER
			pPOINTSET->controlratio[i] = 0.0;
			pPOINTSET->npts++;
			//verify if new point colliding
			if ((i - vorogui_numberofframepoints) > 1)
			{
				for (int ii = vorogui_numberofframepoints; ii < (i - 1); ii++)
				{
					//if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i])<0.0000001)
					//if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i]) < 1.0)
					if (abs(pPOINTSET->px[ii] - pPOINTSET->px[i]) < VOROGUI_X_RESOLUTION) //2021nov29, spi
					{
						//if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i])<0.0000001)
						//if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i]) < 1.0)
						if (abs(pPOINTSET->py[ii] - pPOINTSET->py[i]) < VOROGUI_Y_RESOLUTION) //2021nov29, spi
						{
							//collision, therefore
							//reject newly added point
							i = i - 1;
							pPOINTSET->npts--;
							break;
						}
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////
	//set control ratio according to synth's parameters
	///////////////////////////////////////////////////
	/*
	int ii = -1;
	//vector<ControlParameter> params = global_pSynth->getParameters();
	for (unsigned int i = 0; i < params.size(); i++)
	{
		ii++;
		TonicFloat min = params[i].getMin();
		TonicFloat max = params[i].getMax();
		TonicFloat value = params[i].getValue();
		string mystring = params[i].getName();

		if((max-min)!=0.0) pPOINTSET->controlratio[vorogui_numberofframepoints + i] = (value-min)/(max-min);
			else  pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.0;
		//bypass for now
		//pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.5;
	}
	for (vector<CWindowParameter*>::iterator it = global_windowparameterpointers.begin(); it != global_windowparameterpointers.end(); ++it)
	{
		vector<CParameter> parameters = (*it)->getParameters();
		for (unsigned int i = 0; i < parameters.size(); i++)
		{
			ii++;
			float min = parameters[i].getMin();
			float max = parameters[i].getMax();
			float value = parameters[i].getValue();
			string mystring = parameters[i].getName();

			if ((max - min) != 0.0) pPOINTSET->controlratio[vorogui_numberofframepoints + ii] = (value - min) / (max - min);
			else  pPOINTSET->controlratio[vorogui_numberofframepoints + ii] = 0.0;
			//bypass for now
			//pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.5;
		}
	}
	*/
	//2021nov29, spi, begin
	//pPOINTSET->npts = vorogui_numberofpoints + vorogui_numberofframepoints; //moved earlier
	//updatecontrolsfromparameters(); //original
	//pPOINTSET->npts = vorogui_numberofpoints + vorogui_numberofframepoints; //original
	//2021nov29, spi, end

	/*
	pPOINTSET->xmin = 0.0;
	pPOINTSET->xmax = 0.0 + global_xwidth +1; //+1 to be safe because adding random offset
	pPOINTSET->ymin = 0.0;
	pPOINTSET->ymax = 0.0 + global_yheight +1; //+1 to be safe because adding random offset
	*/
	pPOINTSET->xmin = MAXDBL;
	pPOINTSET->ymin = MAXDBL;
	pPOINTSET->xmax = MINDBL;
	pPOINTSET->ymax = MINDBL;
	for (int i = 0; i < pPOINTSET->npts; i++)
	{
		if (pPOINTSET->px[i] < pPOINTSET->xmin) pPOINTSET->xmin = pPOINTSET->px[i];
		if (pPOINTSET->px[i] > pPOINTSET->xmax) pPOINTSET->xmax = pPOINTSET->px[i];
		if (pPOINTSET->py[i] < pPOINTSET->ymin) pPOINTSET->ymin = pPOINTSET->py[i];
		if (pPOINTSET->py[i] > pPOINTSET->ymax) pPOINTSET->ymax = pPOINTSET->py[i];
	}

	/*
	FILE* pFILE = fopen("debug.txt", "w");
	for (int i = 0; i < pPOINTSET->npts; i++)
	{
		fprintf(pFILE, "%f, %f\n", pPOINTSET->px[i], pPOINTSET->py[i]);
	}
	fclose(pFILE);
	*/
	if (buildtinflag)
	{
		//2022may16, spi, begin
		/*
		BuildTriangleNetwork(pPOINTSET);
		ComputeAllTriangleCenters(pPOINTSET);
		*/
		BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
		//2022may16, spi, end
	}
	

	return pPOINTSET;
}

/*
bool spivorogui::destroypointset(POINTSET* pPOINTSET)
{
	//if (pVOROGUI == NULL) return NULL; 
	DeletePointset(pPOINTSET);
	pPOINTSET = NULL;
	return true;
}
*/
bool AreTriCenterClockwise(POINTSET* pPOINTSET, int* p_triindex, int numtriindexes)
{
	if (pPOINTSET==NULL || p_triindex==NULL)
	{
		return false;
	}
	double sum = 0.0;
	for (int j = 0; j < numtriindexes; j++) 
	{
		double x_v1 = pPOINTSET->ctx[p_triindex[j]];
		double y_v1 = pPOINTSET->cty[p_triindex[j]];
		double x_v2 = pPOINTSET->ctx[p_triindex[(j+1)% numtriindexes]];
		double y_v2 = pPOINTSET->cty[p_triindex[(j+1)% numtriindexes]];
		sum += (x_v2 - x_v1) * (y_v2 + y_v1);
	}
	return sum > 0.0;
}

//maxnumpointoutputpolygon is the number of POINT structure allocated for pPointOutputPolygon
bool spivorogui::getvoronoipolygon(int ivertex, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon, int maxnumpointoutputpolygon)
{
	if (pPointOutputPolygon == NULL || p_numpointoutputpolygon == NULL)
	{
		if(p_numpointoutputpolygon) *p_numpointoutputpolygon = 0;
		return false;
	}

	int numtrifound, numneighborfound;//, itriseed;
	//int itriseed=0;
	int p_arraytri[200];
	int p_arrayneighbor[200];
	if (FindAllValidTriSurroundingVertex(m_pPOINTSET,
		ivertex,
		&(vorogui_itriseed), //&itriseed, //&vorogui_itriseed,
		&numtrifound,
		p_arraytri,
		&numneighborfound,
		p_arrayneighbor) == TRUE)
	{
		//if all surrounding triangles are valid,		 
		// build voronoi polygon using each adjtri's center 
		if (numtrifound < 3)
		{
			assert(false); //development-time error, if allowed, define p_arraytri[n] with n>200
			*p_numpointoutputpolygon = 0;
			return false;
		}
		if (numtrifound > 200 || numtrifound > maxnumpointoutputpolygon-2)
		{
			assert(false); //development-time error, if allowed, define p_arraytri[n] with n>200
			*p_numpointoutputpolygon = 0;
			return false;
		}
		bool clockwise = true; //AreTriCenterClockwise(pPOINTSET, p_arraytri, numtrifound);
		for (int j = 0; j < numtrifound; j++)
		{
			POINT myPoint;
			myPoint.x = m_pPOINTSET->ctx[p_arraytri[j]];
			myPoint.y = m_pPOINTSET->cty[p_arraytri[j]];
			if (clockwise)
			{
				pPointOutputPolygon[j] = myPoint;
			}
			else
			{
				pPointOutputPolygon[numtrifound-1-j] = myPoint;
			}
		}
		// To close the polygon, the last point is equal to the first point 
		pPointOutputPolygon[numtrifound].x = pPointOutputPolygon[0].x;
		pPointOutputPolygon[numtrifound].y = pPointOutputPolygon[0].y;
		*p_numpointoutputpolygon = numtrifound + 1;
		return true;
	}
	*p_numpointoutputpolygon = 0;
	return false;
}

bool spivorogui::getpolygonminmax(POINT* pPointInputPolygon, int numpointinputpolygon, long* p_xmin, long* p_xmax, long* p_ymin, long* p_ymax, int* p_idpointymin)
{
	if (pPointInputPolygon==NULL || p_xmin==NULL || p_xmax==NULL || p_ymin==NULL || p_ymax==NULL) return false;

	*p_xmin = LONG_MAX;
	*p_xmax = LONG_MIN;
	*p_ymin = LONG_MAX;
	*p_ymax = LONG_MIN;
	if(p_idpointymin) *p_idpointymin = -1;
	for (int i = 0; i<numpointinputpolygon; i++)
	{
		if (pPointInputPolygon[i].y<*p_ymin)
		{
			*p_ymin = pPointInputPolygon[i].y;
			if (p_idpointymin) *p_idpointymin = i;
		}
		if (pPointInputPolygon[i].y>*p_ymax)
		{
			*p_ymax = pPointInputPolygon[i].y;
		}
		if (pPointInputPolygon[i].x<*p_xmin)
		{
			*p_xmin = pPointInputPolygon[i].x;
		}
		if (pPointInputPolygon[i].x>*p_xmax)
		{
			*p_xmax = pPointInputPolygon[i].x;
		}
	}
	return true;
}

bool spivorogui::slicepolygon(POINT* pPointInputPolygon, int numpointinputpolygon, double dfValue, POINT* pPointOutputPolygon, int* p_numpointoutputpolygon)
{
	if (pPointInputPolygon==NULL || pPointOutputPolygon==NULL || p_numpointoutputpolygon==NULL || numpointinputpolygon<3)
	{
		if(p_numpointoutputpolygon) *p_numpointoutputpolygon = 0;
		return false;
	}

	if (dfValue >= 1.0 || dfValue < 0.0)
	{
		if (dfValue > 1.0 || dfValue < 0.0)
		{
			assert(false);
			//keep going safely
			if(dfValue > 1.0) dfValue = 1.0;
			if (dfValue < 0.0) dfValue = 0.0;
			//*p_numpointoutputpolygon = 0;
			//return;
		}
		for (int i = 0; i<numpointinputpolygon; i++)
		{
			pPointOutputPolygon[i] = pPointInputPolygon[i];
		}
		*p_numpointoutputpolygon = numpointinputpolygon;
	}
	else
	{
		//1) find ymin and ymax (as well as xmin and xmax)
		long xmin, xmax, ymin, ymax;
		int idpointymin;
		getpolygonminmax(pPointInputPolygon, numpointinputpolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);

		//2) find two points where level line intersect polygon
		/*
		//   and build the two new polygons
		POINT pPointOutputPolygon1[100];
		int numpointoutputpolygon1;
		POINT pPointOutputPolygon2[100];
		int numpointoutputpolygon2;
		*/

		double pfX[100]; //we only need 2, but for safety
		double pfY[100]; //we only need 2, but for safety
		double x3, y3, x4, y4; //level line segment
		x3 = xmin;
		x4 = xmax;
		//y3=ymin+(ymax-ymin)*dfValue;
		y3 = ymax - (ymax - ymin)*dfValue;
		y4 = y3;
		int idintersect = 0;
		for (int i = 0; i<numpointinputpolygon - 1; i++)
		{
			double x1, y1, x2, y2; //polygon segment
			x1 = pPointInputPolygon[i].x;
			y1 = pPointInputPolygon[i].y;
			x2 = pPointInputPolygon[i + 1].x;
			y2 = pPointInputPolygon[i + 1].y;
			if (LineSegmentsIntersect(x1, y1, x2, y2, //line segment 1
				x3, y3, x4, y4, //line segment 2
				&pfX[idintersect], &pfY[idintersect]) == 1)
			{
				idintersect++;
				if (idintersect>2)
				{
					//LineSegmentsIntersect has found a third intersecting point?
					//OK, that happens because of intersection on a vertex.
					//will have to prevent duplicate points in the output.
					//ASSERT(FALSE);
				}
			}

		}
		
		//3) first, only copy intersection points to output (avoid duplicates)
		pPointOutputPolygon[0].x = (long)pfX[0];
		pPointOutputPolygon[0].y = (long)pfY[0];
		*p_numpointoutputpolygon = 1;
		for (int i = 1; i < idintersect; i++)
		{
			//if(pPointOutputPolygon[i].x==pPointOutputPolygon[0].x && pPointOutputPolygon[i].y==pPointOutputPolygon[0].y)
			//if(pfX[i]==pPointOutputPolygon[0].x && pfY[i]==pPointOutputPolygon[0].y)
			if ((((long)pfX[i]) == pPointOutputPolygon[0].x && ((long)pfY[i]) == pPointOutputPolygon[0].y)
				//|| ((i==2)&&(((long)pfX[i])==pPointOutputPolygon[1].x && ((long)pfY[i])==pPointOutputPolygon[1].y)) )
				|| ((i > 1) && (((long)pfX[i]) == pPointOutputPolygon[1].x && ((long)pfY[i]) == pPointOutputPolygon[1].y)))
			{
				//skip duplicate
				continue;
			}
			else
			{
				//pPointOutputPolygon[i].x=pfX[1];
				//pPointOutputPolygon[i].y=pfY[1];
				pPointOutputPolygon[1].x = (long)pfX[i];
				pPointOutputPolygon[1].y = (long)pfY[i];
				assert(*p_numpointoutputpolygon != 2);
				//if this assert fails, we are overwriting onto the second output point.
				//that means more than 2 intersecting points were found (excluding duplicates),
				//this case was not forseen by this implementation. 
				*p_numpointoutputpolygon = 2;
			}
		}
		*p_numpointoutputpolygon = 2;
		//could end here if a floodfill algo is used to fill part of the polygon below splitting line
		//2021nov29, spi, begin
		//4) select polygon and copy polygon points to output
		if (0) //if (*p_numpointoutputpolygon==2)
		{
			//4.1) pick a starting point
			int idstart = idpointymin;
			//int idend = idpointymin - 1;
			//if (idend < 0) idend = numpointinputpolygon - 2;
			//4.2) compute slope of the polygon's splitting line
			double x1 = pPointOutputPolygon[0].x;
			double y1 = pPointOutputPolygon[0].y;
			double x2 = pPointOutputPolygon[1].x;
			double y2 = pPointOutputPolygon[1].y;
			double m = 0.0; 
			double b = 0.0;
			int result = LineMB(x1,y1, x2, y2, //line segment 
								&m, &b); //y = mx + b
			//note: our slicing line is horizontal, m should be 0
			//4.3) collect points of the slice ploygon lying below the splitting line
			if (result)
			{
				bool prevpointwasincluded = false;
				int j = -1;
				//we use below for storing the returned value for the lowest point in y
				int below = IsPointUpLineMB(pPointInputPolygon[idpointymin].x, pPointInputPolygon[idpointymin].y, &m, &b); //is x,y above splitting line? 
				for (int i = 0; i < numpointinputpolygon - 1; i++)
				{
					int index = (idstart + i) % (numpointinputpolygon - 1);
					double x = pPointInputPolygon[index].x;
					double y = pPointInputPolygon[index].y;
					int side = IsPointUpLineMB(x, y, &m, &b); //is x,y above splitting line?
					if (side==below)
					{
						if (i>0 && !prevpointwasincluded)
						{
							//do insert the intersection point (between this segment and the polygon's splitting line)
							int origin_index = (idstart + i - 1) % (numpointinputpolygon - 1); //previous point
							double origin_x = pPointInputPolygon[origin_index].x;
							double origin_y = pPointInputPolygon[origin_index].y;
							//test both intersection points
							int onlinesegment = IsPointOnLineSegment(origin_x, origin_y, x, y, x1, y1, 1.0, 1.0); //1.0 pixel resolution in client coordinate
							if (onlinesegment == false)
							{
								onlinesegment = IsPointOnLineSegment(origin_x, origin_y, x, y, x2, y2, 1.0, 1.0); //1.0 pixel resolution in client coordinate
								if (onlinesegment == false)
								{
									//both points not on this line segment
									assert(false);
									//abort this computation, return safely
									pPointOutputPolygon[0].x = (long) x1;
									pPointOutputPolygon[0].y = (long) y1;
									pPointOutputPolygon[1].x = (long) x2;
									pPointOutputPolygon[1].y = (long) y2;
									*p_numpointoutputpolygon = 2;
									return true;
								}
								j++;
								pPointOutputPolygon[j].x = (long)x2;
								pPointOutputPolygon[j].y = (long)y2;
								prevpointwasincluded = false;
							}
							else
							{
								j++;
								pPointOutputPolygon[j].x = (long)x1;
								pPointOutputPolygon[j].y = (long)y1;
								prevpointwasincluded = false;
							}
						}
						//if point lies below, include point in output polygon
						j++;
						pPointOutputPolygon[j].x = (long) x;
						pPointOutputPolygon[j].y = (long) y;
						prevpointwasincluded = true;
					}
					else
					{
						//if above, don't include this point in output polygon
						if (prevpointwasincluded)
						{
							//but, if prevpoint was included, do insert the intersection point (between this segment and the polygon's splitting line)
							int origin_index = (idstart + i - 1) % (numpointinputpolygon - 1); //previous point
							double origin_x = pPointInputPolygon[origin_index].x;
							double origin_y = pPointInputPolygon[origin_index].y;
							//test both intersection points
							int onlinesegment = IsPointOnLineSegment(origin_x, origin_y, x, y, x1, y1, 1.0, 1.0); //1.0 pixel resolution in client coordinate
							if (onlinesegment == false)
							{
								onlinesegment = IsPointOnLineSegment(origin_x, origin_y, x, y, x2, y2, 1.0, 1.0); //1.0 pixel resolution in client coordinate
								if (onlinesegment==false)
								{
									//both points not on this line segment
									assert(false);
									//abort this computation, return safely
									pPointOutputPolygon[0].x = (long)x1;
									pPointOutputPolygon[0].y = (long)y1;
									pPointOutputPolygon[1].x = (long)x2;
									pPointOutputPolygon[1].y = (long)y2;
									*p_numpointoutputpolygon = 2;
									return true;
								}
								j++;
								pPointOutputPolygon[j].x = (long)x2;
								pPointOutputPolygon[j].y = (long)y2;
								prevpointwasincluded = false;
							}
							else
							{
								j++;
								pPointOutputPolygon[j].x = (long)x1;
								pPointOutputPolygon[j].y = (long)y1;
								prevpointwasincluded = false;
							}
							//j++;
						}
						else
						{
							
						}
						prevpointwasincluded = false;
					}
				}
				//close polygon
				pPointOutputPolygon[j + 1].x = pPointOutputPolygon[0].x;
				pPointOutputPolygon[j + 1].y = pPointOutputPolygon[0].y;
				*p_numpointoutputpolygon = j + 1;
			}
			else
			{
				//abort this computation, return safely
				pPointOutputPolygon[0].x = (long)x1;
				pPointOutputPolygon[0].y = (long)y1;
				pPointOutputPolygon[1].x = (long)x2;
				pPointOutputPolygon[1].y = (long)y2;
				*p_numpointoutputpolygon = 2;
				return true;
			}
		}
		//2021nov29, spi, end
	}
	return false;
}

//2021nov29, spi, begin
bool spivorogui::needsrescale()
{
	double xdelta_f;
	double ydelta_f;

	//assuming both vorogui_xwidth and vorogui_yheight are greater than zero and set to window rect/monitor size ...
	//assuming pPOINTSET->xmin, pPOINTSET->ymin, pPOINTSET->ymin, pPOINTSET->ymax are properly set to pPOINSET extent
	//assuming pPOINTSET->xmin and pPOINTSET->ymin are close to zero, there is no need to translate pPOINTSET
	//assuming some frame points have coordinates exactly matching pPOINTSET->xmax and pPOINTSET->ymax
	//pPOINTSET->xmin
	//pPOINTSET->ymin

	xdelta_f = (double)(vorogui_xwidth) - VOROGUI_X_BORDER - m_pPOINTSET->xmax; //xdelta_f = 0.0 when no rescale required
	ydelta_f = (double)(vorogui_yheight) - VOROGUI_Y_BORDER - m_pPOINTSET->ymax; //ydelta_f = 0.0 when no rescale required
	if ( (abs(xdelta_f)> 2.0*VOROGUI_X_RESOLUTION) || (abs(ydelta_f) > 2.0*VOROGUI_Y_RESOLUTION))
	{
		return true;
	}
	return false;
}
//2021nov29, spi, end


//2022nov15, spi, begin
//bool spivorogui::drawvorogui(HDC hdc, int idpoint/*=POINTSET_ALL*/, FIBITMAP* dib/* = NULL*/, bool bVoroPolygons/* = true*/, bool bWithControls/* = true*/, bool bWithControlLabels/* = true*/)
bool spivorogui::drawvorogui(HDC &hdc, int idpoint/*=POINTSET_ALL*/, FIBITMAP* dib/* = NULL*/, bool bVoroPolygons/* = true*/, bool bWithControls/* = true*/, bool bWithControlLabels/* = true*/)
//2022nov15, spi, end
{
	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HBRUSH hbrush = CreateSolidBrush(RGB(50, 50, 50));
	//HBRUSH hbrush = CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH hatchhbrush = CreateHatchBrush(HS_DIAGCROSS,RGB(50, 50, 50));
	HBRUSH nullhbrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	HPEN oldhpen = (HPEN)SelectObject(hdc, hpen);
	//HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, hbrush); //original
	HBRUSH oldhbrush = (HBRUSH)SelectObject(hdc, nullhbrush); //for debuging

	
	/*
	vector<ControlParameter> params = global_pSynth->getParameters();
	for (unsigned int i = 0; i < params.size(); i++)
	{
		TonicFloat min = params[i].getMin();
		TonicFloat max = params[i].getMax();
		TonicFloat value = params[i].getValue();
		string mystring = params[i].getName();
	}
	*/

	//2021nov29, spi, begin
	bool bforcefullredraw = needsrescale();
	if (bforcefullredraw ==true)
	{
		double x_scale = 1.0;
		double y_scale = 1.0;
		if (m_pPOINTSET->xmax > VOROGUI_X_RESOLUTION)
		{
			x_scale = (float)vorogui_xwidth / m_pPOINTSET->xmax;
		}
		if (m_pPOINTSET->ymax > VOROGUI_Y_RESOLUTION)
		{
			y_scale = (float)vorogui_yheight / m_pPOINTSET->ymax;
		}
		if (x_scale != 1.0 || y_scale != 1.0)
		{
			ScalePointset(m_pPOINTSET, x_scale, y_scale);
			//2022may16, spi, begin
			/*
			BuildTriangleNetwork(m_pPOINTSET);
			ComputeAllTriangleCenters(m_pPOINTSET);
			*/
			BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
			//2022may16, spi, end
		}
	}
	/*
	if (pPOINTSET->px[i] < pPOINTSET->xmin) pPOINTSET->xmin = pPOINTSET->px[i];
	if (pPOINTSET->px[i] > pPOINTSET->xmax) pPOINTSET->xmax = pPOINTSET->px[i];
	if (pPOINTSET->py[i] < pPOINTSET->ymin) pPOINTSET->ymin = pPOINTSET->py[i];
	if (pPOINTSET->py[i] > pPOINTSET->ymax) pPOINTSET->ymax = pPOINTSET->py[i];

	for (int idpoint = vorogui_numberofframepoints; idpoint < pPOINTSET->npts; idpoint++)
	{
	}
	*/
	//2021nov29, spi, end



	//////////////////////
	//draw voronoi regions
	//////////////////////
	const int maxnumpointvoronoipolygon = 200;
	POINT pPointVoronoiPolygon[maxnumpointvoronoipolygon];
	int numpointvoronoipolygon = 0;

	int idpoint_start = vorogui_numberofframepoints;
	int idpoint_end = m_pPOINTSET->npts;
	if( (!bforcefullredraw) && (idpoint != POINTSET_ALL) && (idpoint != POINTSET_NONE) && (idpoint>= idpoint_start) && (idpoint< idpoint_end))
	{
		idpoint_start = idpoint;
		idpoint_end = idpoint+1;
	}
	for (int idpoint_i = idpoint_start; idpoint_i < idpoint_end; idpoint_i++)
	{
		double dfValue = m_pPOINTSET->controlratio[idpoint_i];
		//double dfValue = 1.0;
		if (dfValue >= 0.0 && dfValue <= 1.0) //if (dfValue>=0.0)
		{
			//redraw each voronoi polygon but sliced in "y" according to the control level
			if (getvoronoipolygon(idpoint_i, pPointVoronoiPolygon, &numpointvoronoipolygon, maxnumpointvoronoipolygon) == TRUE)
			{
				//2021nov29, spi, begin
				//redraw background image before redrawing polygon
				//find polygon's bounding box
				long bb_xmin, bb_xmax, bb_ymin, bb_ymax;
				int idpointymin;
				getpolygonminmax(pPointVoronoiPolygon, numpointvoronoipolygon, &bb_xmin, &bb_xmax, &bb_ymin, &bb_ymax, &idpointymin);
				HRGN myHRGN = NULL;
				HRGN prevHRGN = NULL;
				if ( dib && (idpoint != POINTSET_NONE) )
				{
					myHRGN = CreatePolygonRgn(pPointVoronoiPolygon, numpointvoronoipolygon, ALTERNATE); // WINDING);
					if (myHRGN)
					{
						/*
						int res = GetClipRgn(hdc, prevHRGN);						
						if (res == 1) prevHRGN = hrgn;
						*/
						int res = SelectClipRgn(hdc, myHRGN);
						int imagex = bb_xmin;
						int imagey = bb_ymin;
						int imagewidth = bb_xmax-bb_xmin;//rcClient.right - 0;
						int imageheight = bb_ymax-bb_ymin;//rcClient.bottom - 0;
						SetStretchBltMode(hdc, COLORONCOLOR);
						if (dib)
						{
							//2022nov18, spi, begin
							//int inv_prob = 1; //2022dec25, spi, back to 1
							int inv_prob = 3; //1 for 100% original code, 2 for 50%, 3 for 33%
							//int inv_prob = 2; //1 for 100% original code, 2 for 50%, 3 for 33%
							int randomnum = RandomInt(1, inv_prob);
							//if (0) //original
							//if (1) //new code
							if(randomnum==1)
							//2022nov18, spi, end
							{
								/**/
								//2022nov18, new, copy only a small portion of the whole image into the cell
								int width_dib = FreeImage_GetWidth(dib);
								int height_dib = FreeImage_GetHeight(dib);
								int xoffset_max = width_dib - imagewidth;
								int yoffset_max = height_dib - imageheight;
								if ((xoffset_max > 1) && (yoffset_max > 1) && FreeImage_GetBPP(dib) == 24)
								{
									//src
									BYTE* pBYTE = FreeImage_GetBits(dib);;
									int i = RandomInt(0, xoffset_max);
									int j = RandomInt(0, yoffset_max);

									//BYTE bgr = pBYTE[j*width_dib * 3 + i * 3]; //B
									//BYTE b = pBYTE[j*width_dib * 3 + i * 3]; //B
									//BYTE g = pBYTE[j*width_dib * 3 + i * 3 + 1]; //G
									//BYTE r = pBYTE[j*width_dib * 3 + i * 3 + 2]; //R

									if (0)
									{
										/* //not functional yet
										//create bitmap info header for this cell
										int cellwidth = imagewidth;
										int cellheight = imageheight;
										BITMAPINFO myBITMAPINFO;
										myBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
										myBITMAPINFO.bmiHeader.biWidth = cellwidth;
										myBITMAPINFO.bmiHeader.biHeight = cellheight;
										myBITMAPINFO.bmiHeader.biSizeImage = cellwidth * cellheight * 3;
										myBITMAPINFO.bmiHeader.biPlanes = 1;
										myBITMAPINFO.bmiHeader.biCompression = BI_RGB;
										myBITMAPINFO.bmiHeader.biBitCount = 3 * 8; // 24;
										myBITMAPINFO.bmiHeader.biClrUsed = 0;
										myBITMAPINFO.bmiHeader.biClrImportant = 0;
										myBITMAPINFO.bmiHeader.biXPelsPerMeter = 0;
										myBITMAPINFO.bmiHeader.biYPelsPerMeter = 0;
										//
										//&(myBITMAPINFO.bmiColors[0]) = NULL;
										//myBITMAPINFO.bmiColors[0] = RGB(0,0,0);
										//myBITMAPINFO.bmiColors[0] = &(pBYTE[j*width_dib * 3 + i * 3]);
										StretchDIBits(hdc, imagex, imagey, imagewidth, imageheight, 0, 0, width_dib, cellheight,
											(const void*)&(pBYTE[j*width_dib * 3 + i * 3]), (BITMAPINFO*)&myBITMAPINFO, DIB_RGB_COLORS, SRCCOPY);
										*/
									}
									else
									{
										//StretchDIBits(hdc, imagex, imagey, imagewidth, imageheight,
										StretchDIBits(hdc, imagex, imagey, imagewidth, imageheight,
											//0, 0, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
											i, j, imagewidth, imageheight,
											FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS, SRCCOPY);
									}

								}
								else
								{
									//original, stretch the whole image into the cell
									StretchDIBits(hdc, imagex, imagey, imagewidth, imageheight,
										0, 0, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
										FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS, SRCCOPY);
								}
							}
							/**/
							else
							{
								//original, stretch the whole image into the cell
								StretchDIBits(hdc, imagex, imagey, imagewidth, imageheight,
									0, 0, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
									FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS, SRCCOPY);
							}
						}
					}
				}
				//2021nov29, spi, end
				if (bVoroPolygons && (idpoint != POINTSET_NONE))
				{
					HPEN hpreviouspen = (HPEN)SelectObject(hdc, hpen);
					if (dfValue == 1.0 && bWithControls)
					{
						//////////////////////////////////////////////
						//redraw the voronoi polygon completely filled
						//////////////////////////////////////////////
						//2022nov18, spi, begin
						if (vorogui_display_controls_polygons)
						{
							HBRUSH hpreviousbrush = (HBRUSH)SelectObject(hdc, hbrush); // hatchhbrush);
							Polygon(hdc, pPointVoronoiPolygon, numpointvoronoipolygon); //+1 for the last point 
							SelectObject(hdc, hpreviousbrush);
						}
						//2022nov18, spi, end
					}
					else if (dfValue == 0.0 && bWithControls)
					{
						/////////////////////////////////////////////
						//redraw the voronoi polygon completely empty
						/////////////////////////////////////////////
						//2022nov18, spi, begin
						if (vorogui_display_controls_polygons)
						{
							HBRUSH hpreviousbrush = (HBRUSH)SelectObject(hdc, nullhbrush);
							Polygon(hdc, pPointVoronoiPolygon, numpointvoronoipolygon); //+1 for the last point 
							SelectObject(hdc, hpreviousbrush);
						}
						//2022nov18, spi, end
					}
					else
					{
						/////////////////////////////////////////////
						//redraw the voronoi polygon completely empty
						/////////////////////////////////////////////
						//2022nov18, spi, begin
						if (vorogui_display_controls_polygons)
						{
							HBRUSH hpreviousbrush = (HBRUSH)SelectObject(hdc, nullhbrush);
							Polygon(hdc, pPointVoronoiPolygon, numpointvoronoipolygon); //+1 for the last point 
							SelectObject(hdc, hpreviousbrush);
						}
						//2022nov18, spi, end
						if (bWithControls)
						{
							///////////////////////////////////////
							//floodfill part of the voronoi polygon
							///////////////////////////////////////
							POINT pPointOutputPolygon[200];
							int numpointoutputpolygon;
							slicepolygon(pPointVoronoiPolygon, numpointvoronoipolygon, dfValue, pPointOutputPolygon, &numpointoutputpolygon);
							//for now, draw the intersection line
							if (numpointoutputpolygon == 2)
							{
								//2022nov15, spi, begin
								if (vorogui_display_controls_levels) //0 will not draw the control line level
								{
									MoveToEx(hdc, pPointOutputPolygon[0].x, pPointOutputPolygon[0].y, NULL);
									LineTo(hdc, pPointOutputPolygon[1].x, pPointOutputPolygon[1].y);
								}
								//2022nov15, spi, end

								//2022nov15, spi, begin
								/*
								//int iOldMixMode = SetROP2(hdc, R2_NOT); //test
								
								POINT myPOINT;
								myPOINT.x = (pPointOutputPolygon[0].x + pPointOutputPolygon[1].x) / 2;
								myPOINT.y = ((pPointOutputPolygon[0].y + pPointOutputPolygon[1].y) / 2) + 1; //one pixels below //+1; //one pixel below the intersection line's center
								HBRUSH hpreviousbrush = (HBRUSH)SelectObject(hdc, hbrush); //hatchhbrush);
								FloodFill(hdc, myPOINT.x, myPOINT.y, RGB(255, 0, 0));
								SelectObject(hdc, hpreviousbrush);
								
								//if(iOldMixMode) SetROP2(hdc, iOldMixMode); //test
								*/
								//2022nov15, spi, end
							}
							else if(numpointoutputpolygon > 2)
							{
								//2022nov15, spi, begin
								/*
								////////////////////////////////
								//draw the sliced polygon filled
								////////////////////////////////
								HBRUSH hpreviousbrush = (HBRUSH)SelectObject(hdc, hbrush); //hatchhbrush);
								Polygon(hdc, pPointVoronoiPolygon, numpointvoronoipolygon); //+1 for the last point 
								SelectObject(hdc, hpreviousbrush);
								*/
								//2022nov15, spi, end
							}
						}
					}
					SelectObject(hdc, hpreviouspen);
				}
				if (bWithControlLabels)
				{
					if (vorogui_displaytextlabelflag == true)
					{
						//display parameter name
						/*
						string mystring = params[idpoint - vorogui_numberofframepoints].getName();
						*/
						string mystring = "";
						float min = 0.0;
						float max = 0.0;
						float value = 0.0;
						getparameter(idpoint_i - vorogui_numberofframepoints, min, max, value, mystring);
						//2021nov29, spi, begin
						if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
						{
							string mytemp = mystring;
							mystring = to_string(idpoint_i);
							if (global_debugmode & VOROGUI_DEBUG_CONTROLS)
							{
								mystring += ", " + mytemp;
							}
						}
						SIZE mysinglecharextentSIZE = {};
						SIZE mystringextentSIZE = {};
						bool bresult = GetTextExtentPoint32A(hdc, "A", 1, &mysinglecharextentSIZE);
						if (bresult)
						{
							bresult = GetTextExtentPoint32A(hdc, mystring.c_str(), mystring.length(), &mystringextentSIZE);
							if (bresult)
							{
								int maxchar_x = (bb_xmax - bb_xmin) / mysinglecharextentSIZE.cx;
								int maxline_y = (bb_ymax - bb_ymin) / mystringextentSIZE.cy;
								if (maxchar_x > 1 && maxline_y > 1 && mystring.length() > maxchar_x)
								{
									long center_x = (bb_xmin + bb_xmax) / 2;
									long center_y = (bb_ymin + bb_ymax) / 2;
									//break string into as many chunks as required
									vector<string> mystringvector;
									size_t pos = 0;
									string s;
									while ((s = mystring.substr(pos, maxchar_x - 1)).length())
									{
										mystringvector.push_back(s);
										pos += s.length();
									}
									long y = center_y - mystringvector.size()*mystringextentSIZE.cy / 2;
									UINT prev_align = SetTextAlign(hdc, TA_CENTER);
									for (int line = 0; ((line < mystringvector.size()) && (line < maxline_y)); line++)
									{
										string mysubstring = mystringvector[line];
										//TextOutA(hdc, pPOINTSET->px[idpoint], pPOINTSET->py[idpoint], mysubstring.c_str(), mysubstring.length());
										TextOutA(hdc, center_x, y + line * mystringextentSIZE.cy, mysubstring.c_str(), mysubstring.length());
									}
									if (prev_align != GDI_ERROR) SetTextAlign(hdc, prev_align);
								}
								else
								{
									bresult = false;
								}
							}
						}
						if (!bresult)
						{
							TextOutA(hdc, m_pPOINTSET->px[idpoint_i], m_pPOINTSET->py[idpoint_i], mystring.c_str(), mystring.length());
						}
						//2021nov29, spi, end
					}
				}
				//2021nov29, begin
				if (myHRGN && dib && (idpoint != POINTSET_NONE))
				{
					int res = SelectClipRgn(hdc, prevHRGN);
					DeleteObject(myHRGN);
				}
				//2021nov29, end
			}
			else
			{
				/*
				fprintf(pFile, "vertex=%d, numneighbor=%d,  ",ivertex,numneighborfound);
				for(j=0; j<numneighborfound; j++)
				{
				fprintf(pFile, " %d", p_arrayneighbor[j]);
				}
				fprintf(pFile, "\n");
				*/
			}
		}
	}
	SelectObject(hdc, oldhpen);
	SelectObject(hdc, oldhbrush);
	if(hpen) DeleteObject(hpen);
	if(hbrush) DeleteObject(hbrush);
	if(hatchhbrush) DeleteObject(hatchhbrush);
	return true;
}

//2022may18, spi, begin
//major revision to this function
//major vorogui bug was to have getnearestpointsetobject() return directly result from FindNearestNeighbor(m_pPOINTSET, dfX, dfY, p_itriseed),
//which was always returning "the nearest neighboring vertex" of the triangle in which point (dfX,dfY) resides".
//the problem was that the nearest pointset object can also often be a vextex of one of the adjacent triangles.
int spivorogui::getnearestpointsetobject(double dfX, double dfY, int* p_itriseed/*=NULL*/)
{
	int idObject = -1;
	if (dfX<(double)0.0 || dfX>(double)vorogui_xwidth ||
		dfY<(double)0.0 || dfY>(double)vorogui_yheight)
	{
		return idObject;
	}
	int iTriSeed = 0;
	if (p_itriseed == NULL) p_itriseed = &iTriSeed;
	//2022may18, spi, begin
	//idObject = FindNearestNeighbor(m_pPOINTSET, dfX, dfY, p_itriseed);
	idObject = FindNearestNeighbor(m_pPOINTSET, dfX, dfY, p_itriseed, POINTSET_ADJACENTTRIFLAG_FIRSTADJTRI);
	//2022may18, spi, end
	if (idObject==-1)
	{
	}
	else if (idObject==-2)
	{
	}
	else if (idObject == -99)
	{
	}
	return idObject;
}
//2022may18, spi, end

//2022july25, spi, begin
int spivorogui::getnearestpointsetobject_getparameter(double dfX, double dfY, int* p_itriseed, double* p_dfRatio, wstring* p_mywstring, RECT* p_myRECT)
{
	if (p_dfRatio == NULL)
	{
		return -1;
	}
	if (p_mywstring == NULL)
	{
		return -1;
	}
	if (p_myRECT == NULL)
	{
		return -1;
	}

	////////////////////////////////
	//read-only, not changing values 
	////////////////////////////////
	//get nearest pointset object 
	int idpoint = getnearestpointsetobject(dfX, dfY, p_itriseed); 
	if (idpoint >= vorogui_numberofframepoints)
	{
		if ((idpoint != -1) && (vorogui_idpointrecentlydisplayed == -1) && (idpoint != vorogui_idpointrecentlydisplayed) && (idpoint != vorogui_idpointtobedisplayed))
		{
			vorogui_idpointtobedisplayed = idpoint;
		}
		if (vorogui_idpointtobedisplayed != -1)
		{
			const int maxnumpointvoronoipolygon = 200;
			POINT pPointVoronoiPolygon[maxnumpointvoronoipolygon];
			int numpointvoronoipolygon = 0;
			if (getvoronoipolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon, maxnumpointvoronoipolygon) == TRUE)
			{
				long xmin, xmax, ymin, ymax;
				int idpointymin;
				getpolygonminmax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
				//double dfRatio = 0.0;
				if (ymax != ymin)
				{
					*p_dfRatio = (abs(ymax - dfY)*1.0) / (abs(ymax - ymin)*1.0);
				}
				//read-only, not changing values 
				/*
				if (dfRatio >= 0.0 && dfRatio <= 1.0)
				{
					m_pPOINTSET->controlratio[idpoint] = dfRatio;
				}
				else
				{
					//ASSERT(FALSE);
					if (dfRatio > 1.0) dfRatio = 1.0;
					if (dfRatio < 0.0) dfRatio = 0.0;
					m_pPOINTSET->controlratio[idpoint] = dfRatio;
				}
				setparameter(idpoint - vorogui_numberofframepoints, dfRatio);
				*/
				string name = "";
				float min = 0.0;
				float max = 0.0;
				float value = 0.0;
				getparameter(idpoint - vorogui_numberofframepoints, min, max, value, name);
				//set text to label value
				//wstring mywstring = s2ws(name);
				*p_mywstring = s2ws(name);
				//if debuging, set text to numerical value
				if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
				{
					if ((name.find("_mode") != std::string::npos)
						|| (name.find("_nx") != std::string::npos)
						|| (name.find("_ny") != std::string::npos)
						|| (name.find("_showwnd") != std::string::npos)
						|| (name.find("_bgcolor") != std::string::npos)
						|| (name.find("_idcolor") != std::string::npos))
					{
						//found!
						//these are integer values and need to be rounded in order to snap to the closest int
						if (value > 0.75)
						{
							value += 0.5f; //round it up
							if (value > max) value = max; //cap it
						}
					}
					float fractpart, intpart;
					fractpart = modf(value, &intpart);
					int value_int = (int)intpart; //signed int
					int value_frac = (int)((fractpart) * 10); //first decimal only
					//wstring mywstring = std::to_wstring(value_int);
					*p_mywstring = std::to_wstring(value_int);
					if ((value_frac > 0) && (max - min) <= 1.0)
					{
						*p_mywstring += L".";
						*p_mywstring += std::to_wstring(value_frac);
					}
					if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
					{
						wstring mytemp = *p_mywstring;
						*p_mywstring = to_wstring(idpoint);
						//+L", " + mywstring;
						if (global_debugmode & VOROGUI_DEBUG_CONTROLS)
						{
							*p_mywstring += L", " + mytemp;
						}

					}
				}
				//RECT myRECT = { xmin, ymin, xmax, ymax }; //in voroguiwindow client coordinates
				*p_myRECT = { xmin, ymin, xmax, ymax }; //in voroguiwindow client coordinates
			}
		}
	}
	return idpoint;
}

//write, changing vorogui control level
int spivorogui::getnearestpointsetobject_setparameter(double dfX, double dfY, int* p_itriseed, double* p_dfRatio, wstring* p_mywstring, RECT* p_myRECT)
{
	if (p_dfRatio == NULL)
	{
		return -1;
	}
	if (p_mywstring == NULL)
	{
		return -1;
	}
	if (p_myRECT == NULL)
	{
		return -1;
	}

	////////////////////////
	//write, changing values 
	////////////////////////
	//get nearest pointset object 
	int idpoint = getnearestpointsetobject(dfX, dfY, p_itriseed);
	if (idpoint >= vorogui_numberofframepoints)
	{
		if ((idpoint != -1) && (vorogui_idpointrecentlydisplayed == -1) && (idpoint != vorogui_idpointrecentlydisplayed) && (idpoint != vorogui_idpointtobedisplayed))
		{
			vorogui_idpointtobedisplayed = idpoint;
		}
		if (vorogui_idpointtobedisplayed != -1)
		{
			const int maxnumpointvoronoipolygon = 200;
			POINT pPointVoronoiPolygon[maxnumpointvoronoipolygon];
			int numpointvoronoipolygon = 0;
			if (getvoronoipolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon, maxnumpointvoronoipolygon) == TRUE)
			{
				long xmin, xmax, ymin, ymax;
				int idpointymin;
				getpolygonminmax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
				/* //read-only code
				if (ymax != ymin)
				{
					*p_dfRatio = (abs(ymax - dfY)*1.0) / (abs(ymax - ymin)*1.0);
				}
				*/
				/*
				//double dfRatio = 0.0;
				*/
				//write code, for changing values 
				if (*p_dfRatio >= 0.0 && *p_dfRatio <= 1.0)
				{
					m_pPOINTSET->controlratio[idpoint] = *p_dfRatio;
				}
				else
				{
					//ASSERT(FALSE);
					if (*p_dfRatio > 1.0) *p_dfRatio = 1.0;
					if (*p_dfRatio < 0.0) *p_dfRatio = 0.0;
					m_pPOINTSET->controlratio[idpoint] = *p_dfRatio;
				}
				setparameter(idpoint - vorogui_numberofframepoints, *p_dfRatio);
				//
				string name = "";
				float min = 0.0;
				float max = 0.0;
				float value = 0.0;
				getparameter(idpoint - vorogui_numberofframepoints, min, max, value, name);
				//set text to label value
				//wstring mywstring = s2ws(name);
				*p_mywstring = s2ws(name);
				//if debuging, set text to numerical value
				if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
				{
					if ((name.find("_mode") != std::string::npos)
						|| (name.find("_nx") != std::string::npos)
						|| (name.find("_ny") != std::string::npos)
						|| (name.find("_showwnd") != std::string::npos)
						|| (name.find("_bgcolor") != std::string::npos)
						|| (name.find("_idcolor") != std::string::npos))
					{
						//found!
						//these are integer values and need to be rounded in order to snap to the closest int
						if (value > 0.75)
						{
							value += 0.5f; //round it up
							if (value > max) value = max; //cap it
						}
					}
					float fractpart, intpart;
					fractpart = modf(value, &intpart);
					int value_int = (int)intpart; //signed int
					int value_frac = (int)((fractpart) * 10); //first decimal only
					//wstring mywstring = std::to_wstring(value_int);
					*p_mywstring = std::to_wstring(value_int);
					if ((value_frac > 0) && (max - min) <= 1.0)
					{
						*p_mywstring += L".";
						*p_mywstring += std::to_wstring(value_frac);
					}
					if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
					{
						wstring mytemp = *p_mywstring;
						*p_mywstring = to_wstring(idpoint);
						//+L", " + mywstring;
						if (global_debugmode & VOROGUI_DEBUG_CONTROLS)
						{
							*p_mywstring += L", " + mytemp;
						}

					}
				}
				//RECT myRECT = { xmin, ymin, xmax, ymax }; //in voroguiwindow client coordinates
				*p_myRECT = { xmin, ymin, xmax, ymax }; //in voroguiwindow client coordinates
			}
		}
	}
	return idpoint;
}

bool spivorogui::onmousemove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if ( (wParam&MK_SHIFT) || (wParam&MK_CONTROL) )
	{
		//if either shift key AND/OR control key down
		//abort, no need to further process
		return 0; //returning 0, as specified by windows when msg has beeen processed
	}

	//0.1) take position and compute displacement between mousemove
	int mousepointerdisplacement_threshold = 25; //squared of the mouse pointer position variation between two mousemoves
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	int deltaPos = 0; //square of the delta x and y position variation between mousemove
	if ( !((xPos == prev_xPos) && (yPos == prev_yPos)) )
	{
		if ((prev_xPos != -1) && (prev_yPos != -1))
		{
			int dxPos = (xPos - prev_xPos);
			int dyPos = (yPos - prev_yPos);
			deltaPos = dxPos * dxPos + dyPos * dyPos; //not taking the sqrt() to optime computation time
		}
		else
		{
			//when either or both prev_xPos prev_yPos are -1
			//set deltaPos to something ... even if arbitrary
			deltaPos = mousepointerdisplacement_threshold;
		}
	}
	prev_xPos = xPos;
	prev_yPos = yPos;

	//0.2) take time stamp and compute elapsed time between mousemove
	DWORD nowstamp_ms = GetTickCount();
	DWORD elapsedtime_ms = 0;
	if (prev_mousemove_nowstamp_ms!=0)
	{
		elapsedtime_ms = nowstamp_ms - prev_mousemove_nowstamp_ms;
	}
	prev_mousemove_nowstamp_ms = nowstamp_ms;

	//0.3) pre-filter grossly on mouse pointer displacement variation and elapsed time between consecutive mousemoves
	if (deltaPos >= mousepointerdisplacement_threshold || elapsedtime_ms == 0)
	{
		//abort if displacement to great OR elapsed time is 0
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "spivorogui::onmousemove() aborting ..." << endl;
			ss << "deltaPos >= mousepointerdisplacement_threshold" << endl;
			ss << "deltaPos = " << deltaPos << endl;
			ss << "mousepointerdisplacement_threshold = " << mousepointerdisplacement_threshold << endl;
			ss << "AND/OR" << endl;
			ss << "elapsedtime_ms == 0" << endl;
			ss << "elapsedtime_ms = " << elapsedtime_ms << endl;
			ss << endl;
			p_spilogfile->writeline(ss.str());
		}
		return 0; //returning 0, as specified by windows when msg has beeen processed
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//when mouse pointer stops moving, for long enough, do pop up a spitext but only once per mouse pointer immobilization
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float fmousepointerspeed_threshold = 25.0f; //=5.0f; //in square pixels per millisecond
	DWORD elapsedtime_threshold_ms = 1000;
	//1) between these mouse pointer immobilizations, compute mouse pointer speed, the displacement over the time elapsed
	float fmousepointerspeed = (deltaPos*1.0f)/(elapsedtime_ms*1.0f); //in square pixels per millisecond
	//2) if mouse pointer speed is below the threshold (that is when mouse pointer does not move or moves very slow)
	if (fmousepointerspeed < fmousepointerspeed_threshold)
	{
		//start counting elapsed time
		//if ((prev_fmousepointerspeed!=FLT_MAX) && (prev_fmousepointerspeed > fmousepointerspeed_threshold))
		if ((prev_fmousepointerspeed > fmousepointerspeed_threshold))
		{
			vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = 0;
			vorogui_mousepointerspeedbelowthreshold_starttime_ms = nowstamp_ms;
		}
		else
		{
			vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = nowstamp_ms - vorogui_mousepointerspeedbelowthreshold_starttime_ms;
		}
	}
	else
	{
		if (prev_fmousepointerspeed < fmousepointerspeed_threshold)
		{
			//todo ?
		}
	}
	prev_fmousepointerspeed = fmousepointerspeed;

	if (vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms < elapsedtime_threshold_ms)
	{
		//not enough time elapsed for poping a spitext
		//abort, no need to further process
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "spivorogui::onmousemove() aborting ..." << endl;
			ss << "vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms < elapsedtime_threshold_ms" << endl;
			ss << "vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = " << vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms << endl;
			ss << "elapsedtime_threshold_ms = " << elapsedtime_threshold_ms << endl;
			ss << endl;
			p_spilogfile->writeline(ss.str());
		}
		return 0; //returning 0, as specified by windows when msg has beeen processed
	}
	
	//3) query which idpoint label information should be displayed (when SPITEXT enabled)
	//vorogui_idpointtobedisplayed=-1; //when nothing new, that is when point is the same as before therefore no new point label to be displayed
	//vorogui_idpointrecentlydisplayed=-1; //when no point label is recently displayed
	double dfRatio = 0.0;
	wstring mywstring;
	RECT myRECT;
	vorogui_idpointtobedisplayed = -1;
	vorogui_idpointtobedisplayed = getnearestpointsetobject_getparameter(xPos + 0.0, yPos + 0.0, &(vorogui_itriseed), &dfRatio, &mywstring, &myRECT);
	if (vorogui_idpointtobedisplayed < vorogui_numberofframepoints)
	{
		//abort, no need to further process
		return 0; //returning 0, as specified by windows when msg has beeen processed
	}
	if( vorogui_idpointtobedisplayed==vorogui_idpointrecentlydisplayed )
	{
		//reset tracking variables
		vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = 0;
		vorogui_mousepointerspeedbelowthreshold_starttime_ms = 0;
		prev_fmousepointerspeed = FLT_MAX;
		vorogui_idpointrecentlydisplayed = -1; //-1 when no point label is recently displayed
		vorogui_idpointtobedisplayed = -1; //-1 when nothing new, that is when point is the same as before therefore no new point label to be displayed
		//vorogui_idpointrecentlydisplayed_timestamp_ms = nowstamp_ms;

		//abort, no need to further process
		if (p_spilogfile && global_debugmode)
		{
			stringstream ss;
			ss << "spivorogui::onmousemove() aborting ..." << endl;
			ss << "vorogui_idpointtobedisplayed == vorogui_idpointrecentlydisplayed" << endl;
			ss << "vorogui_idpointtobedisplayed = " << vorogui_idpointtobedisplayed << endl;
			ss << "vorogui_idpointrecentlydisplayed = " << vorogui_idpointrecentlydisplayed << endl;
			ss << endl;
			p_spilogfile->writeline(ss.str());
		}

		return 0; //returning 0, as specified by windows when msg has beeen processed
	}
	if(1)
	{
		//if ((nowstamp_ms - vorogui_idpointrecentlydisplayed_timestamp_ms) < (2 * elapsedtime_threshold_ms))
		//{
			//abort, no need to further process
			//return 0; //returning 0, as specified by windows when msg has beeen processed
		//}
		//else
		//{
			//enough time has elapsed since last spitext display
			//reset these variable to allow spitext display to occur
			vorogui_idpointrecentlydisplayed = -1;
			vorogui_idpointrecentlydisplayed_timestamp_ms = 0;
			//return 0; //returning 0, as specified by windows when msg has beeen processed
		//}
	}
#ifdef _WIN64
	//for 64bit, GetWindowLong() call to be replaced by GetWindowLongPtr()
	HINSTANCE hinstance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
#else
	HINSTANCE hinstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
#endif
	launchspitextwindow(mywstring.c_str(), myRECT, hwnd, hinstance); //hwnd is voroguiwindow's hwnd
	//reset tracking variables
	vorogui_mousepointerspeedbelowthreshold_elapsedtime_ms = 0;
	vorogui_mousepointerspeedbelowthreshold_starttime_ms = 0;
	prev_fmousepointerspeed = FLT_MAX;
	vorogui_idpointrecentlydisplayed = vorogui_idpointtobedisplayed; //was -1 //-1 when no point label is recently displayed
	vorogui_idpointtobedisplayed = -1; //was vorogui_idpointtobedisplayed //-1 when nothing new, that is when point is the same as before therefore no new point label to be displayed
	vorogui_idpointrecentlydisplayed_timestamp_ms = nowstamp_ms;

	//read-only, not changing values
	/*
	vorogui_idpointdirty = idpoint;
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
	*/
	return 0; //returning 0, as specified by windows when msg has beeen processed
}
//2022july25, spi, end

bool spivorogui::onlbuttondown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (!(wParam&MK_SHIFT) && !(wParam&MK_CONTROL))
	{
		//////////////////////
		//change control level
		//////////////////////
		//get nearest pointset object 
		int idpoint = getnearestpointsetobject(xPos+0.0, yPos+0.0, &(vorogui_itriseed)); //+0.0 to ensure proper conversion to double
		if (idpoint >= vorogui_numberofframepoints)
		{
			/*
			double dfValue=0.50;
			*/
			const int maxnumpointvoronoipolygon = 200;
			POINT pPointVoronoiPolygon[maxnumpointvoronoipolygon];
			int numpointvoronoipolygon = 0;
			if (getvoronoipolygon(idpoint, pPointVoronoiPolygon, &numpointvoronoipolygon, maxnumpointvoronoipolygon) == TRUE)
			{

				long xmin, xmax, ymin, ymax;
				int idpointymin;
				getpolygonminmax(pPointVoronoiPolygon, numpointvoronoipolygon, &xmin, &xmax, &ymin, &ymax, &idpointymin);
				double dfRatio = 0.0;
				if (ymax != ymin) dfRatio = abs(ymax - yPos)*1.0 / abs(ymax - ymin);

				if (dfRatio >= 0.0 && dfRatio <= 1.0)
				{
					m_pPOINTSET->controlratio[idpoint] = dfRatio;
				}
				else
				{
					//ASSERT(FALSE);
					if (dfRatio>1.0) dfRatio = 1.0;
					if (dfRatio<0.0) dfRatio = 0.0;
					m_pPOINTSET->controlratio[idpoint] = dfRatio;
				}
				/*
				vector<ControlParameter> params = global_pSynth->getParameters();
				TonicFloat min = params[idpoint - vorogui_numberofframepoints].getMin();
				TonicFloat max = params[idpoint - vorogui_numberofframepoints].getMax();
				string mystring = params[idpoint - vorogui_numberofframepoints].getName();
				global_pSynth->setParameter(mystring, dfRatio*(max - min) + min);
				*/
				setparameter(idpoint - vorogui_numberofframepoints, dfRatio);
				//2021dec29, spi, begin
				/*
				CSPITextWindow* pSPITextWindow = get_voroguispitextwindow_vector();
				if (pSPITextWindow)
				{
					//pSPITextWindow->SetCaption(mywstring.c_str());
					RECT myRECT = {xmin, ymin, xmax, ymax };
					SetWindowPos(pSPITextWindow->GetHWND(), HWND_TOP, myRECT.left, myRECT.top, abs(myRECT.right - myRECT.left), abs(myRECT.bottom - myRECT.top), SWP_SHOWWINDOW | SWP_NOACTIVATE);
				}
				*/
				string name = "";
				float min = 0.0;
				float max = 0.0;
				float value = 0.0;
				getparameter(idpoint - vorogui_numberofframepoints, min, max, value, name);
				//WCHAR pwchar[1024];
				//_itow_s(i, pwchar, sizeof(pwchar) / sizeof(WCHAR), 10); //base 10 for decimal
				
				if ( (name.find("_mode") != std::string::npos) 
					|| (name.find("_nx") != std::string::npos) 
					|| (name.find("_ny") != std::string::npos)
					|| (name.find("_showwnd") != std::string::npos)
					|| (name.find("_bgcolor") != std::string::npos)
					|| (name.find("_idcolor") != std::string::npos) )
				{
					//found!
					//these are integer values and need to be rounded in order to snap to the closest int
					if (value > 0.75)
					{
						value += 0.5f; //round it up
						if (value > max) value = max; //cap it
					}
				}
				float fractpart, intpart;
				fractpart = modf(value, &intpart);
				int value_int = (int)intpart; //signed int
				int value_frac = (int)((fractpart) * 10); //first decimal only
				wstring mywstring = std::to_wstring(value_int); 
				if ( (value_frac>0) && (max-min)<=1.0)
				{
					mywstring += L".";
					mywstring += std::to_wstring(value_frac);
				}
				if (global_debugmode & VOROGUI_DEBUG_VORONOIDIAGRAM)
				{
					wstring mytemp = mywstring;
					mywstring = to_wstring(idpoint);
					//+L", " + mywstring;
					if (global_debugmode & VOROGUI_DEBUG_CONTROLS)
					{
						mywstring += L", " + mytemp;
					}

				}
				RECT myRECT = { xmin, ymin, xmax, ymax }; //in voroguiwindow client coordinates
#ifdef _WIN64
				//for 64bit, GetWindowLong() call to be replaced by GetWindowLongPtr()
				HINSTANCE hinstance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
#else
				HINSTANCE hinstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
#endif
				launchspitextwindow(mywstring.c_str(), myRECT, hwnd, hinstance); //hwnd is voroguiwindow's hwnd
				//2021dec29, spi, end
			}
			vorogui_idpointdirty = idpoint;
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
		}
	}
	else if (wParam&MK_CONTROL)
	{
		/////////////////////////
		//store point to be moved
		/////////////////////////
		//get nearest pointset object
		int idpoint = getnearestpointsetobject(xPos+0.0, yPos+0.0, &(vorogui_itriseed));
		if (idpoint >= vorogui_numberofframepoints)
		{
			vorogui_idpointtobemoved = idpoint;
		}
	}
	//2022july25, spi, begin
	/*
	return true;
	*/
	return 0; //returning 0, as specified by windows when msg has beeen processed
	//2022july25, spi, end
}

bool spivorogui::onlbuttonup(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (wParam&MK_CONTROL)
	{
		if (vorogui_idpointtobemoved != -1)
		{
			if (vorogui_idpointtobemoved >= vorogui_numberofframepoints)
			{
				//////////////////////////////////////////
				//check for collision with existing points
				//////////////////////////////////////////
				for (int i = 0; i < m_pPOINTSET->npts; i++)
				{
					if (abs(m_pPOINTSET->px[i] - xPos) < 1.0)
					{
						if (abs(m_pPOINTSET->py[i] - yPos) < 1.0)
						{
							//found collision
							//drop moving request
							vorogui_idpointtobemoved = -1;
							return false;
						}

					}
				}
				////////////////////
				//safe to move point
				////////////////////
				m_pPOINTSET->px[vorogui_idpointtobemoved] = xPos;
				m_pPOINTSET->py[vorogui_idpointtobemoved] = yPos;
				vorogui_idpointtobemoved = -1;
				///////////////////////////////////////////////
				//update pointset's extent, tin and tri centers
				///////////////////////////////////////////////
				m_pPOINTSET->ntri = 0;
				m_pPOINTSET->xmin = MAXDBL;
				m_pPOINTSET->ymin = MAXDBL;
				m_pPOINTSET->xmax = MINDBL;
				m_pPOINTSET->ymax = MINDBL;
				for (int i = 0; i < m_pPOINTSET->npts; i++)
				{
					if (m_pPOINTSET->px[i] < m_pPOINTSET->xmin) m_pPOINTSET->xmin = m_pPOINTSET->px[i];
					if (m_pPOINTSET->px[i] > m_pPOINTSET->xmax) m_pPOINTSET->xmax = m_pPOINTSET->px[i];
					if (m_pPOINTSET->py[i] < m_pPOINTSET->ymin) m_pPOINTSET->ymin = m_pPOINTSET->py[i];
					if (m_pPOINTSET->py[i] > m_pPOINTSET->ymax) m_pPOINTSET->ymax = m_pPOINTSET->py[i];
				}
				//2022may16, spi, begin
				/*
				BuildTriangleNetwork(m_pPOINTSET);
				ComputeAllTriangleCenters(m_pPOINTSET);
				*/
				BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
				//2022may16, spi, end

				vorogui_idpointdirty = POINTSET_ALL;
				RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
			}
		}
	}
	//2022july25, spi, begin
	/*
	return true;
	*/
	return 0; //returning 0, as specified by windows when msg has beeen processed
	//2022july25, spi, end
}

bool spivorogui::onrbuttonup(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	int idpoint = getnearestpointsetobject(xPos, yPos, &(vorogui_itriseed));
	if (idpoint >= vorogui_numberofframepoints)
	{

		if (vorogui_displaytextlabelflag)
		{
			vorogui_displaytextlabelflag = false;
			//vorogui_idpointdirty = POINTSET_ALL;
			vorogui_idpointdirty = POINTSET_NONE;
		}
		else
		{
			vorogui_displaytextlabelflag = true;
			vorogui_idpointdirty = POINTSET_NONE;
		}
	}
	else
	{
		setclustermode_toggletonext();
		vorogui_idpointdirty = POINTSET_ALL;
	}
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
	//2022july25, spi, begin
	/*
	return true;
	*/
	return 0; //returning 0, as specified by windows when msg has beeen processed
	//2022july25, spi, end
}

string spivorogui::getfilename()
{
	//use synth's class name without tags
	string mystring = "spivg";
	if (_pSynth)
	{
		mystring = typeid(_pSynth).name(); //typeid(global_pSynth).name();
		//mystring = mystring.substr(6, mystring.length() - (6 + 2)); //remove "class" and " *" tags
		mystring = mystring.substr(13, mystring.length() - (13 + 2)); //remove "class Tonic::" and " *" tags
	}
	//add .txt extension
	mystring += ".txt";
	return mystring;
}

bool spivorogui::readfromdisk()
{
	//create initial pointset with frame points
	//POINTSET* pPOINTSET = createpointset(false); //already allocated
	POINTSET* pPOINTSET = m_pPOINTSET;
	if (pPOINTSET)
	{
		//if file valid, replace all parameters values and positions
		string filename = getfilename();
		string line;
		ifstream myfile(filename);
		int linecount = 0;
		if (myfile.good())  
		{
			//populate pointset with positions and values from file
			while (getline(myfile, line))  // same as: while (getline( myfile, line ).good())
			{
				linecount++;
				if (linecount == 1)
				{
					//skip header line, do nothing
				}
				else
				{
					MinimalAlloc(VOROGUI_ADDITIONAL_NUMBEROFELEMENTS);
					istringstream buf(line);
					istream_iterator<string> beg(buf), end;
					vector<string> tokens(beg, end);
					int tokencount = 0;
					float parametermin = 0.0;
					float parametermax = 0.0;
					float parametervalue = 0.0;
					string parametername = "";
					for (vector<string>::iterator it = tokens.begin(), end = tokens.end(); it != end; ++it)
					{
						tokencount++;
						if (tokencount == 1)
						{
							//parametername
							/*
							parametername = params[linecount-2].getName();
							*/
							getparameter(linecount - 2, parametermin, parametermax, parametervalue, parametername);
							string parameternamefromfile = (*it);
							if (parametername != parameternamefromfile) break;
						}
						else if (tokencount == 2)
						{
							//parametervalue
							float parametervaluefromfile = atof((*it).c_str());
							/*
							global_pSynth->setParameter(parametername, parametervaluefromfile);
							TonicFloat min = params[linecount - 2].getMin();
							TonicFloat max = params[linecount - 2].getMax();
							*/
							assert((parametermin <= parametervaluefromfile) && (parametervaluefromfile <= parametermax));
							float ratio = 0.0;
							if ((parametermax - parametermin) != 0.0) ratio = (parametervaluefromfile - parametermin) / (parametermax - parametermin);
							else  ratio = 0.0;
							pPOINTSET->controlratio[vorogui_numberofframepoints + linecount - 2] = ratio;
							setparameter(linecount - 2, ratio);
						}
						else if (tokencount == 3)
						{
							pPOINTSET->px[vorogui_numberofframepoints + linecount - 2] = atof((*it).c_str());
						}
						else if (tokencount == 4)
						{
							pPOINTSET->py[vorogui_numberofframepoints + linecount - 2] = atof((*it).c_str());
						}
					}
					pPOINTSET->npts++;
				}
			}
			vorogui_numberofpoints = pPOINTSET->npts;
			pPOINTSET->npts = vorogui_numberofpoints + vorogui_numberofframepoints; //original
			pPOINTSET->xmin = MAXDBL;
			pPOINTSET->ymin = MAXDBL;
			pPOINTSET->xmax = MINDBL;
			pPOINTSET->ymax = MINDBL;
			for (int i = 0; i < pPOINTSET->npts; i++)
			{
				if (pPOINTSET->px[i] < pPOINTSET->xmin) pPOINTSET->xmin = pPOINTSET->px[i];
				if (pPOINTSET->px[i] > pPOINTSET->xmax) pPOINTSET->xmax = pPOINTSET->px[i];
				if (pPOINTSET->py[i] < pPOINTSET->ymin) pPOINTSET->ymin = pPOINTSET->py[i];
				if (pPOINTSET->py[i] > pPOINTSET->ymax) pPOINTSET->ymax = pPOINTSET->py[i];
			}

		}

		//2022dec06, spi, begin
		/*
		//build tin
		//2022may16, spi, begin
		//BuildTriangleNetwork(_pPOINTSET);
		//ComputeAllTriangleCenters(_pPOINTSET);
		BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
		//2022may16, spi, end
		//if (m_pPOINTSET)
		//{
		//	replacepointset(pPOINTSET);
		//}
		*/
		//rescale pointset if needed and rebuild tin
		bool bforcefullredraw = needsrescale();
		if (bforcefullredraw == true)
		{
			double x_scale = 1.0;
			double y_scale = 1.0;
			if (m_pPOINTSET->xmax > VOROGUI_X_RESOLUTION)
			{
				x_scale = (float)vorogui_xwidth / m_pPOINTSET->xmax;
			}
			if (m_pPOINTSET->ymax > VOROGUI_Y_RESOLUTION)
			{
				y_scale = (float)vorogui_yheight / m_pPOINTSET->ymax;
			}
			if (x_scale != 1.0 || y_scale != 1.0)
			{
				ScalePointset(m_pPOINTSET, x_scale, y_scale);
				//BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
			}
		}
		BuildTINAndComputeStatistics(POINTSET_TIN_NORMALIZE); // POINTSET_TIN_NONORMALIZE);
		//2022dec06, spi, end


		return true;
	}
	return false;
}

/*
bool spivorogui::replacepointset(POINTSET* pPOINTSET)
{
	if (pPOINTSET)
	{
		if(m_pPOINTSET) destroypointset(m_pPOINTSET);
		m_pPOINTSET = pPOINTSET;
		return true;
	}
	return false;
}
*/

bool spivorogui::writetodisk()
{
	string filename = getfilename();
	FILE* pFILE = fopen(filename.c_str(), "w");
	if (pFILE)
	{
		fprintf(pFILE, "%s\t%s\t%s\t%s\n", "parametername", "parametervalue", "parameterxposition", "parameteryposition"); //file header
		/*
		vector<ControlParameter> params = global_pSynth->getParameters();
		assert((pPOINTSET->npts - vorogui_numberofframepoints) == params.size());
		*/
		for (int idpoint = vorogui_numberofframepoints; idpoint < m_pPOINTSET->npts; idpoint++)
		{
			/*
			string parametername = params[idpoint - vorogui_numberofframepoints].getName();
			TonicFloat parametervalue = params[idpoint - vorogui_numberofframepoints].getValue();
			*/
			float parametermin = 0.0;
			float parametermax = 0.0;
			float parametervalue = 0.0;
			string parametername = "";
			getparameter(idpoint - vorogui_numberofframepoints, parametermin, parametermax, parametervalue, parametername);
			fprintf(pFILE, "%s\t%f\t%f\t%f\n", parametername.c_str(), parametervalue, m_pPOINTSET->px[idpoint], m_pPOINTSET->py[idpoint]);
		}
		fclose(pFILE);
		return true;
	}
	return false;
}

bool spivorogui::updatecontrolsfromparameters()
{
	///////////////////////////////////////////////////
	//set control ratio according to synth's parameters
	///////////////////////////////////////////////////
	int ii = -1;
	if (_pSynth)
	{
		vector<ControlParameter> params = _pSynth->getParameters();  //global_pSynth->getParameters();
		for (unsigned int i = 0; i < params.size(); i++)
		{
			ii++;
			TonicFloat min = params[i].getMin();
			TonicFloat max = params[i].getMax();
			TonicFloat value = params[i].getValue();
			string mystring = params[i].getName();

			if ((max - min) != 0.0) m_pPOINTSET->controlratio[vorogui_numberofframepoints + i] = (value - min) / (max - min);
			else  m_pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.0;
			//bypass for now
			//pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.5;
		}
	}
	for (vector<CWindowParameter*>::iterator it = p_windowparameterpointers->begin(); it != p_windowparameterpointers->end(); ++it)
	{
		vector<CParameter> parameters = (*it)->getParameters();
		for (unsigned int i = 0; i < parameters.size(); i++)
		{
			ii++;
			float min = parameters[i].getMin();
			float max = parameters[i].getMax();
			float value = parameters[i].getValue();
			string mystring = parameters[i].getName();

			//if ((max - min) != 0.0) m_pPOINTSET->controlratio[vorogui_numberofframepoints + ii] = (value - min) / (max - min);
			if ((max - min) > 0.0) m_pPOINTSET->controlratio[vorogui_numberofframepoints + ii] = (value - min) / (max - min);
			else  m_pPOINTSET->controlratio[vorogui_numberofframepoints + ii] = 0.0;
			//bypass for now
			//pPOINTSET->controlratio[vorogui_numberofframepoints + i] = 0.5;
		}
	}
	return true;
}