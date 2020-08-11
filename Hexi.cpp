#include "Hexi.h"
using namespace std;

#pragma region Misc Functions
jpn::jpn( ) {

}
jpn::~jpn( ) {
	CloseHandle( hProcess );
}

int jpn::chSizeOfArray( char *chArray ) {


	for ( int iLength = 1; iLength < MAX_PATH; iLength++ )
		if ( chArray[iLength] == '*' )
			return iLength;

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
}
int jpn::iSizeOfArray( int *iArray ) {


	for ( int iLength = 1; iLength < MAX_PATH; iLength++ )
		if ( iArray[iLength] == '*' )
			return iLength;

	cout << "\nLENGTH: Failed To Read Length Of Array\n";
	return 0;
}
bool jpn::iFind( int *iAry, int iVal ) {

	for ( int i = 0; i < 64; i++ )
		if ( iVal == iAry[i] && iVal != 0 )
			return true;

	return false;
}

#pragma endregion

#pragma region Memory Functions


void jpn::Process( char* ProcessName ) {
	HANDLE hPID = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof( ProcEntry );

	do
		if ( !strcmp(ProcEntry.szExeFile, ProcessName ) ) {
			dwPID = ProcEntry.th32ProcessID;
			CloseHandle( hPID );

			hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwPID );
			return;
		}
	while ( Process32Next( hPID, &ProcEntry ) );

	cout << "<<Novacaine - failed, run cs go.>>";
	system( "pause" );
	exit( 0 );
}
void jpn::Patch( DWORD dwAddress, char *Patch_Bts, char *Default_Bts ) {
	int iSize = chSizeOfArray( Default_Bts );

	if ( !bPOn )
		for ( int i = 0; i < iSize; i++ )
			cmd< BYTE >( dwAddress + i, Patch_Bts[i] );
	else
		for ( int i = 0; i < iSize; i++ )
			cmd< BYTE >( dwAddress + i, Default_Bts[ i ] );

	bPOn = !bPOn;
}


void jpn::Inject( DWORD dwAddress, char *Inj_Bts, char *Def_Bts, BOOL Type ){
	int i_ISize = chSizeOfArray( Inj_Bts );
	int i_DSize = chSizeOfArray( Def_Bts );
	if ( !bIOn ) {
		if ( i_DSize > 5 )
			for ( int i = 6; i < i_DSize; i++ )
				cmd< BYTE >( dwAddress + i, 0x90 );
		else { cout << "\nINJECTION: Default Bytes Must Be More Than 5\n"; return; }


		dwCaveAddress = ( DWORD )VirtualAllocEx( hProcess, NULL, i_ISize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );


		DWORD dwRetJmp = ( dwAddress + i_DSize ) - dwCaveAddress - 5;
		DWORD dwBaseJmp = dwCaveAddress - dwAddress - 5;

		for ( int i = 0; i <= i_ISize; i++ )
			cmd< BYTE >( dwCaveAddress + i, Inj_Bts[ i ] );


		cmd< BYTE >( dwCaveAddress + i_ISize, Type ? 0xE9 : 0xE8 );
		cmd< DWORD >( dwCaveAddress + i_ISize + 1, dwRetJmp );


		cmd< BYTE >( dwAddress, Type ? 0xE9 : 0xE8 );
		cmd< DWORD >( dwAddress + 1, dwBaseJmp );
	}
	else {

		for ( int i = 0; i < i_DSize; i++ )
			cmd< BYTE >( dwAddress + i, Def_Bts[ i ] );

		VirtualFreeEx( hProcess, ( LPVOID )dwCaveAddress, i_ISize + 5, MEM_DECOMMIT );
	}
	bIOn = !bIOn;
}
DWORD jpn::AOB_Scan( DWORD dwAddress, DWORD dwEnd, char *Bytes ) {


	int iBytesToRead = 0, iTmp = 0;
	int length = chSizeOfArray( Bytes );
	bool bTmp = false;


	if (Bytes[0] == '?') {
		for ( ; iBytesToRead < MAX_PATH; iBytesToRead++ )
			if ( Bytes[ iBytesToRead ] != '?' ) {
				iTmp = ( iBytesToRead + 1 );
				break;
			}
	}
	for ( ; dwAddress < dwEnd; dwAddress++ ) {
		if ( iBytesToRead == length)
			return dwAddress - iBytesToRead;
		if ( nmd< BYTE >( dwAddress ) == Bytes[ iBytesToRead ] || ( bTmp && Bytes[iBytesToRead] == '?' ) ) {
			iBytesToRead++;
			bTmp = true;
		}
		else {
			iBytesToRead = iTmp;
			bTmp = false;
		}
	}
	cout << "\nAOB_SCAN: Failed To Find Byte Pattern\n";
	return 0;
}
DWORD jpn::Module( LPSTR ModuleName ){


	HANDLE hModule = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof( mEntry );

	do
		if ( !strcmp( mEntry.szModule, ModuleName ) ) {
			CloseHandle( hModule );
			return ( DWORD )mEntry.modBaseAddr;
		}
	while ( Module32Next(hModule, &mEntry ) );

	cout << "\nCouldn't find client.dll, retrying...\n";
	return 0;
}
#pragma endregion