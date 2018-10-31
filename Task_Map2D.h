#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//２次元配列マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Map2D
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("フィールド");	//グループ名
	const  string  defName("マップ");			//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		int					cnt_Quake;						//カウンタ画面揺れ
		int					dist_Quake;						//画面揺れ幅
		int					limit_Quake;					//画面を揺らす時間
	public:
		string				imageName;						//マップはリソース共有しない
		int					arr[SIZE_MAP_H][SIZE_MAP_W];	//マップ配列
		int					sizeY, sizeX;					//マップサイズ
		ML::Box2D			hitBase;						//ピクセル単位のマップサイズを持つ
		ML::Box2D			chip[24];						//チップ番号配列
		//マップファイル読み込み(今は仮処理）
		bool Load(const  string&  fpath_);
		//あたり判定
		bool CheckHit(const  ML::Box2D&  h_);
		//マップ外を見せないようにカメラを位置調整する
		void  AjastCameraPos();
		//チップ番号に対応した処理
		//壁の向きに合わせてリソースを変える
		//チップ番号に合わせたキャラクタを生成する
		void Map_Adjust_Chip();
		//エネミーの生成
		//引数	：	（初期座標）
		void Create_Enemy01(const ML::Vec2&);
		//アイテム00の生成
		//引数	：	（初期座標）
		void Create_Item00(const ML::Vec2&);

		//アクセサ
		//画面の揺れ幅を指定する
		void Set_Dist_Quake(const int&);
		//画面を揺らす時間を指定する
		void Set_Limit_Quake(const int&);
	};
}