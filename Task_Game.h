#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Effect.h"
#include  "Task_Tutorials.h"
#include  "Task_Player.h"
#include  "Task_Enemy01.h"

namespace  Game
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("本編");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
	public:
		Resource();
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string base_file_path_game;		//基礎ファイルパス
		string name_bgm_main_game;		//BGM本編
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		bool  Initialize();		//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();			//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	private:
		ML::Vec2 pos_dead;				//前回の死亡地点
		ML::Vec2 pos_spawer;			//ボススポナー座標
		BChara::Angle_LR angle_dead;	//死亡時の向き
		int cnt_transition;				//カウンタ遷移用
		int time_create_fade;			//画面効果生成タイミング
		int time_create_next_task;		//引継ぎタスクの生成タイミング
		int time_create_fade_after_clear;				//自身を消滅させるタイミング
		int limit_message_task;			//目的メッセージの表示時間
		Tutorials::Object* tutorials;	//ポインタメッセージ
		Task_Effect::Object* eff;		//ポインタエフェクト
		string message_clear;			//ボス撃破時のメッセージ
	public:
		//リソースを常駐させる
		Shot00::Resource::SP		shot00_Resource;
		Shot01::Resource::SP		shot01_Resource;
		Task_Effect::Resource::SP	effect_Resource;
		Player::Resource::SP		player_resource;
		Enemy01::Resource::SP		enemy01_resource;
	};
}