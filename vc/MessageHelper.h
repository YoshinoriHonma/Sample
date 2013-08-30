#pragma once

//! @brief		メッセージヘルパークラス
//! @details	SendMessage、PostMessage、PostThreadMessageのヘルパークラスです。
//! @note		通常、PostMessageやPostThreadMessageのパラメータに数値以外を渡す場合は
//! <br>		パラメータをnewしてアドレスを渡すことになりますが、
//! <br>		本クラスを使用することで送信元でのnew、送信先でのdeleteを隠蔽化します。
//! note		本クラスを使用した場合、PostThreadMessageは従来の非同期通信の他に同期通信も行えるようになります。
//! @code
//! 
//! // 送信元
//! void Test()
//! {
//!		// デフォルトコンストラクタで生成することで送信側としてインスタンスを生成します。
//!		MessageHelper<CString, int>		test;
//!		test.wParam = _T("test");
//!		test.PostMessage( m_hWnd/*対象のウィンドウハンドル*/, MessageId/*メッセージID*/ );
//! }
//! 
//! // 送信先
//! LRESULT OnTest( WPARAM wParam, LPARAM lParam )
//! {
//!		// コンストラクタにWPARAM、LPARAMを渡すことで受信側としてインスタンスを生成します。
//!		MessageHelper<CString, int>		test( wParam, lParam );
//!		AfxMessageBox( test.wParam );
//!		return 0;
//! }
//! @endcode
template<class WParamType, class LParamType>
class MessageHelper
{

#pragma region 非公開
	//! @brief		送信モード
	typedef enum MessageType
	{
		MessageTypeNoSend	= 0,		//!< 未送信
		MessageTypeSend,				//!< SendMessage
		MessageTypePost,				//!< PostMessage
		MessageTypePostThreadAsync,		//!< PostThreadMessage(非同期)
		MessageTypePostThreadSync		//!< PostThreadMessage(同期)
	};

	//! @brief	送信パラメータコンテナ
	template<class WParamType, class LParamType>
	class Param
	{
	public:

		//! @brief		コンストラクタ
		Param(void)
			: messageType( MessageTypeNoSend )
		{}

		//! @brief		デストラクタ
		//! @details	同期送信時はロックの解除を行います。
		~Param(void)
		{
		}

		//! @brief		同期送信
		//! @details	同期送信時にロックを行います。
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

		//! @brief		同期モード判定
		//! @retval	true	同期モード
		//! @retval	false	非同期モード
		//! @note	SendSyncが呼び出された場合はtrue、そうでない場合はfalseが返ります。
		bool IsSendSyncMode() const
		{
			return this->pSync != NULL ? true : false;
		}

	public:
		// 公開メンバ
		MessageType			messageType;		//!< メッセージタイプ
		WParamType			wParam;				//!< 送信パラメータ(WPARAM)
		LParamType			lParam;				//!< 送信パラメータ(LPARAM)
	private:
		// 非公開メンバ
		CEvent				sync;				//!< イベント同期オブジェクト
	};

#pragma endregion


#pragma region	使用禁止
private:

	//! @brief		コンストラクタ
	//! @attention	使用禁止
	MessageHelper( const MessageHelper& src )
		: isSender( false )
		, pParam( NULL )
	{
		*this = src;
	}

	//! @brief		operator =
	MessageHelper& operator = ( const MessageHelper& src )
	{
		ASSERT( FALSE );	//! @attention	使用禁止
		return *this;
	}

#pragma endregion

public:

	static const UINT	MessageIdBegin	= WM_APP;

	//! @brief		コンストラクタ
	//! @details	CPostMessageHelperを送信側で生成します。
	//! @note		送信側は送信パラメータの生成は行いますが、破棄しません。
	//! <br>		受信側は送信パラメータの破棄は行いますが、生成しません。
	MessageHelper(void)
		: isAutoDelete( true )
		, pParam( NULL )
	{
		this->pParam = new Param<WParamType, LParamType>();
	}

	//! @brief		コンストラクタ
	//! @details	CPostMessageHelperを受信側で生成します。
	//! @note		送信側は送信パラメータの生成は行いますが、破棄しません。
	//! <br>		受信側は送信パラメータの破棄は行いますが、生成しません。
	//! @param		wParam		in		送信パラメータ(WPARAM)
	//! @param		lParam		in		送信パラメータ(LPARAM)(未使用)
	MessageHelper( WPARAM wParam, LPARAM /*lParam*/ )
		: isAutoDelete( true )
		, pParam( NULL )
	{
		this->pParam	= reinterpret_cast<Param<WParamType, LParamType>*>( wParam );
		ASSERT( this->pParam->messageType != MessageTypeNoSend );		// 未送信パラメータは受け取らない
		// 送信モードによっては破棄しない
		switch ( this->pParam->messageType )
		{
		case MessageTypeSend:			// SendMessage
		case MessageTypePostThreadSync:	// PostThreadMessage(同期)
			this->isAutoDelete = false;
			break;
		}
	}

	//! @brief		デストラクタ
	~MessageHelper(void)
	{
		if ( !this->isAutoDelete && this->pParam != NULL )
		{
			switch ( this->pParam->messageType )
			{
			case MessageTypePostThreadSync:	// PostThreadMessage(同期)
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

	// WPARAMプロパティ宣言
	__declspec(property(get=GetWParam, put=SetWParam))	WParamType wParam;

	//! @brief	WPARAM取得
	//! @return	WPARAMの参照
	WParamType& GetWParam()
	{
		ASSERT( this->pParam != NULL );
		return this->pParam->wParam;
	}

	//! @brief	WPARAM設定
	//! @return	設定後の値
	//! @param	value		in	WPARAMの値
	WParamType& SetWParam( const WParamType& value )
	{
		ASSERT( this->pParam != NULL );
		this->pParam->wParam = value;
		return this->pParam->wParam;
	}

	// LPARAMプロパティ宣言
	__declspec(property(get=GetLParam, put=SetLParam))	LParamType lParam;

	//! @brief	LPARAM取得
	//! @return	LPARAMの参照
	LParamType& GetLParam()
	{
		ASSERT( this->pParam != NULL );
		return this->pParam->lParam;
	}

	//! @brief	LPARAM設定
	//! @return	設定後の値
	//! @param	value		in	LPARAMの値
	LParamType& SetLParam( const LParamType& value )
	{
		ASSERT( this->pParam != NULL );
		this->pParam->lParam = value;
		return this->pParam->lParam;
	}

	//! @brief		SendMessage
	//! @details	SendMessageでメッセージを送信します。
	//! @retval		TRUE		成功
	//! @retval		FALSE		失敗
	//! @param		hWnd		in	送信先のウィンドウハンドル
	//! @param		Msg			in	メッセージのタイプ
	BOOL Send( HWND hWnd, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// 未送信でなければならない
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
		// 成否にかかわらず送信元側で削除する
		this->isAutoDelete = true;
		return isComplete;
	}

	//! @brief		PostMessage
	//! @details	PostMessageでメッセージを送信します。
	//! @retval		TRUE		成功
	//! @retval		FALSE		失敗
	//! @param		hWnd		in	送信先のウィンドウハンドル
	//! @param		Msg			in	メッセージのタイプ
	BOOL Post( HWND hWnd, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// 未送信でなければならない
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePost;
		if ( ::PostMessage( hWnd, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			// パラメータは送信先で破棄する
			this->isAutoDelete = false;
			isComplete = TRUE;
		}
		// メッセージ送信失敗時は送信元側で削除する
		else
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

	//! @brief		PostThreadMessage
	//! @details	非同期でPostThreadMessageでメッセージを送信します。
	//! @retval		TRUE		成功
	//! @retval		FALSE		失敗
	//! @param		idThread	in	送信先のスレッドID
	//! @param		Msg			in	メッセージのタイプ
	BOOL PostThread( DWORD idThread, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// 未送信でなければならない
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePostThreadAsync;
		if ( ::PostThreadMessage( idThread, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			// パラメータは送信先で破棄する
			this->isAutoDelete = false;
			isComplete = TRUE;
		}
		// メッセージ送信失敗時は送信元側で削除する
		if ( !isComplete )
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

	//! @brief		PostThreadMessage
	//! @details	同期でPostThreadMessageでメッセージを送信します。
	//! @retval		TRUE		成功
	//! @retval		FALSE		失敗
	//! @param		idThread	in	送信先のスレッドID
	//! @param		Msg			in	メッセージのタイプ
	BOOL PostThreadSync( DWORD idThread, UINT Msg )
	{
		ASSERT( this->pParam != NULL );
		ASSERT( this->pParam->messageType == MessageTypeNoSend );		// 未送信でなければならない
		BOOL isComplete = FALSE;
		this->pParam->messageType = MessageTypePostThreadSync;
		if ( ::PostThreadMessage( idThread, Msg, reinterpret_cast<WPARAM>( this->pParam ), 0 ) )
		{
			if ( this->pParam->Lock() )
			{
				// メッセージ処理待ちの場合は送信先で破棄しないため、送信元で破棄するようにする
				this->isAutoDelete = true;
				isComplete = TRUE;
			}
		}
		// メッセージ送信失敗時は送信元側で削除する
		if ( !isComplete )
		{
			this->pParam->messageType = MessageTypeNoSend;
			this->isAutoDelete = true;
		}
		return isComplete;
	}

private:
	bool							isAutoDelete;	/*!< 自動削除フラグ
														@args	true	デストラクタで送信パラメータを破棄する
														@args	false	デストラクタで送信パラメータを破棄しない
													*/
	Param<WParamType, LParamType>*	pParam;			//!< 送信パラメータ
};

