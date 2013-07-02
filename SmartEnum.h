#pragma once
//! @file
//! @brief		�X�}�[�genum��`
//! @details	enum��`���`��::�l�Ƃ����`�ŃA�N�Z�X�ł���悤�ɂ��܂��B
//! <br>		�E����̌^�ȊO�̑���͂ł��܂���
//! <br>		�E�^��`���Ƀf�t�H���g�̏����l���`���邱�Ƃɂ��A�������R���h���܂�
//! <br>		�@�t�@�C������擾�����l�ȂǁA�~�ނ𓾂����l�^�̒l��������ꍇ��Import�֐����g���Ă�������
//! @code
//! // �^�̒�`
//! SmartEnumBegin( ABC )
//! {
//! 	OK,
//! 	NG
//! } SmartEnumEnd( ABC, OK );
//!
//! ��`�����^�̗��p
//! ABC a = ABC::OK;
//! ��r���\
//! if ( a == ABC::OK )
//! {
//!		a = ABC::NG;
//! }
//! // ���l����l���Z�b�g���Ȃ��Ă͂Ȃ�Ȃ��ꍇ��Import���g���Ă�������
//! // �� ���̏ꍇ�͌^�`�F�b�N���s���܂���
//! // �@ �K�v�ɉ����ĕʓr�l�̃`�F�b�N���s���Ă�������
//! BBB e;
//! e.Import( 0 );
//! // �t�@�C���ւ̏������݂ł͔O�̂���Export()���Ăяo���Ă�������
//! int value = e.Export();
//! @endcode
//! @attention	�t�@�C���֏o�͂���ꍇ�͔O�̂���Export�֐��Œl���擾���Ă��������B
//! @note		�X�}�[�genum�̓}�N����W�J���ăN���X�𐶐����܂��B


//! @brief		�X�}�[�genum��`�J�n
//! @param		enumName	[in]	��`��
//! @attention	�K��SmartEnumEnd�ƃZ�b�g�Ŏg�p���Ă��������B
//! @note	�{�}�N����W�J���邱�ƂŒ�`���ŃN���X���쐬����܂��B
#define SmartEnumBegin( enumName )							\
class enumName												\
{															\
public:														\
	enum _##enumName

//! @brief		�X�}�[�genum��`�I��
//! @param	enumName		[in]	SmartEnumBegin�Ŏw�肵����`��
//! @param	defaultValue	[in]	�f�t�H���g�l
//! @attention	enumName�͕K��SmartEnumBegin�Ŏw�肵����`�����g���Ă��������B
#define SmartEnumEnd( enumName, defaultValue )				\
	;														\
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
}

