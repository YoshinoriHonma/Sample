#pragma once

//! @brief		���b�Z�[�W�w���p�[�N���X
//! @details	SendMessage�APostMessage�APostThreadMessage�̃w���p�[�N���X�ł��B
//! @note		�ʏ�APostMessage��PostThreadMessage�̃p�����[�^�ɐ��l�ȊO��n���ꍇ��
//! <br>		�p�����[�^��new���ăA�h���X��n�����ƂɂȂ�܂����A
//! <br>		�{�N���X���g�p���邱�Ƃő��M���ł�new�A���M��ł�delete���B�������܂��B
//! note		�{�N���X���g�p�����ꍇ�APostThreadMessage�͏]���̔񓯊��ʐM�̑��ɓ����ʐM���s����悤�ɂȂ�܂��B
//! @code
//! 
//! // ���M��
//! void Test()
//! {
//!		// �f�t�H���g�R���X�g���N�^�Ő������邱�Ƃő��M���Ƃ��ăC���X�^���X�𐶐����܂��B
//!		MessageHelper<CString, int>		test;
//!		test.wParam = _T("test");
//!		test.PostMessage( m_hWnd/*�Ώۂ̃E�B���h�E�n���h��*/, MessageId/*���b�Z�[�WID*/ );
//! }
//! 
//! // ���M��
//! LRESULT OnTest( WPARAM wParam, LPARAM lParam )
//! {
//!		// �R���X�g���N�^��WPARAM�ALPARAM��n�����ƂŎ�M���Ƃ��ăC���X�^���X�𐶐����܂��B
//!		MessageHelper<CString, int>		test( wParam, lParam );
//!		AfxMessageBox( test.wParam );
//!		return 0;
//! }
//! @endcode
template<class WParamType, class LParamType>
class MessageHelper
{

#pragma region ����J
	//! @brief		���M���[�h
	typedef enum MessageType
	{
		MessageTypeNoSend	= 0,		//!< �����M
		MessageTypeSend,				//!< SendMessage
		MessageTypePost,				//!< PostMessage
		MessageTypePostThreadAsync,		//!< PostThreadMessage(�񓯊�)
		MessageTypePostThreadSync		//!< PostThreadMessage(����)
	};

	//! @brief	���M�p�����[�^�R���e�i
	template<class WParamType, class LParamType>
	class Param
	{
	public:

		//! @brief		�R���X�g���N�^
		Param(void)
			: messageType( MessageTypeNoSend )
		{}

		//! @brief		�f�X�g���N�^
		//! @details	�������M���̓��b�N�̉������s���܂��B
		~Param(void)
		{
		}

		//! @brief		�������M
		//! @details	�������M���Ƀ��b�N���s���܂��B
		bool Lock()
		{
			bool isSync = false;

			CSingleLock lock( &this->sync, TRUE );
			isSync = true;
			return isSync;
		}
		void Unlock()
		{
			this->sync.SetEvent();
		}

		//! @brief		�������[�h����
		//! @retval	true	�������[�h
		//! @retval	false	�񓯊����[�h
		//! @note	SendSync���Ăяo���ꂽ�ꍇ��true�A�����łȂ��ꍇ��false���Ԃ�܂��B
		bool IsSendSyncMode() const
		{
			return this->pSync != NULL ? true : false;
		}

	public:
		// ���J�����o
		MessageType			messageType;		//!< ���b�Z�[�W�^�C�v
		WParamType			wParam;				//!< ���M�p�����[�^(WPARAM)
		LParamType			lParam;				//!< ���M�p�����[�^(LPARAM)
	private:
		// ����J�����o
		CEvent				sync;				//!< �C�x���g�����I�u�W�F�N�g
	};

#pragma endregion


#pragma region	�g�p�֎~
private:

	//! @brief		�R���X�g���N�^
	//! @attention	�g�p�֎~
	MessageHelper( const MessageHelper& src )
		: isSender( false )
		, pParam( NULL )
	{
		*this = src;
	}

	//! @brief		operator =
	MessageHelper& operator = ( const MessageHelper& src )
	{
		ASSERT( FALSE );	//! @attention	�g�p�֎~
		return *this;
	}

#pragma endregion

public:

	static const UINT	MessageIdBegin	= WM_APP;

	//! @brief		�R���X�g���N�^
	//! @details	CPostMessageHelper�𑗐M���Ő������܂��B
	//! @note		���M���͑��M�p�����[�^�̐����͍s���܂����A�j�����܂���B
	//! <br>		��M���͑��M�p�����[�^�̔j���͍s���܂����A�������܂���B
	MessageHelper(void)
		: isAutoDelete( true )
		, pParam( NULL )
	{
		this->pParam = new Param<WParamType, LParamType>();
	}

	//! @brief		�R���X�g���N�^
	//! @details	CPostMessageHelper����M���Ő������܂��B
	//! @note		���M���͑��M�p�����[�^�̐����͍s���܂����A�j�����܂���B
	//! <br>		��M���͑��M�p�����[�^�̔j���͍s���܂����A�������܂���B
	//! @param		wParam		in		���M�p�����[�^(WPARAM)
	//! @param		lParam		in		���M�p�����[�^(LPARAM)(���g�p)
	MessageHelper( WPARAM wParam, LPARAM /*lParam*/ )
		: isAutoDelete( true )
		, pParam( NULL )
	{
		this->pParam	= reinterpret_cast<Param<WParamType, LParamType>*>( wParam );
		ASSERT( this->pParam->messageType != MessageTypeNoSend );		// �����M�p�����[�^�͎󂯎��Ȃ�
		// ���M���[�h�ɂ���Ă͔j�����Ȃ�
		switch ( this->pParam->messageType )
		{
		case MessageTypeSend:			// SendMessage
		case MessageTypePostThreadSync:	// PostThreadMessage(����)
			this->isAutoDelete = false;
			break;
		}
	}

	//! @brief		�f�X�g���N�^
	~MessageHelper(void)
	{
		if ( !this->isAutoDelete && this->pParam != NULL )
		{
			switch ( this->pParam->messageType )
			{
			case MessageTypePostThreadSync:	// PostThreadMessage(����)
				this->pParam->Unlock();
				break;
			}
		}
		if ( this->isAutoDelete )
		{
			delete this->pParam;
			this->pParam = NULL;
		}
	}

	// WPARAM�v���p�e�B�錾
	__declspec(property(get=GetWParam, put=SetWParam))	WParamType wParam;

	//! @brief	WPARAM�擾
	//! @return	WPARAM�̎Q��
	WParamType& GetWParam()
	{
		ASSERT( this->pParam != NULL );
		return this->pParam->wParam;
	}

	//! @brief	WPARAM�ݒ�
	//! @return	�ݒ��̒l
	//! @param	value		in	WPARAM�̒l
	WParamType& SetWParam( const WParamType& value )
	{
		ASSERT( this->pParam != NULL );
		this->pParam->wParam = value;
		return this->pParam->wParam;
	}

	// LPARAM�v���p�e�B�錾
	__declspec(property(get=GetLParam, put=SetLParam))	LParamType lParam;

	//! @brief	LPARAM�擾
	//! @return	LPARAM�̎Q��
	LParamType& GetLParam()
	{
		ASSERT( this->pParam != NULL );
		return this->pParam->lParam;
	}

	//! @brief	LPARAM�ݒ�
	//! @return	�ݒ��̒l
	//! @param	value		in	LPARAM�̒l
	LParamType& SetLParam( const LParamType& value )
	{
		ASSERT( this->pParam != NULL );
		this->pParam->lParam = value;
		return this->pParam->lParam;
	}

	//! @brief		SendMessage
	//! @details	SendMessage�Ń��b�Z�[�W�𑗐M���܂��B
	//! @retval		TRUE		����
	//! @retval		FALSE		���s
	//! @param		hWnd		in	���M��̃E�B���h�E�n���h��
	//! @param		Msg			in	���b�Z�[�W�̃^�C�v
	BOOL Send( HWND hWnd, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// �����M�łȂ���΂Ȃ�Ȃ�
		BOOL isComplete = FALSE;
		this->pParam->messageType	= MessageTypeSend;
		if ( ::SendMessage( hWnd, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			isComplete = TRUE;
		}
		else
		{
			this->pParam->messageType = MessageTypeNoSend;
		}
		// ���ۂɂ�����炸���M�����ō폜����
		this->isAutoDelete = true;
		return isComplete;
	}

	//! @brief		PostMessage
	//! @details	PostMessage�Ń��b�Z�[�W�𑗐M���܂��B
	//! @retval		TRUE		����
	//! @retval		FALSE		���s
	//! @param		hWnd		in	���M��̃E�B���h�E�n���h��
	//! @param		Msg			in	���b�Z�[�W�̃^�C�v
	BOOL Post( HWND hWnd, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// �����M�łȂ���΂Ȃ�Ȃ�
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePost;
		if ( ::PostMessage( hWnd, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			// �p�����[�^�͑��M��Ŕj������
			this->isAutoDelete = false;
			isComplete = TRUE;
		}
		// ���b�Z�[�W���M���s���͑��M�����ō폜����
		else
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

	//! @brief		PostThreadMessage
	//! @details	�񓯊���PostThreadMessage�Ń��b�Z�[�W�𑗐M���܂��B
	//! @retval		TRUE		����
	//! @retval		FALSE		���s
	//! @param		idThread	in	���M��̃X���b�hID
	//! @param		Msg			in	���b�Z�[�W�̃^�C�v
	BOOL PostThread( DWORD idThread, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// �����M�łȂ���΂Ȃ�Ȃ�
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePostThreadAsync;
		if ( ::PostThreadMessage( idThread, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			// �p�����[�^�͑��M��Ŕj������
			this->isAutoDelete = false;
			isComplete = TRUE;
		}
		// ���b�Z�[�W���M���s���͑��M�����ō폜����
		if ( !isComplete )
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

	//! @brief		PostThreadMessage
	//! @details	������PostThreadMessage�Ń��b�Z�[�W�𑗐M���܂��B
	//! @retval		TRUE		����
	//! @retval		FALSE		���s
	//! @param		idThread	in	���M��̃X���b�hID
	//! @param		Msg			in	���b�Z�[�W�̃^�C�v
	BOOL PostThreadSync( DWORD idThread, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// �����M�łȂ���΂Ȃ�Ȃ�
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePostThreadSync;
		if ( ::PostThreadMessage( idThread, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			if ( this->pParam->Lock() )
			{
				// ���b�Z�[�W�����҂��̏ꍇ�͑��M��Ŕj�����Ȃ����߁A���M���Ŕj������悤�ɂ���
				this->isAutoDelete = true;
				isComplete = TRUE;
			}
		}
		// ���b�Z�[�W���M���s���͑��M�����ō폜����
		if ( !isComplete )
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

private:
	bool							isAutoDelete;	/*!< �����폜�t���O
														@args	true	�f�X�g���N�^�ő��M�p�����[�^��j������
														@args	false	�f�X�g���N�^�ő��M�p�����[�^��j�����Ȃ�
													*/
	Param<WParamType, LParamType>*	pParam;			//!< ���M�p�����[�^
};

