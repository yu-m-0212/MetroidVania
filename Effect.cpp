//-----------------------------------------------------------------------------
//エフェクトクラス
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Effect.h"

//泡の動き（泡オブジェクトの座標に加算して使用する）
//引数	：	（カウンタ,揺れの周期,揺れ幅,浮上速度）
ML::Vec2 Effect::Move_Bubble(int cnt_, const int& interval_, const float& wide_, const float& speedY_)
{
	float x = float(sin(cnt_ / interval_) * wide_);
	float y = -speedY_;
	return ML::Vec2(x, y);
}