#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボス（タスク）
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Boss
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボス");	//グループ名
	const  string  defName("ボス");		//タスク名
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
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		enum State_Boss
		{
			Floating,	//どこにも接続していない
			Common,		//土台以外と接続状態
			Base		//マップと接続している
		};
	private:
		State_Boss state_boss;	//状態管理
		float speed_fall;		//速度落下
		float max_speed_fall;	//最大速度落下
	public:
		//コンストラクタ
		Object();
		//思考
		void Think();
		//行動
		void Move();
		//攻撃を受けた際、呼び出す
		//引数	：	（呼び出し元,攻撃情報）
		void Recieved(const BChara*, const BChara::AttackInfo&);
		//アニメーション
		BChara::DrawInfo Anim();
	};
}