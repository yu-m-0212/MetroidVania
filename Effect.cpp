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
//�������^��
//����	�F	�i���x,Y���ړ���,�d��,�p�x)
ML::Vec2 Effect::Move_Parabola(const float& speed_, const float& moveY_, const float& gravity_, const float& angle_)
{
	//X���͗^����ꂽ�p�x�ɑ��x��������
	//Y���͗^����ꂽ�����ɏd�͉����x�����Z���Ă���
	return ML::Vec2(cos(angle_)*speed_, moveY_ + gravity_);
}
//�����_����2���̔�������ړ��ʁi�U���j��Ԃ�
//����	�F	�i�J�E���^,X���h��ő�l,Y���h��ő�l,�h��Ԋu�j
ML::Vec2 Effect::Quake(const int& cnt_, const int& dist_x_, const int& dist_y_, const int& interval_)
{
	//x����
	int x = 0;
	//div_zero
	if (dist_x_ != 0)
	{
		x = rand() % dist_x_;
	}
	float quake_x =
		sinf(cnt_ / interval_)*x;

	//y����
	int y = 0;
	//div_zero
	if (dist_y_ != 0)
	{
		y = rand() % dist_y_;
	}
	float quake_y =
		sinf(cnt_ / interval_)*y;

	return ML::Vec2(quake_x, quake_y);
}