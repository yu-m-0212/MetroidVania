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
		string base_file_path_sound;		//サウンドファイルパスベース
		string name_sound_defeat;			//名前効果音爆発
		string name_sound_hit;				//効果音ヒット
		string name_sound_barrier;			//効果音バリア
		string name_sound_barrier_recharge;	//効果音バリアリチャージ
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
			Non,						//未指定
			Hit_Shot,					//ヒットショット（プレイヤ）
			Barrier_Recharge,			//バリアリチャージ
			Heal,						//回復
			Bubble,						//泡
			Appear,						//登場
			Defeat_Enemy,				//撃破（エネミー）
			Debris,						//破片
			Spark,						//火花
			Target_Circle_00,			//ボス登場演出サークル00
			Target_Circle_01,			//ボス登場演出サークル01
			Target_Bar,					//ボス登場演出数値バー
			Barrier,					//バリア発生時
			Highlight_Head,				//ボススタンハイライト
			Boss_Chance_Mark_In_Line,	//ボス弱点表示内側
			Boss_Chance_Mark_Out_Line,	//ボス弱点表示外側
		};
		State_Effect state_effect;			//状態管理
		int num_bubble;						//泡の大きさを指定する
		int interval_bubble;				//泡の揺れ周期
		int limit_erase;					//消滅までの時間
		int limit_erase_hit_shot;			//時間消滅までヒットショット
		int limit_erase_barrier_recharge;	//時間消滅までバリアリチャージ
		int limit_erase_defeat;				//時間消滅まで撃破
		int limit_erase_heal;				//時間消滅まで回復
		int limit_erase_bubble;				//時間消滅まで泡
		int limit_erase_appear;				//時間消滅まで登場
		int limit_erase_debris;				//消滅までの時間破片
		int limit_erase_spark;				//消滅までの時間火花
		int limit_erase_highlight_head;		//消滅までの時間ハイライト
		int limit_effect_target_boss;		//ボス警告エフェクトの消滅時間
		int limit_effect_barrier;			//消滅までの時間バリア
		int limit_effect_boss_chance_mark;	//消滅までの時間ボス弱点
		int limit_quake_defeat;				//消滅エフェクトの画面揺れ時間
		int limit_quake_barrier;			//バリアの画面揺れ時間
		int choice_state;					//外部から状態を指定する際、使用
		int rate_size_circle;				//ターゲットサークルのサイズアニメーション倍率
		float cnt_anim_highlight;			//ボススタン時のハイライトグラデーションカウンタ
		float speed_surfacing;				//泡の浮上速度
		float speed_Debris;					//速度破片
		float speed_spark;					//速度火花
		float wide_bubble;					//泡の揺れ幅
		float dist;							//撃破エフェクトの中心からの距離
		float angle;						//表示する角度
		float add_angle_target_circle;		//ボス警告サークルの回転量
		float add_angle_barrier;			//バリアの回転量
		float gravity;						//重力加速度
		Vec2 center;						//回転軸
		Vec2 dist_quake_defeat;				//撃破エフェクトの画面揺れ幅
		Vec2 dist_quake_barrier;			//バリアの画面揺れ幅
		Effect* eff;						//エフェクトクラス
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