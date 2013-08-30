#pragma once

//! @brief		�\�[�g�\Array
//! @details	CArray�Ƀ\�[�g�@�\��ǉ������N���X�ł��B
//! @note		�f�t�H���g�ł�operator�ɂ��\�[�g���`���Ă��܂����A
//! <br>		�R�[���o�b�N�֐����`���邱�Ƃɂ���āA�\�[�g���J�X�^�}�C�Y�ł��܂��B
template<class Type, class ArgType = const Type&>
class SortArray
	: public CArray<Type, ArgType&>
{
public:

	//! @brief		��r�֐��^
	//! @details	��r�֐��̃v���g�^�C�v
	//! @retval		��	a��b��菬����
	//! @retval		 0	a��b�͓�����
	//! @retval		��	a��b���傫��
	//! @param		pA	[in]	��r�v�fA
	//! @param		pB	[in]	��r�v�fB
	typedef int (__cdecl * CompareFunction)(const void * pA, const void * pB) throw();

	//! @brief		operator�ɂ���r
	//! @details	operator�ɂ���r���`���܂��B
	//! @attention	�{�֐����g�p����ꍇ�AType�ɂ�operator ==��operator >����`����Ă���K�v������܂��B
	//! @retval		��	a��b��菬����
	//! @retval		 0	a��b�͓�����
	//! @retval		��	a��b���傫��
	//! @param		pA	[in]	��r�v�fA
	//! @param		pB	[in]	��r�v�fB
	static int CompareByOperator( Type* pA, Type* pB ) throw()
	{
		int ret = -1;
		if ( *pA == *pB )
		{
			ret = 0;
		}
		else if ( *pA > *pB )
		{
			ret = 1;
		}
		return ret;
	};

	//! @brief	�R���X�g���N�^
	SortArray(void) throw()
		: CArray<Type, Type&>()
	{}

	//! @brief	�f�X�g���N�^
	virtual ~SortArray(void) throw()
	{}

	//! @brief		�\�[�g
	//! @details	�v�f���\�[�g���܂��B
	//! @attention	��r�p�֐��͎g�����ŗp�ӂ��Ă��������B
	//! @note		�v�f���|�C���^�̏ꍇ�A�ʏ�̕��@�ł̓\�[�g���ł��Ȃ����߁A
	//! @br			��r�֐��̈����͗v�f�^�̃|�C���^�ɂ������̂�p�ӂ��āACompareFunction�ɃL���X�g���Ă��������B
	// !@param		compareFunction	[in]	��r�֐�
	//! @code
	//! // ��r�֐�
	//! int Compare( int** ppA, int** ppB )
	//! {
	//! 	return *(*ppA) - *(*ppB);
	//! };
	//! 
	//! void Func()
	//! {
	//! 	CAdpSortArray<int*> ary;
	//! 	int* pI;
	//! 	for ( int i = 0; i < 10; i++ )
	//! 	{
	//! 		pI = new int;
	//! 		*pI = i;
	//! 		ary.Add( pI );
	//! 	}
	//! 	ary.Sort( reinterpret_cast<SortArray::CompareFunction>( &Compare ) );
	//! }
	//! @endcode
	void
		Sort( CompareFunction compareFunction ) throw()
	{
		ASSERT( compareFunction != NULL );
		::qsort(	this->GetData(), static_cast<size_t>( this->GetCount() ),
					sizeof( Type ), compareFunction );
	};

	//! @brief		operator�ɂ��\�[�g
	//! @details	operator�ɂ���r�Ń\�[�g���s���܂�
	//! @attention	�{�֐����g�p����ꍇ�AType�ɂ�operator ==��operator >����`����Ă���K�v������܂��B
	void SortByOperator() throw()
	{
		Sort( reinterpret_cast<CompareFunction>( &CompareByOperator ) );
	}
};


