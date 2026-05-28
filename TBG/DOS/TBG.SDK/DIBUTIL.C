// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   dibutil.c
//
//  PURPOSE:  Routines for processing DIBs.
//
//  FUNCTIONS:
//    DIBError          - Displays a message box with error message
//    FindDIBBits       - Sets a pointer to the DIB bits
//    DIBHeight         - Gets the height of the DIB
//    PaletteSize       - Calculates the buffer size required by a palette
//    DIBNumColors      - Calculates number of colors in the DIB's color table
//    CreateDIBPalette  - Creates a palette from a DIB
//    PaintBitmap       - Paints a bitmap into a DC
//    FillColorTable    - Fill color table of a DIB using system palette entries
//
//  COMMENTS:
//    These routines are taken from the WINCAP32 sample
//

#include <windows.h>
#include <windowsx.h>

#include <stdio.h>
#include <math.h>

#include "dibutil.h"

void GetSizeBMP(LPSBMP lpsbmp);


//
//  FUNCTION: DIBError(int)
//
//  PURPOSE:  Displays a messasge box with error message
//
//  PARAMETERS:
//    nErrNo    - 
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//
//
void DIBError(int nErrNo)
{
    char szBuffer[40];

    if ((nErrNo < ERR_MIN) || (nErrNo >= ERR_MAX))
        nErrNo = ERR_UNDEFINEDERROR;

    sprintf(szBuffer, "Err: %d", nErrNo);
    MessageBox(NULL, szBuffer, "Operation BMP Resource ERROR", MB_OK | MB_ICONHAND);
}


//
//  FUNCTION: FindDIBBits(LPSTR)
//
//  PURPOSE:  Calculates the address of the DIB's bits and returns a
//            pointer to the DIB bits.
//
//  PARAMETERS:
//    LPSTR    - pointer to the DIB bits in packed-DIB memory block
//
//  RETURN VALUE:
//    TRUE  - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//
//
//
LPSTR FindDIBBits(LPSTR lpDIB)
{
   return (lpDIB + *(LPDWORD)lpDIB + PaletteSize(lpDIB));
}

//
//  FUNCTION: DIBHeight(LPSTR)
//
//  PURPOSE:  gets the height of the DIB from the BITMAPINFOHEADER height 
//            field if it is a Windows 3.0-style DIB or from the 
//            BITMAPCOREHEADER height field if it is an OS/2-style DIB.
//
//  PARAMETERS:
//    lpDIB      - pointer to packed-DIB memory block
//
//  RETURN VALUE:
//    The height of the DIB
//
//  COMMENTS:
//
//
//
DWORD DIBHeight(LPSTR lpDIB)
{
   LPBITMAPINFOHEADER   lpbih;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER   lpbmc;  // pointer to an OS/2-style DIB

   // point to the header (whether OS/2 or Win 3.0

   lpbih = (LPBITMAPINFOHEADER)lpDIB;
   lpbmc = (LPBITMAPCOREHEADER)lpDIB;

    // return the DIB height if it is a Win 3.0 DIB
    if (lpbih->biSize == sizeof(BITMAPINFOHEADER))
        return lpbih->biHeight;
    else  // it is an OS/2 DIB, so return its height
        return (DWORD)lpbmc->bcHeight;
}

      
//
//  FUNCTION: PaletteSize(LPSTR)
//
//  PURPOSE: 
//    Gets the size required to store the DIB's palette by multiplying the 
//    number of colors by the size of an RGBQUAD (for a Windows 3.0-style
//    DIB) or by the size of an RGBTRIPLE (for an OS/2-style DIB).
//
//  PARAMETERS:
//    lpDIB      - pointer to packed-DIB memory block
//
//  RETURN VALUE:
//    The size of the color palette of the DIB
//
//  COMMENTS:
//
//
//
WORD PaletteSize(LPSTR lpDIB)
{
    // calculate the size required by the palette
    if (IS_WIN30_DIB(lpDIB))
		  return (WORD)(DIBNumColors(lpDIB) * sizeof(RGBQUAD));
	 else
		  return (WORD)(DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}


//
//  FUNCTION: DIBNumColors(LPSTR)
//
//  PURPOSE:  Calculates the number of colors in the DIB's color table
//            by finding the bits per pixel for the DIB (whether Win3.0 or 
//            OS/2-style DIB). If bits per pixel is 1: colors = 2, if 4: 
//            colors = 16, if 8: colors = 256, if 16 or 24, no colors in 
//            color table.
//
//  PARAMETERS:
//    lpDIB      - pointer to packed-DIB memory block
//
//  RETURN VALUE:
//    The number of colors in the color table
//
//  COMMENTS:
//
//
//
WORD DIBNumColors(LPSTR lpDIB)
{
    WORD wBitCount;  // DIB bit count

    // If this is a Windows-style DIB, the number of colors in the
    // color table can be less than the number of bits per pixel
    // allows for (i.e. lpbih->biClrUsed can be set to some value).
    // If this is the case, return the appropriate value.
    

    if (IS_WIN30_DIB(lpDIB))
    {
        DWORD dwClrUsed;

        dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
        if (dwClrUsed)

        return (WORD)dwClrUsed;
    }

    // Calculate the number of colors in the color table based on
    // the number of bits per pixel for the DIB.
    
    if (IS_WIN30_DIB(lpDIB))
        wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
    else
        wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;

    // return number of colors based on bits per pixel

    switch (wBitCount)
    {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            return 0;
    }
}


//
//  FUNCTION: CreateDIBPalette(HDIB)
//
//  PURPOSE: Creates a palette from a DIB by allocating memory for the
//           logical palette, reading and storing the colors from the DIB's 
//           color table into the logical palette, creating a palette from 
//           this logical palette, and then returning the palette's handle. 
//           This allows the DIB to be displayed using the best possible 
//           colors (important for DIBs with 256 or more colors).
//
//  PARAMETERS:
//    hDIB        - specifies the DIB
//
//  RETURN VALUE:
//    A handle to the palette
//
//  COMMENTS:
//
//
//
HPALETTE CreateDIBPalette(HDIB hDIB)
{
    LPLOGPALETTE        lpPal;          // pointer to a logical palette
    HANDLE              hLogPal;        // handle to a logical palette
    HPALETTE            hPal = NULL;    // handle to a palette
	 int                 i;              // loop index
	 WORD                wNumColors;     // number of colors in color table
	 LPSTR               lpbih;          // pointer to packed-DIB
	 LPBITMAPINFO        lpbi;           // pointer to BITMAPINFO structure (Win3.0)
	 LPBITMAPCOREINFO    lpbmc;          // pointer to BITMAPCOREINFO structure (OS/2)
	 BOOL                fWinStyleDIB;   // Win3.0 DIB?

	 // if handle to DIB is invalid, return NULL
	 if (!hDIB)
		  return NULL;

    // lock DIB memory block and get a pointer to it
    lpbih = GlobalLock(hDIB);

    // get pointer to BITMAPINFO (Win 3.0)
    lpbi = (LPBITMAPINFO)lpbih;

    // get pointer to BITMAPCOREINFO (OS/2 1.x)
    lpbmc = (LPBITMAPCOREINFO)lpbi;

    // get the number of colors in the DIB
    wNumColors = DIBNumColors(lpbih);

    // is this a Win 3.0 DIB?
    fWinStyleDIB = IS_WIN30_DIB(lpbih);
    if (wNumColors)
    {
        // allocate memory block for logical palette
        hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) +
                sizeof(PALETTEENTRY) * wNumColors);

        // if not enough memory, clean up and return NULL
        if (!hLogPal)
        {
            GlobalUnlock(hDIB);
            return NULL;
        }

        // lock memory block and get pointer to it
        lpPal = (LPLOGPALETTE)GlobalLock(hLogPal);

        // set version and number of palette entries
        lpPal->palVersion = PALVERSION;
		  lpPal->palNumEntries = wNumColors;

        // store RGB triples (if Win 3.0 DIB) or RGB quads (if OS/2 DIB)
        // into palette        
        for (i = 0; i < wNumColors; i++)
        {
            if (fWinStyleDIB)
            {
                lpPal->palPalEntry[i].peRed = lpbi->bmiColors[i].rgbRed;
                lpPal->palPalEntry[i].peGreen = lpbi->bmiColors[i].rgbGreen;
                lpPal->palPalEntry[i].peBlue = lpbi->bmiColors[i].rgbBlue;
                lpPal->palPalEntry[i].peFlags = 0;
            }
            else
            {
                lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
                lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
                lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
                lpPal->palPalEntry[i].peFlags = 0;
            }
        }

        // create the palette and get handle to it
        hPal = CreatePalette(lpPal);

        // if error getting handle to palette, clean up and return NULL
        if (!hPal)
        {
            GlobalUnlock(hLogPal);
            GlobalFree(hLogPal);
            return NULL;
        }
    }

    // clean up
    GlobalUnlock(hLogPal);
    GlobalFree(hLogPal);
    GlobalUnlock(hDIB);

    // return handle to DIB's palette
    return hPal;
}


//
//  FUNCTION: PaintBitmap(HDC, LPRECT, HBITMAP, LPRECT, HPALETTE)
//
//  PURPOSE:  Paint a device-dependent bitmap (DDB) to the specified
//            location on the specified device. 
//
//  PARAMETERS:
//    hDC           - DC to perform output to
//    lpDCRect      - rectangle on DC to perform output to
//    hDDB          - handle to device-dependent bitmap (DDB)
//    lpDDBRect     - rectangle of DDB to output into lpDCRect
//    hPal          - handle to the palette to use with hDDB
//
//  RETURN VALUE:
//    TRUE if bitmap was drawn, FALSE otherwise
//
//  COMMENTS:
//    Painting routine for a DDB.  Calls BitBlt() or StretchBlt() to paint 
//    the DDB.  The DDB is output to the specified DC, at the coordinates 
//    given in lpDCRect.  The area of the DDB to be output is given by 
//    lpDDBRect.  The specified palette is used.
//
//    This function always selects the palette as background. Before
//    calling this function, be sure your palette is selected to desired
//    priority (foreground or background).
//
//
BOOL PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB,  LPRECT lpDDBRect, 
        HPALETTE hPal)
{
    HDC         hMemDC;            // Handle to memory DC
    HBITMAP     hOldBitmap;        // Handle to previous bitmap
    HPALETTE    hOldPal1 = NULL;   // Handle to previous palette
    HPALETTE    hOldPal2 = NULL;   // Handle to previous palette
	 BOOL        fSuccess;          // Success/fail flag

    // Create a memory DC
    hMemDC = CreateCompatibleDC(hDC);  
    if (!hMemDC)
        return FALSE;

    // If we have a palette, select and realize it
    if (hPal)
    {
        hOldPal1 = SelectPalette(hDC, hPal, FALSE); 
        RealizePalette(hDC);
        hOldPal2 = SelectPalette(hMemDC, hPal, FALSE);        
    }

    // Select bitmap into the memory DC
    hOldBitmap = SelectObject(hMemDC, hDDB);

    // Make sure to use the stretching mode best for color pictures
    SetStretchBltMode(hDC, COLORONCOLOR);

    // Determine whether to call StretchBlt() or BitBlt()
    if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDDBRect)) &&
            (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect)))
        fSuccess = BitBlt(hDC,
                lpDCRect->left, 
                lpDCRect->top,
                RECTWIDTH(lpDCRect), 
                RECTHEIGHT(lpDCRect), 
                hMemDC, 
                lpDDBRect->left,
                lpDDBRect->top, 
                SRCCOPY);
    else
        fSuccess = StretchBlt(hDC, 
                lpDCRect->left,  
                lpDCRect->top, 
                RECTWIDTH(lpDCRect), 
                RECTHEIGHT(lpDCRect),
                hMemDC, 
                lpDDBRect->left, 
                lpDDBRect->top,  
                RECTWIDTH(lpDDBRect),
                RECTHEIGHT(lpDDBRect), 
                SRCCOPY);

    // Clean up
    SelectObject(hMemDC, hOldBitmap);

    if (hOldPal1)
        SelectPalette(hDC, hOldPal1, FALSE);

    if (hOldPal2)
        SelectPalette(hMemDC, hOldPal2, FALSE);

    DeleteDC(hMemDC);

    // Return with success/fail flag
    return fSuccess;
}

//
//  FUNCTION: FillColorTable(HWND, LPRGBQUAD, HPALETTE, int)
//
//  PURPOSE:  Fill the color table of a DIB using entries from a 
//            logical palette
//
//  PARAMETERS:
//    hWnd          - Handle to application window
//    lpColorTable  - Pointer to the color table
//    hPal          - Handle to palette for filling color table
//    nColorsNeeded - Number of colors needed in color table
//
//  RETURN VALUE:
//    The number of color table entries filled.
//
//  COMMENTS: If a palette is provided, it must be an identity
//    palette with entries 0 - 9 and 246 - 255 containing the static
//    VGA colors for the color table of 1, 4 and 8bpp images to be
//    correct.
//
//
int FillColorTable(HWND hWnd, LPRGBQUAD lpColorTable, HPALETTE hPal,
    int nColorsNeeded)
{                                                      
    int nPalEntries = 0;         // number of entries in palette
    HLOCAL     hLogPal;          // handle to temporary logical palette
    LOGPALETTE *pLogPal;         // pointer to temporary logical palette
    LPPALETTEENTRY lpPalEntries; // pointer to palette entries in log palette    
    int nColors;                 // number of colors copied into color table
    int i;
    HDC hdc;  
   
    // be sure input parameters are valid
    if (!lpColorTable)    
        return 0;

    if (nColorsNeeded < 2 || nColorsNeeded > 256)
        return 0;

    if (nColorsNeeded == 2)
    {
        // do color table for 1bpp
        // the first RGBQUAD is already 0,0,0,0 so point to second RGBQUAD
        LPRGBQUAD lpCT = lpColorTable + 1;
        lpCT->rgbRed = lpCT->rgbGreen = lpCT->rgbBlue  = 255;  
        return 2;
    }
   
    hdc = GetDC(hWnd);

    if (hPal)   
        // get number of entries in palette
        GetObject(hPal, sizeof(int), (LPVOID)&nPalEntries);
    else       
        // use 256 entries in color table unless running VGA
        nPalEntries = min(256, GetDeviceCaps(hdc, NUMCOLORS));
                 
    // create a logical palette to store the palette entries in
    // so we can assign the color values to the color table quickly
    hLogPal = LocalAlloc(LHND,
                         sizeof(LOGPALETTE) +
                         (sizeof(PALETTEENTRY) * nPalEntries));
    
    // get a pointer to the logical palette                     
    pLogPal = (LOGPALETTE *)LocalLock(hLogPal);        
   
    // get a pointer to the palette entries
    lpPalEntries = (LPPALETTEENTRY)(pLogPal->palPalEntry);
                 
    // retrieve the palette entries
    if (hPal)
        // use the provided palette
        GetPaletteEntries(hPal, 0, nPalEntries, lpPalEntries);
    else
        // use the system palette
        GetSystemPaletteEntries(hdc, 0, nPalEntries, lpPalEntries);

    // number of colors can't exceed size of logical palette
    nColors = min(nPalEntries, nColorsNeeded);
    if (nColors < 16)
    {
        OutputDebugString("FillColorTable: Palette is too small\r\n");
        return 0;
    }
    
    if (nColors == 16)
    {
        // get first 8 colors
        for (i = 0; i < 8; i++, lpColorTable++, lpPalEntries++)
        {
            lpColorTable->rgbRed   = lpPalEntries->peRed;     
            lpColorTable->rgbGreen = lpPalEntries->peGreen;
            lpColorTable->rgbBlue  = lpPalEntries->peBlue;
        }

        // get the last 8 colors
        for (lpPalEntries += nPalEntries - 16; i < 16;
                i++, lpColorTable++, lpPalEntries++)
        {
            lpColorTable->rgbRed   = lpPalEntries->peRed;     
            lpColorTable->rgbGreen = lpPalEntries->peGreen;
            lpColorTable->rgbBlue  = lpPalEntries->peBlue;      
        }
    }
    else
    {    
        // fill the color table as much as we can                                                           
        for (i = 0; i < nColors; i++, lpColorTable++, lpPalEntries++)
        {
            lpColorTable->rgbRed   = lpPalEntries->peRed;     
            lpColorTable->rgbGreen = lpPalEntries->peGreen;
            lpColorTable->rgbBlue  = lpPalEntries->peBlue;      
        }
    }

    // clean up
    GlobalUnlock(hLogPal);
    GlobalFree(hLogPal);
    ReleaseDC(hWnd, hdc);
 
    return nColors;
}

//
//  FUNCTION: RemoveDIBSection()
//
//  PURPOSE: Removes the current DIB section and resets global variables
//
//  PARAMETERS:
//    none.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//

void RemoveDIBSection(LPSBMP lpsbmp)
{
    if (lpsbmp->hDIBInfo)
    {
        // free old DIB header
        GlobalFree(lpsbmp->hDIBInfo);
        lpsbmp->hDIBInfo = NULL;
    }

    if (lpsbmp->hBitmap)
    {
        // remove existing DIB section
        DeleteObject(lpsbmp->hBitmap);
        lpsbmp->hBitmap = NULL;
    }

    // bits are gone
    lpsbmp->lpvBits = NULL;

    if (lpsbmp->hPalette)
    {
        // remove existing palette object
        DeleteObject(lpsbmp->hPalette);
        lpsbmp->hPalette = NULL;
    }
             
}


// Dib Header Marker - used in writing DIBs to files
#define DIB_HEADER_MARKER   ((WORD)('M' << 8) | 'B')

//
//  FUNCTION: LoadDIBSection(HWND, LPSTR)
//
//  PURPOSE: Loads the specified DIB from a file, and reads it into the buffer
//    created by CreateDIBSection.
//
//  PARAMETERS:
//    hwnd          - specifies window for display context
//    lpFileName    - specifies the file to load a DIB from
//
//  RETURN VALUE:
//    A handle to a bitmap, or NULL if unsuccessful.
//
//  COMMENTS:
//    The routines in this sample were not all written to handle OS/2 DIBs.
//    This function will reject any file that is not a Windows DIB.
//
//
HBITMAP LoadDIBSection(HWND hwnd, LPSTR lpFileName, LPSBMP lpsbmp)
{
    HDC                 hdc;
    HANDLE              hFile;
    BITMAPFILEHEADER    bmfHeader;
    UINT                nNumColors;
    HDIB                hDIBNew;
    LPBITMAPINFOHEADER  lpbih;
    DWORD               dwBytesRead;

    // Set the cursor to a hourglass, in case the loading operation
    // takes more than a sec, the user will know what's going on.
    SetCursor(LoadCursor(NULL, IDC_WAIT));

    // open the DIB file
    hFile = CreateFile(lpFileName,
                       GENERIC_READ,
                       FILE_SHARE_READ,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        DIBError(ERR_FILENOTFOUND);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return NULL;
    }

    // read the BITMAPFILEHEADER from our file

    if (!ReadFile(hFile,
                  &bmfHeader,
                  sizeof(BITMAPFILEHEADER),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != sizeof(BITMAPFILEHEADER))
    {
        DIBError(ERR_READ);
        goto ErrExit;
    }

    if (bmfHeader.bfType != 0x4d42)  // 'BM'
    {
        DIBError(ERR_NOT_DIB);
        goto ErrExit;
    }

    // Allocate memory for header & color table.
    hDIBNew = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

    if (!hDIBNew)
    {
        DIBError(ERR_MEMORY);
        goto ErrExit;
    }

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBNew);
    if (!lpbih)
    {
        DIBError(ERR_LOCK);
        goto ErrExitFree;
    }

    // read the BITMAPINFOHEADER
    if (!ReadFile(hFile,
                  lpbih,
                  sizeof(BITMAPINFOHEADER),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != sizeof(BITMAPINFOHEADER))
    {
        DIBError(ERR_READ);
        goto ErrExitUnlock;
    }

    // Check to see that it's a Windows DIB -- an OS/2 DIB would cause
    // problems with some of the functions in this sample since the fields
    // in the header are different and color table entries are smaller.
    // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.
    if (lpbih->biSize != sizeof(BITMAPINFOHEADER))
    {
        DIBError(ERR_NOT_DIB);
        goto ErrExitUnlock;
    }

    // check to see if it's a format we support
    switch (lpbih->biBitCount)
    {
        case 1:
        case 4:
        case 8:
        case 16:
        case 24:
        case 32:

            // the format is supported so continue
            break;

        default:
            // the format isn't supported so fail
            DIBError(ERR_UNSUPPORTEDFORMAT);
            goto ErrExitUnlock;
    }

    // Now determine the size of the color table and read it.  Since the
    // bitmap bits are offset in the file by bfOffBits, do some special
    // processing to make sure the bits directly follow the color table
    if (!(nNumColors = (UINT)lpbih->biClrUsed))
    {
        // color table only for 8 bits per pixel or less

        if (lpbih->biBitCount <= 8)
            nNumColors = 1 << lpbih->biBitCount; // standard size table
    }

    // fill in some default values if they are zero
    if (lpbih->biClrUsed == 0)
        lpbih->biClrUsed = nNumColors;

    if (lpbih->biSizeImage == 0)
        lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth * lpbih->biBitCount)
                                * lpbih->biHeight;

    if (nNumColors > 256)   // very rare, but possible
    {
        HDIB hDIBInfoTemp;

        // get a proper-sized buffer for header and color table
        GlobalUnlock(hDIBNew);
        hDIBInfoTemp = GlobalReAlloc(hDIBNew,
                                     lpbih->biSize + nNumColors * sizeof(RGBQUAD),
                                     0);

        if (!hDIBInfoTemp) // can't resize buffer for loading
        {
            DIBError(ERR_MEMORY);
            goto ErrExitFree;
        }
        else
            hDIBNew = hDIBInfoTemp;

        lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBNew);
        if (!lpbih)
        {
            DIBError(ERR_LOCK);
            goto ErrExitFree;
        }
    }

    // read the color table
    if (!ReadFile(hFile,
                  (LPSTR)(lpbih) + lpbih->biSize,
                  nNumColors * sizeof(RGBQUAD),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != nNumColors * sizeof(RGBQUAD))
    {
        DIBError(ERR_READ);
        goto ErrExitUnlock;
    }

    // we've reached the point of no return (no undo)
    // remove existing DIB section
    RemoveDIBSection(lpsbmp);

    // set global variable to new DIB header
    lpsbmp->hDIBInfo = hDIBNew;

    // create new DIB section
    hdc = GetDC(hwnd);
    lpsbmp->hBitmap = CreateDIBSection(hdc,
                               (LPBITMAPINFO)lpbih,
                               DIB_RGB_COLORS,
                               &(lpsbmp->lpvBits),
                               0,
                               0L);
    ReleaseDC(hwnd, hdc);

    if (!lpsbmp->hBitmap || !lpsbmp->lpvBits)
    {
        DIBError(ERR_CREATEDIBSECTION);

        if (lpsbmp->hBitmap)
            DeleteObject(lpsbmp->hBitmap);

        // create default wash for palette
        if (lpsbmp->bPalDevice)
        {
            HDC hdc;

            hdc = GetDC(hwnd);
            lpsbmp->hPalette = CreateHalftonePalette(hdc);
            ReleaseDC(hwnd, hdc);
        }

        goto ErrExitUnlock;
    }

    // fill the buffer allocated by CreateDIBSection with bitmap bits        
    SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
    if (!ReadFile(hFile,
                  lpsbmp->lpvBits,
                  lpbih->biSizeImage,
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != lpbih->biSizeImage)
    {
        // failed to read the DIB bits into the DIB section so get rid of it
        DIBError(ERR_READ);
        DeleteObject(lpsbmp->hBitmap);
        goto ErrExitUnlock;
    }

    // create a palette for the DIB section from the DIB color table
    lpsbmp->hPalette = CreateDIBPalette(lpsbmp->hDIBInfo);
    // if hPalette is NULL it won't lead to failure condition, so don't check it

    // unlock the DIB header but don't free it!
    GlobalUnlock(lpsbmp->hDIBInfo);

    CloseHandle(hFile);
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    GetSizeBMP(lpsbmp);

    return lpsbmp->hBitmap;

ErrExitUnlock:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call
    GlobalUnlock(hDIBNew);

ErrExitFree:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call
    GlobalFree(hDIBNew);

ErrExit:
    CloseHandle(hFile);
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    return NULL;
}
//
//  FUNCTION: InitDIBSection(HWND)
//
//  PURPOSE: Initialize the bitmap surface and paint the display.
//
//  PARAMETERS:
//    hwnd  - The window displaying the bitmap.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//

void InitDIBSection(HWND hwnd, LPSBMP lpsbmp)
{
    HDC hdc, hdcMem;
    HBITMAP hbmOld;
    HPALETTE hpalOld = NULL;
    HPALETTE hpalOldMem = NULL;
    LPBITMAPINFOHEADER lpbih;

    if (!hwnd || !lpsbmp->hDIBInfo || !lpsbmp->hBitmap)
        return;

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(lpsbmp->hDIBInfo);
    if (!lpbih)
        return;

    lpsbmp->sr.top = lpsbmp->sr.left = 0;
    lpsbmp->sr.right  = lpbih->biWidth;
    lpsbmp->sr.bottom = lpbih->biHeight;

    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);
    hbmOld = SelectObject(hdcMem, lpsbmp->hBitmap);

    if (lpsbmp->hPalette)
    {
        hpalOld = SelectPalette(hdc, lpsbmp->hPalette, FALSE);
        RealizePalette(hdc);
        hpalOldMem = SelectPalette(hdcMem, lpsbmp->hPalette, FALSE);
    }

    PatBlt(hdcMem, 0, 0, lpbih->biWidth, lpbih->biHeight, WHITENESS);
    PatBlt(hdc,    0, 0, lpbih->biWidth, lpbih->biHeight, WHITENESS);

    if (hpalOldMem)
        SelectPalette(hdcMem, hpalOldMem, FALSE);
    if (hpalOld)
        SelectPalette(hdc, hpalOld, FALSE);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
    GlobalUnlock(lpsbmp->hDIBInfo);
}

void GetSizeBMP(LPSBMP lpsbmp){

    LPBITMAPINFOHEADER lpbih;

    if (!lpsbmp->hDIBInfo)
        return;

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(lpsbmp->hDIBInfo);
    if (!lpbih)
        return;

    lpsbmp->sr.top = lpsbmp->sr.left = 0;
    lpsbmp->sr.right  = lpbih->biWidth;
    lpsbmp->sr.bottom = lpbih->biHeight;

    return;
    
}/* GetSizeBMP */
