#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボス01
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Task_Boss02
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボス");	//グループ名
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
		bool  Initialize();		//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();			//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//状態管理
		enum State_Boss
		{
			Non,	//未指定
			Base,	//土台
			Common	//通常
		};
	private:
		State_Boss state_boss;	//ボス用状態管理
		int interval_shake;		//間隔左右揺れ
		float std_pos_x;		//基準値横軸座標
		float speed_shake;		//速度揺れ
		float max_speed_fall;	//最大落下速度
	public:
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	(攻撃側,攻撃情報,与無敵時間)
		void Received(BChara* from_, AttackInfo at_, const int&);
		//思考＆状況判断(ステータス決定）
		void  Think();
		//モーションに対応した処理
		void  Move();
		//アニメーション制御
		BChara::DrawInfo  Anim();

		//アクセサ

		void Set_State_Boss(const State_Boss&);
		void Set_Std_Pos_X(const float&);
		void Set_Hit_Body(const ML::Box2D&);
	};
}