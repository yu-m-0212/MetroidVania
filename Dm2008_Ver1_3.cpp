

#include "library.h"
#include "dm2008_Ver1_3.h"


namespace  DM_
{
	//	環境設定
	//	const  DWORD  kMAX_SEGMENT = 10;		//	最大セグメント数
	//	const  DWORD  kMAX_BUFFER = 100;		//	最大バッファ数
	//	const  DWORD  kVALUE_FADE = 20;			//	フェードイン、アウトする一回あたりのボリューム量を指定
	const  DWORD  kSTANDERDSIZE = 88200;	//	基準サイズ（バッファサイズの半分に等しい）
	const  DWORD  kVOLUME_MIN = (DSBVOLUME_MIN + 5000);		//	DSBVOLUME_MINが小さすぎるため、自分で管理



															//	エラー番号
	namespace  Err {
		enum Type
		{
			WRG,		//	ファイルが異常です
			CCIM,		//	DirectMusicの作成に失敗
			CCIS,		//	DirectSoundの作成に失敗
			INIT,		//	Initializeに失敗
			SCL,		//	SetCooperativeLevelに失敗
			CSB,		//	バッファ作成に失敗
			QI,			//	QueryInterfaceに失敗
			SFMT,		//	フォーマット設定に失敗
			IA,			//	InitAudioに失敗
			CCILOADER,	//	ローダー作成に失敗
			MBTWC,		//	unicode変換に失敗
			SSD,		//	ディレクトリ設定に失敗
			PSE,		//	セグメントデータを再生できない
			INVALID,	//	ファイルが読み込めない
			DL,			//	ダウンロードできない
			UNLD,		//	アンロードできない
			INVOPEN,	//	ファイルが開けない
			RIFF,		//	RIFFチャンクが読み込めない
			FMT,		//	fmtチャンクが読み込めない
			NOTPCM,		//	読み込み対象のWAVEはサポートされていない形式
			DATA,		//	dataチャンクは存在しない
			READERROR,	//	読み込み失敗
			NOTHING		//	エラーナシ
		};
	}

	//	エフェクトフラグ番号
	namespace Effect {
		enum Type
		{
			NOTHING = 0,	//	エフェクトはなし
			FADEIN,			//	フェードイン
			FADEOUT,		//	フェードアウト
			STOP,			//	停止中
		};
	}


	//	ストリーミングサウンド情報
	struct MYSOUNDINFO
	{
		//	ストリーム情報
		char					szFileName[MAX_PATH];	//	再生対象のファイル名
		DWORD					dwReadPosition;			//	読みこんでいるファイルポジション
		DWORD					dwHeaderSize;			//	読みこんでいるWAVEヘッダのサイズ
		int						iStreamPos;				//	書き込みの対象を表す（0なら後半、1なら前半）
		bool					bStream;				//	ストリーミング再生を許可するかどうか
														//	ループ情報
		bool					bLoop;					//	ループさせるかどうか
		DWORD					dwReStart;				//	ループさせるとき、どこまで戻るか
		DWORD					dwLoop;					//	どこまできたらループさせるか
		DWORD					dwCnt;					//	何回ループを繰り返すか
														//	フェードインアウト処理
		Effect::Type			effectflag;		//	エフェクトの種類
		int						iFade;			//	フェード量
		long int				iVolume;		//	ボリューム
		IDirectSoundBuffer8*	sec_ptr;	//	セカンダリ（メモリ上の）バッファ
	};

	class Obj
	{
		HWND								wnd;
		vector<weak_ptr<class  Sound> >		sounds;
		typedef  weak_ptr<Obj>  WP;
		static  Obj::WP  winst;		//	１つしか存在できないインスタンスのウィークポインタ
		Obj();
		bool  Initialize(HWND wnd_, DWORD dwBufferSize);
	public:
		IDirectSound8*			ds_ptr;		//	DirectSoundオブジェクト
		IDirectSoundBuffer*		pri_ptr;	//	プライマリ（可聴）バッファ
											//	サウンド環境情報
		DSCAPS					caps;			//	デバイス能力を格納
		DWORD					dwBufferSize;	//	用意するバッファのサイズ
		typedef  shared_ptr<Obj>  SP;
		~Obj();
		//	DIライブラリを生成する
		static  Obj::SP  Create(HWND  wnd_, DWORD dwBufferSize = kSTANDERDSIZE);
		static  Obj::SP  GetInst();
		HWND  Wnd() { return  wnd; }
		IDirectSound8&  DSound() { return  *ds_ptr; }
		//仮
		//static  Err::Type InitDirectSound(HWND, DWORD dwBufferSize = kSTANDERDSIZE);	//	サウンド環境を初期化します
		void Check_Stream(void);			//	ストリーミング再生を監視します(毎フレーム必ず呼ぶ)
		void GetDeviceCaps(DSCAPS*);			//	デバイスの情報を取得します
		void AddChaild(shared_ptr<class  Sound> cs_);
	};



	class Sound
	{
		Sound();
		bool  Initialize();
	public:
		enum  Kind { eStream, eSE };
		MYSOUNDINFO sinfo;
		typedef  weak_ptr<Sound>  WP;
		typedef  shared_ptr<Sound>  SP;
		~Sound();
		//BGM等長い曲を再生する場合
		static  Sound::SP  CreateStream(const char*  szFileName);
		//効果音（ＳＥ）等短い曲を再生する場合
		static  Sound::SP  CreateSE(const char*  szFileName);
		HRESULT SetVolume(long);			//	ボリューム調整を行います
		HRESULT SetFrequency(DWORD);		//	周波数を変更します
		HRESULT SetPan(int);				//	パン設定を行います
		int GetVolume();					//	現在のボリューム値を取得します
		void FadeOut(int);				//	フェードアウトさせます
		void SetLoopInfo(int, int iLoopCnt = 0);	//	ループ情報を設定します
		HRESULT Play_Normal(bool bLoop);	//	バッファ内のデータを再生します
		HRESULT Play_FadeIn(DWORD, int);	//	バッファ内のデータをフェードインして再生します
		void Check_Stream(DWORD  dwBufferSize_);
		HRESULT Stop();					//	再生中のバッファを停止させます

	private:
		bool  CreateStream_Sub(const char*);
		bool  CreateSE_Sub(const char*);
		void LoadStream(int iWriteOffset, int iStreamPos);
		Err::Type WriteWaveStream(const char* szFileName,
			BYTE* data_ptr, int iOffset, DWORD dwSize, int iWriteOffset);
		void WriteSoundData(BYTE* data_ptr, int iOffset, int iSize);
		void WriteMute(int iSize);
		Err::Type LoadWaveStreamFirst(const char* szFileName,
			BYTE** data_ptr, DWORD dwSize, WAVEFORMATEX* wf_ptr);
		DWORD GetStatus();					//	バッファの状態を取得します
		DWORD GetCurrentPosition();
		Err::Type CreateSoundBuffer(WAVEFORMATEX* wf_ptr, DWORD dwBufbyte);

		///////////////////////////////////////////////////////////////////////////////////////
		Err::Type CreateSoundBuffer(int iNumber, WAVEFORMATEX* wf_ptr, DWORD dwBufbyte);
	};
	std::string GetErrorMessage(Err::Type number);// エラー番号から文字列を取得します
}
namespace DM_
{
	Obj::WP Obj::winst;	//	１つしか存在できないインスタンス
	Obj::Obj()
	{
		this->wnd = 0;
		this->ds_ptr = nullptr;
		this->dwBufferSize = 0;
		this->pri_ptr = 0;
		ZeroMemory(&this->caps, sizeof(DSCAPS));
	}
	bool Obj::Initialize(HWND wnd_, DWORD dwBufferSize)
	{
		wnd = wnd_;


		//	DirectSoundの初期化
		if (FAILED(CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_INPROC,
			IID_IDirectSound8, (void **)&this->ds_ptr)))
		{
			return false;	// return Err::CCIS;
		}
		if (FAILED(this->ds_ptr->Initialize(NULL)))
		{
			return false;	// return Err::INIT;
		}
		if (FAILED(this->ds_ptr->SetCooperativeLevel(wnd, DSSCL_PRIORITY)))
		{
			return false;	// return Err::SCL;
		}
		//	プライマリサウンドバッファを作成
		DSBUFFERDESC desc =
		{
			sizeof(DSBUFFERDESC)
		};

		desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		if (FAILED(this->ds_ptr->CreateSoundBuffer(&desc, &this->pri_ptr, NULL)))
		{
			return false;	// return Err::CSB;
		}
		//	情報の初期化
		Obj::ds_ptr->GetCaps(&this->caps);
		Obj::dwBufferSize = dwBufferSize;
		//return Err::NOTHING;
		return true;
	}

	Obj::~Obj()
	{
		//	DirectSound
		kREL(this->pri_ptr);
		kREL(this->ds_ptr);
	}
	//	DIライブラリを生成する
	Obj::SP Obj::Create(HWND		hw_, DWORD dwBufferSize)	//	ウィンドウハンドル
	{
		if (winst.expired()){
			Obj::SP sp = Obj::SP(new Obj());
			if (sp->Initialize(hw_, dwBufferSize)){
				winst = sp;
				return sp;
			}
		}
		return 0;
	}
	Obj::SP Obj::GetInst()
	{
		if (!winst.expired()){
			return winst.lock();
		}
		return 0;
	}



	//--------------------------------------------------------------------------------------------
	void Obj::Check_Stream(void)
	{
		class cNullCheck{
		public:
			// タスクが既に解放されているならtrueを返す
			bool operator()(Sound::WP t_) const {
				bool rtv = t_.expired();
				return  rtv;
			}
		};
		auto endIt = remove_if(this->sounds.begin(), this->sounds.end(), cNullCheck());
		this->sounds.erase(endIt, this->sounds.end());	//既に消失しているものを全てを削除する

		//	読み込みタイミングを調べる
		for (auto it = this->sounds.begin();
			it != this->sounds.end();
			it++)
		{
			auto sp = it->lock();
			sp->Check_Stream(this->dwBufferSize);
		}

	}
	//--------------------------------------------------------------------------------------------
	void Obj::GetDeviceCaps(DSCAPS* caps_ptr)
	{
		memcpy(caps_ptr, &this->caps, sizeof(DSCAPS));
	}
	void Obj::AddChaild(shared_ptr<class  Sound>  cs)
	{
		this->sounds.push_back(cs);
	}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	Err::Type Sound::CreateSoundBuffer(WAVEFORMATEX* wf_ptr, DWORD dwBufbyte)
	{
		DSBUFFERDESC desc;
		LPDIRECTSOUNDBUFFER lpTempBuff = NULL;
		Err::Type hRet = Err::NOTHING;		//	関数戻り値

		//	サウンドデータ
		if (!wf_ptr)
		{
			return Err::SFMT;
		}
		//	サウンドバッファの設定
		ZeroMemory(&desc, sizeof(desc));
		desc.lpwfxFormat = wf_ptr;
		desc.dwSize = sizeof(DSBUFFERDESC);
		desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCDEFER |
			DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;
		desc.dwBufferBytes = dwBufbyte;	//	バッファサイズ
		//	DirectSoundBuffer生成
		auto  dm = Obj::GetInst();
		if (FAILED(dm->ds_ptr->CreateSoundBuffer(&desc, &lpTempBuff, NULL)))
		{
			hRet = Err::CSB;
			goto end;
		}
		//	DirectSoundBuffer8取得
		if (FAILED(lpTempBuff->QueryInterface(IID_IDirectSoundBuffer8, (void**)&this->sinfo.sec_ptr)))
		{
			hRet = Err::QI;
			goto end;
		}
	end:
		kREL(lpTempBuff);
		return hRet;
	}
	//--------------------------------------------------------------------------------------------
	Sound::SP  Sound::CreateSE(const char* szFileName)
	{
		auto sp = SP(new Sound());
		if (sp){
			bool  rtv;
			rtv = sp->CreateSE_Sub(szFileName);
			if (rtv){
				auto  ob = Obj::GetInst();
				ob->AddChaild(sp);
				return sp;
			}
		}
		return 0;
	}
	//--------------------------------------------------------------------------------------------
	Sound::SP  Sound::CreateStream(const char* szFileName)
	{
		auto sp = SP(new Sound());
		if (sp){
			bool  rtv;
			rtv = sp->CreateStream_Sub(szFileName);
			if (rtv){
				auto  ob = Obj::GetInst();
				ob->AddChaild(sp);
				return sp;
			}
		}
		return 0;
	}
	//--------------------------------------------------------------------------------------------
	bool Sound::CreateSE_Sub(const char* szFileName)
	{
		WAVEFORMATEX wf;
		BYTE* data_ptr = NULL;
		bool hRet = true;		//	関数戻り値

		kREL(this->sinfo.sec_ptr);
		ZeroMemory(&wf, sizeof(WAVEFORMATEX));
		//	DirectSoundBuffer設定
		if (Err::NOTHING != this->LoadWaveStreamFirst(szFileName, &data_ptr, 0, &wf))
		{
			hRet = false;// = Err::WRG;
			goto end;
		}
		//	バッファの生成
		if (Err::NOTHING != this->CreateSoundBuffer(&wf, this->sinfo.dwReadPosition - this->sinfo.dwHeaderSize))
		{
			goto end;
		}
		//	バッファロック
		this->WriteSoundData(data_ptr, 0, this->sinfo.dwReadPosition - this->sinfo.dwHeaderSize);
		//	サウンド情報を格納
		strcpy_s(this->sinfo.szFileName, MAX_PATH, szFileName);	//	サウンドファイル名を格納
		this->sinfo.iStreamPos = 0;
		this->sinfo.bLoop = false;
		this->sinfo.bStream = false;
	end:
		verDelete_Vector(data_ptr);
		return hRet;
	}
	//--------------------------------------------------------------------------------------------
	bool Sound::CreateStream_Sub(const char* szFileName)
	{
		BYTE* data_ptr = NULL;
		WAVEFORMATEX wf;
		bool  hRet = true;// Err::NOTHING;		//	関数戻り値

		auto  ds = Obj::GetInst();


		kREL(this->sinfo.sec_ptr);
		//	DirectSoundBuffer設定
		if (Err::NOTHING != this->LoadWaveStreamFirst(szFileName, &data_ptr, ds->dwBufferSize, &wf))
		{
			hRet = false;// Err::WRG;
			goto end;
		}
		//	バッファの生成
		if (Err::NOTHING != this->CreateSoundBuffer(&wf, ds->dwBufferSize * 2))
		{
			goto end;
		}
		//	バッファロック
		this->WriteSoundData(data_ptr, 0, ds->dwBufferSize);
		//	サウンド情報を格納
		strcpy_s(this->sinfo.szFileName, MAX_PATH, szFileName);	//	サウンドファイル名を格納
		this->sinfo.iStreamPos = 0;
		this->sinfo.bStream = true;
	end:
		verDelete_Vector(data_ptr);
		return hRet;
	}

	//--------------------------------------------------------------------------------------------
	Sound::Sound()
	{
		this->sinfo.sec_ptr = nullptr;
		this->sinfo.bLoop = false;
		this->sinfo.bStream = false;
		this->sinfo.dwCnt = 0;
		this->sinfo.dwHeaderSize = 0;
		this->sinfo.dwLoop = 0;
		this->sinfo.dwReadPosition = 0;
		this->sinfo.dwReStart = 0;
	}
	//--------------------------------------------------------------------------------------------
	Sound::~Sound()
	{
		kREL(this->sinfo.sec_ptr);
	}
	//--------------------------------------------------------------------------------------------
	DWORD Sound::GetCurrentPosition()
	{
		return this->sinfo.dwReadPosition;
	}
	//--------------------------------------------------------------------------------------------
	DWORD Sound::GetStatus( )
	{
		DWORD dwStatus;

		this->sinfo.sec_ptr->GetStatus(&dwStatus);
		return dwStatus;
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::Play_FadeIn(DWORD dwPlayFlag, int iValue)
	{
		HRESULT hRet;

		//	初期化されていない場合は再生しない
		if (this->sinfo.sec_ptr == NULL)
		{
			return DSERR_INVALIDCALL;
		}
		hRet = this->sinfo.sec_ptr->Play(0, 0, dwPlayFlag);
		this->SetVolume(kVOLUME_MIN);
		this->sinfo.iVolume = kVOLUME_MIN;
		this->sinfo.effectflag = Effect::FADEIN;
		this->sinfo.iFade = iValue;
		return hRet;
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::Play_Normal(bool bLoop)
	{
		HRESULT hRet;
		DWORD dwPlayFlag = 0;

		//	初期化されていない場合は再生しない
		if (this->sinfo.sec_ptr == NULL)
		{
			return DSERR_INVALIDCALL;
		}
		//	再生
		this->sinfo.sec_ptr->SetCurrentPosition(0);
		if (bLoop || this->sinfo.bStream)
		{
			dwPlayFlag = DSBPLAY_LOOPING;
		}
		hRet = this->sinfo.sec_ptr->Play(0, 0, dwPlayFlag);
		//	ボリュームやエフェクトの初期化
		this->SetVolume(DSBVOLUME_MAX);
		this->sinfo.bLoop = bLoop;
		this->sinfo.iVolume = DSBVOLUME_MAX;
		this->sinfo.effectflag = Effect::NOTHING;
		return hRet;
	}

	//--------------------------------------------------------------------------------------------
	Err::Type Sound::LoadWaveStreamFirst(const char* szFileName,
		BYTE** data_ptr, DWORD dwSize, WAVEFORMATEX* wf_ptr)
	{
		HANDLE hFile = NULL;		//	Waveファイルハンドル
		Err::Type hRet = Err::NOTHING;	//	戻り値を格納します
		DWORD dwResult;			//	ReadFileの結果を格納する
		DWORD dwRiffSize;		//	RIFFチャンクの大きさを取得
		DWORD dwFmtSize;		//	fmtチャンクの大きさを取得
		DWORD dwDataSize;		//	dataチャンクの大きさを取得
		char szChunk[4] = "";	//	チャンクの文字列を格納します（\0は保存しません）
		bool bLoop;

		//	ファイルを開く
		hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hRet = Err::INVOPEN;
			goto end;
		}
		//	RIFFチャンクの読み込み
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	読み込み失敗、もしくはRIFFではなかった場合
		if ((dwResult != 4) || (memcmp("RIFF", szChunk, 4)))
		{
			hRet = Err::RIFF;
			goto end;
		}
		ReadFile(hFile, &dwRiffSize, sizeof(DWORD), &dwResult, NULL);
		ReadFile(hFile, szChunk, 4, &dwResult, NULL);
		//	読み込み失敗、もしくはWAVEではなかった場合
		if ((dwResult != 4) || (memcmp("WAVE", szChunk, 4)))
		{
			hRet = Err::RIFF;
			goto end;
		}
		//	Foramtチャンク
		bLoop = true;
		while (bLoop)
		{
			ReadFile(hFile, szChunk, 4, &dwResult, NULL);
			//	読み込み失敗、もしくは不正なデータ形式だった場合
			if (dwResult != 4)
			{
				hRet = Err::DATA;
				goto end;
			}
			//	目的のチャンクだった場合
			if (memcmp("fmt", szChunk, 3) == 0)
			{
				bLoop = false;
			}
			else
			{
				//	チャンク違い
				ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
				SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
			}
		}
		//	fmtデータサイズ
		ReadFile(hFile, &dwFmtSize, sizeof(DWORD), &dwResult, NULL);
		//	フォーマット情報取得
		ReadFile(hFile, wf_ptr, dwFmtSize, &dwResult, NULL);
		if ((wf_ptr->wFormatTag != WAVE_FORMAT_PCM))
		{
			hRet = Err::NOTPCM;
			goto end;
		}
		wf_ptr->cbSize = 0;
		//	Dataチャンク検索
		bLoop = true;
		while (bLoop)
		{
			ReadFile(hFile, szChunk, 4, &dwResult, NULL);
			//	読み込み失敗、もしくは不正なデータ形式だった場合
			if (dwResult != 4)
			{
				hRet = Err::DATA;
				goto end;
			}
			//	Dataチャンクがみつかった
			if (memcmp("data", szChunk, 4) == 0)
			{
				bLoop = false;
			}
			else
			{
				//	チャンク違い
				ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
				SetFilePointer(hFile, dwDataSize, 0, FILE_CURRENT);
			}
		}
		//	データサイズ読み込み
		ReadFile(hFile, &dwDataSize, 4, &dwResult, NULL);
		//	WAVヘッダサイズを保存しておく（予期しないチャンクデータ対策）
		this->sinfo.dwHeaderSize = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
		//	WAVEデータを読み込む
		if (dwSize == 0)
		{
			dwSize = dwDataSize;
		}
		(*data_ptr) = new BYTE[dwSize];
		//	SetFilePointer(hFile, Sound::sinfo[iNumber].dwHeaderSize+dwOffset, NULL, FILE_BEGIN);
		ReadFile(hFile, *data_ptr, dwSize, &dwResult, NULL);
		//	指定したサイズと読み込んだサイズが違うとき
		if (dwResult != dwSize)
		{
			hRet = Err::READERROR;
			goto end;
		}
		//	ファイルのカレントポジションを取得しておく
		this->sinfo.dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	end:
		verCloseHandle(hFile);
		return hRet;
	}
	//--------------------------------------------------------------------------------------------
	void Sound::SetLoopInfo(int iReStartPos, int iLoopCnt)
	{
		this->sinfo.dwReStart = iReStartPos;
		this->sinfo.dwCnt = iLoopCnt;
	}
	//--------------------------------------------------------------------------------------------
	void Sound::FadeOut(int iValue)
	{
		this->sinfo.effectflag = Effect::FADEOUT;
		this->sinfo.iFade = iValue;
	}
	//--------------------------------------------------------------------------------------------
	int Sound::GetVolume( )
	{
		long iVolume = 0;

		this->sinfo.sec_ptr->GetVolume(&iVolume);
		return iVolume;
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::SetPan(int iPan)
	{
		return this->sinfo.sec_ptr->SetPan(iPan);
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::SetFrequency(DWORD dwFreq)
	{
		return this->sinfo.sec_ptr->SetFrequency(dwFreq);
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::SetVolume(long int iVolume)
	{
		return this->sinfo.sec_ptr->SetVolume(iVolume);
	}
	//--------------------------------------------------------------------------------------------
	void Sound::WriteMute(int iSize)
	{
		LPVOID lpSoundData1 = NULL, lpSoundData2 = NULL;
		DWORD size1, size2;

		this->sinfo.sec_ptr->Lock(0, iSize, &lpSoundData1, &size1,
			&lpSoundData2, &size2, DSBLOCK_ENTIREBUFFER);
		// サウンドデータ読み込み
		memset(lpSoundData1, 0, iSize);
		// ロック解除
		this->sinfo.sec_ptr->Unlock(lpSoundData1, size1, lpSoundData2, size2);
	}
	//--------------------------------------------------------------------------------------------
	HRESULT Sound::Stop( )
	{
		auto  ds = Obj::GetInst();
		if (!this->sinfo.sec_ptr)
		{
			return HRESULT(0);
		}
		//	Sound::sinfo[iNumber].bStream = false;
		WriteMute(ds->dwBufferSize);
		this->sinfo.dwReadPosition = this->sinfo.dwHeaderSize;			//	読みこんでいるファイルポジション
		this->sinfo.iStreamPos = 0;
		return this->sinfo.sec_ptr->Stop();
	}
	//--------------------------------------------------------------------------------------------
	void Sound::WriteSoundData(BYTE* data_ptr, int iOffset, int iSize)
	{
		LPVOID lpSoundData1 = NULL, lpSoundData2 = NULL;
		DWORD size1, size2;

		this->sinfo.sec_ptr->Lock(iOffset, iSize, &lpSoundData1, &size1,
			&lpSoundData2, &size2, DSBLOCK_ENTIREBUFFER);
		// サウンドデータ読み込み
		memcpy(lpSoundData1, data_ptr, iSize);
		// ロック解除
		this->sinfo.sec_ptr->Unlock(lpSoundData1, size1, lpSoundData2, size2);
	}
	//--------------------------------------------------------------------------------------------
	Err::Type Sound::WriteWaveStream(const char* szFileName,
		BYTE* data_ptr, int iOffset, DWORD dwSize, int iWriteOffset)
	{
		HANDLE hFile = NULL;
		DWORD dwResult;
		DWORD dwRead;	//	実際にファイルを読み込んだバイト数
		Err::Type hRet = Err::NOTHING;

		//	ファイルを開く
		hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hRet = Err::INVOPEN;
			goto end;
		}
		//	データを読み込む
		SetFilePointer(hFile, iOffset, NULL, FILE_BEGIN);
		ReadFile(hFile, data_ptr, dwSize, &dwResult, NULL);
		//	指定したサイズと読み込んだサイズが違うとき
		if (dwResult != dwSize)
		{
			//	EOFだったと仮定し、処理を行う
			//	読み込み先のポイントをデータの先頭に設定する
			dwRead = SetFilePointer(hFile, 0, NULL, FILE_CURRENT) - this->sinfo.dwReadPosition;
			if (!this->sinfo.bLoop)
			{
				ZeroMemory(&data_ptr[dwRead], dwSize - dwRead);
				this->sinfo.effectflag = Effect::STOP;
				goto end;
			}
			//	読み込みポジションを設定する
			if (this->sinfo.dwReStart != 0)	//	リスタート地点が設定されているならば
			{
				this->sinfo.dwReadPosition = this->sinfo.dwReStart + this->sinfo.dwHeaderSize;
			}
			else
			{
				this->sinfo.dwReadPosition = this->sinfo.dwHeaderSize;
			}
			//	バッファにデータを書きこむ
			verCloseHandle(hFile);
			this->WriteSoundData(data_ptr, iWriteOffset, dwRead);
			this->WriteWaveStream(szFileName, &data_ptr[dwRead],
				this->sinfo.dwReadPosition, dwSize - dwRead, iWriteOffset + dwRead);
		}
		else
		{
			//	ファイルのカレントポジションを取得しておく
			this->sinfo.dwReadPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
			this->WriteSoundData(data_ptr, iWriteOffset, dwSize);
		}
	end:
		verCloseHandle(hFile);
		return hRet;
	}
	//--------------------------------------------------------------------------------------------
	void Sound::LoadStream(int iWriteOffset, int iStreamPos)
	{
		BYTE* data_ptr = NULL;	//	サウンドデータ

		auto  dm = Obj::GetInst();
		data_ptr = new BYTE[dm->dwBufferSize];
		WriteWaveStream(this->sinfo.szFileName, data_ptr,
			this->sinfo.dwReadPosition, dm->dwBufferSize, iWriteOffset);
		this->sinfo.iStreamPos = iStreamPos;
		verDelete_Vector(data_ptr);
	}
	//--------------------------------------------------------------------------------------------
	void Sound::Check_Stream(DWORD  dwBufferSize_)
	{
		DWORD dwStatus;	//	再生フラグを格納
		DWORD dwPos;	//	再生ポジションを格納
		//	初期化が終わっていない場合
		if (!this->sinfo.sec_ptr)
		{
			return;
		}
		//	ストリーミング再生が許可されていない
		if (!this->sinfo.bStream)
		{
			return;
		}
		//	バッファは再生中かどうかを調べる
		this->sinfo.sec_ptr->GetStatus(&dwStatus);
		if (!(dwStatus & DSBSTATUS_PLAYING))
		{
			return;
		}
		//	読み込みタイミングかどうかをチェック
		this->sinfo.sec_ptr->GetCurrentPosition(&dwPos, NULL);
		//	再生ポジションフラグが0で、かつ0-11025なら22050-44100のデータを更新する。
		if ((this->sinfo.iStreamPos == 0) && (dwPos > 0) && (dwPos < (dwBufferSize_ / 2)))
		{
			this->LoadStream(dwBufferSize_, 1);
		}
		//	再生ポジションフラグが1で、かつ22050-33075なら0-22050のデータを更新。
		if ((this->sinfo.iStreamPos == 1) && (dwPos > dwBufferSize_))
		{
			this->LoadStream(0, 0);
		}
		//	エフェクトを管理
		if (this->sinfo.effectflag == Effect::FADEOUT)
		{
			this->sinfo.iVolume -= this->sinfo.iFade;//kVALUE_FADE;
			if (this->sinfo.iVolume < kVOLUME_MIN)
			{
				this->Stop();
				this->sinfo.effectflag = Effect::NOTHING;
			}
			this->SetVolume(this->sinfo.iVolume);
		}
		if (this->sinfo.effectflag == Effect::FADEIN)
		{
			this->sinfo.iVolume += this->sinfo.iFade;//kVALUE_FADE;
			if (this->sinfo.iVolume > DSBVOLUME_MAX)
			{
				this->sinfo.iVolume = DSBVOLUME_MAX;
				this->sinfo.effectflag = Effect::NOTHING;
			}
			this->SetVolume(this->sinfo.iVolume);
		}
		if (this->sinfo.effectflag == Effect::STOP)
		{
			this->Stop( );
			this->sinfo.effectflag = Effect::NOTHING;
		}
	}



















































	











	//--------------------------------------------------------------------------------------------






















	//--------------------------------------------------------------------------------------------
	//	関数名		：	GetErrorMessage
	//	作成者		：	植山沙欧
	//	作成日		：	2002.06.24
	//	機能		：	エラーメッセージを取得します
	//	機能効果	：	「number」に対応する文字列を「szMsg」にコピーします
	//					「iLength」にはコピー先のバッファサイズを指定してください
	//	引数		：	＜入力＞number		参照するエラーナンバー
	//					＜入力＞szMsg		メッセージの格納先
	//					＜入力＞iLength		格納先の長さ
	//	使用変数	：	char szWork		文字列の位置時格納先
	//	使用関数	：	strcpy_s	文字列コピー
	//					memcpy			メモリコピー
	//	戻り値		：	ありません
	//	更新履歴	：	2002.06.24	Ver1.00		植山沙欧	機能の実現
	//					2007.02.09	Ver1.01		植山沙欧	コピー方法の変更
	//--------------------------------------------------------------------------------------------

	std::string GetErrorMessage(Err::Type number)
	{
		std::string szMsg;

		switch (number)
		{
		case Err::CCIM: szMsg = "DirectMusicの作成に失敗"; break;
		case Err::CCIS: szMsg = "DirectSoundの作成に失敗"; break;
		case Err::INIT: szMsg = "Initializeに失敗"; break;
		case Err::SCL: szMsg = "SetCooperativeLevelに失敗"; break;
		case Err::CSB: szMsg = "バッファ作成に失敗"; break;
		case Err::QI: szMsg = "QueryInterfaceに失敗"; break;
		case Err::SFMT: szMsg = "フォーマット設定に失敗"; break;
		case Err::IA: szMsg = "InitAudioに失敗"; break;
		case Err::CCILOADER: szMsg = "ローダー作成に失敗"; break;
		case Err::MBTWC: szMsg = "unicode変換に失敗"; break;
		case Err::SSD: szMsg = "ディレクトリ設定に失敗"; break;
		case Err::PSE: szMsg = "セグメントデータを再生できない"; break;
		case Err::INVALID: szMsg = "ファイルが読み込めない"; break;
		case Err::DL: szMsg = "ダウンロードできない"; break;
		case Err::UNLD: szMsg = "アンロードできない"; break;
		case Err::INVOPEN: szMsg = "ファイルが開けない"; break;
		case Err::RIFF: szMsg = "RIFFチャンクが読み込めない"; break;
		case Err::FMT: szMsg = "fmtチャンクが読み込めない"; break;
		case Err::NOTPCM: szMsg = "読み込み対象のWAVEはサポートされていない形式"; break;
		case Err::DATA: szMsg = "dataチャンクは存在しない"; break;
		case Err::READERROR: szMsg = "読み込み失敗"; break;
		case Err::NOTHING: szMsg = "エラーはありません"; break;
		default: szMsg = "定義されていないエラー"; break;
		}
		return szMsg;
	}
}



namespace DM
{
	DM_::Obj::SP	obj;
	map<string, DM_::Sound::SP>  sounds;
	bool sounds_NotObject = false;

	//初期化
	bool  Initialize(HWND wnd_, DWORD dwBufferSize)
	{
		if (!obj)
		{
			obj = DM_::Obj::Create(wnd_, dwBufferSize);
		}
		return  obj != nullptr;
	}
	//解放
	void  Finalize()
	{
		Sound_Clear();
		obj.reset();
	}

	void  Check_Stream(void)
	{
		obj->Check_Stream();
	}

	//読み込み＆入れ替え
	void  Sound_CreateStream(const string name_,
		const string fileName_)
	{
		auto  w = DM_::Sound::CreateStream(fileName_.c_str( ));
		sounds[name_] = w;
	}
	void  Sound_CreateSE(const string name_,
		const string fileName_)
	{
		auto  w = DM_::Sound::CreateSE(fileName_.c_str());
		sounds[name_] = w;
	}
	//表示
	void  Image_Draw(const string name_,
		ML::Box2D  draw_,
		ML::Box2D  src_,
		DWORD  color_ = 0xFFFFFFFF);
	//再生
	void  Sound_Play(const string name_, bool loop_)
	{
		auto it = sounds.find(name_);
		if (it == sounds.end()) {
			if (sounds_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("存在しないサウンドバッファに制御を要求しました"), MB_OK);
			}
			sounds_NotObject = true;
			return;
		}
		else { it->second->Play_Normal(loop_);	}
	}
	//ボリューム変更
	void  Sound_Volume(const string name_, long vol_)
	{
		auto it = sounds.find(name_);
		if (it == sounds.end()) {
			if (sounds_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("存在しないサウンドバッファに制御を要求しました"), MB_OK);
			}
			sounds_NotObject = true;
			return;
		}
		else { 
			vol_ = max(0, vol_);
			vol_ = min(1000, vol_);
			long v = -10000 + (vol_ * 10);
			it->second->SetVolume(v); 
		}
	}

	//停止
	void  Sound_Stop(const string name_)
	{
		auto it = sounds.find(name_);
		if (it == sounds.end()) {
			if (sounds_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("存在しないサウンドバッファに制御を要求しました"), MB_OK);
			}
			sounds_NotObject = true;
			return;
		}
		else { it->second->Stop(); }
	}

	//全音声を削除する
	void  Sound_Clear()
	{
		sounds.clear();
	}
	//指定した音声を削除する
	void  Sound_Erase(const string name_)
	{
		sounds.erase(name_);
	}
	//
	void  Sound_FadeOut(const string name_)
	{
		auto it = sounds.find(name_);
		if (it == sounds.end())
		{
			if (sounds_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("存在しないサウンドバッファに制御を要求しました"), MB_OK);
			}
			sounds_NotObject = true;
			return;
		}
		else
		{
			it->second->FadeOut(30);
		}
	}
}