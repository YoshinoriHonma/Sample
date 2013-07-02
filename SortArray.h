#pragma once

//! @brief		ソート可能Array
//! @details	CArrayにソート機能を追加したクラスです。
//! @note		デフォルトではoperatorによるソートを定義していますが、
//! <br>		コールバック関数を定義することによって、ソートをカスタマイズできます。
template<class Type, class ArgType = const Type&>
class SortArray
	: public CArray<Type, ArgType&>
{
public:

	//! @brief		比較関数型
	//! @details	比較関数のプロトタイプ
	//! @retval		負	aはbより小さい
	//! @retval		 0	aとbは等しい
	//! @retval		正	aはbより大きい
	//! @param		pA	[in]	比較要素A
	//! @param		pB	[in]	比較要素B
	typedef int (__cdecl * CompareFunction)(const void * pA, const void * pB) throw();

	//! @brief		operatorによる比較
	//! @details	operatorによる比較を定義します。
	//! @attention	本関数を使用する場合、Typeにはoperator ==とoperator >が定義されている必要があります。
	//! @retval		負	aはbより小さい
	//! @retval		 0	aとbは等しい
	//! @retval		正	aはbより大きい
	//! @param		pA	[in]	比較要素A
	//! @param		pB	[in]	比較要素B
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

	//! @brief	コンストラクタ
	SortArray(void) throw()
		: CArray<Type, Type&>()
	{}

	//! @brief	デストラクタ
	virtual ~SortArray(void) throw()
	{}

	//! @brief		ソート
	//! @details	要素をソートします。
	//! @attention	比較用関数は使う側で用意してください。
	//! @note		要素がポインタの場合、通常の方法ではソートができないため、
	//! @br			比較関数の引数は要素型のポインタにしたものを用意して、CompareFunctionにキャストしてください。
	// !@param		compareFunction	[in]	比較関数
	//! @code
	//! // 比較関数
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

	//! @brief		operatorによるソート
	//! @details	operatorによる比較でソートを行います
	//! @attention	本関数を使用する場合、Typeにはoperator ==とoperator >が定義されている必要があります。
	void SortByOperator() throw()
	{
		Sort( reinterpret_cast<CompareFunction>( &CompareByOperator ) );
	}
};


