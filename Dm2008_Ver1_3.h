
#pragma comment(lib,"dxguid")
#pragma once

#include <windows.h>
#include <string>
#include <dsound.h>
#include "myLib.h"




namespace DM 
{
	const  DWORD  kSTANDERDSIZE = 88200;	//	��T�C�Y�i�o�b�t�@�T�C�Y�̔����ɓ������j
											//������
	bool  Initialize(HWND wnd_, DWORD dwBufferSize= kSTANDERDSIZE);
							//���
	void  Finalize();

	void  Check_Stream(void);

	//�ǂݍ��݁�����ւ�
	void  Sound_CreateSE(const string name_,
		const string fileName_);
	void  Sound_CreateStream(const string name_,
		const string fileName_);

	//�Đ�
	void  Sound_Play(const string name_, bool loop_);
	//��~
	void  Sound_Stop(const string name_);
	//�{�����[���ύX(0�`1000�Őݒ�j
	void  Sound_Volume(const string name_, long vol_);
	//�S�������폜����
	void  Sound_Clear();
	//�w�肵���������폜����
	void  Sound_Erase(const string name_);
	//�w�肵���������t�F�[�h�A�E�g������
	void  Sound_FadeOut(const string name_);
}