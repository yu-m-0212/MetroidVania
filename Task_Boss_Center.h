#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボス（センター）
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"

namespace  Boss_Center
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボス");		//グループ名
	const  string  defName("ボス（センター）");		//タスク名
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
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

		int cnt_move;					//カウンタ行動
		int interval_shot;				//生成時間ショット

		int add_un_hit;					//プレイヤに与える無敵時間

		float speed_chase;				//速度追従
		ML::Vec2 vec_shot;				//移動量ショット
		ML::Box2D hit_shot;				//矩形ショット

		Boss* boss;						//メソッド呼び出し
		Shot01::Object* shot;			//メソッド呼び出し
	public:
		//コンストラクタ
		Object();
		//思考&状況判断(ステータス決定)
		void Think();
		//モーションに対応した処理
		void Move();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
		void Received(BChara* from_, AttackInfo at_, const int&);
		//アニメーション制御
		BChara::DrawInfo Anim();
	};
}