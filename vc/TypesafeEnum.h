//! @file
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief		TypesafeEnum��`
//! @details	enum��`���`��::�l�Ƃ����`�ŃA�N�Z�X�ł���悤�ɂ��܂��B
//! <br>		�E����̌^�ȊO�̑���͂ł��܂���
//! <br>		�E�^��`���Ƀf�t�H���g�̏����l���`���邱�Ƃɂ��A�������R���h���܂�
//! <br>		�@�t�@�C������擾�����l�ȂǁA�~�ނ𓾂����l�^�̒l��������ꍇ��ImportTypesafeEnum�֐����g���Ă�������
//! @code
//! // �^�̒�`
//! TypesafeEnumBegin( Result, OK )
//! {
//! 	OK	= 0,
//! 	NG
//! } TypesafeEnumEnd;
//!
//! // ��`�����^�̗��p
//! Result ret = Result::OK;
//! // ��r���\
//! if ( ret == Result::OK )
//! {
//! 	// ������\
//!		ret = Result::NG;
//! }
//! // ���l����l���Z�b�g���Ȃ��Ă͂Ȃ�Ȃ��ꍇ��ImportTypesafeEnum���g���Ă�������
//! // �� ���̏ꍇ�͌^�`�F�b�N���s���܂���
//! // �@ �K�v�ɉ����ĕʓr�l�̃`�F�b�N���s���Ă�������
//! Result ret;
//! ret.ImportTypesafeEnum( 0 );
//! // �t�@�C���ւ̏������݂ł͔O�̂���ExportTypesafeEnum()���Ăяo���Ă�������
//! int value = ret.ExportTypesafeEnum();
//! @endcode
//! @attention	�t�@�C���֏o�͂���ꍇ��ExportTypesafeEnum�֐��g���Ă��������B
//! @note		TypesafeEnum�̓}�N����W�J���ăN���X�𐶐����܂��B
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief		TypesafeEnum��`�J�n
//! @param		enumName		[in]	��`��
//! @param		defaultValue	[in]	�f�t�H���g�l
//! @attention	�K��TypesafeEnumEnd�ƃZ�b�g�Ŏg�p���Ă��������B
//! @note		�{�}�N����W�J���邱�ƂŒ�`���ŃN���X���쐬����܂��B
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
//! @brief		TypesafeEnum��`�I��
#define TypesafeEnumEnumEnd	; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
