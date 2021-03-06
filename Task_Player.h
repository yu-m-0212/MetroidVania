#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Effect.h"
#include "Task_Tutorials.h"

namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");		//グループ名
	const  string  defName("仮");				//タスク名
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
		string		imageName;				//画像リソース
		string		base_file_path_sound;	//サウンドファイルパスベース
		string		name_sound_landing;		//効果音着地
		string		name_sound_jump;		//効果音ジャンプ
		string		name_sound_shot;		//効果音ショット
		string		name_not_recharge;		//効果音未リチャージバリア
		string		name_damage_player;		//効果音被弾
		string		name_sound_heal;		//効果音回復
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
		bool barrier;					//バリアの使用制限
		bool active_barrier;			//バリアのリチャージフラグ
		string controllerName;			//コントローラ宣言
		string message_dead;			//死亡時のメッセージ
		float decSpeed;					//速度減衰量
		float max_StompFallSpeed;		//ストンプ時の最大降下速度
		float max_speed_fall;			//最大速度降下
		float speed_shot;				//ショット速度
		float init_shot;				//生成位置ショット
		float height_jump;				//ジャンプ初速
		int interval_shot;				//射撃の発射間隔（フレーム）
		int limit_JumpAngleChange;		//ジャンプから一定時間内なら向きを変えられる
		int limit_stomp;				//継続時間ストンプ
		int limit_stompHoldTime;		//硬直時間ストンプ
		int limit_shot;					//継続時間ショット
		int limit_quake_damage;			//画面揺れ時間ダメージ
		int limit_message_dead;			//死亡時のメッセージの消滅時間
		int lv_stomp;					//ストンプアップグレードレベル
		int power_stomp;				//攻撃力ストンプ
		int power_shot;					//攻撃力ショット
		int gauge_melee;				//近接攻撃のリチャージ
		int gauge_melee_max;			//近接攻撃リチャージ上限
		int max_hp;						//上限HP
		ML::Box2D range_stomp;			//範囲ストンプ
		ML::Box2D range_shot;			//範囲ショット
		ML::Vec2 moveBack_stomp;		//ふっとび量ストンプ
		ML::Vec2 dist_quake_damage;		//画面揺れ幅ダメージ
		Task_Effect::Object* eff;		//メソッド呼び出し
		Tutorials::Object* tutorials;	//チュートリアルインスタンス
	public:
		Object();
		//思考＆状況判断(ステータス決定）
		void  Think();
		//モーションに対応した処理
		void  Move();
		//接触時の応答処理（必ず受け身の処理として実装する）
		//引数	：	(攻撃側,攻撃情報,与無敵時間)
		void Recieved(BChara* from_, AttackInfo at_,const int&);
		//アニメーション制御
		BChara::DrawInfo  Anim();
		//ショット生成スタンダード
		void Shot_Appear();
		//ストンプスタンダード
		void Stomp_Std();
		//行動ショット中
		void Move_Shot();

		//アクセサメソッド

		//バリアの取得状況を取得する
		bool Get_Barrier();
		//HPの値を取得する
		int Get_HP();
		//HPの最大値を取得する
		int Get_Max_HP();
		//状態を取得する
		int Get_State();
		//ショットの発射間隔を取得する
		int Get_Interval_Shot();
		//近接攻撃リチャージの値を取得する
		int Get_Gauge_Mlee();
		//近接攻撃リチャージ上限の値を取得する
		int Get_Gauge_Melle_Max();

		//HPの最大値を指定する
		//引数	：	（int）
		void Set_Max_HP(const int&);
		//バリアの取得状況を指定する
		//引数	：	（bool）
		void Set_Barrier(const bool&);
		//バリアのリチャージ
		//引数	：	（bool）
		void Set_Barrier_Recharge(const bool&);
	};
}