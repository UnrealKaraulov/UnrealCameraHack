
#pragma region Headers
#define _WIN32_WINNT 0x0501 
#define WINVER 0x0501 
#define NTDDI_VERSION 0x05010000
//#define BOTDEBUG
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma endregion
HANDLE KeyWatcherID;
#define IsKeyPressed(CODE) (GetAsyncKeyState(CODE) & 0x8000) > 0

BOOL IsNotMIX = FALSE;

int GameDll = 0;

float camheight = 1650.0f;
float duration = 0.5f;
float durationzero = 0.0f;

float angle90 = 90.0f;
float angleofattack = 304.0f;
float fieldofview = 70.0f;

float farzoff = 100000.0f;

BOOL IsChat( )
{
	return *( BOOL* ) ( GameDll + 0xAD15F0 );
}
DWORD WINAPI KeyWatcher( LPVOID )
{

	while ( TRUE )
	{
		if ( IsNotMIX )
		{
			try
			{

				if ( !IsChat( ) )
				{

					if ( IsKeyPressed( VK_SUBTRACT ) )
					{
						int timer = 50;
						while ( IsKeyPressed( VK_SUBTRACT ) )
						{
							timer--;
							if ( timer < 0 )
								break;
							Sleep( 20 );
						}
						if ( camheight < 500.0f )
							continue;
						camheight -= 60.0f;

					}


					if ( IsKeyPressed( VK_ADD ) )
					{
						int timer = 50;
						while ( IsKeyPressed( VK_ADD ) )
						{
							timer--;
							if ( timer < 0 )
								break;
							Sleep( 20 );
						}
						if ( camheight > 8000.0f )
							continue;
						camheight += 60.0f;

					}


					if ( IsKeyPressed( VK_MULTIPLY ) )
					{
						while ( IsKeyPressed( VK_MULTIPLY ) )
						{
							Sleep( 20 );
						}
						camheight = 1650.0f;


					}
				}
			}
			catch ( ... )
			{

			}
		}
		Sleep( 30 );
	}


	return 0;
}

BOOL IsGame( ) // my offset + public
{
	return ( *( int* ) ( ( UINT32 ) GameDll + 0xACF678 ) > 0 || *( int* ) ( ( UINT32 ) GameDll + 0xAB62A4 ) > 0 )/* && !IsLagScreen( )*/;
}

void SetTlsForMe( )
{
	UINT32 Data = *( UINT32 * ) ( GameDll + 0xACEB4C );
	UINT32 TlsIndex = *( UINT32 * ) ( GameDll + 0xAB7BF4 );
	if ( TlsIndex )
	{
		UINT32 v5 = **( UINT32 ** ) ( *( UINT32 * ) ( *( UINT32 * ) ( GameDll + 0xACEB5C ) + 4 * Data ) + 44 );
		if ( !v5 || !( *( LPVOID * ) ( v5 + 520 ) ) )
		{
			Sleep( 1000 );
			SetTlsForMe( );
			return;
		}
		TlsSetValue( TlsIndex , *( LPVOID * ) ( v5 + 520 ) );
	}
	else
	{
		Sleep( 1000 );
		SetTlsForMe( );
		return;
	}
}
HANDLE CamHackID;



typedef void( __cdecl * SetCameraField_GAME )( UINT whichField , float *value , float *duration );
SetCameraField_GAME SetCameraField;

DWORD WINAPI CamHack( LPVOID )
{

	Sleep( 3000 );

	SetTlsForMe( );

	while ( TRUE )
	{
		if ( IsNotMIX )
		{
			if ( IsGame( ) )
			{
				if ( !IsChat( ) )
				{
					SetCameraField( 0 , &camheight , &durationzero );
					Sleep( 50 );
					SetCameraField( 1 , &farzoff , &durationzero );
					Sleep( 50 );
					SetCameraField( 6 , &durationzero , &durationzero );
					Sleep( 50 );
					SetCameraField( 5 , &angle90 , &durationzero );
					Sleep( 50 );
					SetCameraField( 2 , &angleofattack , &durationzero );
					Sleep( 50 );
					SetCameraField( 4 , &durationzero , &durationzero );
					Sleep( 50 );
					SetCameraField( 3 , &fieldofview , &durationzero );
					Sleep( 50 );
				}
			}
		}
		Sleep( 1000 );
	}

	return 0;
}


DWORD __stdcall SetCameraDistance( float * distance )
{
	if ( distance )
	{
		IsNotMIX = TRUE;
		camheight = *( float* ) distance;
	}
	return 0;
}



BOOL WINAPI DllMain( HINSTANCE hDLL , UINT reason , LPVOID reserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		GameDll = ( int ) GetModuleHandle( "Game.dll" );
		SetCameraField = ( SetCameraField_GAME ) ( 0x3B48B0 + GameDll );
		KeyWatcherID = CreateThread( 0 , 0 , KeyWatcher , 0 , 0 , 0 );
		CamHackID = CreateThread( 0 , 0 , CamHack , 0 , 0 , 0 );
	}
	else if ( reason == DLL_PROCESS_DETACH )
	{
		TerminateThread( KeyWatcherID , 0 );
		TerminateThread( CamHackID , 0 );
	}
	return TRUE;
}
