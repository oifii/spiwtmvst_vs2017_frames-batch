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
#if !defined __CPARAMETER_H__
#define __CPARAMETER_H__

typedef enum{
	ParameterTypeContinuous = 0,
	ParameterTypeToggle
} ParameterType;

class CParameter 
{
private:
	string _name;
	float _min;
	float _max;
	float _value;
	ParameterType _type;

public:
	CParameter();
	CParameter(string name, float min, float max, float value, ParameterType type=ParameterTypeContinuous);
	~CParameter();

	void setName(string name) { _name = name; };
	string getName() { return _name; };

	void setMin(float min) { _min = min; };
	float  getMin() { return _min; };

	void setMax(float max) { _max = max; };
	float getMax() { return _max; };

	void setValue(float value) 
	{ 
		//2021nov29, spi, begin
		if (value < _min) value = _min;
		if (value > _max) value = _max;
		//2021nov29, spi, end
		_value = value; 
	};
	float getValue() { return _value; };

	void setType(ParameterType type) { _type = type; };
	ParameterType  getType() { return _type; };
};

#endif
