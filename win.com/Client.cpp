//
// Client.סננ
//

#include <initguid.h>
#include <iostream>
#include <windows.h>
#include "icomtest.h"

int main( int argc, char *argv[] )
{
	CoInitialize( NULL );
		
	IClassFactory *pCF;
	
	// get a class object
	HRESULT hr = CoGetClassObject( CLSID_ComTest,
		CLSCTX_INPROC, NULL,
		IID_IClassFactory, (void**) &pCF ) ;
	if ( FAILED( hr ) )
	{
		std::cout << "cannot get class object" << std::endl;
		return 2;
	}
	// --
	
	// get the IUnknown interface
	IUnknown *pUnk;
	hr = pCF->CreateInstance( NULL, IID_IUnknown, (void**)&pUnk );
	if ( FAILED( hr ) )
	{
		std::cout << "cannot get the IUnknown interface" << std::endl;
		return 3;
	}
	pCF->Release();
	// --
	
	// get the IComTest interface
	IComTest *pTest = NULL;
	hr = pUnk->QueryInterface( IID_IComTest, (LPVOID*)&pTest );
	pUnk->Release();
	// --
	
	// get file content
	char *fileContent = NULL;
	if ( !FAILED( pTest->GetFileContent( __FILE__, &fileContent ) ) )
	{
	//	std::cout << fileContent << std::endl;
	}
	else
	{
		std::cout << "error occured during the GetFileContent call"
			<< std::endl;
	}
	// --
	
	// get modules count
	int modulesCount = -1;
	if ( !FAILED( pTest->GetModulesCount( &modulesCount ) ) )
	{
	//	std::cout << modulesCount << std::endl;
	}
	else
	{
		std::cout << "error occured during the GetModulesCount call"
			<< std::endl;
	}
	// --
	
	pTest->Release();
	CoUninitialize();
	
	return 0;
}