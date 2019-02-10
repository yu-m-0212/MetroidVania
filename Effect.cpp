//-----------------------------------------------------------------------------
//エフェクトクラス
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Effect.h"

//気泡の動き（泡オブジェクトの座標に加算して使用する）
//引数	：	（カウンタ,周期,揺れ幅,浮上速度）
ML::Vec2 Effect::Move_Bubble(const int& cnt_, const int& interval_, const float& wide_, const float& speed_)
{
	float x = float(sin(cnt_ / interval_) * wide_);
	float y = -speed_;
	return ML::Vec2(x, y);
}
//放物線運動
//引数	：	（速度,Y軸移動量,重力,角度)
ML::Vec2 Effect::Move_Parabola(const float& speed_, const float& moveY_, const float& gravity_, const float& angle_)
{
	//X軸は与えられた角度に速度をかける
	//Y軸は与えられた初速に重力加速度を加算していく
	return ML::Vec2(cos(angle_)*speed_, moveY_ + gravity_);
}
//ランダムに2軸の反復する移動量（振動）を返す
//引数	：	（カウンタ,X軸揺れ最大値,Y軸揺れ最大値,揺れ間隔）
ML::Vec2 Effect::Quake(const int& cnt_, const int& dist_x_, const int& dist_y_, const int& interval_)
{
	//x方向
	int x = 0;
	//div_zero
	if (dist_x_ != 0)
	{
		x = rand() % dist_x_;
	}
	float quake_x =
		sinf(cnt_ / interval_)*x;

	//y方向
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