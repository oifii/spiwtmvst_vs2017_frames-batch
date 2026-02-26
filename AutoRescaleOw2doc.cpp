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

#include "oifiilib.h" //note: oifiilib.lib/.dll is an MFC extension and resource DLL
#include "AutoRescaleOw2doc.h"
#include "FreeImagePlus.h"

IMPLEMENT_DYNCREATE(CAutoRescaleOW2Doc, COW2Doc)


/*
BEGIN_MESSAGE_MAP(CAutoRescaleOW2Doc, COW2Doc)
	//{{AFX_MSG_MAP(CAutoRescaleOW2Doc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAutoRescaleOW2Doc, COW2Doc)
	//{{AFX_DISPATCH_MAP(CAutoRescaleOW2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// COW2Doc construction/destruction

CAutoRescaleOW2Doc::CAutoRescaleOW2Doc(HWND hwnd) : COW2Doc(hwnd)
{
}

CAutoRescaleOW2Doc::CAutoRescaleOW2Doc() : COW2Doc()
{
}

CAutoRescaleOW2Doc::~CAutoRescaleOW2Doc()
{	
}

BOOL CAutoRescaleOW2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CreateSettingsAndInitFromAppDefault())
		return FALSE;

	/* //spi, 2016, unicode
	if (m_parentHWND == NULL) //spi 2014
	{
		if (!COWDocument::OnOpenDocument(lpszPathName))
			return FALSE; //error, return safely
	}
	CLogDocument* pLogDocument = NULL;
	if (m_parentHWND == NULL) //spi 2014
	{
		pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();
	}
	char* pszFileExtension = strrchr((char*)lpszPathName, '.');
	//pszFileExtension = _strupr( pszFileExtension );
	if (pszFileExtension != NULL && (strcmp(pszFileExtension, ".OW2") == 0 || strcmp(pszFileExtension, ".ow2") == 0))
	{
		if (m_parentHWND == NULL) pLogDocument->AddText("Opening file as a 2D World Document (.ow2)\r\n");
		return OpenFileAs2DWorldDocument(lpszPathName);
	}
	*/

	//try to load file as a pixelset
	m_pPixelset = OpenFileAsPixelset(lpszPathName);
	//if (m_pPixelset) strcpy(m_pszPixelsetPathName, lpszPathName);
	if (m_pPixelset) strcpy(m_pszPixelsetPathName, (LPCSTR)lpszPathName); //spi, 2016, unicode

	if (m_pPixelset == NULL) return FALSE;

	/*
	if (m_pPixelset == NULL)
	{
		//try to load file as a pointset
		m_pPointset = OpenFileAsPointset(lpszPathName);

		if (m_pPointset)
		{
			strcpy(m_pszPointsetPathName, lpszPathName);
			//build TIN
			BOOL bResult = m_pPointset->BuildTINAndComputeStatistics();
			if (bResult)
			{
				m_pViewDataSettings->bDrawPointset = TRUE;
			}
		}
	}

	if (m_pPixelset == NULL && m_pPointset == NULL)
	{
		//load segments (fusion tree)
		m_pSegmentset = OpenFileAsSegmentset(lpszPathName);
		if (m_pSegmentset) strcpy(m_pszSegmentsetPathName, lpszPathName);

	}

	if (m_pPixelset == NULL && m_pPointset == NULL && m_pSegmentset == NULL)
	{
		//load classes definition 
		m_pClassset = OpenFileAsClassset(lpszPathName);
		if (m_pSegmentset) strcpy(m_pszClasssetPathName, lpszPathName);
	}
	*/
	return TRUE;
}

COWPixelset* CAutoRescaleOW2Doc::OpenFileAsPixelset(LPCTSTR lpszPathName) 
{
	//::MessageBoxA(NULL,"yo", "yo yo", MB_OK);

	COWPixelset* pPixelset = NULL;

 	//CLogDocument* pLogDocument = NULL; //spi 2014
	//if(m_parentHWND==NULL) pLogDocument = ((COIIIApp*)AfxGetApp())->GetLogDocument();


	//1) if file is a raster image file
 	char pszMessage[1024];
	char pszCharBuf[1024];
	//strcpy(pszCharBuf, lpszPathName);
	CStringA myStringA(lpszPathName); //spi, 2016, unicode
	strcpy(pszCharBuf, myStringA); //spi, 2016, unicode

	//int iType;
	//accusoft supported raster file format
	pPixelset = DEBUG_NEW COWPixelset(); // DBG_NEW COWPixelset();
	//pPixelset = DBG_NEW COWPixelset(); // DBG_NEW COWPixelset();

	//wsprintf(pszMessage, "Loading pixelset ... "); //spi, 2016, unicode
	fipWinImage myfipWinImage;
	BOOL bResult=myfipWinImage.load(pszCharBuf);
	if(bResult==FALSE)
	{
		//if(m_parentHWND==NULL) pLogDocument->AddText("error\r\nERROR loading pixelset.\r\n");
		delete pPixelset;
		return NULL;
	}
	//if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");

	//2021nov29, spi, begin
	//BITMAPINFOHEADER* pBITMAPINFOHEADER=myfipWinImage.getInfoHeader(); //original
	BITMAPINFOHEADER* pBITMAPINFOHEADER = (BITMAPINFOHEADER*) myfipWinImage.getInfoHeader(); //original
	/*
	BITMAPINFOHEADER myBITMAPINFOHEADER;
	BITMAPINFOHEADER* pBITMAPINFOHEADER = &myBITMAPINFOHEADER;
	*pBITMAPINFOHEADER = *(myfipWinImage.getInfoHeader());
	*/
	//2021nov29, spi, end
	int new_width = pBITMAPINFOHEADER->biWidth - pBITMAPINFOHEADER->biWidth%4;
	int new_height = pBITMAPINFOHEADER->biHeight;
	myfipWinImage.rescale(new_width, new_height, FILTER_BICUBIC);
	//2021nov29, spi, begin
	//pBITMAPINFOHEADER = myfipWinImage.getInfoHeader(); //original
	pBITMAPINFOHEADER = (BITMAPINFOHEADER*)myfipWinImage.getInfoHeader();
	//*pBITMAPINFOHEADER= *(myfipWinImage.getInfoHeader());
	//2021nov29, spi, end

	pPixelset->AllocDIB(pBITMAPINFOHEADER);
	pPixelset->UpdateDIBFrom(pBITMAPINFOHEADER);


	//set the view parameter directly into this document's m_pViewDataSettings 
	m_pViewDataSettings->bDrawPixelset = TRUE;

	if(pPixelset->IsDIBPresent() && !(pPixelset->IsBIIPresent()) )
	{
		//if(m_parentHWND==NULL) pLogDocument->AddText("     Allocating pixelset buffer ... ");
		if(pPixelset->AllocBII(pPixelset->GetDIBPointer())==FALSE)
		{
			//if(m_parentHWND==NULL) pLogDocument->AddText("Error\r\nError, not enough memory to alloc BII buffer, can't perform texture extraction\r\n");
			delete pPixelset;
			return NULL; //error
		}
		pPixelset->UpdateBIIFrom(pPixelset->GetDIBPointer());
		//if(m_parentHWND==NULL) pLogDocument->AddText("done.\r\n");
	}
		
	return pPixelset;
	/*
    char* pszFileExtension = strrchr((char*)lpszPathName,'.');
	pszFileExtension = _strupr( pszFileExtension );
 	if(pszFileExtension!=NULL  &&  strcmp(pszFileExtension, ".RAW") == 0)
	{
		if(m_parentHWND==NULL) pLogDocument->AddText("Sorry, RAW format not supported yet.\r\n");
		delete pPixelset;
		return NULL; //error
	}

	return NULL;
	*/
	//return COW2Doc::OpenFileAsPixelset(lpszPathName);
}

BOOL CAutoRescaleOW2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return TRUE;
}