// password2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

#define CanvasHeight 12
#define CanvasWidth 9
#define SWH 4
#define Squaresize 32
#define emptyslotcolor RGB(128,128,128)

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

COLORREF a[CanvasHeight][CanvasWidth];
int currentshape;
struct xshape {
	bool a[SWH][SWH];
	POINT cor[SWH][SWH];
	POINT rotationcenter;

	COLORREF color;
	xshape & operator = (xshape & y) {
		this->color = y.color;
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j)
				this->a[i][j] = y.a[i][j];
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j)
				this->cor[i][j] = y.cor[i][j];
		return *this;
	}
};
vector<xshape> allshapes;

void reset(xshape & x) {
	int ci = 0, cj = 0;
	for (int i = -1; i >= -SWH; --i) {
		for (int j = 0; j < SWH; ++j) {
			x.cor[ci][cj].x = i;
			x.cor[ci][cj].y = j;
			++cj;
		}
		++ci;
		cj = 0;
	}
}

void initxshape(xshape& x) {
	for (int i = 0; i < SWH; ++i)
		for (int j = 0; j < SWH; ++j)
			x.a[i][j] = 0;
	for (int i = 0; i < SWH; ++i)
		for (int j = 0; j < SWH; ++j)
			x.cor[i][j].x = x.cor[i][j].y = 0;
	x.color = 0;
	x.rotationcenter.x = 1;
	x.rotationcenter.y = 1;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	srand(time(NULL));
	for(int i = 0; i < CanvasHeight; ++i)
		for (int j = 0; j < CanvasWidth; ++j) {
			a[i][j] = emptyslotcolor;
		}
	ifstream cfg("config.txt");
	int sx, sy, n;
	cfg >> n >> sx >> sy;
	for(int i = 0; i < n; ++i){
		xshape tmp;
		initxshape(tmp);
		for(int j = 0; j < sx; ++ j)
			for (int k = 0; k < sy; ++k) {
				cfg >> tmp.a[j][k];
			}
		cfg >> tmp.rotationcenter.x >> tmp.rotationcenter.y;
		tmp.color = RGB(255, 0, 0);
		reset(tmp);
		allshapes.push_back(tmp);
	}
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PASSWORD2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PASSWORD2);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}





//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_PASSWORD2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_PASSWORD2;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
#define IDT_TIMER_DOWN 0x1
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   SetTimer(hWnd,             // handle to main window 
	   IDT_TIMER_DOWN,            // timer identifier 
	   1000,                 // 10-second interval 
	   (TIMERPROC)NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//



POINT boxdimension;

bool stop = false;

bool inrange(int x, int y) {
	if (x >= CanvasHeight)
		return false;
	if (y < 0 || y >= CanvasWidth)
		return false;
	return true;
}
bool occupy(int x, int y) {
	if (x < 0)
		return false;
	if (a[x][y] != emptyslotcolor) {
		return true;
	}
	return false;
}

bool collide(int x, xshape & y) {
	switch (x) {
	case 1:
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j)
				++y.cor[i][j].x;
		break;
	case 2:
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j)
				--y.cor[i][j].y;
		break;
	case 3:
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j) {
				++y.cor[i][j].y;
			}
		break;
	case 4:
		int cx = y.rotationcenter.x;
		int cy = y.rotationcenter.y;
		int ccx = y.cor[cx][cy].x;
		int ccy = y.cor[cx][cy].y;
		for (int i = 0; i < SWH; ++i)
			for (int j = 0; j < SWH; ++j)
			{
				//rotation goes like this Point A - point center and swap and add a negative sign for the first component and add the center
				int dx = y.cor[i][j].x;
				int dy = y.cor[i][j].y;
				int ny = ccy + (dx - ccx);
				int nx = ccx - (dy - ccy);
				y.cor[i][j].x = nx;
				y.cor[i][j].y = ny;
				
				//swap(y.cor[i][j], y.cor[j][i]);
			}
		break;
	}
	for (int i = 0; i < SWH; ++i)
		for (int j = 0; j < SWH; ++j) {
			int dx = y.cor[i][j].x;
			int dy = y.cor[i][j].y;
			if (!y.a[i][j])
				continue;
			if (!inrange(dx, dy) || occupy(dx, dy)) {
				return true;
			}
		}
	return false;
}

void collision() {
	xshape tmp = allshapes[currentshape];
	bool bad = collide(1,tmp);
	if (bad) {
		stop = true;
	}
}



void controldown(HWND hWnd) {
	if (!stop) {
		collide(1, allshapes[currentshape]);
	}
	collision();
	RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}



void controlleft(HWND hWnd) {
	xshape tmp = allshapes[currentshape];
	bool bad = collide(2, tmp);
	/* collision detection*/
	if (!bad)
		allshapes[currentshape] = tmp;
	RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void controlright(HWND hWnd) {
	xshape tmp = allshapes[currentshape];
	bool bad = collide(3, tmp);
	if (!bad)
		allshapes[currentshape] = tmp;
	RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void controlup(HWND hWnd) {
	xshape tmp = allshapes[currentshape];
	bool bad = collide(4,tmp);
	
	if (!bad)
		allshapes[currentshape] = tmp;
	RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	boxdimension.x = boxdimension.y = Squaresize;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_LEFT:
				controlleft(hWnd);
				// Process the RIGHT ARROW key. 
				break;

			case VK_RIGHT:
				controlright(hWnd);
				// Process the RIGHT ARROW key. 
				break;

			case VK_UP:
				controlup(hWnd);
				// Process the UP ARROW key. 
				break;

			case VK_DOWN:
				controldown(hWnd);
				// Process the DOWN ARROW key. 
				break;
			default:
				break;
			}
		case WM_TIMER:
			switch (wParam)
			{
			case IDT_TIMER_DOWN:
				if (stop) {
					for (int i = 0; i < SWH; ++i)
						for (int j = 0; j < SWH; ++j) {
							int dx = allshapes[currentshape].cor[i][j].x;
							int dy = allshapes[currentshape].cor[i][j].y;
							if(allshapes[currentshape].a[i][j])
								a[dx][dy] = allshapes[currentshape].color;
							}
					for (int i = 0; i < CanvasHeight; ++i) {
						bool occupied = true;
						for (int j = 0; j < CanvasWidth; ++j) {
							if (a[i][j] == emptyslotcolor)
								occupied = false;
						}
						if (occupied) {
							for (int k = i; k >= 1; --k) {
								for (int j = 0; j < CanvasWidth; ++j)
									a[k][j] = a[k - 1][j];
							}
						}
					}
					reset(allshapes[currentshape]);
					int len = allshapes.size();
					currentshape = rand() % len;
					COLORREF tmp = RGB(rand() % 255, rand() % 255, rand() % 255);
					while (tmp == emptyslotcolor) {
						tmp = RGB(rand() % 255, rand() % 255, rand() % 255);
					}
					allshapes[currentshape].color = tmp;
					stop = false;
				}
				controldown(hWnd);
				break;
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			for(int i = 0; i < CanvasHeight; ++i)
				for (int j = 0; j < CanvasWidth; ++j) {

					SetDCBrushColor(hdc, ::a[i][j]);
					int dx = j*boxdimension.x;
					int dy = i*boxdimension.y;
					Rectangle(hdc, dx, dy, dx+ boxdimension.x, dy+ boxdimension.y);
				}
			SetDCBrushColor(hdc, allshapes[currentshape].color);
			for (int i = 0; i < SWH; ++i)
				for (int j = 0; j < SWH; ++j) {
					if (allshapes[currentshape].cor[i][j].x >= 0 && allshapes[currentshape].a[i][j]) {
						int dx = allshapes[currentshape].cor[i][j].y * boxdimension.x;
						int dy = allshapes[currentshape].cor[i][j].x * boxdimension.y;
						Rectangle(hdc, dx, dy, dx + boxdimension.x, dy + boxdimension.y);
					}
				}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}



// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND editBox;
	static TCHAR tcc[50];
	static TCHAR * answer = TEXT("1234");
	TCHAR * x = answer;
	TCHAR * y = tcc;
	switch (message)
	{
		case WM_INITDIALOG:
			    editBox = CreateWindowEx(WS_EX_PALETTEWINDOW, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL, 10, 10, 60, 20, hDlg, NULL , NULL, NULL);
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				GetWindowText(editBox, tcc,10);
				
				while(*x && *y){
					if(*x != *y)
						break;
					++x;++y;
				}
				if (*x == NULL && *y == NULL){
					EndDialog(hDlg,IDOK);
				}

				return TRUE;
			}
			break;
	}
    return FALSE;
}
