#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//�{�X�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

//�{�X�^�X�N���ŋ��L����l
#define MAX_INTERVAL_SHOT 600	//�V���b�g�������Ԃ̏���l
#define LIMIT_SHOT 300			//���Ŏ��ԃV���b�g
#define SPEED_SHOT 5.0f			//���x�V���b�g
#define POWER_SHOT 1			//�U���̓V���b�g
#define HP_BOSS 3				//HP�{�X

class Boss : public BTask
{
public:
	//�ڕW�̍��W��ǂ�������i1���j
	//����	�F	�i�����W,�ڕW���W,���x�j
	float Chase_Target(const float&, const float&, const float&);
	//�������z�֐��̃I�[�o�[���C�h
	void UpDate()
	{
		ML::MsgBox("��������Ă��܂���");
	}
};