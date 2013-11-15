#pragma once

// スコープから外れると自動的にメモリを解放します
//
// 配列はScopePtr<～[]>と宣言すること
// 例
// ScopePtr<int[]> p = new int [ 4 ];


template<class T>
class ScopeSafePtr
{
	T*	p;

	// 使用禁止
	// コピーコンストラクタの禁止
	ScopeSafePtr( const ScopeSafePtr<T>& src )
	{
		*this - src;
	}
	// ScopePtr同士の代入の禁止
	ScopeSafePtr<T>& operator = ( const ScopeSafePtr<T>& src )
	{
		ASSERT( FALSE );
		return *this;
	}
public:
	ScopeSafePtr( T* p = NULL )
	{
		this->p = p;
	}
	~ScopeSafePtr(void)
	{
		Delete();
	}

	bool IsNull(void)
	{
		reutrn this->p == NULL ? true : false;
	}
	void Delete(void)
	{
		if ( this->p != NULL )
		{
			delete this->p;
			this->p = NULL;
		}
	}
	// 所有権の放棄
	// 内部では解放をしない
	T* Release(void)
	{
		T* p = this->p;
		this->p = NULL;
		return p;
	}

	// 代入
	// 今まで保持していた情報は破棄します
	T* operator = ( T* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// アクセス関連
	T* GetPtr(void)
	{
		return this->p;
	}
	const T* GetPtr(void) const
	{
		return this->p;
	}
	// キャスト
	operator T*(void)
	{
		return this->p;
	}
	// キャスト
	operator const T*(void) const
	{
		return this->p;
	}
	// 実体へのアクセス
	T& operator *(void)
	{
		return *this->p;
	}
	// ポインタのアドレスの取得
	T** operator &(void)
	{
		return &this->p;
	}
	// メンバへのアクセス
	T* operator -> (void)
	{
		return this->p;
	}

};

// 配列用
template<class T>
class ScopeSafePtr<T[]>
{
	T*	p;

	// 使用禁止
	// コピーコンストラクタの禁止
	ScopeSafePtr( const ScopeSafePtr<T[]>& src )
	{
		*this - src;
	}
	// ScopePtr同士の代入の禁止
	ScopeSafePtr<T[]>& operator = ( const ScopeSafePtr<T[]>& src )
	{
		ASSERT( FALSE );
		return *this;
	}
public:

	ScopeSafePtr( T* p = NULL )
	{
		this->p = p;
	}
	~ScopeSafePtr(void)
	{
		Delete();
	}

	bool IsNull(void)
	{
		reutrn this->p == NULL ? true : false;
	}
	// delete
	void Delete(void)
	{
		if ( this->p != NULL )
		{
			delete [] this->p;
			this->p = NULL;
		}
	}
	// 所有権の放棄
	// 内部では解放をしない
	T* Release(void)
	{
		T* p = this->p;
		this->p = NULL;
		return p;
	}

	// 代入
	// 今まで保持していた情報は破棄します
	T* operator = ( T* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// アクセス関連
	T* GetPtr(void)
	{
		return this->p;
	}
	const T* GetPtr(void) const
	{
		return this->p;
	}

	// 配列アクセス
	T* operator []( size_t i )
	{
		return this->p + i;
	}
	// 配列アクセス
	const T* operator []( size_t i ) const
	{
		return this->p + i;
	}

};

