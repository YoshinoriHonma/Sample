#pragma once


// ScopeSafePtr
//
// If it separates from ScopeSafePtr from a scope, it will release a memory automatically. 
// Since the same access as a pointer can be performed, the replacement from the existing code is easy. 
//
/* Sample Code
void Sample()
{
	// Single
	{
		ScopeSafePtr<int>	p	= new int;
		*p = 10;
		int value = *p;
	}

	// Array
	{
		ScopeSafePtr<int[]> pArray = new int [ 4 ];
		for ( size_t i = 0; i < 4; i++ )
		{
			pArray[i] = static_cast<int>( i );
		}
	}
}

BYTE* SampleFileRead( const char* filePath, size_t buffSize )
{
	ScopeSafePtr<BYTE[]> pBuff = new BYTE[ buffSize ];			// create buffer
	FILE* pFile = fopen( filePath, "rb" );
	if ( pFile == NULL ) return NULL;							// auto delete
	if ( fread( pBuff, buffSize, 1, pFile ) < 1 ) return NULL;	// auto delete
	fclose( pFile );
	return pBuff.Release();	// return buffer
}

*/
template<class Type>
class ScopeSafePtr
{
	Type*	p;

	// Don't use.
	ScopeSafePtr( const ScopeSafePtr<Type>& src )
	{
		*this = src;
	}
	// Don't use.
	ScopeSafePtr<Type>& operator = ( const ScopeSafePtr<Type>& src )
	{
		ASSERT( FALSE );	// Don't use.
		return *this;
	}
	// Don't use.
	static void* operator new( size_t){ return NULL; }
	// Don't use.
	static void* operator new[]( size_t){ return NULL; }
	// Don't use.
	static void operator delete(void*){}
	// Don't use.
	static void operator delete[](void*){}
	// Don't use.
	Type& operator []( size_t index ) { return *( this->p + index ); }
	// Don't use.
	const Type& operator []( size_t index ) const{ return *( this->p + index ); }
#ifdef DEBUG_NEW
	// Don't use.
	static void* operator new( size_t, LPCSTR, int ){ return NULL; }
	// Don't use.
	static void* operator new[]( size_t, LPCSTR, int ){ return NULL; }
	// Don't use.
	static void operator delete(void*, LPCSTR, int ) {}
	// Don't use.
	static void operator delete[](void*, LPCSTR, int ) {}
#endif
public:
	ScopeSafePtr( Type* p = NULL )
	{
		this->p = p;
	}
	~ScopeSafePtr(void)
	{
		Delete();
	}

	bool IsNull(void) const
	{
		reutrn this->p == NULL ? true : false;
	}
	void Delete(void)
	{
		if ( this->p != NULL )
		{
			delete this->p;
		}
		this->p = NULL;
	}
	// Abandonment of ownership
	// Don't delete.
	Type* Release(void)
	{
		Type* p = this->p;
		this->p = NULL;
		return p;
	}
	// Delete old pointer
	Type* operator = ( Type* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// accesser
	Type* GetPtr(void)
	{
		return this->p;
	}
	const Type* GetPtr(void) const
	{
		return this->p;
	}
	operator Type*(void)
	{
		return this->p;
	}
	operator const Type*(void) const
	{
		return this->p;
	}
	Type** operator &(void)
	{
		return &this->p;
	}
	const Type** operator &(void) const
	{
		return &this->p;
	}
	Type& operator *(void)
	{
		return *this->p;
	}
	Type* operator -> (void)
	{
		return this->p;
	}

};

template<class Type>
class ScopeSafePtr<Type[]>
{
	Type*	p;

	// Don't use.
	ScopeSafePtr( const ScopeSafePtr<Type[]>& src )
	{
		*this = src;
	}
	// Don't use.
	ScopeSafePtr<Type[]>& operator = ( const ScopeSafePtr<Type[]>& src )
	{
		ASSERT( FALSE );	// Don't use.
		return *this;
	}
	// Don't use.
	static void* operator new( size_t){ return NULL; }
	// Don't use.
	static void* operator new[]( size_t){ return NULL; }
	// Don't use.
	static void operator delete(void*){}
	// Don't use.
	static void operator delete[](void*){}
#ifdef DEBUG_NEW
	// Don't use.
	static void* operator new( size_t, LPCSTR*, int ){ return NULL; }
	// Don't use.
	static void* operator new[]( size_t, LPCSTR*, int ){ return NULL; }
	// Don't use.
	static void operator delete(void*, LPCSTR, int ) {}
	// Don't use.
	static void operator delete[](void*, LPCSTR, int ) {}
#endif
public:

	ScopeSafePtr( Type* p = NULL )
	{
		this->p = p;
	}
	~ScopeSafePtr(void)
	{
		Delete();
	}
	bool IsNull(void) const
	{
		reutrn this->p == NULL ? true : false;
	}
	void Delete(void)
	{
		if ( this->p != NULL )
		{
			delete [] this->p;
		}
		this->p = NULL;
	}
	// Abandonment of ownership
	// Don't delete.
	Type* Release(void)
	{
		Type* p = this->p;
		this->p = NULL;
		return p;
	}
	// Delete old pointer
	Type* operator = ( Type* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// accesser
	Type* GetPtr(void)
	{
		return this->p;
	}
	const Type* GetPtr(void) const
	{
		return this->p;
	}
	operator Type*(void)
	{
		return this->p;
	}
	operator const Type*(void) const
	{
		return this->p;
	}
	Type** operator &(void)
	{
		return &this->p;
	}
	const Type** operator &(void) const
	{
		return &this->p;
	}
	
	Type& operator []( size_t index )
	{
		return *( this->p + index );
	}
	const Type& operator []( size_t index ) const
	{
		return *( this->p + index );
	}

};


// Don't use.
template<class Type>
class ScopeSafePtr<Type*>
{
	// Don't use.
	ScopeSafePtr(void) {}
	// Don't use.
	~ScopeSafePtr(void) {}
	// Don't use.
	ScopeSafePtr( const ScopeSafePtr<Type*>& src )
	{
		*this = src;
	}
	// Don't use.
	ScopeSafePtr<Type*>& operator = ( const ScopeSafePtr<Type*>& src )
	{
		ASSERT( FALSE );	// Don't use.
		return *this;
	}
};
