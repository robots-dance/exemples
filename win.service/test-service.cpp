#include <windows.h>
#include <tchar.h>

// service-specific constants
#define SERVICE_NAME _T( "Test Service" )
#define EVENT_WAIT_TIME INFINITE

// error messages
#define SERVICE_MAIN_ERR_HDR _T( ": ServiceMain - " )
#define SERVICE_CTRL_ERR_HDR _T( ": ServiceCtrlHandler - " )
#define SET_SERVICE_STATUS_ERR _T( "can't set service status" )

#define BUILD_DBG_MESSAGE( hdr, message ) ( SERVICE_NAME hdr message )

// checkpoints
#define AT_BAD_EVENT 1
#define AT_CONTROL_STOP 3
#define AT_SERVICE_END 4

// ==== global variables ====
SERVICE_STATUS g_serviceStatus = { 0 };
SERVICE_STATUS_HANDLE g_statusHandle = NULL;
HANDLE g_serviceStopEvent = INVALID_HANDLE_VALUE;

// ==== prototypes ====
VOID WINAPI ServiceMain( DWORD argc, LPTSTR *argv );
VOID WINAPI ServiceCtrlHandler( DWORD );
DWORD WINAPI ServiceWorkerProc( LPVOID lpParam );

// ==== functions ====
int _tmain( int argc, TCHAR **argv )
{
	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ SERVICE_NAME, ( LPSERVICE_MAIN_FUNCTION )ServiceMain },
		{ NULL, NULL }
	};
	
	if ( StartServiceCtrlDispatcher( serviceTable ) == FALSE )
	{
		return GetLastError();
	}
	
	return 0;
}

VOID WINAPI ServiceMain( DWORD argc, LPTSTR *argv )
{
	// register our service control handler with the SCM
	g_statusHandle = RegisterServiceCtrlHandler( SERVICE_NAME,
		ServiceCtrlHandler );
	if ( NULL == g_statusHandle )
	{
		return;
	}
	
	// tell the service controller we are starting
	ZeroMemory( &g_serviceStatus, sizeof( g_serviceStatus ) );
	g_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_serviceStatus.dwControlsAccepted = 0;
	g_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_serviceStatus.dwWin32ExitCode = 0;
	g_serviceStatus.dwServiceSpecificExitCode = 0;
	g_serviceStatus.dwCheckPoint = 0;
	
	if ( SetServiceStatus( g_statusHandle, &g_serviceStatus ) == FALSE )
	{
		OutputDebugString( BUILD_DBG_MESSAGE( SERVICE_MAIN_ERR_HDR,
			SET_SERVICE_STATUS_ERR ) );
	}
	
	// create a service stop event to wait in a work thread
	g_serviceStopEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	if ( NULL == g_serviceStopEvent )
	{
		g_serviceStatus.dwControlsAccepted = 0;
		g_serviceStatus.dwCurrentState = SERVICE_STOPPED;
		g_serviceStatus.dwWin32ExitCode = GetLastError();
		g_serviceStatus.dwCheckPoint = AT_BAD_EVENT;
		
		if ( SetServiceStatus( g_statusHandle, &g_serviceStatus ) == FALSE )
		{
			OutputDebugString( BUILD_DBG_MESSAGE( SERVICE_MAIN_ERR_HDR,
				SET_SERVICE_STATUS_ERR ) );
		}
		return;
	}
	
	// tell the service controller we are started
	g_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_serviceStatus.dwCurrentState = SERVICE_RUNNING;
	g_serviceStatus.dwWin32ExitCode = 0;
	g_serviceStatus.dwCheckPoint = 0;
	
	if ( SetServiceStatus( g_statusHandle, &g_serviceStatus ) == FALSE )
	{
		OutputDebugString( BUILD_DBG_MESSAGE( SERVICE_MAIN_ERR_HDR,
			SET_SERVICE_STATUS_ERR ) );
	}
	
	// start a thread that will perform the main task of the service
	HANDLE hThread = CreateThread( NULL, 0, ServiceWorkerProc, NULL, 0, NULL );
	WaitForSingleObject( hThread, INFINITE );
	
	// perform cleanup tasks
	CloseHandle( g_serviceStopEvent );
	
	// tell the service controller we are stopped
	g_serviceStatus.dwControlsAccepted = 0;
	g_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	g_serviceStatus.dwWin32ExitCode = 0;
	g_serviceStatus.dwCheckPoint = AT_SERVICE_END;
	
	if ( SetServiceStatus( g_statusHandle, &g_serviceStatus ) == FALSE )
	{
		OutputDebugString( BUILD_DBG_MESSAGE( SERVICE_MAIN_ERR_HDR,
			SET_SERVICE_STATUS_ERR ) );
	}
	
	return;
}

VOID WINAPI ServiceCtrlHandler( DWORD ctrlCode )
{
	switch ( ctrlCode )
	{
		case SERVICE_CONTROL_STOP:
		{
			if ( SERVICE_RUNNING == g_serviceStatus.dwCurrentState )
			{
				g_serviceStatus.dwControlsAccepted = 0;
				g_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
				g_serviceStatus.dwWin32ExitCode = 0;
				g_serviceStatus.dwCheckPoint = AT_CONTROL_STOP;
				
				if ( SetServiceStatus( g_statusHandle,
					&g_serviceStatus ) == FALSE )
				{
					OutputDebugString( BUILD_DBG_MESSAGE( SERVICE_CTRL_ERR_HDR,
						SET_SERVICE_STATUS_ERR ) );
				}
				
				// this will signal the worker thread to start shutting down
				SetEvent( g_serviceStopEvent );
			}
		}
		break;
		
		default:
		break;
	}
}

DWORD WINAPI ServiceWorkerProc( LPVOID lpParam )
{
	while ( WaitForSingleObject( g_serviceStopEvent,
		EVENT_WAIT_TIME ) != WAIT_OBJECT_0 )
	{
		;
	}
	return ERROR_SUCCESS;
}
