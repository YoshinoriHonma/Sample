//! @file
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief		TypesafeEnum定義
//! @details	enum定義を定義名::値という形でアクセスできるようにします。
//! <br>		・同一の型以外の代入はできません
//! <br>		・型定義時にデフォルトの初期値を定義することにより、初期化漏れを防ぎます
//! <br>		　ファイルから取得した値など、止むを得ず数値型の値を代入する場合はImport関数を使ってください
//! @code
//! // 型の定義
//! TypesafeEnumBegin( Result, OK )
//! {
//! 	OK	= 0,
//! 	NG
//! } TypesafeEnumEnd;
//!
//! // 定義した型の利用
//! Result ret = Result::OK;
//! // 比較も可能
//! if ( ret == Result::OK )
//! {
//! 	// 代入も可能
//!		ret = Result::NG;
//! }
//! // 数値から値をセットしなくてはならない場合はImportTypesafeEnumを使ってください
//! // ※ この場合は型チェックが行われません
//! // 　 必要に応じて別途値のチェックを行ってください
//! Result ret;
//! ret.ImportTypesafeEnum( 0 );
//! // ファイルへの書き込みでは念のためExportTypesafeEnum()を呼び出してください
//! int value = ret.ExportTypesafeEnum();
//! @endcode
//! @attention	ファイルへ出力する場合はExportTypesafeEnum関数使ってください。
//! @note		TypesafeEnumはマクロを展開してクラスを生成します。
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief		TypesafeEnum定義開始
//! @param		enumName		[in]	定義名
//! @param		defaultValue	[in]	デフォルト値
//! @attention	必ずTypesafeEnumEndとセットで使用してください。
//! @note		本マクロを展開することで定義名でクラスが作成されます。
#define TypesafeEnumBegin( enumName, defaultValue )			\
class enumName												\
{															\
public:														\
	enum _##enumName;										\
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
	_##enumName ImportTypesafeEnum( int value )				\
	{														\
		this->value = static_cast<_##enumName>( value );	\
		return this->value;									\
	}														\
	int ExportTypesafeEnum() const							\
	{														\
		return static_cast<int>( this->value );				\
	}														\
private:													\
	_##enumName value;										\
public:														\
	enum _##enumName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief		TypesafeEnum定義終了
#define TypesafeEnumEnumEnd	; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
