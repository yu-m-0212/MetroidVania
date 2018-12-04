#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボス（アッパー）
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"

namespace  Boss_Upper
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボス");		//グループ名
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
		string name_image;	//画像リソース
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
		//横揺れ
		float speed_shake;		//速度横揺れ
		float std_pos_x;		//横揺れ基準値
		float cnt_shake;		//カウンタ横揺れ
		float interval_shake;	//間隔横揺れ
		//ショット生成
		int cnt_move;			//カウンタ行動
		int interval_shot;		//生成時間ショット
		//ショット情報
		ML::Vec2 vec_shot;		//移動量ショット
		ML::Box2D hit_shot;		//矩形ショット
		//インスタンス
		Boss* boss;				//ボスクラスポインタ
		Shot01::Object* shot;	//ショットオブジェクトポインタ
	public:
	};
}