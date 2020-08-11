/* novacaine header */
#include <iostream>
//Another non built in include as presented with the ""'s but where is it in the main folder directory? Confusing ~ 
#include "Windows.h"
//special include of Hexi.h
#include "Hexi.h"
using namespace std;
jpn user;

/* booleans */
/*
	So this seems to be where we can enable or disbale functionallity. I will try and buld with GlowOn set to false
	as we don't need that functionallity just yet
*/
bool GlowOn = true;
bool TriggerOn = true;

/* functions */
void WPMshoot( );
void WPMshoot2( );

/* defines */
#define FL_ONGROUND ( 1 << 0 )

/* offsets */
//See offset dumper
//Offsets incomplete Don't build till complete
DWORD dwLocal = 0xA9ADEC;
DWORD dwGlow = 0x4F94920;
DWORD dwEntity = 0x4A77AFC;
DWORD dwTeam = 0xF0;
DWORD dwGlowIndex = 0xA310;
DWORD dwCrosshair = 0xB2A4;
DWORD dwAttack = 0x2EB9EAC;
DWORD dwJump = 0x4F0ED64;
DWORD dwFlags = 0x100;
DWORD dwHealth = 0x000000FC;
DWORD dwActiveWeapon = 0x2EE8;
DWORD dwFlash = 0xA2E8;
DWORD dwItemDefinition = 0x00002F88;
DWORD dwCrosshairColor = 0x58FACE4C;
DWORD Client;

struct ReadData { 

	DWORD GetBase( ) {

		return user.nmd< DWORD >( Client + dwLocal );

	}

	DWORD GetGlow( ) {

		return user.nmd< DWORD >( Client + dwGlow );

	}

	DWORD GetJump( ) {

		return user.nmd< DWORD >( Client + dwJump );

	}

	/*weapon data*/
	DWORD GetWeaponHandle( ) {

		DWORD LB = Data.GetBase( );

		if ( LB ) {

			return user.nmd< DWORD >( LB + dwActiveWeapon );

		}

	}

	int GetWeaponEntity( ) {

		return user.nmd< int >( Data.GetWeaponHandle( ) + 0xFFF );

	}

	DWORD GetCurrentWeapon( ) {

		return user.nmd< DWORD >( Client + dwEntity + ( GetWeaponEntity( ) - 1 ) * 0x10 );

	}

	int GetWeaponID( ) {

		return user.nmd< int >( Data.GetCurrentWeapon( ) + dwItemDefinition );

	}

	/* |weapon data| */

	int GetFlags( ) {

		DWORD LB = Data.GetBase( );

		if ( LB ) { 

			return user.nmd< int >( LB + dwFlags );

		}
	}

	int GetCrosshair( ) {

		DWORD LB = GetBase( );

		if ( LB ) {

			return user.nmd< int >( LB + dwCrosshair );

		}

	}

	int GetTeam( ) {

		DWORD LB = GetBase( );

		if ( LB ) {

			return user.nmd< int >( LB + dwTeam );

		}

	}

	int GetHealth( ) {

		DWORD LB = GetBase( );

		int EntityData = user.nmd< int >( Client + dwEntity + ( ( Data.GetCrosshair( ) - 1 ) * 0x10 ) );

		if ( LB ) {

			return user.nmd< int >( EntityData + dwHealth );

		}

	}

	int GetEnemyTeam( ) {

		DWORD LB = GetBase( );

		int EntityData = user.nmd< int >( Client + dwEntity + ( ( Data.GetCrosshair( ) - 1 ) * 0x10 ) );

		if ( LB ) {

			return user.nmd< int >( EntityData + dwTeam );

		}

	}

} Data;

void XHair( ) {
	while ( true ) {
		user.cmd< DWORD >( Client + dwCrosshairColor, 2 );
		Sleep( 1 );
	}
}

class cGlow {
public:
	void Glow( ) { 

		while ( true ) {

			if ( GlowOn == true ) {

			    for (int i = 0; i < 64; i++) {

				    int GlowCurrentPlayer = user.nmd< int >( Client + dwEntity + ( (i - 1) * 16) );

				    int GlowCurrentPlayerTeam = user.nmd< int >( GlowCurrentPlayer + dwTeam );

				    int GlowCurrentPlayerIndex = user.nmd< int >( GlowCurrentPlayer + dwGlowIndex );

					int GlowCurrentPlayerFlashed = user.nmd< int >( GlowCurrentPlayer + dwFlash );

					if ( Data.GetTeam( ) != GlowCurrentPlayerTeam ) {

					    user.cmd< float >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0x4 ) ), 0.f );
					    user.cmd< float >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0x8 ) ), 0.f );
				        user.cmd< float >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0xC ) ), 1.f );
					    user.cmd< float >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0x10 ) ), 0.49f );
					    user.cmd< BOOL >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0x24 ) ), true );
					    user.cmd< BOOL >( ( Data.GetGlow( ) + ( ( GlowCurrentPlayerIndex * 0x38 ) + 0x25 ) ), false );

					}

				}

			}

			Sleep( 1 );

		}

	}

};

/* triggerbot */
// nothing special 4 rn 
void Trigger( ) { 

	while ( true ) {

		if ( TriggerOn == true ) {
			//previous was VK_Menu ~ 
		    if ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) { 

				if ( Data.GetHealth( ) > 0  ) {

					if ( Data.GetEnemyTeam( ) != Data.GetTeam( ) ) {

						WPMshoot( );

					}

				}

		    }

		}

		Sleep( 1 );

	}

}

void WPMshoot( ) {

	user.cmd< int >( Client + dwAttack, 5 );

    Sleep( 7 );

    user.cmd< int >( Client + dwAttack, 4 );

}

void Bunny( ) {

	while ( true ) {

		if ( Data.GetJump( ) == 5 ) {

			while ( GetAsyncKeyState( VK_SPACE ) & 0x8000 ) {

				if ( Data.GetFlags( ) & FL_ONGROUND ) {

					user.cmd< int >( Client + dwJump, 5 );

				}

				else {

					user.cmd< int >( Client + dwJump, 4 );

				}

			}

		}

	}

}
