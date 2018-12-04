//-----------------------------------------------------------------------------
//ボスクラス
//-----------------------------------------------------------------------------
#include "MyPG.h"
#include "Boss.h"

//目標の座標を追い続ける（1軸）
//引数	：	（自座標,目標座標,速度）
float Boss::Chase_Target(const float& my_pos_, const float& target_pos_, const float& speed_)
{
	float target = target_pos_ - my_pos_;
	return target * speed_;
}