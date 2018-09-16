#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ガン
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Gun
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ガン");	//グループ名
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
		string imageName;
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

	private:
		//描画角度
		float angle;
		//発砲時の揺れ幅
		float tremor;
		//
		string controllerName;
	public:
		//プレイヤが消滅したとき、プレイヤ側からKill
		void Gun_Kill();
		//角度を返す
		float Get_Angle();
		//思考
		void Think();
		//行動
		void Move();
	};
}