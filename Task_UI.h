#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UI表示
//-------------------------------------------------------------------
#include "BChara.h"
#include "Effect.h"

namespace  UI
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("UI");	//グループ名
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
		string hpImageName;
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
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		int cnt;					//アニメーションカウンタ
		
		string controllerName;		//コントローラー宣言
		
		ML::Vec2 pos_hp_boss;		//ボスのHPバー座標
		bool flag_highlight_bar;	//ボスHPバー強調表示フラグ
		int max_range_hp_bar;		//ボスHPの表示最大幅
		int cnt_highllight_bar;		//ボスHPバー表示時間カウンタ
		int limit_highlight_bar;	//ボスHPバー強調表示時間
		float cnt_gradation_bar;	//ボスHPバーグラデーションカウンタ

		int max_dist_quake_x_bar;		//HPバーX軸揺れ最大値
		int max_dist_quake_y_bar;		//HPバーy軸揺れ最大値
		int interval_quake_bar;			//HPバー揺れ間隔

		Effect* eff;				//エフェクトクラスインスタンス
	public:
		//アクセサ

		//HPバーの強調表示を外部から指定する
		void Set_Highlight_Bar();
	};
}