#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//敵00(スライム
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Effect.h"

namespace  Enemy01
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵");	//グループ名
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
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		string imageName;				//名前画像
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
		bool  Initialize();		//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();			//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		int interval_Caution;		//プレイヤが視界から外れた後、再度警戒に入るまでの時間
		int interval_Attack;		//弾を生成する間隔
		int interval_Flash;			//点滅間隔
		int size_h_resource;		//被弾時、ホワイトアウトする際の基準値
		int add_un_hit;				//プレイヤに与える無敵時間
		float decSpeed;				//速度減衰量
		float max_speed_fall;		//最大速度降下
		ML::Box2D searchBase;		//視認矩形
		ML::Vec2 shot_Init;			//弾の生成時、自身の座標に足す値
		Task_Effect::Object* eff;	//メソッド呼び出し
	public:
		//コンストラクタ
		Object();
		//思考&状況判断(ステータス決定)
		void Think();	
		//モーションに対応した処理
		void Move();		
		//プレイヤを発見したら警戒モードに入る
		bool Search_Player();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
		void Received(BChara* from_, AttackInfo at_,const int&);
		//アニメーション制御
		BChara::DrawInfo Anim();	
		//エネミーの生成
		//引数	：	（初期座標）
		void Create_Enemy01(const ML::Vec2&);

		//アクセサ
		//探知矩形を取得する
		ML::Box2D Get_Search();
	};
}