#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_Effect.h"

namespace  Title
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タイトル");	//グループ名
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
		string	back_title;					//画像背景
		string	button_title;				//画像Optionボタン
		string	logo_title;					//画像タイトル

		string	base_file_path;				//ベースファイルパス
		string	name_sound_bubble;			//効果音泡
		string	name_sound_title_push;		//効果音タイトルプッシュ
		string	name_environmental_title;	//環境音タイトル
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
		bool flag_transition;		//画面遷移フラグ
		int cnt_transition;			//消滅カウンタ
		int cnt_available_controll;	//操作を受け付けるまでのカウンタ
		int cnt_create_bubble;		//エフェクトの生成カウンタ
		int limit_transparency;		//画面遷移時の消滅時間
		int time_avalable_controll;	//操作を受け付けるまでの時間
		float init_bubble_pos_y;	//泡のY軸初期座標
		float posY;					//背景Y軸座標
		float posY_std;				//背景Y軸座標基準値
		float height_anim_back;		//背景アニメ揺れ幅
		float cnt_anim_button;		//ボタンアニメカウンタ
		float cnt_anim_back;		//背景アニメカウンタ
		float interval_button;		//ボタンアニメ周期
		float interval_anim_back;	//背景アニメ周期
		ML::Vec2 pos_button;		//座標ボタン
		ML::Vec2 pos_title;			//座標タイトル
		Task_Effect::Object* eff;	//メソッド呼び出し
		string controllerName;		//コントローラー宣言
	};
}