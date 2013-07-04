//! @file
//! @brief		スマートenum定義
//! @details	enum定義を定義名::値という形でアクセスできるようにします。
//! <br>		・同一の型以外の代入はできません
//! <br>		・型定義時にデフォルトの初期値を定義することにより、初期化漏れを防ぎます
//! <br>		　ファイルから取得した値など、止むを得ず数値型の値を代入する場合はImport関数を使ってください
//! @code
//! // 型の定義
//! SmartEnumBegin( ABC, OK )
//! {
//! 	OK,
//! 	NG
//! } SmartEnumEnd;
//!
//! 定義した型の利用
//! ABC a = ABC::OK;
//! 比較も可能
//! if ( a == ABC::OK )
//! {
//!		a = ABC::NG;
//! }
//! // 数値から値をセットしなくてはならない場合はImportを使ってください
//! // ※ この場合は型チェックが行われません
//! // 　 必要に応じて別途値のチェックを行ってください
//! BBB e;
//! e.Import( 0 );
//! // ファイルへの書き込みでは念のためExport()を呼び出してください
//! int value = e.Export();
//! @endcode
//! @attention	ファイルへ出力する場合は念のためExport関数で値を取得してください。
//! @note		スマートenumはマクロを展開してクラスを生成します。

//! @brief		スマートenum定義開始
//! @param		enumName		[in]	定義名
//! @param		defaultValue	[in]	デフォルト値
//! @attention	必ずSmartEnumEndとセットで使用してください。
//! @note	本マクロを展開することで定義名でクラスが作成されます。
#define SmartEnumBegin( enumName, defaultValue )			\
class enumName												\
{															\
public:														\
	enum _##enumName;										\
															\
	enumName(void)											\
	: value( enumName##::##defaultValue )					\
	{}														\
	enumName( _##enumName value )							\
	{														\
		*this = value;										\
	}														\
	~enumName(void){}										\
	_##enumName operator = ( _##enumName value )			\
	{														\
		this->value = value;								\
		return this->value;									\
	}														\
	operator _##enumName(void) const						\
	{														\
		return this->value;									\
	}														\
	_##enumName Import( int value )							\
	{														\
		this->value = static_cast<_##enumName>( value );	\
		return this->value;									\
	}														\
	int Export() const										\
	{														\
		return static_cast<int>( this->value );				\
	}														\
private:													\
	_##enumName value;										\
public:														\
	enum _##enumName


//! @brief		スマートenum定義終了
#define SmartEnumEnd	; };
