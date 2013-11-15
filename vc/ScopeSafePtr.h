#pragma once

// �X�R�[�v����O���Ǝ����I�Ƀ�������������܂�
//
// �z���ScopePtr<�`[]>�Ɛ錾���邱��
// ��
// ScopePtr<int[]> p = new int [ 4 ];


template<class T>
class ScopeSafePtr
{
	T*	p;

	// �g�p�֎~
	// �R�s�[�R���X�g���N�^�̋֎~
	ScopeSafePtr( const ScopeSafePtr<T>& src )
	{
		*this - src;
	}
	// ScopePtr���m�̑���̋֎~
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
	// ���L���̕���
	// �����ł͉�������Ȃ�
	T* Release(void)
	{
		T* p = this->p;
		this->p = NULL;
		return p;
	}

	// ���
	// ���܂ŕێ����Ă������͔j�����܂�
	T* operator = ( T* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// �A�N�Z�X�֘A
	T* GetPtr(void)
	{
		return this->p;
	}
	const T* GetPtr(void) const
	{
		return this->p;
	}
	// �L���X�g
	operator T*(void)
	{
		return this->p;
	}
	// �L���X�g
	operator const T*(void) const
	{
		return this->p;
	}
	// ���̂ւ̃A�N�Z�X
	T& operator *(void)
	{
		return *this->p;
	}
	// �|�C���^�̃A�h���X�̎擾
	T** operator &(void)
	{
		return &this->p;
	}
	// �����o�ւ̃A�N�Z�X
	T* operator -> (void)
	{
		return this->p;
	}

};

// �z��p
template<class T>
class ScopeSafePtr<T[]>
{
	T*	p;

	// �g�p�֎~
	// �R�s�[�R���X�g���N�^�̋֎~
	ScopeSafePtr( const ScopeSafePtr<T[]>& src )
	{
		*this - src;
	}
	// ScopePtr���m�̑���̋֎~
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
	// ���L���̕���
	// �����ł͉�������Ȃ�
	T* Release(void)
	{
		T* p = this->p;
		this->p = NULL;
		return p;
	}

	// ���
	// ���܂ŕێ����Ă������͔j�����܂�
	T* operator = ( T* p )
	{
		Delete();
		this->p = p;
		return p;
	}

	// �A�N�Z�X�֘A
	T* GetPtr(void)
	{
		return this->p;
	}
	const T* GetPtr(void) const
	{
		return this->p;
	}

	// �z��A�N�Z�X
	T* operator []( size_t i )
	{
		return this->p + i;
	}
	// �z��A�N�Z�X
	const T* operator []( size_t i ) const
	{
		return this->p + i;
	}

};

