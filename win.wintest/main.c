#include <windows.h>
#include <tlhelp32.h>

#define CLS_NAME "test_app_class"

#define LST_PADDING 10
#define NEXT_ID GetMenuElemId()
#define WIN_HEIGHT 400
#define WIN_WIDTH 600

HWND listbox;

inline int GetMenuElemId();
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int );
LRESULT CALLBACK WinProc( HWND, UINT, WPARAM, LPARAM );

void prepareWindow( WNDCLASSEXA *wclass );

int WinMainCRTStartup( void )
{
	STARTUPINFO sinfo;
	int ret;
	GetStartupInfo( &sinfo );
	ret = WinMain( GetModuleHandle( NULL ), NULL, GetCommandLine(),
		sinfo.dwFlags & STARTF_USESHOWWINDOW ? sinfo.wShowWindow : SW_SHOWDEFAULT );
	ExitProcess( 0 );
	return ret;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nShowCmd )
{
	WNDCLASSEXA winc;
	prepareWindow( &winc );
	winc.hInstance = hInstance;
	winc.lpfnWndProc = WinProc;
	if ( !RegisterClassExA( &winc ) )
	{
		return 1;
	}
	
	const int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	const int screenHeight = GetSystemMetrics( SM_CYSCREEN );
	HWND hwnd = CreateWindowExA( 0,
		CLS_NAME,
		"Test app",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		( screenWidth - WIN_WIDTH ) / 2, ( screenHeight - WIN_HEIGHT ) / 2,
		WIN_WIDTH, WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL );
	if ( !hwnd )
	{
		return 2;
	}
	int style = GetWindowLong( hwnd, GWL_STYLE );
	style &= ~WS_MAXIMIZEBOX;
	SetWindowLong( hwnd, GWL_STYLE, style );
	
	HMENU menu = CreateMenu();
	HMENU fileMenu = CreatePopupMenu();
	HMENU helpMenu = CreatePopupMenu();
	AppendMenu( menu, MF_STRING | MF_POPUP, ( UINT_PTR )fileMenu, "&File" );
	HMENU createMenu = CreatePopupMenu();
	AppendMenu( fileMenu, MF_STRING | MF_POPUP,
		( UINT_PTR )createMenu, "&Create file" );
	const char *fmItems[] = { "Open...", "Save", "Save as...", "Close" };
	for ( short i = 0; i < sizeof( fmItems ) / sizeof( char* ); i++ )
	{
		AppendMenu( fileMenu, MF_STRING, NEXT_ID, fmItems[ i ] );
	}
	AppendMenu( createMenu, MF_STRING, NEXT_ID, "New file" );
	AppendMenu( createMenu, MF_STRING, NEXT_ID, "From template" );
	AppendMenu( menu, MF_STRING | MF_POPUP, ( UINT_PTR )helpMenu, "&Help" );
	AppendMenu( helpMenu, MF_STRING, NEXT_ID, "Help Contents" );
	AppendMenu( helpMenu, MF_STRING, NEXT_ID, "About program" );
	SetMenu( hwnd, menu );
	SetWindowTextA( hwnd, "Hello, world!" );
	
	ShowWindow( hwnd, SW_SHOWDEFAULT );
	UpdateWindow( hwnd );
	
	MSG message;
	while ( GetMessage( &message, NULL, 0, 0 ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}
	DestroyMenu( fileMenu );
	DestroyMenu( helpMenu );
	DestroyMenu( menu );
	return message.wParam;
}

LRESULT CALLBACK WinProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch ( msg )
	{
		case WM_CREATE:
		{
			listbox = CreateWindowExA( 0,
				"listbox",
				NULL,
				WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL,
				LST_PADDING, LST_PADDING,
				WIN_WIDTH - LST_PADDING * 2 - 5, WIN_HEIGHT -
					LST_PADDING * 2 - 40,
				hwnd,
				NULL,
				NULL,
				NULL );
			HANDLE procHnd = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 );
	 		PROCESSENTRY32 procInfo;
			procInfo.dwSize = sizeof( PROCESSENTRY32 );
			BOOL res = Process32First (procHnd, &procInfo );
			if ( res )
			{
				char *exePath;
				while ( res )
				{
					SendMessage( listbox, LB_ADDSTRING, 0,
						( LPARAM )procInfo.szExeFile );
					res = Process32Next( procHnd, &procInfo );
				}
			}
		}
		break;
		
		case WM_SIZING:
		{
			RECT rc;
			GetWindowRect( hwnd, &rc );
			*( ( LPRECT )lparam ) = rc;
		}
		break;
		
		case WM_PAINT:
		{
			;
		}
		break;
		
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	}
	return DefWindowProc( hwnd, msg, wparam, lparam );
}

inline int GetMenuElemId()
{
	#define START_ID 22
	static int id = START_ID;
	return id++;
}

void prepareWindow( WNDCLASSEXA *wclass )
{
	wclass->cbSize = sizeof( *wclass );
	wclass->cbClsExtra = 0;
	wclass->cbWndExtra = 0;
	wclass->hCursor = LoadCursor( NULL, IDC_ARROW );
	wclass->hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wclass->hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wclass->hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );
	wclass->lpszClassName = CLS_NAME;
	wclass->lpszMenuName = NULL;
	wclass->style = 0;
}
