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

#include "CWindowTransparent.h"
#include "CWindowMultimonitor.h"

#include <string>
#include "CParameter.h"
#include <map>
#include <vector>
#include "CWindowParameter.h"

#include <assert.h>

using namespace std;



CWindowParameter::CWindowParameter(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx)
: CWindowMultimonitor(lpszCaption, lpwndClassEx)
{

}

CWindowParameter::CWindowParameter(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CWindowParameter::CWindowParameter(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowMultimonitor(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CWindowParameter::~CWindowParameter()
{
}

LRESULT CALLBACK CWindowParameter::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);

			//break;
			return 0;
		}
	case WM_KEYDOWN:
		{
			CWindowMultimonitor::wndProc(hwnd, uMsg, wParam, lParam);

			//break;
			return 0;
		}
	default:
		//return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
		return 0;
	}
}

CParameter CWindowParameter::addParameter(string name, float min, float max, float value, ParameterType type)
{
	if (_parameters.find(name) == _parameters.end())
	{
		CParameter param(name, min, max, value, type);
		_parameters[name] = param;
		_orderedParameterNames.push_back(name);
	}
	return _parameters[name];
}

void CWindowParameter::addParameter(CParameter parameter)
{
	string name = parameter.getName();
	_parameters[name] = parameter;
	_orderedParameterNames.push_back(name);
}

//2022nov18, spi, begin
//CParameter CWindowParameter::removeParameter(string name)
void CWindowParameter::removeParameter(string name)
{
	if (_parameters.find(name) == _parameters.end())
	{
		//not found
	}
	else
	{
		//found, therefore proceed to removal of both map entry and vector entry
		_parameters.erase(name);
		//_orderedParameterNames.erase(name);
		//vector<CParameter> returnParams;
		for (std::vector<string>::iterator it = _orderedParameterNames.begin(); it != _orderedParameterNames.end(); it++)
		{
			if (*it == name)
			{
				_orderedParameterNames.erase(it);
				break;
			}
		}
		//return _parameters[*(_orderedParameterNames.begin())]; //2022dec06, spi, on success, just return the first parameter from vector
		return;

	}
	//return _parameters[name]; //2022dec06, spi, when not found, returns the not deleted parameter
	return;
}
//2022nov18, spi, end

vector<CParameter> CWindowParameter::getParameters()
{
	vector<CParameter> returnParams;
	for (std::vector<string>::iterator it = _orderedParameterNames.begin(); it != _orderedParameterNames.end(); it++)
	{
		std::map<string, CParameter>::iterator paramIt = _parameters.find(*it);
		if (paramIt != _parameters.end())
		{
			returnParams.push_back(paramIt->second);
		}
	}
	return returnParams;
}

float CWindowParameter::getParameterValue(string name)
{
	float value = 0.0;
	CParameter param = _parameters[name];
	value=param.getValue();
	return value;
}

float CWindowParameter::getParameterValueRandom(string name)
{
	float randomvalue = 0.0;
	CParameter param = _parameters[name];
	randomvalue = RandomFloat(param.getMin(), param.getMax());
	return randomvalue;
}

CParameter CWindowParameter::getParameter(string name)
{
	CParameter param = _parameters[name];
	return param;
}

void CWindowParameter::setParameter(string name, float value)
{
	if (_parameters.find(name) != _parameters.end()) 
	{
		CParameter & param = _parameters[name];
		param.setValue(value);
		//now notify derived class to update its member variable if needed
		UpdateParameter(name);
	}
	else
	{
		assert(false);
	}

}
