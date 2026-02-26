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
//////////////////////////////////////////////////////
//include all headers required to have a minimal 
//transparent windows application with tonic portaudio
//////////////////////////////////////////////////////

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
//#define new DBG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL
#include "AutoRescaleOw2doc.h"
#define COMPILING_SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_CPP		1
#include "spiwindowtransparentmidivoroguispectrumtimeframe.h"
#include "FreeImage.h"
#include <shellapi.h> //for CommandLineToArgW()
#include <mmsystem.h> //for timeSetEvent()
#include <stdio.h> //for swprintf()
#include <assert.h>
#include "spiwavsetlib.h"



#include "portaudio.h"

#ifdef WIN32
#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif

#include "Tonic.h"
/*
#include "ControlSwitcherTestSynth.h"
#include "ControlSwitcherExpSynth.h"
#include "BasicSynth.h"
#include "SimpleInstrumentSynth.h"
#include "StepSequencerSynth.h"
#include "StepSequencerExpSynth.h"
#include "StepSequencerBufferPlayerExpSynth.h"
#include "StepSequencerBufferPlayerEffectExpSynth.h"
//#include "EventsSynth.h"
#include "EventsExpSynth.h"
#include "BufferPlayerExpSynth.h"
#include "ArbitraryTableLookupSynth.h"
#include "BandlimitedOscillatorTestSynth.h"
#include "CompressorDuckingTestSynth.h"
#include "CompressorTestSynth.h"
#include "CompressorExpSynth.h"
#include "ControlSnapToScaleTestSynth.h"
#include "DelayTestSynth.h"
#include "FilteredNoiseSynth.h"
#include "FilterExpSynth.h"
#include "FMDroneSynth.h"
#include "InputDemoSynth.h"
#include "InputExpSynth.h"
#include "InputProcessSynth.h"
#include "InputProcessFlangerSynth.h"
#include "LFNoiseTestSynth.h"
#include "ReverbTestSynth.h"
#include "SimpleStepSeqSynth.h"
#include "SineSumSynth.h"
#include "StereoDelayTestSynth.h"
#include "SynthsAsGeneratorsDemoSynth.h"
#include "XYSpeedSynth.h"
*/
#include "InputProcessSynth.h"
using namespace Tonic;

#include <iostream>
#include <fstream>

#include <string>
//2022nov18, spi, begin
#include <vector>
//2022nov18, spi, end
#include <map>
using namespace std;

#include "spiutility.h"
#include "defs.h"
#include "resource.h"


/////////////////////////////////////
//include all headers related to midi
/////////////////////////////////////
#include "portmidi.h"
#include "porttime.h"
#include "spimidi.h"


//////////////////////////////////////////////////////
//include all headers related to your visual interface
//////////////////////////////////////////////////////

//#include "CMainWindow.h"
#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"
#include "CParameter.h"
#include "CWindowParameter.h"

#include "CThirdWindow.h"
#include "CFourthWindow.h"
#include "CFifthWindow.h"
#include "CTextWindow.h"

#include "CSPITextWindow.h"

#include "CSpectrumWindow.h"
#include "CSpectrum24bitWindow.h"
#include "CImageWindow.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "ChannelBuffersHandler.h"
#include "SpectrumDisplay.h"
#include "AutocorrDisplay.h"
#include "WaveformDisplay.h"
#include "AutocorrHandler.h"
#include <math.h>

#include "CTimeframeWindow.h"

//#include "c_pointset.h"
#include "spivorogui.h"
#include "CVoroguiWindow.h"


//2022oct25, spi, begin
#include "spiaudiodevice.h"
int global_numchannels = NUM_CHANNELS;
SPIAudioDevice mySPIAudioDevice;
//2022oct25, spi, end

//2022nov18, spi, begin
#include "CVorogsWindow.h"
#include "CVorossWindow.h"
//2022nov18, spi, end

//2024fev04, spi, begin
#include "PolySynth.h"
#include "spitonicsynths.h"
//2024fev04, spi, end

/////////////////////
//globals
/////////////////////
CThirdWindow* global_pBlackWindow = NULL;
CTextWindow* global_pTextWindow = NULL;
CSpectrumWindow* global_pSpectrumWindow = NULL;
CSpectrum24bitWindow* global_pSpectrum24bitWindow = NULL;
CImageWindow* global_pImageWindow = NULL;
CTimeframeWindow* global_pTimeframeWindow = NULL;

//spi, begin
//2018apr24, frames-batch modif
CVorogsWindow* global_pVorogsWindow = NULL;
//spi, end

//2022nov18, spi, begin
CVorossWindow* global_pVorossWindow = NULL;
//2022nov18, spi, end

bool global_blackwindowdisplay = 1;
bool global_textwindowdisplay = 1;
bool global_spectrumwindowdisplay = 0;
bool global_spectrum24bitwindowdisplay = 0;
bool global_imagewindowdisplay = 1;
bool global_timeframewindowdisplay = 0;

//2021dec29, spi, begin
CSPITextWindow* global_pSPITextWindow = NULL;
vector<std::unique_ptr<CSPITextWindow>> spitextwindow_vector; //vector<CSPITextWindow*> spitextwindow_vector;
vector<std::unique_ptr<CSPITextWindow>> voroguispitextwindow_vector;
CSPITextWindow* get_voroguispitextwindow_vector()
{
	return voroguispitextwindow_vector[0].get();
}

int global_spitextwindowdisplay = 0;
//2021dec29, spi, end


CHAR pCHAR[1024];
WCHAR pWCHAR[1024];

PmStream* global_pPmStreamMIDIOUT = NULL; // midi output
PmStream* global_pPmStreamMIDIIN=NULL;      // midi input 
bool global_active = false;     // set when global_pPmStreamMIDIIN is ready for reading
bool global_inited = false;     // suppress printing during command line parsing 
std::map<string, int> global_inputmididevicemap;
std::map<string, int> global_outputmididevicemap;
//string global_inputmididevicename = "Q49";
//string global_outputmididevicename = "Out To MIDI Yoke:  1";
string global_inputmididevicename = "";
string global_outputmididevicename = "";
int global_inputmidideviceid = 11; //alesis q49 midi port id (when midi yoke installed)
//int inputmididevice =  1; //midi yoke 1 (when midi yoke installed)
int global_inputmidichannel = -1; //-1 for all
int global_outputmidideviceid = 13; //device id 13, for "Out To MIDI Yoke:  1", when 8 yoke installed for spi
int global_outputmidichannel = 1;

bool in_sysex = false;   // we are reading a sysex message 
bool done = false;       // when true, exit 
bool notes = true;       // show notes? 
bool controls = true;    // show continuous controllers 
bool bender = true;      // record pitch bend etc.? 
bool excldata = true;    // record system exclusive data? 
bool verbose = true;     // show text representation? 
bool realdata = true;    // record real time messages? 
bool clksencnt = true;   // clock and active sense count on 
bool chmode = true;      // show channel mode messages 
bool pgchanges = true;   // show program changes 
bool flush = false;	    // flush all pending MIDI data 

uint32_t filter = 0;            // remember state of midi filter 

uint32_t clockcount = 0;        // count of clocks 
uint32_t actsensecount = 0;     // cout of active sensing bytes 
uint32_t notescount = 0;        // #notes since last request 
uint32_t notestotal = 0;        // total #notes 

char val_format[] = "    Val %d\n";





RingBufferWriter global_RingBufferWriter;

// Static smart pointer for our Synth
//2024fev04, spi, begin
static PolySynth poly;
//2024fev04, spi, end
/*
static Synth synth;
*/
//static ControlSwitcherTestSynth synth;
//static ControlSwitcherExpSynth synth;
//static BasicSynth synth;
//static SimpleInstrumentSynth synth;
//static StepSequencerSynth synth;
//static StepSequencerExpSynth synth;
//static StepSequencerBufferPlayerExpSynth synth;
//static StepSequencerBufferPlayerEffectExpSynth synth;
//static EventsSynth synth;
//static EventsExpSynth synth;
//static BufferPlayerExpSynth synth;
//static ArbitraryTableLookupSynth synth;
//static BandlimitedOscillatorTestSynth synth;
//static CompressorDuckingTestSynth synth;
//static CompressorTestSynth synth;
//static CompressorExpSynth synth;
//static ControlSnapToScaleTestSynth synth;
//static DelayTestSynth synth;
//static FilteredNoiseSynth synth;
//static FilterExpSynth synth;
//static FMDroneSynth synth;
//static InputDemoSynth* global_pSynth;
//static InputExpSynth* global_pSynth;
static InputProcessSynth* global_pSynth;
//static InputProcessFlangerSynth* global_pSynth;
//static LFNoiseTestSynth synth;
//static ReverbTestSynth synth;
//static SimpleStepSeqSynth synth;
//static SineSumSynth synth;
//static StereoDelayTestSynth synth;
//static SynthsAsGeneratorsDemoSynth synth;
//static XYSpeedSynth synth;

//2022nov14, spi, begin
/*
std::map<string, int> global_inputdevicemap;
std::map<string, int> global_outputdevicemap;

PaStream* global_stream;
PaStreamParameters global_inputParameters;
PaStreamParameters global_outputParameters;
PaError global_err;
string global_audioinputdevicename = "";
string global_audiooutputdevicename = "";
int global_inputAudioChannelSelectors[2];
int global_outputAudioChannelSelectors[2];
PaAsioStreamInfo global_asioInputInfo;
PaAsioStreamInfo global_asioOutputInfo;
*/
//2022nov14, spi, end

FILE* pFILE = NULL;

// Global Variables:

FIBITMAP* global_dib=NULL;
FIBITMAP* global_quantizeddib = NULL;
RGBQUAD* global_quantizeddibpalette = NULL;

string global_imagefolder = "";
string global_imageextension = "";
vector<string> global_imagefilenames;


HFONT global_hFont;
HFONT global_hFontHuge;
HWND global_hwnd = NULL;
MMRESULT global_timer = 0;
//#define MAX_GLOBALTEXT	4096
//WCHAR global_text[MAX_GLOBALTEXT+1];
//int global_delay_ms=5000; //default to 5 seconds delay
float global_cutfreq_hz = 1000.0f; //default to 1000.0 Hz
float global_duration_sec = 180;
int global_x = 100;
int global_y = 200;
int global_xwidth = 400;
int global_yheight = 400;
BYTE global_alpha = 200;
int global_fontheight = 24;
//int global_fontwidth = -1; //will be computed within WM_PAINT handler
int global_staticalignment = 0; //0 for left, 1 for center and 2 for right
//int global_staticheight = -1; //will be computed within WM_SIZE handler
//int global_staticwidth = -1; //will be computed within WM_SIZE handler 
//COLORREF global_statictextcolor=RGB(0xFF, 0xFF, 0xFF); //white
COLORREF global_statictextcolor = RGB(0xFF, 0x00, 0x00); //red
//spi, begin
//int global_imageheight = -1; //will be computed within WM_SIZE handler
//int global_imagewidth = -1; //will be computed within WM_SIZE handler 
//spi, end
int global_titlebardisplay = 1; //0 for off, 1 for on
int global_acceleratoractive = 0; //0 for off, 1 for on
int global_menubardisplay = 0; //0 for off, 1 for on

DWORD global_startstamp_ms;
//FILE* global_pFILE=NULL;
string global_line;
std::ifstream global_ifstream;

/*
#define IDC_MAIN_EDIT	100
#define IDC_MAIN_STATIC	101
*/

//HINSTANCE hInst;								// current instance
TCHAR global_szTitle[1024] = { L"spiwindowtransparentmidivoroguititle" };					// The title bar text
TCHAR global_szWindowClass[1024] = { L"spiwindowtransparentmidivoroguiclass" };				// the main window class name

//new parameters
int global_statictextcolor_red = 255;
int global_statictextcolor_green = 0;
int global_statictextcolor_blue = 0;
string global_begin = "begin.ahk";
string global_end = "end.ahk";

bool global_abort = false;


//2021nov29, spi, begin
int global_maxnumimage = -1; //only used when greater than 0 AND when global_imagefolder is not a folder but rather a txt filename containing a list of image filenames 
int global_maxphysicalmemory_mb = SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MAX_PHYSICALMEMORYUSAGE_MB; //-1; // MAX_PHYSICALMEMORYUSAGE_MB; //only used if user specifies it
float global_maxcpu_inpercent_f = SPIWINDOWTRANSPARENTMIDIVOROGUISPECTRUMTIMEFRAME_VS2017_MAX_CPUUSAGE_INPERCENT; //-1.0f; // MAX_CPUUSAGE_INPERCENT; //10%
//2024feb22, spi, begin
//spilogfile global_debugfile("spiwtmvst-exe_debug.txt", true);
//2024feb22, spi, end
string global_modestring = ""; //"DEBUG,SPITEXT,CPULOCAL" would both log to debug file and launch spitext.exe to display a string showing cpu avail to this process 
//string global_modestring = ""; //"DEBUG,SPITEXT,CPUGLOBAL" would both log to debug file and launch spitext.exe to display a string showing cpu avail to the system 
//string global_modestring = ""; //"DEBUG,SPITEXT,MEMLOCAL" would both log to debug file and launch spitext.exe to display a string showing mem avail to this process 
//string global_modestring = ""; //"DEBUG,SPITEXT,MEMGLOBAL" would both log to debug file and launch spitext.exe to display a string showing mem avail to the system 
int global_debugmode = 0;
int global_spitextmode = 0;
int global_spivoicemode = 0;
int global_spirecordmode = 0;
int global_cpulocalmode = 0;
int global_cpuglobalmode = 0;
int global_memlocalmode = 0;
int global_memglobalmode = 0;
//2022may20, spi, begin
int global_voroguiclustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY; //original
//int global_voroguiclustermode = VOROGUI_DISPLAY_CONTROLS_CLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY; //2022may20
//2022may20, spi, end
//2021nov29, spi, end

//2022dec22, spi, begin
int global_vorossclustermode = VOROGUI_DISPLAY_CONTROLS_NOTCLUSTERED | VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY; 
//2022dec22, spi, end

//2021dec29, spi, begin
//the #define SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT is 
//"\"VALUE\" -1 1 -1 -1 -1 \"Arial\" 1 \"spitextclass\" \"spitexttitle\" \"\" \"\" \"\" \"\" 100 \"\" \"\" \"\" \"\" \"\" \"center\" \"center\" \"0.5\" \"0.5\" 1 "
const int spitextcmdstringcount = 2;
const char* spitextcmdtring[spitextcmdstringcount] = {
	SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_LT,
	SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_LT
};
const enum spitextcmdtringindexes { SPITEXT_VOROGUI_CONTROL_VALUE=0 , SPITEXT_VOROGUI_CONTROL_NAME=1};

HINSTANCE global_hinstance = NULL;
static int spitexttitleandclassid = 0;
void launchspitextwindow(LPCTSTR text, HWND hwnd, HINSTANCE hinstance/* = NULL*/)
{
	RECT rect = { global_x, global_y, global_x+global_xwidth, global_y+global_yheight }; //default for safety, will be overwritten
	GetWindowRect(hwnd, &rect);
	launchspitextwindow(text, rect, hwnd, hinstance);
}
void launchspitextwindow(LPCTSTR text,  RECT rect, HWND hwnd, HINSTANCE hinstance/* = NULL*/ )
{
	if (hinstance == NULL) hinstance = global_hinstance;
	if (global_spitextmode)
	{
		//coordinates are relative to hwnd (i.e. when hwnd not null and when hwnd is the voroguiwindow's hwnd)
		int x = rect.left;
		int y = rect.top;
		int xwidth = abs(rect.right - rect.left);
		int yheight = abs(rect.bottom - rect.top);
		//
		wstring spitextwindowtitle = L"voroguispitextwindowtitle";
		WCHAR pwchar[1024];
		int i = spitexttitleandclassid++;;
		_itow_s(i, pwchar, sizeof(pwchar) / sizeof(WCHAR), 10); //base 10 for decimal
		spitextwindowtitle += pwchar;
		wstring myclassname = global_szWindowClass;
		myclassname += L"VOROGUISPITEXT";
		myclassname += pwchar;
		
		string mydefaultcmdstring = spitextcmdtring[SPITEXT_VOROGUI_CONTROL_VALUE]; //this string starts with "\"VALUE\" -1 2 -1 -1 -1 \"Arial\"
		wstring mywtext = text;
		string mytext = utf8_encode(mywtext);
		//mycmdstring = "\"" + mytext + "\"" + mycmdstring.substr(SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_QUOTEVALUEQUOTE); //for replacing the text string, need to truncate the first 7 characters, which are \"VALUE\"
		//mycmdstring = "\"" + mytext + "\"" + mycmdstring.substr(SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_QUOTESTRINGQUOTE,
		//	SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_HWND-SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_QUOTESTRINGQUOTE); 
		string mycmdstring = "\"" + mytext + "\"" + " -1 1 "; //" -1 1 " stands for a starttime not specified and endtime of 1 second 
		mycmdstring += to_string(x) + " " + to_string(y) + " " + to_string(yheight) + " " + mydefaultcmdstring.substr(SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_SPACEFONTHEIGHTSPACE,
			SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_HWND - SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_SPACEFONTHEIGHTSPACE);
		mycmdstring += "\"" + to_string((int)hwnd) + "\"" + mydefaultcmdstring.substr(SPITEXT_VOROGUI_CONTROL_NEGATIVEXY_NEGATIVEFONTHEIGHT_HWND);
		global_pSPITextWindow = NULL; //never store nor delete the pointer when using SPITEXT_NOPOSTQUITMESSAGE
		//CSPITextWindow* pSPITextWindow = DBG_NEW CSPITextWindow(L"myspitextwindowtitle", hInstance, myclassname.c_str(), SPITEXT_EXAMPLE_1, SPITEXT_NOPOSTQUITMESSAGE,
		CSPITextWindow* pSPITextWindow = (CSPITextWindow*)DBG_NEW CSPITextWindow(spitextwindowtitle.c_str(), hinstance, myclassname.c_str(), mycmdstring.c_str(), SPITEXT_NOPOSTQUITMESSAGE,
			x, y, xwidth, yheight, global_alpha, global_titlebardisplay, global_menubardisplay); //255, global_titlebardisplay, global_menubardisplay);
		if (pSPITextWindow)
		{
			//CSPITextWindow pointer is stored just in case application terminates before CSPITextWindow destroys itself.
			//CSPITextWindow pointer is stored in the smart pointer vector vector<std::unique_ptr<CSPITextWindow>>
			voroguispitextwindow_vector.emplace_back(pSPITextWindow); //store pointer as a smart/unique pointer 
			pSPITextWindow->Create();
			pSPITextWindow->Show();
		}
	}
	return;
}
//2021dec29, spi, end


/////////////////////////////////////////////////////////////////////////////
//               put_pitch
// Inputs:
//    int p: pitch number
// Effect: write out the pitch name for a given number
/////////////////////////////////////////////////////////////////////////////

static int put_pitch(int p)
{
	char result[8];
	static char *ptos[] = {
		"c", "cs", "d", "ef", "e", "f", "fs", "g",
		"gs", "a", "bf", "b" };
	// note octave correction below 
	sprintf(result, "%s%d", ptos[p % 12], (p / 12) - 1);
	sprintf(pCHAR, "%s", result); StatusAddTextA(pCHAR);
	return strlen(result);
}


/////////////////////////////////////////////////////////////////////////////
//               showbytes
// Effect: print hex data, precede with newline if asked
/////////////////////////////////////////////////////////////////////////////

char nib_to_hex[] = "0123456789ABCDEF";

static void showbytes(PmMessage data, int len, bool newline)
{
	int count = 0;
	int i;

	//    if (newline) {
	//        putchar('\n');
	//        count++;
	//    } 
	for (i = 0; i < len; i++)
	{
		putchar(nib_to_hex[(data >> 4) & 0xF]);
		putchar(nib_to_hex[data & 0xF]);
		count += 2;
		if (count > 72)
		{
			putchar('.');
			putchar('.');
			putchar('.');
			break;
		}
		data >>= 8;
	}
	putchar(' ');
}

///////////////////////////////////////////////////////////////////////////////
//               output
// Inputs:
//    data: midi message buffer holding one command or 4 bytes of sysex msg
// Effect: format and print  midi data
///////////////////////////////////////////////////////////////////////////////
char vel_format[] = "    Vel %d\n";
static void output(PmMessage data)
{
	int command;    // the current command 
	int chan;   // the midi channel of the current event 
	int len;    // used to get constant field width 

	// printf("output data %8x; ", data); 

	command = Pm_MessageStatus(data) & MIDI_CODE_MASK;
	chan = Pm_MessageStatus(data) & MIDI_CHN_MASK;

	if (in_sysex || Pm_MessageStatus(data) == MIDI_SYSEX) {
#define sysex_max 16
		int i;
		PmMessage data_copy = data;
		in_sysex = true;
		// look for MIDI_EOX in first 3 bytes 
		// if realtime messages are embedded in sysex message, they will
		// be printed as if they are part of the sysex message
		//
		for (i = 0; (i < 4) && ((data_copy & 0xFF) != MIDI_EOX); i++)
			data_copy >>= 8;
		if (i < 4) {
			in_sysex = false;
			i++; // include the EOX byte in output 
		}
		showbytes(data, i, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "System Exclusive\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (command == MIDI_ON_NOTE && Pm_MessageData2(data) != 0) {
		notescount++;
		if (notes) {
			showbytes(data, 3, verbose);
			if (verbose)
			{
				sprintf(pCHAR, "NoteOn  Chan %2d Key %3d ", chan, Pm_MessageData1(data)); StatusAddTextA(pCHAR);
				len = put_pitch(Pm_MessageData1(data));
				sprintf(pCHAR, vel_format + len, Pm_MessageData2(data)); StatusAddTextA(pCHAR);
			}
		}
	}
	else if ((command == MIDI_ON_NOTE // && Pm_MessageData2(data) == 0
		|| command == MIDI_OFF_NOTE) && notes) {
		showbytes(data, 3, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "NoteOff Chan %2d Key %3d ", chan, Pm_MessageData1(data)); StatusAddTextA(pCHAR);
			len = put_pitch(Pm_MessageData1(data));
			sprintf(pCHAR, vel_format + len, Pm_MessageData2(data)); StatusAddTextA(pCHAR);
		}
	}
	else if (command == MIDI_CH_PROGRAM && pgchanges) {
		showbytes(data, 2, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "  ProgChg Chan %2d Prog %2d\n", chan, Pm_MessageData1(data) + 1); StatusAddTextA(pCHAR);
		}
	}
	else if (command == MIDI_CTRL) {
		// controls 121 (MIDI_RESET_CONTROLLER) to 127 are channel
		// mode messages. 
		if (Pm_MessageData1(data) < MIDI_ALL_SOUND_OFF) {
			showbytes(data, 3, verbose);
			if (verbose)
			{
				sprintf(pCHAR, "CtrlChg Chan %2d Ctrl %2d Val %2d\n",
					chan, Pm_MessageData1(data), Pm_MessageData2(data)); StatusAddTextA(pCHAR);
			}
		}
		else if (chmode) { // channel mode 
			showbytes(data, 3, verbose);
			if (verbose) {
				switch (Pm_MessageData1(data))
				{
				case MIDI_ALL_SOUND_OFF:
					sprintf(pCHAR, "All Sound Off, Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				case MIDI_RESET_CONTROLLERS:
					sprintf(pCHAR, "Reset All Controllers, Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				case MIDI_LOCAL:
					sprintf(pCHAR, "LocCtrl Chan %2d %s\n",
						chan, Pm_MessageData2(data) ? "On" : "Off"); StatusAddTextA(pCHAR);
					break;
				case MIDI_ALL_OFF:
					sprintf(pCHAR, "All Off Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				case MIDI_OMNI_OFF:
					sprintf(pCHAR, "OmniOff Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				case MIDI_OMNI_ON:
					sprintf(pCHAR, "Omni On Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				case MIDI_MONO_ON:
					sprintf(pCHAR, "Mono On Chan %2d\n", chan); StatusAddTextA(pCHAR);
					if (Pm_MessageData2(data))
					{
						sprintf(pCHAR, " to %d received channels\n", Pm_MessageData2(data)); StatusAddTextA(pCHAR);
					}
					else
					{
						sprintf(pCHAR, " to all received channels\n"); StatusAddTextA(pCHAR);
					}
					break;
				case MIDI_POLY_ON:
					sprintf(pCHAR, "Poly On Chan %2d\n", chan); StatusAddTextA(pCHAR);
					break;
				}
			}
		}
	}
	else if (command == MIDI_POLY_TOUCH && bender) {
		showbytes(data, 3, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "P.Touch Chan %2d Key %2d ", chan, Pm_MessageData1(data)); StatusAddTextA(pCHAR);
			len = put_pitch(Pm_MessageData1(data));
			printf(val_format + len, Pm_MessageData2(data));
		}
	}
	else if (command == MIDI_TOUCH && bender) {
		showbytes(data, 2, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "  A.Touch Chan %2d Val %2d\n", chan, Pm_MessageData1(data)); StatusAddTextA(pCHAR);
		}
	}
	else if (command == MIDI_BEND && bender) {
		showbytes(data, 3, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "P.Bend  Chan %2d Val %2d\n", chan,
				(Pm_MessageData1(data) + (Pm_MessageData2(data) << 7))); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_SONG_POINTER) {
		showbytes(data, 3, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Song Position %d\n",
				(Pm_MessageData1(data) + (Pm_MessageData2(data) << 7))); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_SONG_SELECT) {
		showbytes(data, 2, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Song Select %d\n", Pm_MessageData1(data)); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_TUNE_REQ) {
		showbytes(data, 1, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Tune Request\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_Q_FRAME && realdata) {
		showbytes(data, 2, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Time Code Quarter Frame Type %d Values %d\n",
				(Pm_MessageData1(data) & 0x70) >> 4, Pm_MessageData1(data) & 0xf); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_START && realdata) {
		showbytes(data, 1, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Start\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_CONTINUE && realdata) {
		showbytes(data, 1, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Continue\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_STOP && realdata) {
		showbytes(data, 1, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    Stop\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_SYS_RESET && realdata) {
		showbytes(data, 1, verbose);
		if (verbose)
		{
			sprintf(pCHAR, "    System Reset\n"); StatusAddTextA(pCHAR);
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_TIME_CLOCK) {
		if (clksencnt) clockcount++;
		else if (realdata) {
			showbytes(data, 1, verbose);
			if (verbose)
			{
				sprintf(pCHAR, "    Clock\n"); StatusAddTextA(pCHAR);
			}
		}
	}
	else if (Pm_MessageStatus(data) == MIDI_ACTIVE_SENSING) {
		if (clksencnt) actsensecount++;
		else if (realdata) {
			showbytes(data, 1, verbose);
			if (verbose)
			{
				sprintf(pCHAR, "    Active Sensing\n"); StatusAddTextA(pCHAR);
			}
		}
	}
	else showbytes(data, 3, verbose);
	fflush(stdout);
}


///////////////////////////
//implementation
///////////////////////////
//2024fev04, spi, begin
/*
void receive_poll(PtTimestamp timestamp, void *userData)
{
	PmEvent event;
	int count;
	if (!global_active) return;
	while ((count = Pm_Read(global_pPmStreamMIDIIN, &event, 1)))
	{
		if (count == 1)
		{
			//1) output message
			if (global_pTextWindow && global_pTextWindow->IsWindowVisible()) output(event.message);
			//StatusAddTextA("This is a line\n");

		}
		else
		{
			//sprintf(pCHAR, "%s\n", Pm_GetErrorText((PmError)count)); StatusAddTextA(pCHAR); //spi a cast as (PmError)
		}
	}
}
*/
void receive_poll(PtTimestamp timestamp, void *userData)
{
	PmEvent event;
	int count;
	if (!global_active) return;
	while ((count = Pm_Read(global_pPmStreamMIDIIN, &event, 1)))
	{
		if (count == 1)
		{
			//0) detect channel
			int command = Pm_MessageStatus(event.message) & MIDI_CODE_MASK;
			int chan = Pm_MessageStatus(event.message) & MIDI_CHN_MASK;
			int data1 = Pm_MessageData1(event.message);
			int data2 = Pm_MessageData2(event.message);
			if (global_inputmidichannel==-1 || (chan == global_inputmidichannel) )
			{
				//1) output message
				if (global_pTextWindow && global_pTextWindow->IsWindowVisible()) output(event.message);

				//2) 
				if (command == MIDI_OFF_NOTE || (command == MIDI_ON_NOTE && data2 == 0))
				{
					int midinotenumber = data1; //range 0 to 127
					int midinotevelocity = data2; //range 0 to 127
					poly.noteOff(midinotenumber);
				}
				else if (command == MIDI_ON_NOTE)
				{
					/*
					//2.1) set a parameter that we created when we defined the synth
					synth.setParameter("midiNote", midinotenumber);
					synth.setParameter("midiNoteVelocity", midinotevelocity);
					//2.2) trigger note
					//simply setting the value of a parameter causes that parameter to send a "trigger" message to any using them as triggers
					synth.setParameter("trigger", 1);
					*/
					int midinotenumber = data1; //range 0 to 127
					int midinotevelocity = data2; //range 0 to 127
					poly.noteOn(midinotenumber, midinotevelocity);
				}
				else if (command == MIDI_CH_PROGRAM)
				{

				}
				else if (command == MIDI_CTRL)
				{
					if (data1 == 0 && data2 == 0)
					{
						/*
						//output(event.message);
						ShellExecuteA(NULL, "open", ".\\begin.ahk", "", NULL, false);
						*/
					}
				}


			}



		}
		else
		{
			//printf(Pm_GetErrorText((PmError)count)); //spi a cast as (PmError)
			//sprintf(pCHAR, Pm_GetErrorText((PmError)count)); StatusAddTextA(pCHAR);
		}
	}
}
//2024fev04, spi, end

static int renderCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData);

static int gNumNoInputs = 0;
// This routine will be called by the PortAudio engine when audio is needed.
// It may be called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
//
static int renderCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	SAMPLE *out = (SAMPLE*)outputBuffer;
	const SAMPLE *in = (const SAMPLE*)inputBuffer;
	unsigned int i;
	(void)timeInfo; // Prevent unused variable warnings.
	(void)statusFlags;
	(void)userData;

	if (global_abort == true) return paAbort;

	//2024feb25, spi, begin
	//if (inputBuffer == NULL)
	if (inputBuffer == NULL && !(mySPIAudioDevice.global_audioinputdevicename.empty())
		&& !(mySPIAudioDevice.global_audiooutputdevicename.empty()))
	//2024feb25, spi, end
	{
		//2024feb24, spi, begin
		/*
		for (i = 0; i<framesPerBuffer; i++)
		{
			*out++ = 0;  // left - silent
			*out++ = 0;  // right - silent 
		}
		*/
		memset(outputBuffer, 0, sizeof(float)*framesPerBuffer*NUM_CHANNELS);
		//2024feb24, spi, end
		gNumNoInputs += 1;
	}
	else
	{
		/*
		for (i = 0; i<framesPerBuffer; i++)
		{
		*out++ = *in++;  // left - unprocessed
		*out++ = *in++;  // right - unprocessed
		}
		*/
		//2024feb25, spi, begin
		//global_RingBufferWriter.write((float*)inputBuffer, framesPerBuffer, NUM_CHANNELS);
		if(!(mySPIAudioDevice.global_audioinputdevicename.empty()))
		{
			global_RingBufferWriter.write((float*)inputBuffer, framesPerBuffer, NUM_CHANNELS);
		}
		//2024feb25, spi, end

		//2024feb25, spi, begin
		if (!(mySPIAudioDevice.global_audiooutputdevicename.empty()))
		{
		//2024feb25, spi, end
			global_pSynth->fillBufferOfFloats((float*)outputBuffer, framesPerBuffer, NUM_CHANNELS);

			//if (0) pthread_mutex_lock(&g_mutex);
			//callbackFilledAudioBuffers->acquireAndSeparateNewBuffer((float*)outputBuffer);
			//g_ready = true;
			//if (0) pthread_mutex_unlock(&g_mutex);
			if (global_pSpectrumWindow && global_pSpectrumWindow->IsWindowVisible())
			{
				global_pSpectrumWindow->AcquireAudioBuffer((float*)outputBuffer, framesPerBuffer, NUM_CHANNELS);
			}
			if (global_pSpectrum24bitWindow && global_pSpectrum24bitWindow->IsWindowVisible())
			{
				global_pSpectrum24bitWindow->AcquireAudioBuffer((float*)outputBuffer, framesPerBuffer, NUM_CHANNELS);
			}
			if (global_pTimeframeWindow && global_pTimeframeWindow->IsWindowVisible())
			{
				global_pTimeframeWindow->AcquireAudioBuffer((float*)outputBuffer, framesPerBuffer, NUM_CHANNELS);
			}
		//2024feb25, spi, begin
		}
		//2024feb25, spi, end
	}


	return paContinue;
}

//2022oct25, spi, begin
/*
bool SelectAudioInputDevice()
{
	const PaDeviceInfo* deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	for (int i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		string devicenamestring = deviceInfo->name;
		global_inputdevicemap.insert(pair<string, int>(devicenamestring, i));
		if (pFILE) fprintf(pFILE, "id=%d, name=%s\n", i, devicenamestring.c_str());
	}

	int deviceid = Pa_GetDefaultInputDevice(); // default input device 
	std::map<string, int>::iterator it;
	it = global_inputdevicemap.find(global_audioinputdevicename);
	if (it != global_inputdevicemap.end())
	{
		deviceid = (*it).second;
		//printf("%s maps to %d\n", global_audiodevicename.c_str(), deviceid);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
		//assert(inputAudioChannelSelectors[0]<deviceInfo->maxInputChannels);
		//assert(inputAudioChannelSelectors[1]<deviceInfo->maxInputChannels);
	}
	else
	{
		//Pa_Terminate();
		//return -1;
		//printf("error, audio device not found, will use default\n");
		//MessageBox(win,"error, audio device not found, will use default\n",0,0);
		deviceid = Pa_GetDefaultInputDevice();
	}


	global_inputParameters.device = deviceid;
	if (global_inputParameters.device == paNoDevice)
	{
		//MessageBox(win,"error, no default input device.\n",0,0);
		return false;
	}
	//global_inputParameters.channelCount = 2;
	global_inputParameters.channelCount = NUM_CHANNELS;
	global_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	global_inputParameters.suggestedLatency = Pa_GetDeviceInfo(global_inputParameters.device)->defaultLowOutputLatency;
	//inputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioInputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioInputInfo.hostApiType = paASIO;
	global_asioInputInfo.version = 1;
	global_asioInputInfo.flags = paAsioUseChannelSelectors;
	global_asioInputInfo.channelSelectors = global_inputAudioChannelSelectors;
	if (deviceid == Pa_GetDefaultInputDevice())
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO)
	{
		global_inputParameters.hostApiSpecificStreamInfo = &global_asioInputInfo;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS)
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}



bool SelectAudioOutputDevice()
{
	const PaDeviceInfo* deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	for (int i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		string devicenamestring = deviceInfo->name;
		global_outputdevicemap.insert(pair<string, int>(devicenamestring, i));
		if (pFILE) fprintf(pFILE, "id=%d, name=%s\n", i, devicenamestring.c_str());
	}

	int deviceid = Pa_GetDefaultOutputDevice(); // default output device 
	std::map<string, int>::iterator it;
	it = global_outputdevicemap.find(global_audiooutputdevicename);
	if (it != global_outputdevicemap.end())
	{
		deviceid = (*it).second;
		//printf("%s maps to %d\n", global_audiodevicename.c_str(), deviceid);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
		//assert(inputAudioChannelSelectors[0]<deviceInfo->maxInputChannels);
		//assert(inputAudioChannelSelectors[1]<deviceInfo->maxInputChannels);
	}
	else
	{
		//Pa_Terminate();
		//return -1;
		//printf("error, audio device not found, will use default\n");
		//MessageBox(win,"error, audio device not found, will use default\n",0,0);
		deviceid = Pa_GetDefaultOutputDevice();
	}


	global_outputParameters.device = deviceid;
	if (global_outputParameters.device == paNoDevice)
	{
		//MessageBox(win,"error, no default output device.\n",0,0);
		return false;
	}
	//global_inputParameters.channelCount = 2;
	global_outputParameters.channelCount = NUM_CHANNELS;
	global_outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	global_outputParameters.suggestedLatency = Pa_GetDeviceInfo(global_outputParameters.device)->defaultLowOutputLatency;
	//outputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioOutputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioOutputInfo.hostApiType = paASIO;
	global_asioOutputInfo.version = 1;
	global_asioOutputInfo.flags = paAsioUseChannelSelectors;
	global_asioOutputInfo.channelSelectors = global_outputAudioChannelSelectors;
	if (deviceid == Pa_GetDefaultOutputDevice())
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO)
	{
		global_outputParameters.hostApiSpecificStreamInfo = &global_asioOutputInfo;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS)
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}
*/
//2022oct25, spi, end


//for debugging
void myPurecallHandler(void)
{
	//printf("In _purecall_handler.");
	exit(0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_set_purecall_handler(myPurecallHandler); //for debugging

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//2021dec29, spi, begin
	global_hinstance = hInstance;
	//2021dec29, spi, end

	global_startstamp_ms = GetTickCount();

	//LPWSTR *szArgList;
	LPSTR *szArgList;
	int nArgs;
	int i;

	//szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	szArgList = CommandLineToArgvA(GetCommandLineA(), &nArgs);
	if (NULL == szArgList)
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}
	LPWSTR *szArgListW;
	int nArgsW;
	szArgListW = CommandLineToArgvW(GetCommandLineW(), &nArgsW);
	if (NULL == szArgListW)
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}
	//global_audioinputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	mySPIAudioDevice.global_audioinputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	if (nArgs > 1)
	{
		//global_filename = szArgList[1];
		//global_audioinputdevicename = szArgList[1];
		mySPIAudioDevice.global_audioinputdevicename = szArgList[1];
	}
	//global_inputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	//global_inputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	mySPIAudioDevice.global_inputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	mySPIAudioDevice.global_inputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	//global_inputAudioChannelSelectors[0] = 2; // on emu patchmix ASIO device channel 3 (left)
	//global_inputAudioChannelSelectors[1] = 3; // on emu patchmix ASIO device channel 4 (right)
	//global_inputAudioChannelSelectors[0] = 8; // on emu patchmix ASIO device channel 9 (left)
	//global_inputAudioChannelSelectors[1] = 9; // on emu patchmix ASIO device channel 10 (right)
	//global_inputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	//global_inputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)
	if (nArgs > 2)
	{
		//global_inputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[2])); //0 for first asio channel (left) or 2, 4, 6, etc.
		mySPIAudioDevice.global_inputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[2])); //0 for first asio channel (left) or 2, 4, 6, etc.
	}
	if (nArgs > 3)
	{
		//global_inputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[3])); //1 for second asio channel (right) or 3, 5, 7, etc.
		mySPIAudioDevice.global_inputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[3])); //1 for second asio channel (right) or 3, 5, 7, etc.
	}
	//global_audiooutputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	mySPIAudioDevice.global_audiooutputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	if (nArgs > 4)
	{
		//global_filename = szArgList[1];
		//global_audiooutputdevicename = szArgList[4];
		mySPIAudioDevice.global_audiooutputdevicename = szArgList[4];
	}
	//global_outputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	//global_outputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	mySPIAudioDevice.global_outputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	mySPIAudioDevice.global_outputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	//global_outputAudioChannelSelectors[0] = 2; // on emu patchmix ASIO device channel 3 (left)
	//global_outputAudioChannelSelectors[1] = 3; // on emu patchmix ASIO device channel 4 (right)
	//global_outputAudioChannelSelectors[0] = 8; // on emu patchmix ASIO device channel 9 (left)
	//global_outputAudioChannelSelectors[1] = 9; // on emu patchmix ASIO device channel 10 (right)
	//global_outputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	//global_outputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)
	if (nArgs > 5)
	{
		//global_outputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[5])); //0 for first asio channel (left) or 2, 4, 6, etc.
		mySPIAudioDevice.global_outputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[5])); //0 for first asio channel (left) or 2, 4, 6, etc.
	}
	if (nArgs > 6)
	{
		//global_outputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[6])); //1 for second asio channel (right) or 3, 5, 7, etc.
		mySPIAudioDevice.global_outputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[6])); //1 for second asio channel (right) or 3, 5, 7, etc.
	}


	if (nArgs > 7)
	{
		global_cutfreq_hz = atof(szArgList[7]);
	}
	if (nArgs > 8)
	{
		global_duration_sec = atof(szArgList[8]);
	}


	if (nArgs > 9)
	{
		global_x = atoi(szArgList[9]);
	}
	if (nArgs > 10)
	{
		global_y = atoi(szArgList[10]);
	}
	if (nArgs > 11)
	{
		global_xwidth = atoi(szArgList[11]);
	}
	if (nArgs > 12)
	{
		global_yheight = atoi(szArgList[12]);
	}
	if (nArgs > 13)
	{
		global_alpha = atoi(szArgList[13]);
	}
	if (nArgs > 14)
	{
		global_titlebardisplay = atoi(szArgList[14]);
	}
	if (nArgs > 15)
	{
		global_menubardisplay = atoi(szArgList[15]);
	}
	if (nArgs > 16)
	{
		global_acceleratoractive = atoi(szArgList[16]);
	}
	if (nArgs > 17)
	{
		global_fontheight = atoi(szArgList[17]);
	}

	//new parameters
	if (nArgs > 18)
	{
		global_statictextcolor_red = atoi(szArgList[18]);
	}
	if (nArgs > 19)
	{
		global_statictextcolor_green = atoi(szArgList[19]);
	}
	if (nArgs > 20)
	{
		global_statictextcolor_blue = atoi(szArgList[20]);
	}
	if (nArgs > 21)
	{
		wcscpy(global_szWindowClass, szArgListW[21]);
	}
	if (nArgs > 22)
	{
		wcscpy(global_szTitle, szArgListW[22]);
	}
	if (nArgs > 23)
	{
		global_begin = szArgList[23];
	}
	if (nArgs > 24)
	{
		global_end = szArgList[24];
	}

	if (nArgs > 25)
	{
		global_inputmididevicename = szArgList[25];
	}
	if (nArgs > 26)
	{
		global_inputmidichannel = atoi(szArgList[26]);
	}
	if (nArgs > 27)
	{
		global_outputmididevicename = szArgList[27];
	}
	if (nArgs > 28)
	{
		global_outputmidichannel = atoi(szArgList[28]);
	}

	if (nArgs > 29)
	{
		global_imagefolder = szArgList[29];
	}
	if (nArgs > 30)
	{
		global_imageextension = szArgList[30];
	}

	if (nArgs > 31)
	{
		global_blackwindowdisplay = atoi(szArgList[31]);
	}
	if (nArgs > 32)
	{
		global_textwindowdisplay = atoi(szArgList[32]);
	}
	if (nArgs > 33)
	{
		global_spectrumwindowdisplay = atoi(szArgList[33]);
	}
	if (nArgs > 34)
	{
		global_spectrum24bitwindowdisplay = atoi(szArgList[34]);
	}
	if (nArgs > 35)
	{
		global_imagewindowdisplay = atoi(szArgList[35]);
	}
	if (nArgs > 36)
	{
		global_timeframewindowdisplay = atoi(szArgList[36]);
	}
	//2021nov29, spi, begin
	if (nArgs > 37)
	{
		global_spitextwindowdisplay = atoi(szArgList[37]);
	}
	if (nArgs > 38)
	{
		global_modestring = szArgList[38];
		std::transform(global_modestring.begin(), global_modestring.end(), global_modestring.begin(), ::toupper);
		if (!global_modestring.empty())
		{
			if (global_modestring.find("DEBUG") != std::string::npos)
			{
				//found!
				global_debugmode = 1;
				global_debugmode = global_debugmode | VOROGUI_DEBUG_CONTROLS;
			}
			if ((global_modestring.find("VOROGUIDBG") != std::string::npos) ||
				(global_modestring.find("VOROGUIDEBUG") != std::string::npos))
			{
				//found!
				global_debugmode = global_debugmode & ~VOROGUI_DEBUG_CONTROLS; //to disable, perform a bitwise AND with bitwise NOT of the value you want unset
				global_debugmode = global_debugmode | VOROGUI_DEBUG_VORONOIDIAGRAM; //to enable, combine flags using bitwise OR operator
			}
			if (global_modestring.find("SPITEXT") != std::string::npos)
			{
				//found!
				global_spitextmode = 1;
			}
			if (global_modestring.find("SPIVOICE") != std::string::npos)
			{
				//found!
				global_spivoicemode = 1;
			}
			if (global_modestring.find("SPIRECORD") != std::string::npos)
			{
				//found!
				global_spirecordmode = 1;
			}
			if (global_modestring.find("CPULOCAL") != std::string::npos)
			{
				//found!
				global_cpulocalmode = 1;
			}
			if (global_modestring.find("CPUGLOBAL") != std::string::npos)
			{
				//found!
				global_cpuglobalmode = 1;
			}
			if (global_modestring.find("MEMLOCAL") != std::string::npos)
			{
				//found!
				global_memlocalmode = 1;
			}
			if (global_modestring.find("MEMGLOBAL") != std::string::npos)
			{
				//found!
				global_memglobalmode = 1;
			}
			if (global_modestring.find("VOROGUICLUSTER") != std::string::npos)
			{
				//found!
				global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_CLUSTERED;
			}
			if (global_modestring.find("VOROGUIGRIDLIKE") != std::string::npos)
			{
				//found!
				global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_GRIDLIKE;
				//now, to unset VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY
				//perform a Bitwise AND with Bitwise NOT of the value you want unset
				global_voroguiclustermode = global_voroguiclustermode & ~VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY;
				if (global_modestring.find("VOROGUIRANDOMIZED") != std::string::npos)
					//found!
				{
					global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_GRIDLIKE_RANDOMIZED;
				}
			}
			if (global_modestring.find("VOROGUILISTLIKE") != std::string::npos)
			{
				//found!
				global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL;
				//now, to unset VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY
				//perform a Bitwise AND with Bitwise NOT of the value you want unset
				global_voroguiclustermode = global_voroguiclustermode & ~VOROGUI_DISPLAY_CONTROLS_RANDOM_COMPLETELY;
				if (global_modestring.find("VOROGUIVERTICAL") != std::string::npos)
				{
					//found!
					global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL;
					if (global_modestring.find("VOROGUIRANDOMIZED") != std::string::npos)
						//found!
					{
						global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_VERTICAL_RANDOMIZED;
					}
				}
				if (global_modestring.find("VOROGUIHORIZONTAL") != std::string::npos)
				{
					//found!
					global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL;
					if (global_modestring.find("VOROGUIRANDOMIZED") != std::string::npos)
						//found!
					{
						global_voroguiclustermode = global_voroguiclustermode | VOROGUI_DISPLAY_CONTROLS_LISTLIKE_HORIZONTAL_RANDOMIZED;
					}
				}
			}
		}
	}
	if (nArgs > 39)
	{
		global_maxnumimage = atoi(szArgList[39]);
	}
	//2021nov29, spi, end

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	global_statictextcolor = RGB(global_statictextcolor_red, global_statictextcolor_green, global_statictextcolor_blue);
	LocalFree(szArgList);
	LocalFree(szArgListW);

	ofstream myfile;
	myfile.open("classname.txt");
	wstring mywstring = global_szWindowClass;
	myfile << utf8_encode(mywstring);
	myfile.close();

	//spilogfile debug_spilogfile("debug.txt", true);


	int nShowCmd = false;
	//ShellExecuteA(NULL, "open", "begin.bat", "", NULL, nShowCmd);
	if (!global_begin.empty()) ShellExecuteA(NULL, "open", global_begin.c_str(), "", NULL, nCmdShow);


	//////////////////////////
	//initialize random number
	//////////////////////////
	//srand((unsigned)time(0));
	//2022nov18, spi, begin
	srand((unsigned)GetTickCount());
	//2022nov18, spi, end

	//2022nov14, spi, begin
	/*
	pFILE = fopen("devices.txt", "w");

	///////////////////////
	//initialize port audio
	///////////////////////
	global_err = Pa_Initialize();
	if (global_err != paNoError)
	{
		//MessageBox(0,"portaudio initialization failed",0,MB_ICONERROR);
		if (pFILE)
		{
			fprintf(pFILE, "portaudio initialization failed.\n");
			fclose(pFILE);
		}
		return 1;
	}
	////////////////////////
	//audio device selection
	////////////////////////
	SelectAudioInputDevice();
	SelectAudioOutputDevice();
	*/
	mySPIAudioDevice.m_pFILE = fopen("devices.txt", "w");
	///////////////////////
	//initialize port audio
	///////////////////////
	mySPIAudioDevice.global_err = Pa_Initialize();
	if (mySPIAudioDevice.global_err != paNoError)
	{
		//MessageBox(0,"portaudio initialization failed",0,MB_ICONERROR);
		if (mySPIAudioDevice.m_pFILE) fprintf(mySPIAudioDevice.m_pFILE, "portaudio initialization failed.\n");
		if (mySPIAudioDevice.m_pFILE) fclose(mySPIAudioDevice.m_pFILE);
		return 1;
	}
	////////////////////////
	//audio device selection
	////////////////////////
	//2024feb25, spi, begin
	//mySPIAudioDevice.SelectAudioInputDevice();
	//mySPIAudioDevice.SelectAudioOutputDevice();
	if (!(mySPIAudioDevice.global_audioinputdevicename.empty()))
	{
		mySPIAudioDevice.SelectAudioInputDevice();
	}
	if (!(mySPIAudioDevice.global_audiooutputdevicename.empty()))
	{
		mySPIAudioDevice.SelectAudioOutputDevice();
	}
	//2024feb25, spi, end
	if (mySPIAudioDevice.m_pFILE) fflush(mySPIAudioDevice.m_pFILE);
	//2022nov14, spi, end


	// You don't necessarily have to do this - it will default to 44100 if not set.
	Tonic::setSampleRate(SAMPLE_RATE);

	//2024fev04, spi, begin
	//poly.addVoices(createSynthVoice, 8); //basic synth
	//poly.addVoices(createSynthVoice_v2, 8); //control switcher synth
	//poly.addVoices(createSynthVoice_v3, 8); //arbitrary table lookup synth
	//poly.addVoices(createSynthVoice_v4, 8); //events bufferplayer synth (under construction)
	//poly.addVoices(createSynthVoice_v5, 8); //band limited oscillator synth
	poly.addVoices(createSynthVoice_v6, 8); //compressor synth
	//poly.addVoices(createSynthVoice_v7, 8); //control snap to scale synth (under construction)
	//poly.addVoices(createSynthVoice_v8, 8); //delay synth
	//poly.addVoices(createSynthVoice_v9, 8); //events synth
	//poly.addVoices(createSynthVoice_v10, 8); //filtered noise synth
	//poly.addVoices(createSynthVoice_v11, 8); //filter synth (under construction)
	//poly.addVoices(createSynthVoice_v12, 8); //fm drone synth 
	//poly.addVoices(createSynthVoice_v13, 8); //lf noise synth 
	//poly.addVoices(createSynthVoice_v14, 8); //reverb synth 
	//poly.addVoices(createSynthVoice_v15, 8); //simple step seq synth (under construction) 
	//poly.addVoices(createSynthVoice_v16, 8); //sine sum synth 
	//2024fev04, spi, end

	//tonic ringbuffer initialization
	global_RingBufferWriter = RingBufferWriter("input", FRAMES_PER_BUFFER, NUM_CHANNELS);
	//tonic synth creation
	//global_pSynth = DBG_NEW(InputDemoSynth);
	//global_pSynth = DBG_NEW(InputExpSynth);
	global_pSynth = DBG_NEW(InputProcessSynth);
	//global_pSynth = DBG_NEW(InputProcessFlangerSynth);

	//2024fev04, spi, begin
	if (global_pSynth) global_pSynth->setOutputGen(global_pSynth->getOutputGen() + poly);
	//2024fev04, spi, end

	//////////////
	//setup stream  
	//////////////
	//2024feb25, spi, begin
	/*
	mySPIAudioDevice.global_err = Pa_OpenStream(
		&mySPIAudioDevice.global_stream,
		&mySPIAudioDevice.global_inputParameters, //NULL, //&global_inputParameters,
		&mySPIAudioDevice.global_outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		0, //paClipOff,      // we won't output out of range samples so don't bother clipping them
		renderCallback,
		NULL); //no callback userData
	*/
	PaStreamParameters* pPaStreamParameters_input = NULL;
	if (!(mySPIAudioDevice.global_audioinputdevicename.empty()))
	{
		pPaStreamParameters_input = &mySPIAudioDevice.global_inputParameters;
	}
	PaStreamParameters* pPaStreamParameters_output = NULL;
	if (!(mySPIAudioDevice.global_audiooutputdevicename.empty()))
	{
		pPaStreamParameters_output = &mySPIAudioDevice.global_outputParameters;
	}
	mySPIAudioDevice.global_err = Pa_OpenStream(
		&mySPIAudioDevice.global_stream,
		pPaStreamParameters_input, //NULL, //&global_inputParameters,
		pPaStreamParameters_output,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		0, //paClipOff,      // we won't output out of range samples so don't bother clipping them
		renderCallback,
		NULL); //no callback userData
	//2024feb25, spi, end

	if (mySPIAudioDevice.global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Unable to open stream: %s\n", Pa_GetErrorText(mySPIAudioDevice.global_err));
		//MessageBox(0,errorbuf,0,MB_ICONERROR);
		if (mySPIAudioDevice.m_pFILE)
		{
			fprintf(mySPIAudioDevice.m_pFILE, "%s\n", errorbuf);
			fclose(mySPIAudioDevice.m_pFILE);
		}
		return 1;
	}



	//////////////
	//start stream  
	//////////////
	mySPIAudioDevice.global_err = Pa_StartStream(mySPIAudioDevice.global_stream);
	if (mySPIAudioDevice.global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Unable to start stream: %s\n", Pa_GetErrorText(mySPIAudioDevice.global_err));
		//MessageBox(0,errorbuf,0,MB_ICONERROR);
		if (mySPIAudioDevice.m_pFILE)
		{
			fprintf(mySPIAudioDevice.m_pFILE, "%s\n", errorbuf);
			fclose(mySPIAudioDevice.m_pFILE);
		}
		return 1;
	}

	fclose(mySPIAudioDevice.m_pFILE);
	mySPIAudioDevice.m_pFILE = NULL;

	PmError err;
	if ((!global_inputmididevicename.empty()) || (!global_outputmididevicename.empty()))
	{
		/////////////////////
		//initialize portmidi
		/////////////////////
		Pm_Initialize();
	}

	if (!global_inputmididevicename.empty())
	{
		/////////////////////////////
		//input midi device selection
		/////////////////////////////
		const PmDeviceInfo* deviceInfo;
		int numDevices = Pm_CountDevices();
		for (int i = 0; i < numDevices; i++)
		{
			deviceInfo = Pm_GetDeviceInfo(i);
			if (deviceInfo->input)
			{
				string devicenamestring = deviceInfo->name;
				global_inputmididevicemap.insert(pair<string, int>(devicenamestring, i));
			}
		}
		std::map<string, int>::iterator it;
		it = global_inputmididevicemap.find(global_inputmididevicename);
		if (it != global_inputmididevicemap.end())
		{
			global_inputmidideviceid = (*it).second;
			sprintf(pCHAR, "%s maps to %d\n", global_inputmididevicename.c_str(), global_inputmidideviceid); StatusAddTextA(pCHAR);
			deviceInfo = Pm_GetDeviceInfo(global_inputmidideviceid);
		}
		else
		{
			assert(false);
			for (it = global_inputmididevicemap.begin(); it != global_inputmididevicemap.end(); it++)
			{
				sprintf(pCHAR, "%s maps to %d\n", (*it).first.c_str(), (*it).second); StatusAddTextA(pCHAR);
			}
			swprintf(pWCHAR, L"input midi device not found\n"); StatusAddText(pWCHAR);
			return 0;
		}
	}

	if (!global_outputmididevicename.empty())
	{
		//////////////////////////////
		//output midi device selection
		//////////////////////////////
		const PmDeviceInfo* deviceInfo;
		int numDevices = Pm_CountDevices();
		for (int i = 0; i < numDevices; i++)
		{
			deviceInfo = Pm_GetDeviceInfo(i);
			if (deviceInfo->output)
			{
				string devicenamestring = deviceInfo->name;
				global_outputmididevicemap.insert(pair<string, int>(devicenamestring, i));
			}
		}
		std::map<string, int>::iterator it;
		it = global_outputmididevicemap.find(global_outputmididevicename);
		if (it != global_outputmididevicemap.end())
		{
			global_outputmidideviceid = (*it).second;
			sprintf(pCHAR, "%s maps to %d\n", global_outputmididevicename.c_str(), global_outputmidideviceid); StatusAddTextA(pCHAR);
			deviceInfo = Pm_GetDeviceInfo(global_outputmidideviceid);
		}
		else
		{
			assert(false);
			for (it = global_outputmididevicemap.begin(); it != global_outputmididevicemap.end(); it++)
			{
				sprintf(pCHAR, "%s maps to %d\n", (*it).first.c_str(), (*it).second); StatusAddTextA(pCHAR);
			}
			swprintf(pWCHAR, L"output midi device not found\n"); StatusAddText(pWCHAR);
		}

		// list device information 
		swprintf(pWCHAR, L"MIDI output devices:\n"); StatusAddText(pWCHAR);
		for (int i = 0; i < Pm_CountDevices(); i++)
		{
			const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
			if (info->output)
			{
				sprintf(pCHAR, "%d: %s, %s\n", i, info->interf, info->name); StatusAddTextA(pCHAR);
			}
		}
		swprintf(pWCHAR, L"device %d selected\n", global_outputmidideviceid); StatusAddText(pWCHAR);
		//err = Pm_OpenInput(&midi_in, inp, NULL, 512, NULL, NULL);
		err = Pm_OpenOutput(&global_pPmStreamMIDIOUT, global_outputmidideviceid, NULL, 512, NULL, NULL, 0); //0 latency
		if (err)
		{
			sprintf(pCHAR, "%s\n", Pm_GetErrorText(err)); StatusAddTextA(pCHAR);
			//Pt_Stop();
			//mmexit(1);
			return 0;
		}
	}


	if (!global_inputmididevicename.empty())
	{
		//////////////////////////
		//start polling midi input
		//////////////////////////

		// use porttime callback to empty midi queue and print 
		Pt_Start(1, receive_poll, 0);
		// list device information 
		swprintf(pWCHAR, L"MIDI input devices:\n"); StatusAddText(pWCHAR);
		for (int i = 0; i < Pm_CountDevices(); i++)
		{
			const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
			if (info->input)
			{
				sprintf(pCHAR, "%d: %s, %s\n", i, info->interf, info->name); StatusAddTextA(pCHAR);
			}
		}
		//inputmididevice = get_number("Type input device number: ");
		swprintf(pWCHAR, L"device %d selected\n", global_inputmidideviceid); StatusAddText(pWCHAR);

		err = Pm_OpenInput(&global_pPmStreamMIDIIN, global_inputmidideviceid, NULL, 512, NULL, NULL);
		if (err)
		{
			/*
			sprintf(pCHAR, Pm_GetErrorText(err));StatusAddTextA(pCHAR);
			Pt_Stop();
			return;
			*/
			//disconnect portmidi input
			global_active = false;
			swprintf(pWCHAR, L"****************************\n"); StatusAddText(pWCHAR);
			swprintf(pWCHAR, L"midi input port DISCONNECTED\n"); StatusAddText(pWCHAR);
			swprintf(pWCHAR, L"****************************\n"); StatusAddText(pWCHAR);
		}
		else
		{
			Pm_SetFilter(global_pPmStreamMIDIIN, filter);
			global_inited = true; // now can document changes, set filter 
			//swprintf(pWCHAR, L"spimidiarpeggiator_polywin32 ready.\n");StatusAddText(pWCHAR);
			global_active = true;
			swprintf(pWCHAR, L"*************************\n"); StatusAddText(pWCHAR);
			swprintf(pWCHAR, L"midi input port CONNECTED\n"); StatusAddText(pWCHAR);
			swprintf(pWCHAR, L"*************************\n"); StatusAddText(pWCHAR);
		}
	}


	///////////////////////
	//load background image
	///////////////////////
	global_dib = FreeImage_Load(FIF_JPEG, "background.jpg", JPEG_DEFAULT);
	//global_dib = FreeImage_Load(FIF_BMP, "background.bmp", BMP_DEFAULT);

	if (global_dib)
	{
		global_quantizeddib = FreeImage_ColorQuantizeEx(global_dib);
		if (global_quantizeddib)
		{
			global_quantizeddibpalette = FreeImage_GetPalette(global_quantizeddib);
		}

		//////////////
		//update icons 
		//////////////
		//2021nov29, spi, begin
		FIBITMAP* local_16x16xrgbdib = FreeImage_Rescale(global_dib, 16, 16, FILTER_BICUBIC);
		if (local_16x16xrgbdib)
		{
			//16x16
			FreeImage_Save(FIF_ICO, local_16x16xrgbdib, "background_16x16xrgb-new.ico"); //rgb
			bool bresult = copyFile("background_16x16x16.ico", "background_16x16x16.bak.ico"); //bak
			FIBITMAP* local_quantizeddib = FreeImage_ColorQuantizeEx(local_16x16xrgbdib, FIQ_WUQUANT, 16); //16 colors
			if (local_quantizeddib)
			{
				FreeImage_Save(FIF_ICO, local_quantizeddib, "background_16x16x16-new.ico"); //16 colors
				bool bresult = copyFile("background_16x16x16-new.ico", "background_16x16x16.ico"); //overwrite
				FreeImage_Unload(local_quantizeddib);
			}
			FreeImage_Unload(local_16x16xrgbdib);
		}
		FIBITMAP* local_32x32xrgbdib = FreeImage_Rescale(global_dib, 32, 32, FILTER_BICUBIC);
		if (local_32x32xrgbdib)
		{
			//32x32
			FreeImage_Save(FIF_ICO, local_32x32xrgbdib, "background_32x32xrgb-new.ico"); //rgb
			bool bresult = copyFile("background_32x32x16.ico", "background_32x32x16.bak.ico"); //bak
			FIBITMAP* local_quantizeddib = FreeImage_ColorQuantizeEx(local_32x32xrgbdib, FIQ_WUQUANT, 16); //16 colors
			if (local_quantizeddib)
			{
				FreeImage_Save(FIF_ICO, local_quantizeddib, "background_32x32x16-new.ico"); //32 colors
				bool bresult = copyFile("background_32x32x16-new.ico", "background_32x32x16.ico"); //overwrite
				FreeImage_Unload(local_quantizeddib);
			}
			FreeImage_Unload(local_32x32xrgbdib);
		}
		FIBITMAP* local_48x48xrgbdib = FreeImage_Rescale(global_dib, 48, 48, FILTER_BICUBIC);
		if (local_48x48xrgbdib)
		{
			//48x48
			FreeImage_Save(FIF_ICO, local_48x48xrgbdib, "background_48x48xrgb-new.ico"); //rgb
			bool bresult = copyFile("background_48x48x256.ico", "background_48x48x256.bak.ico"); //bak
			FIBITMAP* local_quantizeddib = FreeImage_ColorQuantizeEx(local_48x48xrgbdib, FIQ_WUQUANT, 256); //256 colors
			if (local_quantizeddib)
			{
				FreeImage_Save(FIF_ICO, local_quantizeddib, "background_48x48x256-new.ico"); //32 colors
				bool bresult = copyFile("background_48x48x256-new.ico", "background_48x48x256.ico"); //overwrite
				FreeImage_Unload(local_quantizeddib);
			}
			FreeImage_Unload(local_48x48xrgbdib);
		}
		//2021nov29, spi, end
	}
	//////////////////////////
	//populate image filenames
	//////////////////////////
	//spi, begin
	//2018april24, frames-batch
	//0) feed the global_imagefilenames list with background.jpg image fullfilename
	char fullpath[_MAX_PATH];
	_fullpath(fullpath, "background.jpg", _MAX_PATH);
	global_imagefilenames.push_back(fullpath);
	//spi, end

	//2022nov18, spi, begin
	/* //commenting out former code and generalizing
	if (global_imagefolder != "" && global_imageextension != "")
	{
		//1) execute cmd line to get all folder's image filenames
		string quote = "\"";
		string pathfilter;
		string path = global_imagefolder;
		//pathfilter = path + "\\*.bmp";
		pathfilter = path + "\\*" + global_imageextension;
		string systemcommand;
		//systemcommand = "DIR " + quote + pathfilter + quote + "/B /O:N > wsic_filenames.txt"; //wsip tag standing for wav set (library) instrumentset (class) populate (function)
		systemcommand = "DIR " + quote + pathfilter + quote + "/B /S /O:N > spiwtmvst_filenames.txt"; // /S for adding path into "spiwtmvs_filenames.txt"
		system(systemcommand.c_str());
		//2) load in all "spiwtmvs_filenames.txt" file
		//vector<string> global_imagefilenames;
		ifstream ifs("spiwtmvst_filenames.txt");
		string temp;
		while (getline(ifs, temp))
		{
			//txtfilenames.push_back(path + "\\" + temp);
			global_imagefilenames.push_back(temp);
		}
	}
	*/
	//adding flexibility for specifying image paths
	string imagefoldersfilter = global_imagefolder;
	vector<string> global_imagefoldernames;
	if (isvalidfilterstring(imagefoldersfilter))
	{
		//if valid image folders filter string, then:
		//use dir command to find these folders
		//populate global_imagefoldernames vector with foldernames
		//1) execute cmd line to get all image folders
		string quote = "\"";
		string syscommand;
		//systemcommand = "DIR " + quote + imagefoldersfilter + quote + " /A:D /B /S /O:N > spislideshowlauncher_foldernames.txt"; // /S for adding path into "spislideshowlauncher_foldernames.txt"
		syscommand = "DIR " + quote + imagefoldersfilter + quote + " /A:D /B /S /O:N > spissl_foldernames.txt"; // /S for adding path into "spislideshowlauncher_foldernames.txt"
		//fix for dir cmd not returning when wildcard is used and first dir name is partial
		string ff = firstfolder(imagefoldersfilter);
		bool doadddriveletterpath = false;
		string driveletter = getdriveletter(imagefoldersfilter);
		if (haswildcard(ff))
		{
			//do not use /S in this case
			syscommand = removesubstr(syscommand, "/S ");
			if (!getdriveletter(imagefoldersfilter).empty())
			{
				doadddriveletterpath = true;
			}
		}
		//if (ofs_debug.is_open()) ofs_debug << syscommand << endl;
		system(syscommand.c_str());

		//2) load in all "spislideshowlauncher_foldernames.txt" file
		ifstream ifs("spissl_foldernames.txt");
		string temp;
		while (getline(ifs, temp))
		{
			//if (!temp.empty() && !PathIsDirectoryEmptyA(temp.c_str()))
			if (!temp.empty())
			{
				if (doadddriveletterpath)
				{
					temp = driveletter + "\\" + temp;
				}
				global_imagefoldernames.push_back(temp);
			}
			//}
		}
		if (global_imagefoldernames.empty())
		{
			cout << "global_imagefoldernames is empty, imagefoldersfilter may be too restrictive" << endl;
			return 1;
		}
	}
	else if (isvalidtxtfilename_containingfoldernames(imagefoldersfilter))
	{
		//if valid txt filename of image foldernames, then:
		//read in each line and
		//populate global_imagefoldernames vector with foldernames
		ifstream ifs(imagefoldersfilter);
		string temp;
		while (getline(ifs, temp))
		{
			if (!temp.empty())
			{
				global_imagefoldernames.push_back(temp);
			}
		}
	}
	else if (isvalidtxtfilename_containingfilenames(imagefoldersfilter))
	{
		//if valid txt filename of image filenames, then leave global_imagefoldernames empty
		/*
		ifstream ifs(imagefoldersfilter);
		string temp;
		string folder;
		string prev_folder = "";
		int maxnumimg = 0;
		int num12kimg = 0;
		while (getline(ifs, temp))
		{

			if (!temp.empty())
			{
				folder = getfolderfromfilename(temp);
				//prevent adding duplicate when consecutive folder is the same, i.e. when inside video frames folder
				if (!temp.empty() && (folder!=prev_folder))
				{
					std::size_t found = temp.find("12000x12000");
					if (found != std::string::npos)
					{
						//exclude 12000x12000 image folders
						num12kimg++;
					}
					else
					{
						global_imagefoldernames.push_back(folder);
						maxnumimg++; //count number of images
					}
				}
			}
			prev_folder = folder;
		}
		*/
		ifstream ifs(imagefoldersfilter);
		string temp;
		string folder;
		int maxnumimg = 0;
		int num12kimg = 0;
		while (getline(ifs, temp))
		{

			if (!temp.empty())
			{
				if (1)
				{
					//filter out the 12000x12000 images
					std::size_t found = temp.find("12000x12000");
					if (found != std::string::npos)
					{
						//exclude 12000x12000 images
						num12kimg++;
					}
					else
					{
						global_imagefilenames.push_back(temp);
						maxnumimg++; //count number of images
					}
				}
				else
				{
					//include all images
					global_imagefilenames.push_back(temp);
					maxnumimg++; //count number of images
				}
			}
		}
	}
	//2022dec22, spi, begin
	else if (global_imagefolder != "" && global_imageextension != "")
	{
		//1) execute cmd line to get all folder's image filenames
		string quote = "\"";
		string pathfilter;
		string path = global_imagefolder;
		//pathfilter = path + "\\*.bmp";
		pathfilter = path + "\\*" + global_imageextension;
		string systemcommand;
		//systemcommand = "DIR " + quote + pathfilter + quote + "/B /O:N > wsic_filenames.txt"; //wsip tag standing for wav set (library) instrumentset (class) populate (function)
		systemcommand = "DIR " + quote + pathfilter + quote + "/B /S /O:N > spiwtmvst_filenames.txt"; // /S for adding path into "spiwtmvs_filenames.txt"
		system(systemcommand.c_str());
		//2) load in all "spiwtmvs_filenames.txt" file
		//vector<string> global_imagefilenames;
		ifstream ifs("spiwtmvst_filenames.txt");
		string temp;
		while (getline(ifs, temp))
		{
			//txtfilenames.push_back(path + "\\" + temp);
			global_imagefilenames.push_back(temp);
		}
	}

	//2022dec22, spi, end

	//only if global_imagefoldernames is not empty
	if (!global_imagefoldernames.empty())
	{
		//remove duplicates from vector
		removeduplicates(global_imagefoldernames);

		//now, iterate through each folder to find all image filenames
		vector<string>::iterator itvs;
		for (itvs = global_imagefoldernames.begin(); itvs != global_imagefoldernames.end(); itvs++)
		{
			global_imagefolder = *itvs;
			//former block of code prior to 2022nov18
			if (global_imagefolder != "" && global_imageextension != "")
			{
				//1) execute cmd line to get all folder's image filenames
				string quote = "\"";
				string pathfilter;
				string path = global_imagefolder;
				//pathfilter = path + "\\*.bmp";
				pathfilter = path + "\\*" + global_imageextension;
				string systemcommand;
				//systemcommand = "DIR " + quote + pathfilter + quote + "/B /O:N > wsic_filenames.txt"; //wsip tag standing for wav set (library) instrumentset (class) populate (function)
				systemcommand = "DIR " + quote + pathfilter + quote + "/B /S /O:N > spiwtmvst_filenames.txt"; // /S for adding path into "spiwtmvs_filenames.txt"
				system(systemcommand.c_str());
				//2) load in all "spiwtmvs_filenames.txt" file
				//vector<string> global_imagefilenames;
				ifstream ifs("spiwtmvst_filenames.txt");
				string temp;
				while (getline(ifs, temp))
				{
					//txtfilenames.push_back(path + "\\" + temp);
					global_imagefilenames.push_back(temp);
				}
			}
		}
	}
	//2022nov18, spi, end


	//////////////
	//create fonts 
	//////////////
	//global_hFont=CreateFontW(global_fontheight,0,0,0,FW_NORMAL,0,0,0,0,0,0,2,0,L"SYSTEM_FIXED_FONT");
	global_hFont = CreateFontW(global_fontheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe Script");
	global_hFontHuge = CreateFontW(global_yheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe Script");


	wstring myclassname = L"";
	if (global_blackwindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"BLACK";
		//CMainWindow* pMainWindow = DBG_NEW CMainWindow(L"mymainwindowtitle", hInstance, myclassname.c_str(),
		global_pBlackWindow = DBG_NEW CThirdWindow(L"myblackwindowtitle", hInstance, myclassname.c_str(),
			global_x, global_y, global_xwidth, global_yheight,
			255, global_titlebardisplay, global_menubardisplay);
		global_pBlackWindow->Create();
		global_pBlackWindow->Show();
	}

	/*
	myclassname = global_szWindowClass;
	myclassname += L"2";
	//CFirstWindow* pFirstWindow = DBG_NEW CFirstWindow(L"myfirstwindowtitle", hInstance, myclassname.c_str(),
	CFourthWindow* pFirstWindow = DBG_NEW CFourthWindow(L"myfirstwindowtitle", hInstance, myclassname.c_str(),
		global_x, global_y, global_xwidth, global_yheight,
		global_alpha, global_titlebardisplay, 0);
	pFirstWindow->Create();
	pFirstWindow->Show();

	myclassname = global_szWindowClass;
	myclassname += L"3";
	CFifthWindow* pSecondWindow = DBG_NEW CFifthWindow(L"mysecondwindowtitle", hInstance, myclassname.c_str(),
		global_x, global_y, global_xwidth, global_yheight,
		global_alpha, global_titlebardisplay, 0);
	pSecondWindow->Create();
	pSecondWindow->Show();
	*/

	if (global_textwindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"TEXT";
		global_pTextWindow = DBG_NEW CTextWindow(L"mytextwindowtitle", hInstance, myclassname.c_str(),
			global_staticalignment, global_statictextcolor, global_hFont, global_dib,
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		global_pTextWindow->Create();
		global_pTextWindow->Show();
	}

	if (global_spectrumwindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"SPECTRUM";
		global_pSpectrumWindow = DBG_NEW CSpectrumWindow(L"myspectrumwindowtitle", hInstance, myclassname.c_str(),
			global_quantizeddibpalette,
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		global_pSpectrumWindow->Create();
		global_pSpectrumWindow->Show();
	}

	if (global_spectrum24bitwindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"SPECTRUM24BIT";
		global_pSpectrum24bitWindow = DBG_NEW CSpectrum24bitWindow(L"myspectrum24bitwindowtitle", hInstance, myclassname.c_str(),
			global_quantizeddibpalette,
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		global_pSpectrum24bitWindow->Create();
		global_pSpectrum24bitWindow->Show();
	}

	if (global_imagewindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"IMAGE";
		global_pImageWindow = DBG_NEW CImageWindow(L"myimagewindowtitle", hInstance, myclassname.c_str(),
			global_dib,
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		global_pImageWindow->Create();
		global_pImageWindow->Show();
	}

	if (global_timeframewindowdisplay)
	{
		myclassname = global_szWindowClass;
		myclassname += L"TIMEFRAME";
		global_pTimeframeWindow = DBG_NEW CTimeframeWindow(L"mytimeframewindowtitle", hInstance, myclassname.c_str(),
			"background.jpg", global_imagefilenames,
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		global_pTimeframeWindow->Create();
		global_pTimeframeWindow->Show();
	}

	if (global_spitextwindowdisplay)
	{
		const int spitextcmdstringcount = 16;
		const char* spitextcmdtring[spitextcmdstringcount] = {
			SPITEXT_EXAMPLE_MONITOR_1_1_16,
			SPITEXT_EXAMPLE_MONITOR_1_2_16,
			SPITEXT_EXAMPLE_MONITOR_1_3_16,
			SPITEXT_EXAMPLE_MONITOR_1_4_16,
			SPITEXT_EXAMPLE_MONITOR_1_5_16,
			SPITEXT_EXAMPLE_MONITOR_1_6_16,
			SPITEXT_EXAMPLE_MONITOR_1_7_16,
			SPITEXT_EXAMPLE_MONITOR_1_8_16,
			SPITEXT_EXAMPLE_MONITOR_1_9_16,
			SPITEXT_EXAMPLE_MONITOR_1_10_16,
			SPITEXT_EXAMPLE_MONITOR_1_11_16,
			SPITEXT_EXAMPLE_MONITOR_1_12_16,
			SPITEXT_EXAMPLE_MONITOR_1_13_16,
			SPITEXT_EXAMPLE_MONITOR_1_14_16,
			SPITEXT_EXAMPLE_MONITOR_1_15_16,
			SPITEXT_EXAMPLE_MONITOR_1_16_16
		};
		wstring spitextwindowtitle = L"myspitextwindowtitle";
		WCHAR pwchar[1024];
		for (int i = 0; i < global_spitextwindowdisplay; i++)
		{
			if (i == spitextcmdstringcount)
			{
				global_spitextwindowdisplay = spitextcmdstringcount;
				break;
			}
			_itow_s(i, pwchar, sizeof(pwchar) / sizeof(WCHAR), 10); //base 10 for decimal
			spitextwindowtitle += pwchar;
			myclassname = global_szWindowClass;
			myclassname += L"SPITEXT";
			myclassname += pwchar;
			global_pSPITextWindow = NULL; //never store nor delete the pointer when using SPITEXT_NOPOSTQUITMESSAGE
			//CSPITextWindow* pSPITextWindow = DBG_NEW CSPITextWindow(L"myspitextwindowtitle", hInstance, myclassname.c_str(), SPITEXT_EXAMPLE_1, SPITEXT_NOPOSTQUITMESSAGE,
			CSPITextWindow* pSPITextWindow = (CSPITextWindow*)DBG_NEW CSPITextWindow(spitextwindowtitle.c_str(), hInstance, myclassname.c_str(), spitextcmdtring[i], SPITEXT_NOPOSTQUITMESSAGE,
				global_x, global_y, global_xwidth, global_yheight, global_alpha, global_titlebardisplay, global_menubardisplay); //255, global_titlebardisplay, global_menubardisplay);
			if (pSPITextWindow)
			{
				//vector<std::unique_ptr<CSPITextWindow>>
				spitextwindow_vector.emplace_back(pSPITextWindow); //store pointer as a smart/unique pointer
				pSPITextWindow->Create();
				pSPITextWindow->Show();
			}
		}
	}

	vector<CWindowParameter*> wndparampointers;
	if (global_pBlackWindow) wndparampointers.push_back(global_pBlackWindow);
	if (global_pTextWindow) wndparampointers.push_back(global_pTextWindow);
	if (global_pSpectrumWindow) wndparampointers.push_back(global_pSpectrumWindow);
	if (global_pSpectrum24bitWindow) wndparampointers.push_back(global_pSpectrum24bitWindow);
	if (global_pImageWindow) wndparampointers.push_back(global_pImageWindow);
	if (global_pTimeframeWindow) wndparampointers.push_back(global_pTimeframeWindow);
	//if (global_pSPITextWindow) wndparampointers.push_back(global_pSPITextWindow); //never store nor delete the pointer when using SPITEXT_NOPOSTQUITMESSAGE

	//2022nov18, spi, begin
	/*
	myclassname = global_szWindowClass;
	myclassname += L"VOROGUI";
	//CFirstWindow* pFirstWindow = DBG_NEW CFirstWindow(L"myfirstwindowtitle", hInstance, myclassname.c_str(),
	CVoroguiWindow* pVoroguiWindow = DBG_NEW CVoroguiWindow(L"myvoroguiwindowtitle", hInstance, myclassname.c_str(),
		global_pSynth, wndparampointers, global_dib, global_imagefilenames, global_voroguiclustermode, //2021nov29, spi, global_voroguiclustermode added
		global_x, global_y, global_xwidth, global_yheight,
		global_alpha, global_titlebardisplay, 0);
	pVoroguiWindow->Create();
	pVoroguiWindow->Show();
	//spi, begin
	//2018apr24, frames-batch modif
	global_pVoroguiWindow = pVoroguiWindow;
	//spi, end
	*/
	if (1) //with vorogui
	//if (0) //for no vorogui 
	{
		myclassname = global_szWindowClass;
		myclassname += L"VOROGS";  //L"VOROGUI";//L"VOROGS"; //vorogs for voro gui surface //formerly L"VOROGUI";
		CVorogsWindow* pVorogsWindow = DBG_NEW CVorogsWindow(L"myvorogswindowtitle", hInstance, myclassname.c_str(),
			//2024fev04, spi, begin
			//global_pSynth, wndparampointers, global_dib, global_imagefilenames, global_voroguiclustermode, //2021nov29, spi, global_voroguiclustermode added
			&(poly.allocator.voiceData[0].synth), wndparampointers, global_dib, global_imagefilenames, global_voroguiclustermode, //2021nov29, spi, global_voroguiclustermode added
			//2024fev04, spi, end
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		pVorogsWindow->Create();
		pVorogsWindow->Show();
		//spi, begin
		//2018apr24, frames-batch modif
		global_pVorogsWindow = pVorogsWindow;
		//spi, end
	}

	if(1)
	//if(0) //2024fev04, spi
	{
		//if (0)
		if(1) //2022dec22, spi, to test
		{
			wndparampointers.clear();
		}
		myclassname = global_szWindowClass;
		myclassname += L"VOROSS"; //L"VOROGUI"; //L"VOROSS"; //voross for voro slide show
		//CFirstWindow* pFirstWindow = DBG_NEW CFirstWindow(L"myfirstwindowtitle", hInstance, myclassname.c_str(),
		CVorossWindow* pVorossWindow = DBG_NEW CVorossWindow(L"myvorosswindowtitle", hInstance, myclassname.c_str(),
			//global_pSynth, wndparampointers, global_dib, global_imagefilenames, global_vorossclustermode, //2022dec22, spi, global_vorossclustermode added
			NULL, wndparampointers, global_dib, global_imagefilenames, global_vorossclustermode, //2022dec22, spi, global_vorossclustermode added
			global_x, global_y, global_xwidth, global_yheight,
			global_alpha, global_titlebardisplay, 0);
		pVorossWindow->Create();
		pVorossWindow->Show();
		global_pVorossWindow = pVorossWindow;

	}
	//2022nov18, spi, end


	//// pump messages:
	MSG  msg;
	HACCEL hAccelTable;
	int status;
	if (global_acceleratoractive)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPIWAVWIN32));
	}
	else
	{
		hAccelTable = NULL;
	}
	while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (status == -1) 
		{
			// handle the error, break
			break;
		}
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	////////////////////
	//terminate portmidi
	////////////////////
	global_active = false;
	if (!global_inputmididevicename.empty())
	{
		Pm_Close(global_pPmStreamMIDIIN);
	}
	if (!global_outputmididevicename.empty())
	{
		Pm_Close(global_pPmStreamMIDIOUT);
	}
	if (!global_inputmididevicename.empty())
	{
		Pt_Stop();
	}
	if ((!global_inputmididevicename.empty()) || (!global_outputmididevicename.empty()))
	{
		Pm_Terminate();
	}

	/////////////////////
	//terminate portaudio
	/////////////////////
	//mySPIAudioDevice.
	mySPIAudioDevice.global_err = Pa_StopStream(mySPIAudioDevice.global_stream);
	if (mySPIAudioDevice.global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Error stoping stream: %s\n", Pa_GetErrorText(mySPIAudioDevice.global_err));
		MessageBoxA(0, errorbuf, 0, MB_ICONERROR);
		return 1;
	}
	mySPIAudioDevice.global_err = Pa_CloseStream(mySPIAudioDevice.global_stream);
	if (mySPIAudioDevice.global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Error closing stream: %s\n", Pa_GetErrorText(mySPIAudioDevice.global_err));
		MessageBoxA(0, errorbuf, 0, MB_ICONERROR);
		return 1;
	}
	Pa_Terminate();

	/////////////////////
	//free tonic
	/////////////////////
	//global_RingBufferWriter.release();
	//global_RingBufferWriter.reset();
	if (global_pSynth)
	{
		delete global_pSynth;
	}

	//ShellExecute with nShowCmd 0 sends a WM_ACTIVATEAPP so we must put it here 
	//before deleting CWindowTransparent derived classes
	if (!global_begin.empty()) ShellExecuteA(NULL, "open", global_end.c_str(), "", NULL, 0);

	///////////////////////
	//free memory
	///////////////////////
	if (global_pTimeframeWindow) delete global_pTimeframeWindow;
	if (global_pImageWindow) delete global_pImageWindow;
	if (global_pSpectrum24bitWindow) delete global_pSpectrum24bitWindow;
	if (global_pSpectrumWindow) delete global_pSpectrumWindow;
	if (global_pVorogsWindow) delete global_pVorogsWindow;; //delete pVoroguiWindow;
	if (global_pTextWindow) delete global_pTextWindow;
	if (global_pSPITextWindow) delete global_pSPITextWindow;
	
	//2022nov18, spi, begin
	if (global_pVorossWindow) delete global_pVorossWindow;
	//2022nov18, spi, end

	//smart pointer
	if(spitextwindow_vector.size()>1)
	{
		for (int i=0; i< spitextwindow_vector.size(); i++)
		{
			spitextwindow_vector[i].reset();
		}
		
	}
	//smart pointer
	if (voroguispitextwindow_vector.size() > 1)
	{
		for (int i = 0; i < spitextwindow_vector.size(); i++)
		{
			spitextwindow_vector[i].reset();
		}

	}

	
	/*
	delete pSecondWindow;
	delete pFirstWindow;
	*/
	if (global_pBlackWindow) delete global_pBlackWindow;
	if(global_dib) FreeImage_Unload(global_dib);
	if(global_quantizeddib) FreeImage_Unload(global_quantizeddib);
	DeleteObject(global_hFont);
	DeleteObject(global_hFontHuge);

	return msg.wParam;
}



