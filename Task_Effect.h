#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include "BChara.h"
#include "Effect.h"

using namespace ML;

namespace  Task_Effect
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エフェクト");	//グループ名
	const  string  defName("NoName");			//タスク名
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
		string effectImage;
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
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();//「2D描画」１フレーム毎に行う処理
		bool  Finalize();	//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//状態管理
		enum State_Effect
		{
			Non,			//未指定
			Hit_Shot,		//ヒットショット（プレイヤ）
			Barrier,		//バリア
			Heal,			//回復
			Bubble,			//泡
			Appear,			//登場
			Defeat_Enemy,	//撃破（エネミー）
			Debris			//破片
		};
		State_Effect state_effect;	//状態管理
		int num_bubble;				//泡の大きさを指定する
		int interval_bubble;		//泡の揺れ周期
		int limit_erase;			//消滅までの時間
		int limit_erase_hit_shot;	//時間消滅までヒットショット
		int limit_erase_barrier;	//時間消滅までバリア
		int limit_erase_defeat;		//時間消滅まで撃破
		int limit_erase_heal;		//時間消滅まで回復
		int limit_erase_bubble;		//時間消滅まで泡
		int limit_erase_appear;		//時間消滅まで登場
		int limit_erase_debris;		//消滅までの時間破片
		int choice_state;			//外部から状態を指定する際、使用
		float speed_surfacing;		//泡の浮上速度
		float speed_Debris;			//速度破片
		float wide_bubble;			//泡の揺れ幅
		float dist;					//撃破エフェクトの中心からの距離
		float angle;				//表示する角度
		float gravity;				//重力加速度
		Vec2 center;				//回転軸
		Effect* eff;				//エフェクトクラス
	public:
		Object();
		//エフェクトを生成する（角度を指定しない）
		//引数	：	（状態,座標）
		static void Create_Effect(const int&, const ML::Vec2&);
		//エフェクトを生成する（角度を指定する）
		//引数	：	（状態,座標,角度,向き）
		void Create_Effect(const int&, const ML::Vec2&, const float&, const BChara::Angle_LR&);
		//状態ごとに行動を指定する
		void Move();
		//アニメーション
		BChara::DrawInfo Anim();
	};
}