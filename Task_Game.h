#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Effect.h"

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
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
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
		ML::Vec2 deadPos;	//前回の死亡地点
		int cnt_Scenes;		//カウンタ遷移用
	public:
		//リソースを常駐させる
		Shot00::Resource::SP	shot00_Resource;
		Shot01::Resource::SP	shot01_Resource;
		Effect::Resource::SP	effect_Resource;
		//チュートリアル表示キャラクタの生成
		//引数	：	（座標,メッセージ）
		void Create_Message(const ML::Vec2&, const string&);
		//エネミーの生成
		//引数	：	（初期座標,移動速度,HP）
		void Create_Enemy01(const ML::Vec2&);
	};
}