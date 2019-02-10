#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ボス（ヘッド）
//-------------------------------------------------------------------
#include "BChara.h"
#include "Boss.h"
#include "Task_Shot01.h"
#include "Task_Map2D.h"
#include "Task_Effect.h"

namespace  Boss_Head
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボス（ヘッド）");			//グループ名
	const  string  defName("ボス（ヘッド）");		//タスク名
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
		string name_image;					//リソース名
		string name_rect;					//判定矩形リソース名
		string base_file_path_sound;		//基礎ファイルパス
		string name_sound_wait_under_00;	//待機中SE00
		string name_sound_wait_under_01;	//待機中SE01
		string name_sound_hiding_under;		//潜行中SE
		string name_sound_appear_under;		//登場SE
		string name_sound_hit_reflect;		//反射弾被弾時
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
		//攻撃情報
		int add_un_hit;								//プレイヤに与える無敵時間
		//横揺れ
		float speed_shake;							//実処理を行う横揺れ速度
		float speed_shake_def;						//通常時の横揺れ速度
		float speed_shake_ref;						//反射被弾時の横揺れ速度
		float speed_shake_stn;						//気絶時の横揺れ速度
		float std_pos_x;							//横揺れ基準値
		float cnt_shake;							//カウンタ横揺れ
		float interval_shake;						//間隔横揺れ
		float interval_shake_stan;					//気絶時の横揺れ速度
		//ショット生成
		int num_shot;								//弾の生成数
		float angle_create_shot;					//弾を生成する角度の間隔
		//ショット情報
		ML::Vec2 vec_shot;							//移動量ショット
		ML::Box2D hit_shot;							//矩形ショット
		//状態遷移用変数
		bool flag_alive_base;						//土台の生死
		int interval_to_appear;						//登場するまでの時間
		int interval_create_effect;					//エフェクトの生成間隔
		int interval_shot;							//ショットに移る時間
		int interval_return;						//ショットから戻るまでの時間
		int cnt_defeat_parts;						//胴体パーツを破壊すると加算し、上限に達すると気絶する
		int limit_stan;								//気絶時間
		int limit_move_vertically;					//縦向き時の登場移動時間
		int correction_pos_weak_point;				//弱点矩形の本体からの座標補正
		float speed_move_under;						//縦向き時の登場・退場速度
		float speed_chase_hiding;					//潜行中プレイヤに接近する割合
		ML::Box2D hit_vertically_long;				//縦長の時の矩形（hitBaseに代入して使用）
		ML::Box2D hit_horizontally_long;			//横長の時の矩形（hitBaseに代入して使用）
		//画面揺れ用変数
		ML::Vec2 dist_quake_boss_return_appear;		//ボス用登場退場画面揺れ幅
		int limit_quake_boss_return_appear;			//ボス用登場退場画面揺れ時間
		//インスタンス
		Boss* boss;									//ボスクラスポインタ
		Shot01::Object* shot;						//ショットオブジェクトポインタ
		Task_Effect::Object* eff;					//エフェクトオブジェクトポインタ
		//エフェクト生成関連
		ML::Box2D rect_create_effect_end;			//エンドパターン中のエフェクト生成範囲
		float dist_shake_end_pattern;					//エンドパターン時の横揺れ幅
	public:
		//コンストラクタ
		Object();
		//思考&状況判断(ステータス決定)
		void Think();
		//モーションに対応した処理
		void Move();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
		void Recieved(BChara* from_, AttackInfo at_, const int&);
		//アニメーション制御
		BChara::DrawInfo Anim();

		//アクセサ

		//潜行から登場までの時間を減少させる
		//引数	：	（減少量）
		void Decrease_Interval_To_Appear(const int&);
		//登場からショットに移行するまでの時間を減少させる
		//引数	：	（減少量）
		void Decrease_Interval_Shot(const int&);
		//胴体パーツの破壊数を加算する
		void Add_Defeat_Parts();
		//胴体パーツの破壊数を取得する
		int Get_Defeat_Parts();
		//自身の座標から弱点座標への補正量を取得する
		int Get_Correction_Pos_Weak_Point();
	};
}