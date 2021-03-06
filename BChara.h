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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

	//キャラクタの行動状態フラグ
	enum State
	{
		Unnon = -1,			//	無効(使えません）
		Stand,				//	停止
		Dash,				//	ダッシュ
		SlowDown,			//	減速
		Air,				//	空中攻撃
		PreStomp,			//	ストンプの予備動作
		LandStomp,			//	ストンプ
		AirStomp,			//	空中からの降下
		StompLanding,		//	ストンプ着地
		Shoot,				//	射撃(地上)
		Jumpshoot,			//	射撃(空中上昇)
		Fallshoot,			//	射撃(空中降下)
		Jump, Jump2,		//	ジャンプ
		Fall, Fall2,		//	落下
		Landing,			//	着地
		Damage,				//	被弾
		Walk,				//	歩行
		Turn,				//	旋回
		TakeOff,			//	飛び立つ瞬間
		Bound,				//	ダメージを受けて吹き飛んでいる
		End_Pattern_Boss,	//	ボスエンドパターン
		Lose,				//	消える・昇天
		Caution,			//	エネミー警戒時
		TargetLost,			//	対象を見失ったとき
		//以下ボス用状態
		//ボスタスクかクラスに入れたい
		Hiding_Side,						//潜伏横（ボス用）
		Hiding_Under,						//潜伏下（ボス用）
		Appear_Side,						//出現横（ボス用）
		Appear_Under,						//出現下（ボス用）
		Wait_Side,							//待機横（ボス用）
		Wait_Under,							//待機下（ボス用）
		Shot_Side_,							//攻撃横（ボス用）
		Shot_Under,							//攻撃下（ボス用）
		Return_Side,						//戻る横（ボス用）
		Return_Under,						//戻る下（ボス用）
		Choice_Vertically_Or_Horizontally,	//縦横の選択
		Stan								//気絶中（ボス頭用）
	};
	//左右の向き（2D横視点ゲーム専用）
	enum Angle_LR { Left, Right };
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
	//キャラクタ共通メンバ変数
	//徐々に非公開にすること
	//BCharaメソッドに使用するものはここに持たせること
	Angle_LR	angle_LR;
	State		state;				//現在の行動を示すフラグ
	ML::Vec2    pos;				//キャラクタ位置
	ML::Vec2	moveVec;			//移動ベクトル
	ML::Box2D   hitBase;			//マップとの判定範囲(キャラクタとの接触は別に行う)
	ML::Box2D	recieveBase;		//キャラクタとの判定範囲(マップ用から変更がない場合はhitbaseで初期化する)
	int			moveCnt;			//行動カウンタ
	int			time_un_hit;		//無敵時間カウンタ
	int			animCnt;			//アニメーションカウンタ
	int			hp;					//ヘルスポイント
	int			limit_hit_reflect;	//反射被弾状態のリミット
	float		gravity;			//フレーム単位の加算量
	float		maxSpeed;			//左右方向への移動の最大速
	float		addSpeed;			//左右方向への移動の加算量
private:
	ML::Vec2	move_back;			//攻撃のふっとび量
	bool		tip;				//ノックバックの発生しない攻撃の場合、弾生成時にtrue
	bool		range_Wide;			//攻撃が広範囲か否か(ふっとび方向を決める際に使用)
	bool		flag_erase;			//遺体に触れたとき、true
	float		angle;				//角度
public:
	//メソッド
	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: state(Stand)
		, angle_LR(Right)
		, pos(0, 0)
		, moveVec(0, 0)
		, move_back(0, 0)
		, hitBase(0, 0, 0, 0)
		, recieveBase(0, 0, 0, 0)
		, moveCnt(0)
		, time_un_hit(0)
		, animCnt(0)
		, hp(1)
		, gravity(0)
		, maxSpeed(0)
		, addSpeed(0)
		, tip(false)
		, range_Wide(false)
		, flag_erase(false)
		, angle(0)
		, limit_hit_reflect(0)
	{
	}
	virtual  ~BChara(){}
	//親クラス仮想関数のオーバーライド
	void UpDate()
	{
		ML::MsgBox("実装されていません");
	}
	//キャラクタ共通メソッド
	//めり込まない移動処理
	virtual  void  CheckMove(ML::Vec2&  est_);
	//足元接触判定
	virtual  bool  CheckFoot();
	//頭上接触判定
	virtual  bool  CheckHead();
	//正面接触判定（サイドビューゲーム専用）
	virtual bool CheckFront_LR();
	//背面接触判定（サイドビューゲーム専用）
	virtual bool CheckBack_LR();
	//正面足元チェック（サイドビューゲーム専用）
	virtual bool CheckFrontFoot_LR();
	//接触判定
	virtual bool CheckHit(const ML::Box2D& hit);
	//自身から見た対象の位置を左右で返す
	//引数	：	（現在の向き,比較対象のX座標）
	BChara::Angle_LR Check_LR(const BChara::Angle_LR&,const float&);
	//モーションを更新（変更なしの場合	false)
	bool  UpdateMotion(State  nm_);
	//接触時の応答処理（これ自体はダミーのようなモノ）
	virtual	void Recieved(BChara* from_, AttackInfo at_,const int&)
	{
		ML::MsgBox("Recieved 実装されていません");
	}

	//アクセサメソッド

	//状態を指定する
	//引数	：	（State型）
	void Set_State(const State&);
	//範囲攻撃か否かを指定する
	//引数	：	（0か1）
	void Set_Range_Wide(const int&);
	//ノックバックするか否かを指定する
	//引数	：	（0か1）
	void Set_Tip(const int&);
	//遺体の消滅フラグを指定する
	//引数	：	（0か1）
	void Set_Flag_Erase(const int&);
	//ふっとび量を指定する
	//引数	：	（ML::Vec2）
	void Set_Move_Back(const ML::Vec2&);
	//範囲攻撃か否かを取得する
	bool Get_Range_Wide();
	//ノックバックするか否かを取得する
	bool Get_Tip();
	//遺体の消滅フラグを取得する
	bool Get_Flag_Erase();
	//角度を取得する
	float Get_Angle();
	//ふっとび量を取得する
	ML::Vec2 Get_Move_Back();
};