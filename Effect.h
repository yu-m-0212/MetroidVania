#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//エフェクトクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class Effect : public BTask
{
public:
	//気泡の動き（泡オブジェクトの座標に加算して使用する）
	//引数	：	（カウンタ,周期,揺れ幅,浮上速度）
	ML::Vec2 Move_Bubble(const int&, const int&, const float&, const float&);
	//放物線運動
	//引数	：	（速度,Y軸移動量,重力,角度)
	ML::Vec2 Move_Parabola(const float&, const float&, const float&, const float&);
	//純粋仮想関数のオーバーライド
	void UpDate()
	{
		ML::MsgBox("実装されていません");
	}
};