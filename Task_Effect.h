#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include "BChara.h"
using namespace ML;

namespace  Effect
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エフェクト");	//グループ名
	const  string  defName("NoName");			//タスク名
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
		string effectImage;
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
		int cntLimit;	//消滅までの時間
		float dist;		//撃破エフェクトの中心からの距離
		float angle;	//表示する角度
		Vec2 center;	//回転軸
	public:
		//呼び出す際に消滅までの時間を指定する
		//引数	：	（消滅までの時間）
		void Set_Limit(const int&);
		//中心点から広がるエフェクトを呼び出す際、中心からの初期位置を指定する
		//引数	：	（中心点からの初期位置)
		void Set_Dist(const float&);
		//表示する角度を指定する
		//引数	：	 (ML::ToRadian(角度))
		void Set_Angle(const float&);
		//回転の中心を外部から指定する
		//引数	：	（中心座標）
		void Set_Center(const Vec2&);
		//中心点から広がるエフェクトの中心からの距離を取得する
		float Get_Dist();
		//状態ごとに行動を指定する
		void Move();
		//アニメーション
		BChara::DrawInfo Anim();
	};
}