//-----------------------------------------------------------------------------
//�G�t�F�N�g�N���X
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Effect.h"

//�C�A�̓����i�A�I�u�W�F�N�g�̍��W�ɉ��Z���Ďg�p����j
//����	�F	�i�J�E���^,����,�h�ꕝ,���㑬�x�j
ML::Vec2 Effect::Move_Bubble(const int& cnt_, const int& interval_, const float& wide_, const float& speed_)
{
	float x = float(sin(cnt_ / interval_) * wide_);
	float y = -speed_;
	return ML::Vec2(x, y);
}