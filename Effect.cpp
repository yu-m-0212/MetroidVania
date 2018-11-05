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