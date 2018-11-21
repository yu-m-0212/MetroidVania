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
	return ML::Vec2(cos(angle_)*speed_, moveY_+gravity_);
}