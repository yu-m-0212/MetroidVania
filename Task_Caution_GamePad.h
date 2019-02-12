#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ゲームパッド注意
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Caution
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("注意");		//グループ名
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		string name_image_caution;	//画像リソース
		string name_font;			//フォント
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
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		bool transition_scene;		//シーン切り替えフラグ
		int rate_close_up;			//描画拡大率
		int add_rate_close_up;		//描画拡大率増加量
		int speed_rate_close_up;	//描画拡大速度
		int time_flag;				//遷移フラグを反転させる時間
		int time_erase;				//フラグ反転後、消滅までの時間
		string name_pad;			//パッド宣言
		string text_skip;			//スキップ表示
		ML::Vec2 pos_font;			//フォント座標
		ML::Color color_font;		//フォント色
	public:
		//コンストラクタ
		Object();					
		//画面切り替えフラグ管理
		bool Transition_Scene();
	};
}