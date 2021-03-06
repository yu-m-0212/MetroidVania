#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//かわいい妖精
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Sprite
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("オプション");	//グループ名
	const  string  defName(		"妖精？");		//タスク名
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
		string		imageName;
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
	private:
		string controllerName;		//コントローラー宣言
		float speed_to_Vec;			//目標への移動量
		float dist_to_Vec;			//プレイヤからの距離
		float dist_height;			//プレイヤから上方位置
		weak_ptr<BChara> target;	//プレイヤを指すスマートポインター
	public:
		Object();
		//追従対象を指定
		void Set_Target(const weak_ptr<BChara>);
		//マップ外を表示しない
		bool Check_Out_OF_Map_Top();
		bool Check_Out_OF_Map_Bottom();
		bool Check_Out_OF_Map_Left();
		bool Check_Out_OF_Map_Right();
	};
}