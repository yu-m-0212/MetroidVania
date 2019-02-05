#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//アイテム01(バリアアビリティ)
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Tutorials.h"

namespace  Item01
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
		string name_image;				//画像リソース
		string name_se_pick_up_item;	//効果音
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
	public:
		Object();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	private:
		int limit_message;					//メッセージの時間制限
		int time_erase;						//自身の消滅タイミング
		ML::Vec2 center;					//画面中央座標
		ML::Vec2 init_pos_create_tutorial;	//反射チュートリアルの生成座標
		Tutorials::Object* tutorials;		//メソッド呼び出し
	public:
		//アニメーション制御
		BChara::DrawInfo Anim();
		//接触時の応答処理（必ず受け身の処理として実装する）
		void Recieved(BChara* from_, AttackInfo at_, const int& un_hit_);
		//アイテム01の生成
		//引数	：	（初期座標）
		void Create_Item01(const ML::Vec2&);
	};
}