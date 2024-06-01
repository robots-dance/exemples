#include "icomtest.h"

extern long g_lObjs;
extern long g_lLocks;

class ComTest: public IComTest
{
	public:
		ComTest();
		~ComTest();
		
		// IUnknown
		STDMETHOD( QueryInterface( REFIID, OUT void ** ) );
		STDMETHOD_( ULONG, AddRef() );
		STDMETHOD_( ULONG, Release() );
		
		// IComTest
		STDMETHOD( GetFileContent( const char *, OUT char ** ) );
		STDMETHOD( GetModulesCount( OUT int * ) );
	
	private:
		long m_lRef;
};

class ComTestClassFactory : public IClassFactory
{
	public:
		ComTestClassFactory();
		~ComTestClassFactory();
		
		// IUnknown
		STDMETHOD( QueryInterface( REFIID, OUT void ** ) );
		STDMETHOD_( ULONG, AddRef() );
		STDMETHOD_( ULONG, Release() );
		
		// IClassFactory
		STDMETHOD( CreateInstance( LPUNKNOWN, REFIID, void ** ) );
		STDMETHOD( LockServer( BOOL ) );
	
	protected:
		long m_lRef;
};