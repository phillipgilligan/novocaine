#ifndef JPN_H 
#define JPN_H 

//Got our includes here nothing but regular includes. ~ Binary
#include <windows.h> 
#include <iostream> 
#include <TlHelp32.h> 
#include <string> 
#include <sstream> 

//Here we have a class named jpn, need to reverse this. ~ Binary
class jpn {
protected:
	HANDLE hProcess;
	DWORD dwPID, dwProtection, dwCaveAddress;

	BOOL bPOn, bIOn, bProt;
public:

	jpn( );
	~jpn( );
	int chSizeOfArray( char *chArray );
	int iSizeOfArray( int *iArray );
	bool iFind( int *iAry, int iVal );

#pragma region TEMPLATE MEMORY FUNCTIONS

	template < class cData >
	void Protection( DWORD dwAddress ) {
		if ( !bProt )
			VirtualProtectEx( hProcess, ( LPVOID )dwAddress, sizeof( cData ), PAGE_EXECUTE_READWRITE, &dwProtection ); 
		else
			VirtualProtectEx( hProcess, ( LPVOID )dwAddress, sizeof( cData ), dwProtection, &dwProtection );

		bProt = !bProt;
	}
	template < class cData >
	cData nmd( DWORD dwAddress ) {
		cData cRead;
		ReadProcessMemory( hProcess, ( LPVOID )dwAddress, &cRead, sizeof( cData ), NULL );
		return cRead;
	}
	template < class cData >
	cData nmd( DWORD dwAddress, char *Offset, BOOL Type ) {
		int iSize = iSizeOfArray( Offset ) - 1;
		dwAddress = nmd< DWORD >( dwAddress );

		for ( int i = 0; i < iSize; i++ )
			dwAddress = nmd< DWORD >( dwAddress + Offset[ i ] );

		if ( !Type )
			return dwAddress + Offset[ iSize ];
		else
			return nmd< cData >( dwAddress + Offset[ iSize ] );
	}
	template < class cData >
	void cmd( DWORD dwAddress, cData Value ) {
		WriteProcessMemory( hProcess, ( LPVOID )dwAddress, &Value, sizeof( cData ), NULL );
	}
	template < class cData >
	void cmd( DWORD dwAddress, char *Offset, cData Value ) {
		cmd< cData >( nmd< cData >( dwAddress, Offset, false ), Value );
	}
	virtual void Process(char* ProcessName);
	virtual void Patch(DWORD dwAddress, char *chPatch_Bts, char *chDefault_Bts);
	virtual void Inject(DWORD dwAddress, char *chInj_Bts, char *chDef_Bts, BOOL Type);
	virtual DWORD AOB_Scan(DWORD dwAddress, DWORD dwEnd, char *chPattern);
	virtual DWORD Module(LPSTR ModuleName);

#pragma endregion	

};
#endif


