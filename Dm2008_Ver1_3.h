
#pragma comment(lib,"dxguid")
#pragma once

#include <windows.h>
#include <string>
#include <dsound.h>
#include "myLib.h"




namespace DM 
{
	const  DWORD  kSTANDERDSIZE = 88200;	//	基準サイズ（バッファサイズの半分に等しい）
											//初期化
	bool  Initialize(HWND wnd_, DWORD dwBufferSize= kSTANDERDSIZE);
							//解放
	void  Finalize();

	void  Check_Stream(void);

	//読み込み＆入れ替え
	void  Sound_CreateSE(const string name_,
		const string fileName_);
	void  Sound_CreateStream(const string name_,
		const string fileName_);

	//再生
	void  Sound_Play(const string name_, bool loop_);
	//停止
	void  Sound_Stop(const string name_);
	//ボリューム変更(0～1000で設定）
	void  Sound_Volume(const string name_, long vol_);
	//全音声を削除する
	void  Sound_Clear();
	//指定した音声を削除する
	void  Sound_Erase(const string name_);
	//指定した音声をフェードアウトさせる
	void  Sound_FadeOut(const string name_);
}