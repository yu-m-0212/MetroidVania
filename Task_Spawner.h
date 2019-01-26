#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//スポナー
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Game.h"
#include "Task_Effect.h"

namespace  Spawner
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("スポナー");		//グループ名
	const  string  defName("ボススポナー");		//タスク名
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		string name_bgm_boss;	//ボスBGM
		string name_image;		//判定矩形画像
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
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
		int time_create_boss;			//ボスの生成タイミング
		int time_bgm;					//ボスBGMの再生開始時間
		bool flag_spawn_boss;			//ボス出現フラグ
		ML::Vec2 init_pos_boss;			//ボスの初期座標
		Task_Effect::Object* eff;		//エフェクトインスタンス
	public:
		//ボスの出現フラグを返す
		bool Get_Flag_Spawn_Boss();
	};
}