#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//�G�t�F�N�g�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class Effect : public BTask
{
public:
	//�C�A�̓����i�A�I�u�W�F�N�g�̍��W�ɉ��Z���Ďg�p����j
	//����	�F	�i�J�E���^,�h��̎���,�h�ꕝ,���㑬�x�j
	ML::Vec2 Move_Bubble(int,const int&,const float&,const float&);
	//
	void UpDate()
	{
		ML::MsgBox("��������Ă��܂���");
	}
};