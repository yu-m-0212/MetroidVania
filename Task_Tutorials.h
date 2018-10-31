#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//チュートリアルキャラクタ
//-------------------------------------------------------------------
#include "BChara.h"

#define	FONT_WIDTH 16
#define FONT_HIGHT 32

namespace  Tutorials
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("チュートリアル");	//グループ名
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
		//共有する変数はここに追加する
		string imageName,
			   fontName;
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
		string message;	//表示するメッセージを格納する
		int limit;		//消滅までの時間
	public:
		//コンストラクタ
		Object();
		//外部から状態を指定する際使用
		//引数	：	（対応する番号）
		void Set_Message(const string&);
		//消滅時間を指定する
		//引数	：	（消滅までの時間）
		void Set_Limit(const int&);
		//メッセージを生成する
		//引数	：	（メッセージ,座標,消滅時間）
		void Create_Message(const string&, const ML::Vec2&,const int&);
	};
}