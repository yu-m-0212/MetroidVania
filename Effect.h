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
	//����	�F	�i�J�E���^,����,�h�ꕝ,���㑬�x�j
	ML::Vec2 Move_Bubble(const int&, const int&, const float&, const float&);
	//�������^��
	//����	�F	�i���x,Y���ړ���,�d��,�p�x)
	ML::Vec2 Move_Parabola(const float&, const float&, const float&, const float&);
	//�����_����2���̔�������ړ��ʁi�U���j��Ԃ�
	//����	�F	�i�J�E���^,X���h��ő�l,Y���h��ő�l,�h��Ԋu�j
	ML::Vec2 Quake(const int&, const int&, const int&, const int&);
	//�������z�֐��̃I�[�o�[���C�h
	void UpDate()
	{
		ML::MsgBox("��������Ă��܂���");
	}
};