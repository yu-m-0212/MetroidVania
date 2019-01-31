//-------------------------------------------------------------------
//ボス（ヘッド）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Head.h"
#include  "Task_Player.h"
#include  "Task_Boss_Lower.h"

namespace  Boss_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "head";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		this->name_rect = "rect";
		DG::Image_Create(this->name_rect, "./data/image/UI.png");
		//リソース名
		this->base_file_path_sound = "./data/sound/wav/";
		this->name_sound_hit_reflect = "sound_hit_reflect";
		this->name_sound_wait_under_00 = "sound_wait_under00";
		this->name_sound_wait_under_01 = "sound_wait_under01";
		this->name_sound_hiding_under = "sound_hiding_under";
		this->name_sound_appear_under = "sound_appear_under";
		//サウンドの生成
		DM::Sound_CreateSE(this->name_sound_hit_reflect, this->base_file_path_sound + "hit_reflect.wav");
		DM::Sound_CreateSE(this->name_sound_wait_under_00, this->base_file_path_sound + "wait_under_head00.wav");
		DM::Sound_CreateSE(this->name_sound_wait_under_01, this->base_file_path_sound + "wait_under_head01.wav");
		DM::Sound_CreateSE(this->name_sound_hiding_under, this->base_file_path_sound + "hiding_under.wav");
		DM::Sound_CreateSE(this->name_sound_appear_under, this->base_file_path_sound + "appear_under.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DG::Image_Erase(this->name_rect);
		DM::Sound_Erase(this->name_sound_hit_reflect);
		DM::Sound_Erase(this->name_sound_wait_under_00);
		DM::Sound_Erase(this->name_sound_wait_under_01);
		DM::Sound_Erase(this->name_sound_hiding_under);
		DM::Sound_Erase(this->name_sound_appear_under);
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
		this->state = Hiding_Under;									//状態管理
		this->render2D_Priority[1] = 0.85f;							//描画順
		this->hitBase = ML::Box2D(-92, -145, 184, 290);				//表示矩形
		this->recieveBase = ML::Box2D(-92, -46, 184, 92);			//キャラクタとの判定矩形
		this->correction_pos_weak_point = -75;						//弱点矩形の本体からの座標補正
		this->std_pos_x = 8382.0f;									//横揺れ基準値
		this->speed_shake_def = 64.0f;								//通常時の横揺れ速度
		this->speed_shake_ref = 128.0f;								//反射被弾時の横揺れ速度
		this->speed_shake_stn = 32.0f;								//気絶時の横揺れ速度
		this->speed_shake = this->speed_shake_def;					//実処理を行う追従速度
		this->cnt_shake = 0.0f;										//カウンタ横揺れ
		this->interval_shake = 12.0f;								//間隔横揺れ
		this->interval_shake_stan = 24.0f;							//気絶時の横揺れ速度
		this->shot = new Shot01::Object();							//ショットオブジェクトインスタンス
		this->boss = new Boss();									//ボスクラスインスタンス
		this->eff = new Task_Effect::Object();						//エフェクトオブジェクトインスタンス
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);				//移動量ショット
		this->hit_shot = ML::Box2D(-16, -16, 32, 32);				//矩形ショット
		this->interval_shot = 360;									//生成時間ショット
		this->hp = 40;												//HPボス
		this->add_un_hit = 60;										//プレイヤに与える無敵時間
		this->num_shot = 5;											//弾の生成数
		this->angle_create_shot = 18.0f;							//弾を生成する間隔
		this->flag_alive_base = true;								//土台の生死
		this->interval_to_appear = 240;								//登場するまでの時間
		this->interval_create_effect = 30;							//エフェクトの生成間隔
		this->interval_return = 180;								//ショットから戻るまでの時間
		this->limit_move_vertically = 57;							//縦向き時の登場移動時間
		this->speed_move_under = 10.0f;								//縦向き時の登場、退場速度
		this->speed_chase_hiding = 0.025f;							//潜行中プレイヤに接近する割合
		this->hit_vertically_long = ML::Box2D(-92, -46, 184, 92);	//縦長の時の矩形（hitBaseに代入して使用）
		this->hit_horizontally_long = ML::Box2D(-46, -92, 92, 184);	//横長の時の矩形（hitBaseに代入して使用）
		this->dist_quake_boss_return_appear = ML::Vec2(0,5);		//ボス用登場退場画面揺れ幅
		this->limit_quake_boss_return_appear = 90;					//ボス用登場退場画面揺れ時間
		this->cnt_defeat_parts = 0;									//胴体パーツを破壊すると加算し、上限に達すると気絶する
		this->limit_stan = 360;										//気絶時間

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->shot;
		delete this->boss;
		delete this->eff;
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
		//ポーズ
		if (ge->pause) { return; }
		//カウンタ
		this->moveCnt++;
		this->animCnt++;
		this->cnt_shake += 1.0f;
		//無敵時間の減少
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//反射を受けた場合、リミットを減少させる
		if (this->limit_hit_reflect > 0)
		{
			this->limit_hit_reflect--;
		}
		//思考・状況判断
		this->Think();
		//現モーションに対応した制御
		this->Move();
		//移動処理
		this->pos += this->moveVec;
		//このタスクを倒すとクリアフラグ成立
		if (this->hp <= 0)
		{
			ge->clear = true;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->name_image, di.draw, di.src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		//頭部判定矩形の表示
		if (ge->debugMode)
		{
			ML::Box2D draw = this->recieveBase;
			draw.Offset(this->pos);
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//本体座標から弱点の位置補正
			draw.y += this->correction_pos_weak_point;
			DG::Image_Draw(this->res->name_rect, draw, src, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
		}
	}
	//思考&状況判断(ステータス決定)
	void Object::Think()
	{
		//とりあえず今の状態を指定
		BChara::State nm = this->state;
		//思考（入力）や状況に応じてモーションを変更することを目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm)
		{
		case Appear_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Wait_Under; }
			break;
		case Wait_Under:
			if (this->moveCnt > this->interval_shot) { nm = Shot_Under; }
			if (this->cnt_defeat_parts == DEFEAT_PARTS) { nm = Stan; }//各パーツが破壊されると気絶状態に遷移する
			break;
		case Shot_Under:
			if (this->moveCnt > this->interval_return) { nm = Return_Under; }
			if (this->cnt_defeat_parts == DEFEAT_PARTS) { nm = Stan; }//各パーツが破壊されると気絶状態に遷移する
			break;
		case Return_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Choice_Vertically_Or_Horizontally; }
			break;
		case Hiding_Under:
			if (this->moveCnt > this->interval_to_appear) { nm = Appear_Under; }
			break;
		case Choice_Vertically_Or_Horizontally:
			//仮に縦向きのみを選択する
			nm = Hiding_Under;
			break;
		case End_Pattern_Boss:
			if (this->moveCnt >= LIMIT_END_PATTERN_BOSS_HEAD) { nm = Lose; }
			break;
		case Stan:		//気絶
			if (this->moveCnt > this->limit_stan) { nm = Return_Under; }
			break;
		case Lose:		//死亡
			break;
		}
		//死亡処理
		if (this->hp <= 0) { nm = End_Pattern_Boss; }
		//モーション更新
		this->UpdateMotion(nm);
	}
	//モーションに対応した処理
	void Object::Move()
	{
		//複数の状態で共通して行う処理はここに記述する
		//例外がある場合はswitch文の下方に記述する

		//当たり判定 
		ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
		auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
		if (nullptr == targets) { return; }
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			//相手に接触の有無を確認させる
			if ((*it)->CheckHit(me)) 
			{
				//相手にダメージの処理を行わせる
				BChara::AttackInfo at = { 1,0,0 };
				(*it)->Recieved(this, at, this->add_un_hit);
				break;
			}
		}
		//プレイヤの位置を左右で判断する
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr != pl)
		{
			this->angle_LR = this->Check_LR(this->angle_LR, pl->pos.x);
		}
		//反射弾被弾のSE再生
		//代入後、すぐにデクリメントされているため-1で対応
		//再考の余地あり
		if (this->limit_hit_reflect == LIMIT_HIT_REFLECT - 1)
		{
			//再生中の鳴き声を停止
			DM::Sound_Stop(this->res->name_sound_wait_under_00);
			DM::Sound_Stop(this->res->name_sound_wait_under_01);
			DM::Sound_Play_Volume(this->res->name_sound_hit_reflect, false, VOLUME_SE_HIT_REFLECTION);
		}
		//-------------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state)
		{
		default:
			break;
		case Hiding_Under:
		{
			//SEの再生
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_hiding_under, false, VOLUME_ALL_GAME);
			}
			auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			//潜行中はプレイヤの座標にやんわり合わせる
			float to_vec_x = pl->pos.x - this->pos.x;
			this->pos.x += to_vec_x * this->speed_chase_hiding;
			//状態遷移の直前に揺れの基準点を現在の座標に補正する
			if (this->moveCnt == this->interval_to_appear)
			{
				this->std_pos_x = this->pos.x;
			}
			//潜行中のエフェクトを生成する
			//生成時間と変数宣言再考
			if (this->moveCnt % 10 == 0)
			{
				for (int i = 0; i < 5; ++i)
				{
					//生成位置を宣言する
					ML::Vec2 init = ML::Vec2(this->pos.x, this->pos.y - float(this->hitBase.h / 2));
					//エフェクトの生成
					this->eff->Create_Effect(7,init,-(float(rand() % 180)),this->angle_LR);
				}
			}
			//SEの停止
			if (this->moveCnt == this->interval_to_appear)
			{
				DM::Sound_Stop(this->res->name_sound_hiding_under);
			}
			break;
		}
		case Appear_Under:
		{
			//登場時に画面を揺らす
			if (this->moveCnt == 0)
			{
				auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_boss_return_appear, this->limit_quake_boss_return_appear);
				//登場時に全てのパーツが破壊されていたらここで回復する
				if (this->cnt_defeat_parts == DEFEAT_PARTS)
				{
					this->cnt_defeat_parts = 0;
				}
				//SE再生
				DM::Sound_Play_Volume(this->res->name_sound_appear_under, false, VOLUME_SE_APPEAR_UNDER);
			}
			//定位置まで移動する
			if (this->moveCnt<this->limit_move_vertically)
			{
				this->moveVec.y = -this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		}
		case Wait_Under:
		{
			//SEの再生
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_00, false, VOLUME_ALL_GAME);
			}
			if (this->moveCnt == this->interval_shot / 2)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_01, false, VOLUME_ALL_GAME);
			}
			if (this->moveCnt == this->interval_shot)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_00, false, VOLUME_ALL_GAME);
			}
			//反射弾を受けた後は一定時間、行動速度が上昇する
			if (this->limit_hit_reflect)
			{
				//一定時間動きが速くなる
				this->speed_shake = this->speed_shake_ref;
			}
			else
			{
				//通常速度
				this->speed_shake = this->speed_shake_def;
			}
			//左右に揺れる
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake)*this->speed_shake;
			break;
		}
		case Shot_Under:
			if (this->moveCnt == 0)
			{
				//標的（プレイヤ）ポインタの宣言
				auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
				if (nullptr == pl) { return; }
				//目標座標を出してから方向のみを抽出する
				ML::Vec2 to_vec = pl->pos - this->pos;
				to_vec = to_vec.Normalize();
				//角度の増加量
				float increase = ML::ToRadian(this->angle_create_shot);
				//弾の生成
				for (int i = -this->num_shot / 2; i <= this->num_shot / 2; i++)
				{
					//角度の基準値に増加量を生成順に与える
					float ang = acos(to_vec.x) + increase * i;
					ML::Vec2 vec = ML::Vec2(cos(ang), sin(ang));
					this->shot->Create_Shot(this->pos, vec*SPEED_SHOT, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
				}
			}
			break;
		case Return_Under:
			//画面揺れ
			if (this->moveCnt == 0)
			{
				auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_boss_return_appear, this->limit_quake_boss_return_appear);
			}
			//頭が地面に潜るまで下に移動
			if (this->moveCnt<this->limit_move_vertically)
			{
				this->moveVec.y = +this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		case Stan:
			//一度だけ、弱点を示すハイライトエフェクトを生成する
			if (this->moveCnt == 0)
			{
				this->eff->Create_Effect(13, this->pos);
			}
			//気絶中はゆっくり横揺れする
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake_stan)*this->speed_shake_stn;
			break;
		case End_Pattern_Boss:
			if (this->moveCnt == 0)
			{
				//移動をやめる
				this->moveVec=ML::Vec2(0.0f,0.0f);
			}
			//一定間隔で爆発エフェクトを生成
			if (this->moveCnt % INTERVAL_CREATE_END_EFFECT_HEAD == 0)
			{
				//ランダムな生成位置を角度から指定
				float ang = float(rand() % 360);
				//自身の矩形の範囲内にランダムに生成
				float x = float(cos(ang)*float(rand() % this->hitBase.w / 2));
				float y = float(sin(ang)*float(rand() % this->hitBase.h / 2));
				this->eff->Create_Effect(3, this->pos + ML::Vec2(x, y));
			}
			break;
		}
	}
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
	void Object::Recieved(BChara* from_, AttackInfo at_, const int& un_hit_)
	{
		//気絶時以外はダメージを受けない
		if (this->state != Stan) { return; }
		//無敵時間中はダメージを受けない
		if (this->time_un_hit > 0)
		{
			return;
		}
		//撃破時はヒット表示等を行わない
		if (this->state == End_Pattern_Boss) { return; }
		if (this->state == Lose) { return; }
		this->hp -= at_.power;
		//無敵時間
		this->time_un_hit = un_hit_;
		//SEの再生
		DM::Sound_Play_Volume(this->res->name_sound_hit_reflect, false, VOLUME_SE_HIT_REFLECTION);
		//このタスクではDamageに遷移しない
		//ヒット表示は無敵時間の有無で管理する
	}
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		//デフォルトの値を宣言
		ML::Color dc(1, 1, 1, 1);
		;
		BChara::DrawInfo imageTable[] =
		{
			//draw		   src			 color
			{ this->hitBase,ML::Box2D(0, 368, 184, 290),dc },//default	[0]
			{ this->hitBase,ML::Box2D(0, 658, 184, 290),dc } //Lose	[1]
		};
		BChara::DrawInfo rtv;
		//アニメーション用変数

		switch (this->state)
		{
		default:
			rtv = imageTable[0];
			break;
		case Stand:
			rtv = imageTable[0];
			break;
		case Lose:
			rtv = imageTable[1];
			break;
		}
		//ヒット表示
		if (this->time_un_hit > 0)
		{
			rtv.src.y += rtv.src.h;
		}
		//左右の反転処理
		if (false == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
		/*return imageTable[int(this->state)];*/
	}
	//潜行から登場までの時間を減少させる
	//引数	：	（減少量）
	void Object::Decrease_Interval_To_Appear(const int& decrease_)
	{
		this->interval_to_appear -= decrease_;
	}
	//登場からショットに移行するまでの時間を減少させる
	//引数	：	（減少量）
	void Object::Decrease_Interval_Shot(const int& decrease_)
	{
		this->interval_shot -= decrease_;
	}
	//胴体パーツの破壊数を加算する
	void Object::Add_Defeat_Parts()
	{
		this->cnt_defeat_parts++;
	}
	//胴体パーツの破壊数を取得する
	int Object::Get_Defeat_Parts()
	{
		return this->cnt_defeat_parts;
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