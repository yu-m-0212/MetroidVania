#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Ending
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エンディング");	//グループ名
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
		string name_image;	//名前画像
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
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		bool transition;			//フラグ画面遷移
		int cnt_transition;			//カウンタ画面遷移
		int size_image_w;			//サイズリソース幅
		int size_image_h;			//サイズリソース高さ
		int time_kill_this;			//時間自身を消滅させる
		float pos_staff_roll;		//表示位置スタッフロール
		float add_pos_staff_roll;	//加算量表示位置スタッフロール
	public:
	};
}