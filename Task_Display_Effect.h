#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//画面効果
//-------------------------------------------------------------------
#include "BChara.h"

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
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();		//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();			//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		enum State_Display_Effect
		{
			Non,							//未指定
			Fade,							//フェードイン、アウト
			Letter_Box						//レターボックス
		};
		State_Display_Effect state_display;	//状態管理
		int	num_state;						//外部から指定する際の状態番号

		bool flag_switching_slide;			//スライドイン/アウトフラグ
		int move_letter;					//レターボックスアニメカウンタ
		int add_move_letter;				//レターボックススライド速度
		int limit_move_letter;				//レターの移動量上限
		int cnt_switch_letter;				//スライドイン/アウトの切り替え間隔カウンタ
		int time_switch_letter;				//スライドイン/アウトの切り替えタイミング

		bool flag_in_out;					//0=in 1=out
		int cnt_transition;					//画面遷移カウンタ
		int time_create_next_task;			//引継ぎタスクを生成するタイミング
		int time_start_fade_out;			//フェードアウトを始めるタイミング
		float transparency;					//透明度
	public:
		//外部から状態を指定する
		//引数	：	（整数）
		//0：フェードイン/アウト
		//1：レターボックス
		void Set_Num_State(const int&);
		//画面全体の効果を生成する
		//引数	：	（状態番号）
		void Create_Display_Effect(const int&);
	};
}