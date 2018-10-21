#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");		//グループ名
	const  string  defName("仮");				//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string		imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		string controllerName;
		float decSpeed;				//速度減衰量
		float max_StompFallSpeed;	//ストンプ時の最大効果速度
		float speed_Shot;			//ショット速度
		float init_shot;			//生成位置ショット
		float height_Jump;			//ジャンプ初速
		int addUnHitTime;			//被弾時に得られる無敵時間
		int interval_Shot;			//射撃の発射間隔（フレーム）
		int limit_JumpAngleChange;	//ジャンプから一定時間内なら向きを変えられる
		int limit_Stomp;			//継続時間ストンプ
		int limit_StompEffect;		//継続時間ストンプ効果
		int limit_StompHoldTime;	//硬直時間ストンプ
		int limit_Quake;			//画面揺れ時間
		int limit_Shot;				//継続時間ショット
		int limit_HealEffect;		//継続時間回復効果
		int lv_Stomp;				//ストンプアップグレードレベル
		int dist_Quake;				//画面揺れ幅
		int power_Stomp;			//攻撃力ストンプ
		int power_Shot;				//攻撃力ショット
		int gauge_melee;			//近接攻撃のリチャージ
		int gauge_melee_max;		//近接攻撃リチャージ上限
		ML::Box2D range_Stomp;		//範囲ストンプ
		ML::Box2D range_Shot;		//範囲ショット
		ML::Vec2 moveBack_Stomp;	//ふっとび量ストンプ
	public:
		//思考＆状況判断(ステータス決定）
		void  Think();
		//モーションに対応した処理
		void  Move();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	(攻撃側,攻撃情報)
		void Received(BChara* from_, AttackInfo at_);
		//アニメーション制御
		BChara::DrawInfo  Anim();
		//ショット生成スタンダード
		void Shot_Appear();
		//ストンプスタンダード
		void Stomp_Std();
		//行動ショット中
		void Move_Shot();

		//アクセサメソッド
		//HPの値を取得する
		int Get_HP();
		//HPの最大値を取得する
		int Get_Max_HP();
		//状態を取得する
		int Get_State();
		//ショットの発射間隔を取得する
		int Get_Interval_Shot();
		//近接攻撃リチャージの値を取得する
		int Get_Gauge_Mlee();
		//近接攻撃リチャージ上限の値を取得する
		int Get_Gauge_Melle_Max();
	};
}