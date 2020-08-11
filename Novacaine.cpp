//Not too much to do here ~ Binary

/* novacaine main */
//Special include Novacaine.h
#include "Novacaine.h"
cGlow * pGlow;

void Novacaine( ) {
	/* Set The Console Title */
	SetConsoleTitle( "Novacaine" );

	/* Attach to CS:GO */
	user.Process( "csgo.exe" );  

	/* Client.dll */
	Client = user.Module( "client.dll" );

	pGlow->Glow( );

}

int main( ) {
	HANDLE Threads[ ] = {

		CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) Novacaine , 0 , 0 , 0 ),

		CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) XHair , 0 , 0 , 0 ),

		CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) Trigger , 0 , 0 , 0 ),

		CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) Bunny , 0 , 0 , 0 )

	};

	WaitForMultipleObjects( sizeof( Threads ) / sizeof( HANDLE ), Threads, TRUE, INFINITE );

	return( 0 );

}