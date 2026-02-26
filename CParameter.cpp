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

#include <string>
using namespace std;

#include "CParameter.h"

#include <assert.h>




CParameter::CParameter()
{
	_name = "";
	_min = 0.0;
	_max = 1.0;
	_value = 0.5;
	_type = ParameterTypeContinuous;
}

CParameter::CParameter(string name, float min, float max, float value, ParameterType type)
{
	//2021nov29, spi, begin
	if (value < min) value = min;
	if (value > max) value = max;
	//2021nov29, spi, end
	_name = name;
	_min = min;
	_max = max;
	_value = value;
	_type = type;
}

CParameter::~CParameter()
{
}

