#define PALVERSION            0x300
#define HDIB                  HANDLE           // handle to a DIB
#define IS_WIN30_DIB(lpbi)    ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.
#define WIDTHBYTES(bits)      ((((bits) + 31) & ~31) >> 3)
 
// Function prototypes
HPALETTE CreateDIBPalette(HDIB);
void DIBError(int);
DWORD DIBHeight(LPSTR);
WORD DIBNumColors(LPSTR);
LPSTR FindDIBBits(LPSTR);
WORD PaletteSize(LPSTR);
WORD SaveDIB(HDIB, LPVOID, LPSTR);
BOOL PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE); 
int  FillColorTable(HWND, LPRGBQUAD, HPALETTE, int);

typedef
   struct SBMP {
      HBITMAP hBitmap;
      LPVOID lpvBits;
      BOOL bPalDevice;
      HPALETTE hPalette;
      HANDLE hDIBInfo;
      RECT sr;
   }SBMP, *LPSBMP;
   
#define ERR_MIN                         4096
#define ERR_NOT_DIB                     4096
#define ERR_UNSUPPORTEDFORMAT           4097
#define ERR_MEMORY                      4098
#define ERR_READ                        4099
#define ERR_SAVE                        5000
#define ERR_LOCK                        5001
#define ERR_OPEN                        5002
#define ERR_CREATEPAL                   5003
#define ERR_GETDC                       5004
#define ERR_FILENOTFOUND                5005
#define ERR_INVALIDHANDLE               5006
#define ERR_DIBFUNCTION                 5007
#define ERR_CREATEDIBSECTION            5008
#define ERR_FILLCOLORTABLE              5009
#define ERR_WIDTH                       5010
#define ERR_HEIGHT                      5011
#define ERR_STRETCHDIBITS               5012
#define ERR_UNDEFINEDERROR              5013
#define ERR_MAX                         5013
