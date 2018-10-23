#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//画面効果
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Display_Effect
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("画面効果");	//グループ名
	const  string  defName("NoName");		//タスク名
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
		string imageName;
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
		float transparency;			//透明度
		bool flag_in_out;			//0=in 1=out
		int cnt_transition;			//画面遷移カウンタ
		int time_create_next_task;	//引継ぎタスクを生成するタイミング
		int time_start_fade_out;	//フェードアウトを始めるタイミング
		int next_scene;				//引継ぎタスクを指定する変数
	public:
		//引継ぎタスクを指定する
		//引数	：	（整数）
		//0=Game	1=Retry		2=Ending	3=Title
		void Set_Next_Scene(const int&);
	};
}