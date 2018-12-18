#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//ボスクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

//ボスタスク内で共有する値
#define MAX_INTERVAL_SHOT 600	//ショット生成時間の上限値
#define LIMIT_SHOT 300			//消滅時間ショット
#define SPEED_SHOT 5.0f			//速度ショット
#define POWER_SHOT 1			//攻撃力ショット
#define HP_BOSS 3				//HPボス

class Boss : public BTask
{
public:
	//目標の座標を追い続ける（1軸）
	//引数	：	（自座標,目標座標,速度）
	float Chase_Target(const float&, const float&, const float&);
	//純粋仮想関数のオーバーライド
	void UpDate()
	{
		ML::MsgBox("実装されていません");
	}
};