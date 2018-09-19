#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エネミーの発生させる判定矩形(弾）
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Shot01
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("弾");	//グループ名
	const  string  defName("NoName");				//タスク名
	//-------------------------------------------------------------------//リソース
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
		string imageName;
	};
	//-------------------------------------------------------------------//オブジェクト
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
		bool flag_Erase;	//壁や敵に衝突したとき、消滅するか否か
		int limit_Erase;	//消滅するまでの時間
		int power;			//攻撃力
	public:
		//攻撃毎に寿命を設ける際に使用
		//引数	：	（寿命）
		void Set_Limit(const int&);
		//壁や敵に衝突したとき、消えるか否かを指定する
		//引数	：	（0か1）
		void Set_Erase(const int&);
		//外部から生成する際、攻撃力を指定
		//引数	：	（整数値）
		void Set_Power(const int&);
		//状態ごとに行動を指定する
		void Move();
		//消滅する際、状態に応じてエフェクトを生成
		//引数	：	（エフェクトを生成する座標）
		void Effect_Hit(const ML::Vec2&);
	};
}