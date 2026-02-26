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
#if !defined __CWINDOWPARAMETER_H__
#define __CWINDOWPARAMETER_H__


class CWindowParameter : public CWindowMultimonitor
{
private:
	std::map<string, CParameter> _parameters;
	std::vector<string> _orderedParameterNames;

public:
	CWindowParameter(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CWindowParameter(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	CWindowParameter(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int xwidth = CW_USEDEFAULT, int yheight = CW_USEDEFAULT,
		BYTE alpha = 150, int titlebardisplay = 1, int menubardisplay = 1);
	virtual ~CWindowParameter();
	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	virtual void dummy() = 0;

	CParameter addParameter(string name, float min, float max, float value, ParameterType type = ParameterTypeContinuous);
	void addParameter(CParameter parameter);
	//2022nov18, spi, begin
	//CParameter removeParameter(string name);
	void removeParameter(string name);
	//2022nov18, spi, end
	vector<CParameter>  getParameters();
	float getParameterValue(string name);
	float getParameterValueRandom(string name);
	CParameter getParameter(string name);
	void setParameter(string name, float value);
	virtual void UpdateParameter(string name)=0; //to update derived class' member variable
};

#endif
