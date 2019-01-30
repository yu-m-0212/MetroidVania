#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//リトライ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BChara.h"
#include "Task_Effect.h"
using namespace ML;
namespace  Retry
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("リトライ");	//グループ名
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
		string back_retry;
		string button_retry;
		string name_se_retry;	//リトライボタンプッシュ
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
		string controllerName;			//コントローラの宣言
		Vec2 pos_dead;					//前回の死亡地点
		BChara::Angle_LR angle_dead;	//前回の死亡時の向き
		bool flag_transition;			//画面遷移用フラグ
		bool title_or_game;				//引継ぎタスクの選択フラグ
		int cnt_transition;				//カウンタ遷移用
		int cnt_create_bubble;			//エフェクトの生成カウンタ
		int cnt_available_controll;		//生成後、操作を受け付けるまでのカウンタ
		int time_create_next_task;		//引継ぎタスクの生成タイミング
		int time_kill_game;				//自身を消滅させるタイミング
		int time_available_controll;	//生成後、操作を受け付けるまでの時間
		float interval_anim_back;		//背景アニメ周期
		float cnt_anim_back;			//背景アニメカウンタ
		float posY;						//背景Y軸座標
		float posY_std;					//背景Y軸座標基準値
		float height_anim_back;			//背景アニメ揺れ幅
		float init_bubble_pos_y;		//泡のY軸初期座標
		Task_Effect::Object* eff;		//メソッド呼び出し
	public:
		//死亡した座標を保存する
		//引数	：	（Vec2)
		void Set_Pos_Dead(const Vec2&);
		//前回死亡した座標を返す
		ML::Vec2 Get_Pos_Dead();
		//死亡時の向きを指定する
		//引数	：	（BChara::Angle_LR)
		void Set_Angle_Dead(const BChara::Angle_LR&);
		//死亡時の向きを取得する
		BChara::Angle_LR Get_Angle_Dead();
	};
}