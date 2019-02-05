#pragma once
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------
//ボスクラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

//ボスタスク内で共有する値
#define MAX_INTERVAL_SHOT 300				//ショット生成時間の上限値
#define LIMIT_SHOT 300						//消滅時間ショット
#define SPEED_SHOT 5.0f						//速度ショット
#define POWER_SHOT 1						//攻撃力ショット
#define HP_BOSS_PARTS 1//0					//HPボスパーツ
#define HP_BOSS_HEAD 50						//ボスヘッドのHP
#define LIMIT_HIT_REFLECT 60				//反射被弾状態の時間制限
#define INTERVAL_CREATE_END_EFFECT_DEF 10	//頭以外のエンドパターンエフェクト生成間隔
#define INTERVAL_CREATE_END_EFFECT_HEAD 5	//頭タスクのエンドパターンエフェクト生成間隔
#define LIMIT_END_PATTERN_BOSS_HEAD 300		//ボスエンドパターンからの遷移時間
#define LIMIT_END_PATTERN_BOSS_PARTS 120	//各パーツのエンドパターン（爆破）時間
#define DECREASE_INTERVAL_DEFEAT 30			//各パーツを破壊するごとにこの値分、頭の行動が速くなる
#define DEFEAT_PARTS 4						//頭部を気絶させるのに破壊する胴体の数

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