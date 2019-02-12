//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Effect.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_Boss_Head.h"
#include  "Task_Display_Effect.h"

using namespace ML;
namespace  Task_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->effectImage = "effectImage";
		DG::Image_Create(this->effectImage,"./data/image/effect.png");
		//ファイルパス
		this->base_file_path_sound = "./data/sound/wav/";
		this->name_sound_defeat = "sound_defeat";
		this->name_sound_hit = "hit_shot";
		this->name_sound_barrier = "sound_barrier";
		this->name_sound_barrier_recharge = "sound_barrier_recharge";
		//音の生成
		//wavファイルの中でも再生できないものあり
		DM::Sound_CreateSE(this->name_sound_defeat, this->base_file_path_sound + "explosion_enemy01.wav");
		DM::Sound_CreateSE(this->name_sound_hit, this->base_file_path_sound + "hit_shot_player01.wav");
		DM::Sound_CreateSE(this->name_sound_barrier, this->base_file_path_sound + "activate_barrier_player01.wav");
		DM::Sound_CreateSE(this->name_sound_barrier_recharge, this->base_file_path_sound + "barrier_recharge.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
		DM::Sound_Erase(this->name_sound_defeat);
		DM::Sound_Erase(this->name_sound_hit);
		DM::Sound_Erase(this->name_sound_barrier);
		DM::Sound_Erase(this->name_sound_barrier_recharge);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->state_effect = Non;					//状態管理
		this->limit_erase = 0;						//時間消滅まで
		this->dist = 0.0f;							//回転する際の中心からの距離
		this->angle = 0.0f;							//角度
		this->add_angle_target_circle = 0.05f;		//ボス警告サークルの回転量
		this->add_angle_barrier = 0.025f;			//バリアの回転量
		this->center = Vec2(0, 0);					//回転軸
		this->num_bubble = 0;						//泡の大きさ
		this->interval_bubble = 16;					//泡の揺れ周期
		this->wide_bubble = 5.0f;					//泡の揺れ幅
		this->limit_erase_hit_shot = 18;			//時間消滅までヒットショット
		this->limit_erase_barrier_recharge = 15;	//時間消滅までバリアリチャージ
		this->limit_erase_defeat = 36;				//時間消滅まで撃破
		this->limit_erase_heal = 24;				//時間消滅まで回復
		this->limit_erase_bubble = 600;				//時間消滅まで泡
		this->limit_erase_appear = 180;				//時間消滅まで登場
		this->limit_erase_debris = 120;				//消滅までの時間破片
		this->limit_erase_spark = 180;				//消滅までの時間火花
		this->limit_erase_highlight_head = 30;		//消滅までの時間ハイライト
		this->limit_effect_target_boss = 240;		//ボス警告エフェクトの消滅時間
		this->limit_effect_barrier = 30;			//消滅までの時間バリア
		this->limit_effect_boss_chance_mark = 360;	//消滅までの時間ボス弱点
		this->limit_quake_defeat = 30;				//撃破エフェクトの画面揺れ時間
		this->limit_quake_barrier = 15;				//バリアの画面揺れ時間
		this->dist_quake_defeat = ML::Vec2(2, 2);	//撃破エフェクトの画面揺れ幅
		this->dist_quake_barrier = ML::Vec2(0, 5);	//バリアの画面揺れ幅
		this->speed_surfacing = 3.0f;				//速度浮上
		this->speed_Debris = 6.0f;					//速度破片
		this->speed_spark = 6.0f;					//速度火花
		this->gravity = ML::Gravity(SIZE_CHIP);		//重力加速度
		this->render2D_Priority[1] = 0.3f;			//描画順
		this->choice_state = -1;					//外部から状態を指定する際、使用
		Effect* eff = new Effect();					//メソッド呼び出し
		this->rate_size_circle = 10;				//ターゲットサークルのサイズ倍率
		this->cnt_anim_highlight=0.0f;				//ボススタン時のハイライトグラデーションカウンタ

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//状態が未指定なら一度だけ指定する
		if (this->state_effect == Non)
		{
			switch (this->choice_state)
			{
			default:
			case -1:	//-1は初期化値
			case 0:		//未指定
				break;
			case 1:		//ヒットショット
				this->state_effect = Hit_Shot;
				this->limit_erase = this->limit_erase_hit_shot;
				//左右によって角度を修正する
				if (this->angle_LR == Left)
				{
					this->angle = this->angle - 135.0f;
				}
				//SEの再生
				DM::Sound_Play_Volume(this->res->name_sound_hit, false, VOLUME_SE_HIT_SHOT_PLAYER);
				break;
			case 2:		//バリアリチャージ
				this->state_effect = Barrier_Recharge;
				this->limit_erase = this->limit_erase_barrier_recharge;
				//SE再生
				DM::Sound_Play_Volume(this->res->name_sound_barrier_recharge, false, VOLUME_ALL_GAME);
				break;
			case 3:		//エネミー撃破
			{
				this->state_effect = Defeat_Enemy;
				this->limit_erase = this->limit_erase_defeat;
				//画面揺れ
				auto map =
					ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_defeat, this->limit_quake_defeat);
				//SEの再生
				DM::Sound_Play_Volume(this->res->name_sound_defeat, false, VOLUME_ALL_GAME);
				break;
			}
			case 4:		//回復
				this->state_effect = Heal;
				this->limit_erase = this->limit_erase_heal;
				break;
			case 5:		//泡
				this->state_effect = Bubble;
				//三種類の中から指定する
				this->num_bubble = rand() % 3;
				//描画角度
				this->angle = float(rand() % 360);
				//消滅までの時間
				this->limit_erase = this->limit_erase_bubble;
				break;
			case 6:		//登場
				this->state_effect = Appear;
				this->limit_erase = this->limit_erase_appear;
				break;
			case 7:		//破片
				this->state_effect = Debris;
				this->limit_erase = this->limit_erase_debris;
				//初速
				this->moveVec.y = sin(this->angle)*this->speed_Debris;
				break;
			case 8:		//火花
				this->state_effect = Spark;
				this->limit_erase = this->limit_erase_spark;
				//初速
				this->moveVec.y = sin(this->angle)*this->speed_spark;
				break;
			case 9:		//ボス演出サークル00
				this->state_effect = Target_Circle_00;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 10:	//ボス演出サークル01
				this->state_effect = Target_Circle_01;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 11:	//ボス演出バー
				this->state_effect = Target_Bar;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 12:	//バリア
			{
				this->state_effect = Barrier;
				this->limit_erase = this->limit_effect_barrier;
				//SEの再生
				DM::Sound_Play_Volume(this->res->name_sound_barrier, false, VOLUME_SE_BARRIER);
				//画面を揺らすための設定を行う
				auto map = ge->GetTask_One_G<Map2D::Object>(Map2D::defGroupName);
				map->Set_Quake(this->dist_quake_barrier, this->limit_quake_barrier);
				break;
			}
			case 13:	//ハイライト
				this->state_effect = Highlight_Head;
				this->limit_erase = this->limit_erase_highlight_head;
				break;
			case 14:	//ボス弱点表示内側
				this->state_effect = Boss_Chance_Mark_In_Line;
				this->limit_erase = this->limit_effect_boss_chance_mark;
				break;
			case 15:	//ボス弱点表示外側
				this->state_effect = Boss_Chance_Mark_Out_Line;
				this->limit_erase = this->limit_effect_boss_chance_mark;
				break;
			}
		}
		this->animCnt++;
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		
		//状態毎の行動
		this->Move();
		//移動
		this->pos += this->moveVec;
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->limit_erase)
		{
			//消滅申請
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->effectImage, this->angle, ML::Vec2(float(di.draw.w / 2), float(di.draw.h / 2)));
		DG::Image_Draw(this->res->effectImage, di.draw, di.src,di.color);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		//デフォルトカラーを宣言
		Color dc(1, 1, 1, 1);
		//各エフェクトをテーブルで用意する
		BChara::DrawInfo imageTable[]
		{
			{ Box2D( -96, -64, 192, 128),Box2D(   0,   0, 192, 128),dc },//ストンプ着地の衝撃1								[ 0]
			{ Box2D( -96, -64, 192, 128),Box2D(   0, 128, 192, 128),dc },//ストンプ着地の衝撃2								[ 1]
			{ Box2D( -96, -64, 192, 128),Box2D(   0, 256, 192, 128),dc },//ストンプ着地の衝撃3								[ 2]
			{ Box2D( -96, -64, 192, 128),Box2D( 192,   0, 192, 128),dc },//パンチ風切り1										[ 3]
			{ Box2D( -96, -64, 192, 128),Box2D( 192, 128, 192, 128),dc },//パンチ風切り2										[ 4]
			{ Box2D( -96, -64, 192, 128),Box2D( 192, 256, 192, 128),dc },//パンチ風切り3										[ 5]
			{ Box2D( -96, -64, 192, 128),Box2D( 384,   0, 192, 128),dc },//パンチの衝撃1										[ 6]
			{ Box2D( -96, -64, 192, 128),Box2D( 384, 128, 192, 128),dc },//パンチの衝撃2										[ 7]
			{ Box2D( -96, -64, 192, 128),Box2D( 384, 256, 192, 128),dc },//パンチの衝撃3										[ 8]
			{ Box2D( -96, -64, 192, 128),Box2D( 576,   0, 192, 128),dc },//遺体から回復1										[ 9]
			{ Box2D( -96, -64, 192, 128),Box2D( 576, 128, 192, 128),dc },//遺体から回復2										[10]
			{ Box2D( -96, -64, 192, 128),Box2D( 576, 256, 192, 128),dc },//遺体から回復3										[11]
			{ Box2D( -96, -96, 192, 192),Box2D( 768,   0, 192, 192),dc },//エネミー爆散1										[12]
			{ Box2D( -96, -96, 192, 192),Box2D( 768, 192, 192, 192),dc },//エネミー爆散2										[13]
			{ Box2D( -96, -96, 192, 192),Box2D( 768, 384, 192, 192),dc },//エネミー爆散3										[14]
			{ Box2D( -96, -96, 192, 192),Box2D(1344,   0, 192, 192),dc },//バリアリチャージ1									[15]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 192, 192, 192),dc },//バリアリチャージ2									[16]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 384, 192, 192),dc },//バリアリチャージ3									[17]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 576, 192, 192),dc },//バリアリチャージ4									[18]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 768, 192, 192),dc },//バリアリチャージ5									[19]
			{ Box2D( -48, -48,  96,  96),Box2D(1536,   0,  96,  96),ML::Color(0.3f,1,1,1)},//泡1								[20]
			{ Box2D( -48, -48,  96,  96),Box2D(1536,  96,  96,  96),ML::Color(0.3f,1,1,1)},//泡2								[21]
			{ Box2D( -48, -48,  96,  96),Box2D(1536, 192,  96,  96),ML::Color(0.3f,1,1,1)},//泡3								[22]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,   0,  32,  32),dc },//エネミー撃破1										[23]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  32,  32,  32),dc },//エネミー撃破2										[24]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  64,  32,  32),dc },//エネミー撃破3										[25]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  96,  32,  32),dc },//エネミー撃破4										[26]
			{ Box2D( -16, -16,  32,  32),Box2D(1632, 128,  32,  32),dc },//エネミー撃破5										[27]
			{ Box2D( -16, -16,  32,  32),Box2D(1632, 160,  32,  32),dc },//エネミー撃破6										[28]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,   0,  32,  32),dc },//爆発破片1											[29]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  32,  32,  32),dc },//爆発破片2											[30]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  64,  32,  32),dc },//爆発破片3											[31]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  96,  32,  32),dc },//爆発破片4											[32]
			{ Box2D( -16, -16,  32,  32),Box2D(1664, 128,  32,  32),dc },//爆発破片5											[33]
			{ Box2D( -16, -16,  32,  32),Box2D(1664, 160,  32,  32),dc },//爆発破片6											[34]
			{ Box2D(  -6,  -6,  12,  12),Box2D(1728,   0,  12,  12),dc },//火花												[35]
			{ Box2D(-128,-128, 256, 256),Box2D(1792,   0, 256, 256),dc },//ターゲットサークル1								[36]
			{ Box2D(-128,-128, 256, 256),Box2D(2048,   0, 256, 256),dc },//ターゲットサークル2								[37]
			{ Box2D(-192,-192, 384, 384),Box2D(2304,   0, 384, 384),dc },//ターゲットバー00									[38]
			{ Box2D(-192,-192, 384, 384),Box2D(2304, 384, 384, 384),dc },//ターゲットバー01									[39]
			{ Box2D( -96, -96, 192, 192),Box2D(2688,   0, 192, 192),ML::Color(0.5f,1.0f,1.0f,1.0f) },//バリア				[40]
			{ Box2D( -96,-148, 192, 296),Box2D(2880,   0, 192, 296),dc },//ボススタンハイライト								[41]
			{ Box2D(-128,-128, 256, 256),Box2D(3086,   0, 256, 256),dc },//ウィークポイントサークル1							[42]
			{ Box2D(-128,-128, 256, 256),Box2D(3342,   0, 256, 256),dc },//ウィークポイントサークル2							[43]
			{ Box2D(-960,-540,1920,1080),Box2D(3600,   0,1920,1080),dc } //HPバーハイライト									[44]
		};
		//返す変数を用意
		BChara::DrawInfo  rtv;
		//各アニメーションの際、この変数にanimCntを入れ計算を行う
		int effectCnt = 0;
		//状態ごとの描画処理
		switch (this->state_effect)
		{
		//アニメーションを付ける場合
		//切り替わるフレーム数
		//パターン数
		//effectCntに対象の要素番号を足して返す
		case Unnon:
			break;
		case Hit_Shot:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case Barrier_Recharge:
			effectCnt	= this->animCnt / 3;
			effectCnt %= 5;
			rtv = imageTable[effectCnt+15];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Defeat_Enemy:
			effectCnt = this->animCnt / 6;
			effectCnt %= 6;
			rtv = imageTable[effectCnt + 23];
			break;
		case Bubble:
			rtv = imageTable[this->num_bubble + 20];
			break;
		case Appear:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 9];
			rtv.color = ML::Color(1, 0, 0, 1);
			break;
		case Debris:
			effectCnt = this->animCnt / (this->limit_erase_debris / 6);
			effectCnt %= 6;
			rtv = imageTable[effectCnt + 29];
			break;
		case Spark:
			rtv = imageTable[35];
			break;
		case Target_Circle_00:
			rtv = imageTable[36];
			//大きい状態で登場し徐々にフォーカスする感じで縮小
			rtv.draw.x *= this->rate_size_circle;
			rtv.draw.y *= this->rate_size_circle;
			rtv.draw.w *= this->rate_size_circle;
			rtv.draw.h *= this->rate_size_circle;
			if (this->rate_size_circle > 1)
			{
				//消滅時にちょうど1倍になる
				this->rate_size_circle -= this->animCnt / (this->limit_effect_target_boss/this->rate_size_circle);
			}
			break;
		case Target_Circle_01:
			rtv = imageTable[37];
			//大きい状態で登場し徐々にフォーカスする感じで縮小
			rtv.draw.x *= this->rate_size_circle;
			rtv.draw.y *= this->rate_size_circle;
			rtv.draw.w *= this->rate_size_circle;
			rtv.draw.h *= this->rate_size_circle;
			if (this->rate_size_circle > 1)
			{
				//消滅時にちょうど1倍になる
				this->rate_size_circle -= this->animCnt / (this->limit_effect_target_boss / this->rate_size_circle);
			}
			break;
		case Target_Bar:
			effectCnt = this->animCnt / 2;
			effectCnt %= 2;
			rtv = imageTable[38+effectCnt];
			break;
		case Barrier:
			rtv = imageTable[40];
			break;
		case Highlight_Head:
		{
			rtv = imageTable[41];
			this->cnt_anim_highlight += 0.1f;
			float highlight_red_green =
				sinf(this->cnt_anim_highlight);
			if (highlight_red_green < 0.0f)
			{
				highlight_red_green = 0.0f;
			}
			rtv.color.Set(1.0f, highlight_red_green, highlight_red_green, 1.0f);
		}
			break;
		case Boss_Chance_Mark_In_Line:
			rtv = imageTable[42];
			break;
		case Boss_Chance_Mark_Out_Line:
			rtv = imageTable[43];
			break;
		}
		//	向きに応じて画像を左右反転する
		if (this->angle_LR == Left)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//エフェクトを生成する（角度を指定しない）
	//引数	：	（状態,座標）
	void Object::Create_Effect(const int& choice_, const ML::Vec2&pos_)
	{
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
	}
	//エフェクトを生成する（角度を指定する）
	//引数	：	（状態,座標,角度,向き）
	void Object::Create_Effect(const int& choice_, const ML::Vec2& pos_, const float& angle_, const BChara::Angle_LR& angle_lr_)
	{
		//ポインタを宣言して確実に代入すること
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
		eff->angle = angle_;
		eff->angle_LR = angle_lr_;
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		switch (this->state_effect)
		{
		default:
			break;
		case Non:
			break;
		case Heal:
			if (nullptr == pl) { return; }
			this->moveVec = pl->moveVec;
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Bubble:
			this->moveVec = this->eff->Move_Bubble(this->moveCnt,this->interval_bubble,this->wide_bubble,this->speed_surfacing);
			break;
		case Debris:
			this->moveVec = this->eff->Move_Parabola(this->speed_Debris,this->moveVec.y,this->gravity,this->angle);
			break;
		case Spark:
			this->moveVec = this->eff->Move_Parabola(this->speed_spark, this->moveVec.y, this->gravity, this->angle);
			break;
		case Target_Circle_00:
		case Target_Circle_01:
		case Target_Bar:
		{
			//ボスヘッドの頭上に常に移動する
			auto boss_head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			if (nullptr == boss_head) { break; }
			ML::Vec2 pos_boss_head_top =
				boss_head->pos - ML::Vec2(0.0f, float(boss_head->hitBase.h / 2));
			this->pos = pos_boss_head_top;
			//画面効果が消えたところで自身をキル
			auto display_effect =
				ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr == display_effect)
			{
				this->Kill();
			}
			//外側のサークルは反時計周り
			if (this->state_effect == Target_Circle_00)
			{
				this->angle -= this->add_angle_target_circle;
			}
			//内側のサークルは時計回り
			else if (this->state_effect == Target_Circle_01)
			{
				this->angle += this->add_angle_target_circle;
			}
			break;
		}
		case Barrier:
			//プレイヤを追従する
			this->pos = pl->pos;
			//回転
			if (pl->angle_LR == Left)
			{
				this->angle += this->add_angle_barrier;
			}
			else
			{
				this->angle -= this->add_angle_barrier;
			}
			break;
		case Highlight_Head:
		{
			auto head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			//ボスヘッドの弱点を追従する
			this->pos = head->pos;
			break;
		}
		case Boss_Chance_Mark_In_Line:
		case Boss_Chance_Mark_Out_Line:
			auto head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			if (nullptr == head) { break; }
			//ボスヘッドの弱点を追従する
			int correction_pos = head->Get_Correction_Pos_Weak_Point();
			ML::Vec2 chase_head =
				head->pos + ML::Vec2(0.0f, float(correction_pos));
			this->pos = chase_head;
			//外側のサークルは反時計周り
			if (this->state_effect == Boss_Chance_Mark_In_Line)
			{
				this->angle -= this->add_angle_target_circle;
			}
			//内側のサークルは時計回り
			else if (this->state_effect == Boss_Chance_Mark_Out_Line)
			{
				this->angle += this->add_angle_target_circle;
			}
			//ボスの状態に応じて消滅
			if (head->state != Stan)
			{
				this->Kill();
			}
			break;
		}
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}