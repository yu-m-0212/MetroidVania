//-----------------------------------------------------------------------------
//�G�t�F�N�g�N���X
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Effect.h"

//�A�̓����i�A�I�u�W�F�N�g�̍��W�ɉ��Z���Ďg�p����j
//����	�F	�i�J�E���^,�h��̎���,�h�ꕝ,���㑬�x�j
ML::Vec2 Effect::Move_Bubble(int cnt_, const int& interval_, const float& wide_, const float& speedY_)
{
	float x = float(sin(cnt_ / interval_) * wide_);
	float y = -speedY_;
	return ML::Vec2(x, y);
}