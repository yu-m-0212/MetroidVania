//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Player.h"
#include	"Task_Sprite.h"
#include	"Task_Shot00.h"
#include	"Task_Barrier.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Goal.h"
#include	"Task_Retry.h"
#include	"Task_Corpse.h"
#include	"Task_Gun.h"
#include	"Task_Player_Head.h"
#include	"Task_Enemy01.h"
#include	"Task_Tutorials.h"
#include	"Task_Display_Effect.h"
#include	"Task_Item00.h"
#include	"Task_Item01.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/player.png");

		this->base_file_path_sound = "./data/sound/wav/";

		this->name_sound_landing = "sound_landing";
		DM::Sound_CreateSE(this->name_sound_landing, this->base_file_path_sound + "landing_player01.wav");

		this->name_sound_jump = "jump_sound";
		DM::Sound_CreateSE(this->name_sound_jump, this->base_file_path_sound + "jump_player01.wav");

		this->name_sound_shot = "shot_sound";
		DM::Sound_CreateSE(this->name_sound_shot, this->base_file_path_sound + "shot_player01.wav");
		
		this->name_not_recharge = "not_recharge";
		DM::Sound_CreateSE(this->name_not_recharge, this->base_file_path_sound + "not_recharge_barrier_player01.wav");

		this->name_damage_player = "damage_player";
		DM::Sound_CreateSE(this->name_damage_player, this->base_file_path_sound + "damage_player.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_sound_landing);
		DM::Sound_Erase(this->name_sound_jump);
		DM::Sound_Erase(this->name_sound_shot);
		DM::Sound_Erase(this->name_not_recharge);
		DM::Sound_Erase(this->name_damage_player);
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
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(-23, -46, 46, 92);
		this->recieveBase = this->hitBase;
		this->angle_LR = Right;
		this->controllerName = "P1";
		this->barrier = true;								//バリアの使用制限
		this->state = Stand;								//キャラ初期状態
		this->max_hp = 5;									//HP最大値
		this->hp = this->max_hp;							//HP初期値
		this->maxSpeed = 7.5f;								//最大移動速度（横）
		this->addSpeed = 0.75f;								//歩行加速度（地面の影響である程度打ち消される
		this->decSpeed = 0.5f;								//接地状態の時の速度減衰量（摩擦
		this->max_speed_fall = 15.0f;						//最大落下速度
		this->max_StompFallSpeed = 17.5f;					//ストンプの最大降下速度
		this->height_jump = -10.0f;							//ジャンプ力（初速）
		this->gravity = ML::Gravity(SIZE_CHIP);				//重力加速度＆時間速度による加算量
		this->init_shot = 48.0f;							//生成位置ショット
		this->speed_shot = 10;								//ショット速度
		this->limit_stompHoldTime = 30;						//ストンプ着地時の硬直時間
		this->limit_stomp = 15;								//ストンプ継続時間
		this->limit_quake = 15;								//画面揺れ時間
		this->limit_shot = 60;								//継続時間ショット
		this->limit_JumpAngleChange = 16;					//ジャンプから一定時間内なら向きを変えられる
		this->dist_quake = 5;								//画面揺れ幅
		this->lv_stomp = 1;									//ストンプアップグレードレベル
		this->interval_shot = 12;							//射撃の発射間隔（フレーム）
		this->range_stomp = ML::Box2D(-112, -112, 224, 224);//範囲ストンプ
		this->range_shot = ML::Box2D(-8, -8, 16, 16);		//範囲ショット
		this->power_stomp = 1;								//攻撃力ストンプ
		this->power_shot = 1;								//攻撃力ショット
		this->gauge_melee_max = 100;						//近接攻撃リチャージ上限
		this->gauge_melee = this->gauge_melee_max;			//近接攻撃のリチャージ
		this->moveBack_stomp = ML::Vec2(16, -6);			//ストンプふっとび量
		this->eff = new Task_Effect::Object();				//メソッド呼び出し

		//★タスクの生成
		auto gun = Gun::Object::Create(true);
		gun->pos = this->pos;
		auto plHead = Player_Head::Object::Create(true);
		plHead->pos = this->pos;

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		delete this->eff;
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//フェードアウトしてから、操作を始める
		auto display_effect =
			ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
		if (nullptr != display_effect) { return; }
		//クリア、敗北時
		if (ge->clear || ge->failure) { return; }
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//無敵時間の減少
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//近接攻撃のリチャージ
		if (this->barrier)
		{
			if (this->gauge_melee < 100)
			{
				this->gauge_melee++;
			}
		}
		//思考・状況判断
		this->Think();
		//現モーションに対応した制御
		this->Move();
		//めり込まない移動
		ML::Vec2  est = this->moveVec;
		this->CheckMove(est);
		//アイテムとの接触判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("アイテム");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//相手に接触の有無を確認させる
					if ((*it)->CheckHit(me)) {
						//相手にダメージの処理を行わせる
						BChara::AttackInfo at = { 0,0,0 };
						(*it)->Recieved(this, at,0);
						break;
					}
				}
			}
		}
		//遺体との接触判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("遺体");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//相手に接触の有無を確認させる
					if ((*it)->CheckHit(me)) {
						//相手にダメージの処理を行わせる
						BChara::AttackInfo at = { 0,0,0 };
						(*it)->Recieved(this, at,0);
						//一度だけエフェクトを生成する
						if (!(*it)->Get_Flag_Erase())
						{
							//回復エフェクトを生成
							eff->Create_Effect(4, this->pos);
							//フラグ反転
							(*it)->Set_Flag_Erase(1);
						}
						break;
					}
				}
			}
		}
		//カメラの位置を再調整
		{
			//自分を画面の何処に置くか（今回は画面中央）
			int  px = ge->camera2D.w / 2;
			int  py = ge->camera2D.h / 2;
			//自分を画面中央に置いた時のカメラの左上座標を求める
			int  cpx = int(this->pos.x) - px;
			int  cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//マップ外を写さないようにする調整処理
		auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
		if (nullptr != map) {
			map->AjastCameraPos();
		}
		//hp0でゲームオーバー
		if (this->hp <= 0)
		{
			//ゲームオーバーフラグ成立
			ge->failure = true;
		}
		//仮ゴールとの接触判定
		auto goal = ge->GetTask_One_G<Goal::Object>(Goal::defGroupName);
		if (nullptr != goal)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = goal->hitBase.OffsetCopy(goal->pos);
			if (you.Hit(me))
			{
				ge->clear = true;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//無敵中は8フレーム中4フレーム画像を表示しない（点滅する）
		if (this->time_un_hit > 0) {
			if ((this->time_un_hit / 4) % 2 == 0) {
				return;
			}
		}
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//他タスクの回転が干渉しないように修正
		DG::Image_Rotation(this->res->imageName,0.0f,ML::Vec2(float(this->hitBase.w/2),float(this->hitBase.h/2)));
		DG::Image_Draw(this->res->imageName, di.draw, di.src);
	}
	//-----------------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
	void Object::Recieved(BChara* from_, AttackInfo at_,const int& un_hit_)
	{
		//無敵時間中はダメージを受けない
		if (this->time_un_hit > 0) { return; }
		//SEの再生
		DM::Sound_Play_Volume(this->res->name_damage_player, false, VOLUME_SE_DAMAGE_PLAYER);
		//無敵時間の発生
		this->time_un_hit = un_hit_;
		//ダメージ処理
		if (!ge->debugMode)
		{
			this->hp -= at_.power;
		}
		//画面効果
		auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName,Map2D::defName);
		map->Set_Quake(this->dist_quake, this->limit_quake);
		//吹き飛ばされる
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+6.5f, -2.0f); }
		else							{ this->moveVec = ML::Vec2(-6.5f, -2.0f); }
		this->UpdateMotion(Damage);
		//from_は攻撃してきた相手、カウンターなどで逆にダメージを与えたいときに使う
	}
	//-----------------------------------------------------------------------------
	//思考＆状況判断　モーション決定
	void  Object::Think()
	{
		BChara::State  nm = this->state;	//とりあえず今の状態を指定
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//思考（入力）や状況に応じてモーションを変更する事を目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm) {
		case  Stand:	//立っている
			//押した瞬間は方向転換のみ行う
			if (in.LStick.L.on && this->moveCnt >= 6) { nm = Walk; }
			if (in.LStick.R.on && this->moveCnt >= 6) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (!this->CheckFoot()) { nm = Fall; }//足元 障害　無し
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			break;
		case SlowDown:	//減速中
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt >= 12) { nm = Stand; }
			break;
		case  TakeOff:	//飛び立ち
			if (this->moveCnt >= 3) { nm = Jump; }
			break;
		case  Jump:		//上昇中
			if (this->moveVec.y >= 0) { nm = Fall; }
			if (in.R1.on) { nm = Jumpshoot; }
			break;
		case  Fall:		//落下中
			if (this->CheckFoot() == true) { nm = Landing; }//足元　障害　有り
			if (in.R1.on) { nm = Fallshoot; }
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Landing:	//着地
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			if (this->moveCnt >= 6) { nm = Stand; }
			break;
		case  Shoot:	//射撃
			if (in.R1.off) {nm = Stand;}
			if (in.B2.down) { nm = TakeOff; }
			if (!this->CheckFoot()) { nm = Fallshoot; }
			break;
		case Jumpshoot:	//空中射撃
			if (in.R1.off) { nm = Fall; }
			if (this->moveVec.y >= 0) { nm = Fallshoot; }
			break;
		case Fallshoot:	//落下射撃
			if (in.R1.off) { nm = Fall; }
			if (this->CheckFoot()) { nm = Shoot; }
			break;
		case LandStomp:
			if (this->moveCnt > 0) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case AirStomp:
			if (this->moveCnt > 0) { nm = Fall; }
			break;
		case Damage:	//ダメージを受けて吹き飛んでいる
			if (this->moveCnt >= 12 &&
				this->CheckFoot()) {nm = Stand;}
			else if (this->moveCnt >= 12 &&
				!this->CheckFoot()){nm = Fall;}
			break;
		}
		//バリアの発生を割り込ませる
		if (in.B1.down)
		{
			//接地
			if (this->CheckFoot())
			{
				nm = LandStomp;
			}
			else
			{
				nm = AirStomp;
			}
		}
		//モーション更新
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//モーションに対応した処理
	//(モーションの変更はThinkで行うこと）
	void  Object::Move()
	{
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//重力加速
		switch (this->state) {
		default:
			//ジャンプボタンを押す長さでジャンプの高さが変わる
			if ((this->state == Jump ||
				this->state == Jumpshoot ||
				this->state == AirStomp)
				&& in.B2.on)
			{
				this->gravity = ML::Gravity(32) * 2;
			}
			else
			{
				this->gravity = ML::Gravity(32) * 5;
			}
			//上昇中もしくは足元に地面が無い
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) {
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
			}
			//地面に接触している
			else 
			{
				this->moveVec.y = 0.0f;
			}
			break;
			//重力加速を無効化する必要があるモーションは下にcaseを書く（現在対象無し）
		case Unnon:	
		case Damage:
			break;
		}
		//足場から落下してほしくない状態は以下に記述
		switch (this->state)
		{
		default:
			break;
		case SlowDown:
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		}
		//移動速度減衰
		switch (this->state) {
		default:
			if (this->moveVec.x < 0) {
				this->moveVec.x = min(this->moveVec.x + this->decSpeed, 0);
			}
			else {
				this->moveVec.x = max(this->moveVec.x - this->decSpeed, 0);
			}
			break;
			//移動速度減衰を無効化する必要があるモーションは下にcaseを書く
		case Damage:
		case Unnon:
			break;
		}
		//右スティックのX軸の値によって方向を変える
		switch (this->state)
		{
		default:
			if (in.RStick.axis.x < 0.0f)
			{
				this->angle_LR = Left;
			}
			else if (in.RStick.axis.x > 0.0f)
			{
				this->angle_LR = Right;
			}
			break;
		//例外
		case SlowDown:
		case Landing:
		case LandStomp:
		case AirStomp:
		case Damage:
			break;
		}
		//-----------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state) 
		{
		case  Stand:	//立っている
			//方向だけ変える
			if (in.LStick.L.down)
			{
				this->moveCnt = 0;
				this->angle_LR = Left;
			}
			if (in.LStick.R.down)
			{
				this->moveCnt = 0;
				this->angle_LR = Right;
			}
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.on)
			{
				//右スティックが倒されているときは歩き始めた瞬間のみ、向きを変える
				if (in.RStick.axis != ML::Vec2(0.0f, 0.0f))
				{
					if (this->moveCnt == 0)
					{
						this->angle_LR = Left;
					}
				}
				else
				{
					this->angle_LR = Left;
				}
				//後退もできるが移動速度が落ちる
				if(this->angle_LR == Right)
				{
					this->moveVec.x = -this->maxSpeed / 2.0f;
				}
				else
				{
					this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
				}
			}
			if (in.LStick.R.on)
			{
				if (in.RStick.axis != ML::Vec2(0.0f, 0.0f))
				{
					if (this->moveCnt == 0)
					{
						this->angle_LR = Right;
					}
				}
				else
				{
					this->angle_LR = Right;
				}
				if(this->angle_LR == Left)
				{
					this->moveVec.x = +this->maxSpeed / 2.0f;
				}
				else
				{
					this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
				}
			}
			break;
		case SlowDown:
			break;
		case  Jump:		//上昇中
			if (this->moveCnt == 0)
			{
				//初速設定
				this->moveVec.y = this->height_jump;
				//SE再生
				DM::Sound_Play_Volume(this->res->name_sound_jump,false,VOLUME_ALL_GAME);
			}
			//向きの変更は倒した瞬間
			if (this->moveCnt<this->limit_JumpAngleChange&&in.LStick.L.down)
			{
				this->angle_LR = Left;
			}
			if (this->moveCnt<this->limit_JumpAngleChange && in.LStick.R.down)
			{
				this->angle_LR = Right;
			}
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) 
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//天井に衝突で上昇力を0に
			if (this->CheckHead() == true) { this->moveVec.y = 0; }
			break;
		case  Fall:		//落下中
			if (in.LStick.L.on) 
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			break;
		case Landing:
			//SE再生
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_landing, false,VOLUME_SE_LADING_PLAYER);
			}
			break;
		case LandStomp:
		case AirStomp:
			if (this->moveCnt == 0)
			{
				//リチャージ済み
				if (this->gauge_melee == this->gauge_melee_max)
				{
					this->Stomp_Std();
					this->gauge_melee = 0;
				}
				//未リチャージ
				else
				{
					DM::Sound_Play_Volume(this->res->name_not_recharge, false,VOLUME_ALL_GAME);
				}
			}
			//歩行関連
			//左向きに移動
			if (in.LStick.L.on)
			{
				//後退時は移動速度が落ちる
				if (this->angle_LR == Right)
				{
					this->moveVec.x = -this->maxSpeed / 2.0f;
				}
				//前進は通常速度
				else
				{
					this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
				}
			}
			//右向きに移動
			if (in.LStick.R.on)
			{
				//後退
				if (this->angle_LR == Left)
				{
					this->moveVec.x = +this->maxSpeed / 2.0f;
				}
				//前進
				else
				{
					this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
				}
			}
			break;
		case  Shoot:
			this->Move_Shot();
			this->Shot_Appear();
			break;
		case Jumpshoot:
			this->Move_Shot();
			this->Shot_Appear();
			if (this->CheckHead() == true) { this->moveVec.y = 0; }	
			break;
		case Fallshoot:
			this->Move_Shot();
			this->Shot_Appear();
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo  Object::Anim()
	{
		//デフォルトの値を用意
		int wide = 138, height = 92;
		ML::Box2D dd(-wide / 2, -height / 2, wide, height);
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw			src
			{ dd, ML::Box2D(wide *  0,height * 0,wide,height),dc },	//停止1				[0]
			{ dd, ML::Box2D(wide *  0,height * 1,wide,height),dc },	//停止2				[1]
			{ dd, ML::Box2D(wide *  0,height * 2,wide,height),dc },	//停止3				[2]
			{ dd, ML::Box2D(wide *  0,height * 3,wide,height),dc },	//停止4				[3]
			{ dd, ML::Box2D(wide *  1,height * 0,wide,height),dc },	//歩行1				[4]
			{ dd, ML::Box2D(wide *  1,height * 1,wide,height),dc },	//歩行2				[5]
			{ dd, ML::Box2D(wide *  1,height * 2,wide,height),dc },	//歩行3				[6]
			{ dd, ML::Box2D(wide *  1,height * 3,wide,height),dc },	//歩行4				[7]
			{ dd, ML::Box2D(wide *  2,height * 0,wide,height),dc },	//減速				[8]
			{ dd, ML::Box2D(wide *  3,height * 0,wide,height),dc },	//離陸				[9]
			{ dd, ML::Box2D(wide *  4,height * 0,wide,height),dc },	//ジャンプ			[10]
			{ dd, ML::Box2D(wide *  5,height * 0,wide,height),dc },	//降下				[11]
			{ dd, ML::Box2D(wide *  6,height * 0,wide,height),dc },	//着地				[12]
			{ dd, ML::Box2D(wide *  7,height * 0,wide,height),dc },	//ショット1			[13]
			{ dd, ML::Box2D(wide *  7,height * 1,wide,height),dc },	//ショット2			[14]
			{ dd, ML::Box2D(wide *  7,height * 2,wide,height),dc },	//ショット3			[15]
			{ dd, ML::Box2D(wide *  7,height * 3,wide,height),dc },	//ショット4			[16]
			{ dd, ML::Box2D(wide *  8,height * 0,wide,height),dc },	//ジャンプショット	[17]
			{ dd, ML::Box2D(wide *  9,height * 0,wide,height),dc },	//降下ショット		[18]
			{ dd, ML::Box2D(wide * 10,height * 0,wide,height),dc },	//ストンプ予備動作	[19]
			{ dd, ML::Box2D(wide * 11,height * 0,wide,height),dc },	//地上ストンプ		[20]
			{ dd, ML::Box2D(wide * 12,height * 0,wide,height),dc },	//空中ストンプ		[21]
			{ dd, ML::Box2D(wide * 13,height * 0,wide,height),dc },	//ストンプ着地		[22]
			{ dd, ML::Box2D(wide * 14,height * 0,wide,height),dc },	//ダメージ			[23]
		};
		BChara::DrawInfo  rtv;
		int  walkAnim,shootAnim;
		switch (this->state) {
		default:			rtv = imageTable[0];	break;
		//	停止----------------------------------------------------------------------------
		case  Stand:		rtv = imageTable[0];	break;
		//	歩行----------------------------------------------------------------------------
		case  Walk:
			walkAnim = this->animCnt / 8;
			walkAnim %= 4;
			rtv = imageTable[walkAnim + 4];
			break;
		//	減速----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[8];	break;
		//	離陸----------------------------------------------------------------------------
		case TakeOff:		rtv = imageTable[9];	break;
		//	ジャンプ------------------------------------------------------------------------
		case Jump:			rtv = imageTable[10];	break;
			//	降下----------------------------------------------------------------------------
		case Fall:			rtv = imageTable[11];	break;
			//	着地----------------------------------------------------------------------------
		case Landing:		rtv = imageTable[12];	break;
			//	ショット------------------------------------------------------------------------
		case Shoot:
			//ショットは歩いている時のみ、アニメーションする
			if (this->moveVec.x != 0.0f)
			{
				shootAnim = this->animCnt / 8;
				shootAnim %= 4;
				rtv = imageTable[shootAnim + 13];
			}
			else
			{
				rtv = imageTable[13];
			}
			break;
		//	ジャンプショット-----------------------------------------------------------------
		case Jumpshoot:		rtv = imageTable[17];	break;
		//	降下ショット--------------------------------------------------------------------
		case Fallshoot:		rtv = imageTable[18];	break;
		//	地上ストンプ--------------------------------------------------------------------
		case LandStomp:		rtv = imageTable[20];	break;
		//	空中ストンプ--------------------------------------------------------------------
		case AirStomp:		rtv = imageTable[21];	break;
		//	ダメージ------------------------------------------------------------------------
		case Damage:		rtv = imageTable[23];	break;
		}
		//	向きに応じて画像を左右反転する
		if (false == this->angle_LR) {
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
			////	表示位置の逆転
			//rtv.draw.x = -(rtv.draw.x + rtv.draw.w);
			////	画像位置の逆転
			//rtv.src.x = (rtv.src.x + rtv.src.w);
			//rtv.src.w = -rtv.src.w;
		}
		return rtv;
	}
	//ショット生成スタンダード
	void Object::Shot_Appear()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		float angle=atan2(in.RStick.axis.y,in.RStick.axis.x);
		//一定間隔で弾を生成
		if (this->moveCnt % this->interval_shot == 0)
		{
			auto shot = Shot00::Object::Create(true);
			shot->state = Shoot;
			//攻撃毎に攻撃範囲を生成時に指定
			shot->hitBase = this->range_shot;
			shot->Set_Limit(this->limit_shot);
			shot->Set_Power(this->power_shot);
			shot->angle_LR = this->angle_LR;
			shot->Set_Tip(1);
			//右スティックが入力されていなければ一定の軌道を描く
			if (in.RStick.axis == ML::Vec2(0.0f, 0.0f))
			{
				shot->Set_Angle(0.0f);
				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->init_shot, this->pos.y);
					shot->Set_Angle(0.0f);
					shot->moveVec = ML::Vec2(+this->speed_shot, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->init_shot, this->pos.y);
					shot->Set_Angle(135.0f);
					shot->moveVec = ML::Vec2(-this->speed_shot, 0);
				}
			}
			//スティックの入力方向へ発射
			else
			{
				shot->pos = this->pos + ML::Vec2(cos(angle),sin(angle)) * this->init_shot;
				shot->moveVec = ML::Vec2(cos(angle), sin(angle)) * this->speed_shot;
				shot->Set_Angle(angle);
			}
			//SE再生
			DM::Sound_Play_Volume(this->res->name_sound_shot,false,VOLUME_SE_SHOT);
		}
	}
	//行動ショット中
	void Object::Move_Shot()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//発射中は移動速度が落ちる
		if (in.LStick.L.on)
		{
			this->moveVec.x = -this->maxSpeed / 2.0f;
		}
		else if (in.LStick.R.on)
		{
			this->moveVec.x = +this->maxSpeed / 2.0f;
		}
		//発射方向によって向きを変える
		if (in.RStick.axis.x > 0)
		{
			this->angle_LR = Right;
		}
		else if (in.RStick.axis.x<0)
		{
			this->angle_LR = Left;
		}
	}
	//ストンプスタンダード
	void Object::Stomp_Std()
	{
		//足元に攻撃矩形を生成
		auto barrier = Barrier::Object::Create(true);
		barrier->state = this->state;
		//攻撃毎に攻撃範囲を生成時に指定
		barrier->hitBase = this->range_stomp;
		barrier->recieveBase = this->range_stomp;
		barrier->pos = this->pos;
		barrier->Set_Limit(this->limit_stomp);
		barrier->Set_Power(this->power_stomp);
		barrier->Set_Angle(0.0f);
		//範囲攻撃のふっとび量xは+の値で指定する（符号反転は当たった際に行う）
		barrier->Set_Move_Back(this->moveBack_stomp);
		//範囲攻撃であることを知らせるフラグをtrue
		barrier->Set_Range_Wide(1);
		//画面を揺らすための設定を行う
		auto map = ge->GetTask_One_G<Map2D::Object>(Map2D::defGroupName);
		map->Set_Quake(this->dist_quake, this->limit_quake);
		//エフェクトの生成
		this->eff->Create_Effect(2, this->pos);
	}
	//バリアの取得状況を取得する
	bool Object::Get_Barrier()
	{
		return this->barrier;
	}
	//ショットの発射間隔を取得する
	int Object::Get_Interval_Shot()
	{
		return this->interval_shot;
	}
	//HPの値を取得する
	int Object::Get_HP()
	{
		return this->hp;
	}
	//HPの最大値を取得する
	int Object::Get_Max_HP()
	{
		return this->max_hp;
	}
	//状態を取得する
	int Object::Get_State()
	{
		return this->state;
	}
	//近接攻撃リチャージの値を取得する
	int Object::Get_Gauge_Mlee()
	{
		return this->gauge_melee;
	}
	//近接攻撃リチャージ上限の値を取得する
	int Object::Get_Gauge_Melle_Max()
	{
		return this->gauge_melee_max;
	}
	//HPの最大値を指定する
	//引数	：	（int）
	void Object::Set_Max_HP(const int& max_)
	{
		this->max_hp = max_;
	}
	//バリアの取得状況を指定する
	//引数	：	（bool）
	void Object::Set_Barrier(const bool& flag_)
	{
		this->barrier = flag_;
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