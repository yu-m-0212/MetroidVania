#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//�{�X�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

//�{�X�^�X�N���ŋ��L����l
#define MAX_INTERVAL_SHOT 300				//�V���b�g�������Ԃ̏���l
#define LIMIT_SHOT 300						//���Ŏ��ԃV���b�g
#define SPEED_SHOT 5.0f						//���x�V���b�g
#define POWER_SHOT 1						//�U���̓V���b�g
#define HP_BOSS_PARTS 1//0					//HP�{�X�p�[�c
#define HP_BOSS_HEAD 50						//�{�X�w�b�h��HP
#define LIMIT_HIT_REFLECT 60				//���˔�e��Ԃ̎��Ԑ���
#define INTERVAL_CREATE_END_EFFECT_DEF 10	//���ȊO�̃G���h�p�^�[���G�t�F�N�g�����Ԋu
#define INTERVAL_CREATE_END_EFFECT_HEAD 5	//���^�X�N�̃G���h�p�^�[���G�t�F�N�g�����Ԋu
#define LIMIT_END_PATTERN_BOSS_HEAD 300		//�{�X�G���h�p�^�[������̑J�ڎ���
#define LIMIT_END_PATTERN_BOSS_PARTS 120	//�e�p�[�c�̃G���h�p�^�[���i���j�j����
#define DECREASE_INTERVAL_DEFEAT 30			//�e�p�[�c��j�󂷂邲�Ƃɂ��̒l���A���̍s���������Ȃ�
#define DEFEAT_PARTS 4						//�������C�₳����̂ɔj�󂷂铷�̂̐�

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