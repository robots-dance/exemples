#include <windows.h>
#include <stdio.h>
#include "comtest.h"

// ComTest class
ComTest::ComTest()
{
	m_lRef = 0;
	InterlockedIncrement( &g_lObjs );
}

ComTest::~ComTest()
{
	InterlockedDecrement( &g_lObjs );
}

STDMETHODIMP ComTest::QueryInterface( REFIID riid, OUT void **ppv )
{
	*ppv = 0;
	if ( IID_IUnknown == riid || IID_IComTest == riid ) {
		*ppv = this;
	}
	if ( *ppv )
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ComTest::AddRef()
{
	return InterlockedIncrement( &m_lRef );
}

STDMETHODIMP_ (ULONG) ComTest::Release()
{
	if ( InterlockedDecrement( &m_lRef ) == 0 )
	{
		delete this;
		return 0;
	}
	return m_lRef;
}

STDMETHODIMP ComTest::GetFileContent( const char *filePath, OUT char **outputContent )
{
	printf("GetFileContent no implemented\n");
	return S_OK;
}

STDMETHODIMP ComTest::GetModulesCount( OUT int * )
{
	printf("GetModulesCount no implemented\n");
	return S_OK;
}
// --------


// ComTestClassFactory class
ComTestClassFactory::ComTestClassFactory()
{
	m_lRef = 0;
}

ComTestClassFactory::~ComTestClassFactory()
{
	;
}

STDMETHODIMP ComTestClassFactory::QueryInterface( REFIID riid, void **ppv )
{
	*ppv = 0;
	if ( IID_IUnknown == riid || IID_IClassFactory == riid )
	{
		*ppv = this;
	}
	if ( *ppv )
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ComTestClassFactory::AddRef()
{
	 return InterlockedIncrement( &m_lRef );
}

STDMETHODIMP_(ULONG) ComTestClassFactory::Release()
{
	if ( InterlockedDecrement( &m_lRef ) == 0 )
	{
		delete this;
		return 0;
	}
	return m_lRef;
}

STDMETHODIMP ComTestClassFactory::CreateInstance(
	LPUNKNOWN pUnkOuter, REFIID riid, void **ppvObj )
{
	ComTest *pTest;
	HRESULT hr;
	*ppvObj = 0;
	pTest = new ComTest();
	if ( NULL == pTest ) {
		return E_OUTOFMEMORY;
	}
	hr = pTest->QueryInterface( riid, ppvObj );
	if ( FAILED( hr ) ) {
		delete pTest;
	}
	return hr;
}

STDMETHODIMP ComTestClassFactory::LockServer( BOOL fLock )
{
	if ( fLock ) {
		InterlockedIncrement( &g_lLocks );
	}
	else {
		InterlockedDecrement( &g_lLocks );
	}
	return S_OK;
}
// --------