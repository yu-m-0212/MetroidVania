//-----------------------------------------------------------------------------
//�{�X�N���X
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Boss.h"

//�ڕW�̍��W��ǂ�������i1���j
//����	�F	�i�����W,�ڕW���W,���x�j
float Boss::Chase_Target(const float& my_pos_, const float& target_pos_, const float& speed_)
{
	float target = target_pos_ - my_pos_;
	return target * speed_;
}