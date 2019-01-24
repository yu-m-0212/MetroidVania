#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//アイテム00(体力上限アップ)
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Tutorials.h"
#include "Task_Effect.h"

namespace  Item00
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("アイテム");		//グループ名
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
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	private:
		int add_Hp;						//プレイヤのHP増加量
		int limit_message;				//制限時間メッセージ
		ML::Vec2 center;				//画面中央
		Tutorials::Object* tutorials;	//ポインタチュートリアル
	public:
		//コンストラクタ
		Object();
		//アニメーション制御
		BChara::DrawInfo Anim();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
		void Recieved(BChara* from_, AttackInfo at_, const int&);
		//アイテム00の生成
		//引数	：	（初期座標）
		void Create_Item00(const ML::Vec2&);
	};
}