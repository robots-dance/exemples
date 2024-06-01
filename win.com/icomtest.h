// {87D2133E-870A-4ea7-9CA3-6432F2F8D4DC}
DEFINE_GUID( CLSID_ComTest,
0x87d2133e, 0x870a, 0x4ea7, 0x9c, 0xa3, 0x64, 0x32, 0xf2, 0xf8, 0xd4, 0xdc);

// {87D2133E-870A-4ea7-9CA3-6432F2F8D4DC}
DEFINE_GUID( IID_IComTest,
0x87d2133e, 0x870a, 0x4ea7, 0x9c, 0xa3, 0x64, 0x32, 0xf2, 0xf8, 0xd4, 0xdc);

class IComTest: public IUnknown
{
	public:
		STDMETHOD( GetFileContent( const char *, OUT char ** ) ) PURE;
		STDMETHOD( GetModulesCount( OUT int * ) ) PURE;
};