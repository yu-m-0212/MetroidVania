#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタの行動状態フラグ
	enum State
	{
		Unnon = -1,		//	無効(使えません）
		Stand,			//	停止
		Dash,			//	ダッシュ
		SlowDown,		//	減速
		Punch1,			//	パンチ1
		Punch2,			//	パンチ2
		Air,			//	空中攻撃
		Stomp,			//	ストンプ
		StompLanding,	//	ストンプ着地
		Shoot,			//	射撃(地上)
		Airshoot,		//	射撃(空中)
		Bunker1,		//	バンカー構え
		Bunker2,		//	バンカー射出
		Bunker3,		//	バンカー腕戻し
		Jump,Jump2,		//	ジャンプ
		Fall,Fall2,		//	落下
		Landing,		//	着地
		Damage,			//	被弾
		Walk,			//	歩行
		Turn,			//	旋回
		TakeOff,		//	飛び立つ瞬間
		Bound,			//	ダメージを受けて吹き飛んでいる
		Lose,			//	消える・昇天
	};
	//左右の向き（2D横視点ゲーム専用）
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	WP			target;
	//キャラクタ共通メンバ変数
	State		state;			//現在の行動を示すフラグ
	ML::Vec2    pos;			//キャラクタ位置
	ML::Vec2	moveVec;		//移動ベクトル
	ML::Box2D   hitBase;		//あたり判定範囲
	int			moveCnt;		//行動カウンタ
	int			unHitTime;		//無敵時間カウンタ
	int			animCnt;		//アニメーションカウンタ
	int			hp;				//ヘルスポイント
	float		jumpPow;		//ジャンプ初速
	float		maxFallSpeed;	//落下最大速度
	float		gravity;		//フレーム単位の加算量
	float		maxSpeed;		//左右方向への移動の加算量
	float		addSpeed;		//左右方向への移動の加算量
	float		decSpeed;		//左右方向への移動の減衰量
	float		reach;			//パンチの射程
	float		slide;			//パンチ時に前進する
	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		:state(Stand)
		, angle_LR(Right)
		, pos(0, 0)
		, moveVec(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveCnt(0)
		, unHitTime(0)
		,animCnt(0)
		, hp(1)
		, jumpPow(0)
		, maxFallSpeed(0)
		, gravity(0)
		, maxSpeed(0)
		, addSpeed(0)
		, decSpeed(0)
		, reach(0)
		, slide(0)
	{
	}
	virtual  ~BChara(){}
	//キャラクタ共通メソッド
	//めり込まない移動処理
	virtual  void  CheckMove(ML::Vec2&  est_);
	//足元接触判定
	virtual  bool  CheckFoot();
	//頭上接触判定
	virtual  bool  CheckHead();
	//正面接触判定（サイドビューゲーム専用）
	virtual bool CheckFront_LR();
	//正面足元チェック（サイドビューゲーム専用）
	virtual bool CheckFrontFoot_LR();
	//接触判定
	virtual bool CheckHit(const ML::Box2D& hit);
	//モーションを更新（変更なしの場合	false)
	bool  UpdateMotion(State  nm_);

	//アニメーション情報構造体
	//draw,src,color
	struct DrawInfo {
		ML::Box2D		draw, src;
		ML::Color		color;
	};
	//攻撃情報
	//(攻撃の威力,命中精度,攻撃の属性)
	struct AttackInfo {
		int power;	//攻撃の威力
		int hit;	//命中精度
		int element;//攻撃の属性
		//その他必要に応じて
	};
	//接触時の応答処理（これ自体はダミーのようなモノ）
	virtual	void Received(BChara* from_, AttackInfo at_)
	{
		ML::MsgBox("Recieved 実装されていません");
	}
};