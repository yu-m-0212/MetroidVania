//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Player.h"
#include	"Task_Sprite.h"
#include	"Task_Shot00.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Goal.h"
#include	"Task_Retry.h"
#include	"Task_Corpse.h"
#include	"Task_Gun.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/chara(仮)01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->state = Stand;								//キャラ初期状態
		this->max_Hp = 10;									//HP最大値
		this->hp = this->max_Hp;							//HP初期値
		this->maxSpeed = 7.5f;								//最大移動速度（横）
		this->addSpeed = 0.75f;								//歩行加速度（地面の影響である程度打ち消される
		this->decSpeed = 0.5f;								//接地状態の時の速度減衰量（摩擦
		this->maxFallSpeed = 15.0f;							//最大落下速度
		this->max_StompFallSpeed = 17.5f;					//ストンプの最大降下速度
		this->height_Jump = -10.0f;							//ジャンプ力（初速）
		this->gravity = ML::Gravity(32);					//重力加速度＆時間速度による加算量
		this->reach = 32.0;									//射程
		this->speed_Shot = 10;								//ショット速度
		this->limit_StompHoldTime = 30;						//ストンプ着地時の硬直時間
		this->limit_Stomp = 15;								//ストンプ継続時間
		this->limit_StompEffect = 18;						//継続時間ストンプ効果
		this->limit_Quake = 15;								//画面揺れ時間
		this->limit_Shot = 40;								//継続時間ショット
		this->limit_JumpAngleChange = 24;					//ジャンプから一定時間内なら向きを変えられる
		this->limit_HealEffect = 24;						//継続時間回復エフェクト
		this->dist_Quake = 5;								//画面揺れ幅
		this->lv_Stomp = 1;									//ストンプアップグレードレベル
		this->addUnHitTime = 30;							//被弾時に得られる無敵時間
		this->interval_Shot = 12;							//射撃の発射間隔（フレーム）
		this->range_Stomp = ML::Box2D(-96, -24, 192, 46);	//範囲ストンプ
		this->range_Shot = ML::Box2D(-8, -8, 16, 16);		//範囲ショット
		this->power_Stomp = 3;								//攻撃力ストンプ
		this->power_Shot = 1;								//攻撃力ショット
		this->moveBack_Stomp = ML::Vec2(12, -4);			//ストンプふっとび量

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
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
		//クリア、敗北時
		if (ge->clear || ge->failure) { return; }
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//無敵時間の減少
		if (this->unHitTime > 0) { this->unHitTime--; }
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
						(*it)->Received(this, at);
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
						BChara::AttackInfo at = { 1,0,0 };
						(*it)->Received(this, at);
						//回復エフェクトを生成
						auto healEffect = Effect::Object::Create(true);
						healEffect->pos = this->pos;
						healEffect->Set_Limit(this->limit_HealEffect);
						healEffect->state = Heal;
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
		auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
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
		auto goal = ge->GetTask_One_G<Goal::Object>("ゴール");
		if (nullptr != goal)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = goal->hitBase.OffsetCopy(goal->pos);
			if (you.Hit(me))
			{
				ge->clear = true;
			}
		}
		auto in = DI::GPad_GetState(this->controllerName);
		if (in.R3.on&&in.L3.on)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//無敵中は8フレーム中4フレーム画像を表示しない（点滅する）
		if (this->unHitTime > 0) {
			if ((this->unHitTime / 4) % 2 == 0) {
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
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		if (this->unHitTime > 0) {
			return;//無敵時間中はダメージを受けない
		}
		this->unHitTime = this->addUnHitTime;
		this->hp -= at_.power;	//仮処理
		//吹き飛ばされる
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+2.0f, -4.5f); }
		else							{ this->moveVec = ML::Vec2(-2.0f, -4.5f); }
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
			if (in.B1.down) { nm = PreStomp; }
			if (!this->CheckFoot()) { nm = Fall; }//足元 障害　無し
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (in.B1.down) { nm = PreStomp; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			break;
		case SlowDown:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.B1.down) { nm = PreStomp; }
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
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Fall:		//落下中1
			if (this->CheckFoot() == true) { nm = Landing; }//足元　障害　有り
			if (in.R1.on) { nm = Fallshoot; }
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Landing:	//着地
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = Jump; }
			if (in.B1.down) { nm = PreStomp; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			if (this->moveCnt >= 6) { nm = Stand; }
			break;
		case  Shoot:
			if (in.R1.off) {nm = Stand;}
			if (in.B2.down) { nm = TakeOff; }
			if (!this->CheckFoot()) { nm = Fallshoot; }
			break;
		case Jumpshoot:
			if (in.R1.off) { nm = Fall; }
			if (this->moveVec.y >= 0) { nm = Fallshoot; }
			break;
		case Fallshoot:
			if (in.R1.off) { nm = Fall; }
			if (this->CheckFoot()) { nm = Shoot; }
			break;
		case PreStomp:
			if (this->moveCnt > 24) { nm = LandStomp; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case LandStomp:
			if (this->moveCnt > 12) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case AirStomp:
			if (this->CheckFoot()) { nm = StompLanding; }
			if (!this->CheckFoot() && this->moveCnt >= 60) { nm = Fall; }
			break;
		case StompLanding:
			if (this->moveCnt >= this->limit_StompHoldTime) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case	Damage:	//ダメージを受けて吹き飛んでいる
			if (this->moveCnt >= 12 && this->CheckFoot()) 
			{
				nm = Stand;
			}
			else if (this->moveCnt >= 12 && !this->CheckFoot())
			{
				nm = Fall;
			}
			break;
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
			if (in.B2.on)
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
				this->moveVec.y=min(this->moveVec.y + this->gravity, this->maxFallSpeed);
			}
			//地面に接触している
			else 
			{
				this->moveVec.y = 0.0f;
			}
			break;
			//重力加速を無効化する必要があるモーションは下にcaseを書く（現在対象無し）
		case Unnon:	break;
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
			//右スティックによって向きを変える
			if (in.RStick.axis.x > 0)
			{
				this->angle_LR = Right;
			}
			else if (in.RStick.axis.x<0)
			{
				this->angle_LR = Left;
			}
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case SlowDown:
			//目の前に足場がなければ前進をやめる(足場から落下しない)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			//右スティックによって向きを変える
			if (in.RStick.axis.x > 0)
			{
				this->angle_LR = Right;
			}
			else if (in.RStick.axis.x<0)
			{
				this->angle_LR = Left;
			}
			break;
		case  Jump:		//上昇中
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->height_Jump;//初速設定
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
			//移動量減衰によって滑ると足場の移動が困難なので
			//左右の移動量を0に
			if (this->moveCnt == 0)
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case PreStomp:
			break;
		case LandStomp:
			if (this->moveCnt == 0)
			{
				this->Stomp_Std();
			}
			break;
		case AirStomp:
			//出だしだけ少し浮かせる
			if (this->moveCnt == 0)
			{
				//無敵時間あり
				this->unHitTime = 60;
			}
			else if (this->moveCnt <= 10)
			{
				this->moveVec.y = -0.5f;
			}
			else if (this->moveCnt > 20)
			{
				if (this->moveVec.y <= this->maxFallSpeed)
				{
					this->moveVec.y += 1.5f;
				}
			}
			break;
		case StompLanding:
			//着地の際、自身の左中右に攻撃
			if (this->moveCnt == 0)
			{
				this->Stomp_Std();
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
	//HPの値を取得する
	int Object::Get_HP()
	{
		return this->hp;
	}
	//HPの最大値を取得する
	int Object::Get_Max_HP()
	{
		return this->max_Hp;
	}
	//状態を取得する
	int Object::Get_State()
	{
		return this->state;
	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo  Object::Anim()
	{
		//デフォルトカラーを宣言
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(  0,  0, 48, 96), dc },//停止1			[0]
			{ this->hitBase, ML::Box2D(  0, 96, 48, 96), dc },//停止2			[1]
			{ this->hitBase, ML::Box2D(  0,192, 48, 96), dc },//停止3			[2]
			{ this->hitBase, ML::Box2D(  0,288, 48, 96), dc },//停止4			[3]
			{ this->hitBase, ML::Box2D(  0,384, 48, 96), dc },//停止5			[4]
			{ this->hitBase, ML::Box2D( 48,  0, 48, 96), dc },//歩行1			[5]
			{ this->hitBase, ML::Box2D( 48, 96, 48, 96), dc },//歩行2			[6]
			{ this->hitBase, ML::Box2D( 48,192, 48, 96), dc },//歩行3			[7]
			{ this->hitBase, ML::Box2D(96,   0, 48, 96), dc },//減速				[8]
			{ this->hitBase, ML::Box2D(144,  0, 48, 96), dc },//パンチ1			[9]
			{ this->hitBase, ML::Box2D(192,  0, 48, 96), dc },//パンチ2			[10]
			{ this->hitBase, ML::Box2D(240,  0, 48, 96), dc },//空中攻撃			[11]
			{ this->hitBase, ML::Box2D(288,  0, 48, 96), dc },//ストンプ			[12]
			{ this->hitBase, ML::Box2D(336,  0, 48, 96), dc },//ストンプ着地		[13]
			{ this->hitBase, ML::Box2D(384,  0, 48, 96), dc },//射撃				[14]
			{ this->hitBase, ML::Box2D(432,  0, 48, 96), dc },//空中射撃			[15]
			{ this->hitBase, ML::Box2D(480,  0, 48, 96), dc },//バンカー1		[16]
			{ this->hitBase, ML::Box2D(528,  0, 48, 96), dc },//バンカー2		[17]
			{ this->hitBase, ML::Box2D(576,  0, 48, 96), dc },//バンカー3		[18]
			{ this->hitBase, ML::Box2D(624,  0, 48, 96), dc },//ジャンプ			[19]
			{ this->hitBase, ML::Box2D(672,  0, 48, 96), dc },//落下				[20]
			{ this->hitBase, ML::Box2D(720,  0, 48, 96), dc },//着地				[21]
			{ this->hitBase, ML::Box2D(768,  0, 48, 96), dc },//ダメージ			[22]
		};
		BChara::DrawInfo  rtv;
		int  standAnim,walkAnim;
		switch (this->state) {
		default:			rtv = imageTable[0];	break;
		//	停止----------------------------------------------------------------------------
		case  Stand:
			//切り替わるフレーム数
			standAnim = this->animCnt / 12;
			//パターン数
			standAnim %= 5;
			//出た値に一枚目の要素番号を足す
			rtv = imageTable[standAnim + 0];
			break;
		//	歩行----------------------------------------------------------------------------
		case  Walk:
			walkAnim = this->animCnt / 8;
			walkAnim %= 3;
			rtv = imageTable[walkAnim + 5];
			break;
		//	減速----------------------------------------------------------------------------
		case SlowDown:
		case PreStomp:
			rtv = imageTable[8];
			break;
		//	空中攻撃-------------------------------------------------------------------------
		case Air:			rtv = imageTable[11];	break;
		//	ストンプ-------------------------------------------------------------------------
		case AirStomp:			rtv = imageTable[12];	break;
		//	ストンプ着地---------------------------------------------------------------------
		case StompLanding:
		case LandStomp:
			rtv = imageTable[13];
			break;
		//	地上射撃-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[14];	break;
		//	空中射撃-------------------------------------------------------------------------
		case Jumpshoot:
		case Fallshoot:		rtv = imageTable[15];	break;
		//	ジャンプ------------------------------------------------------------------------
		case  Jump:			rtv = imageTable[19];	break;
		//	落下----------------------------------------------------------------------------
		case  Fall:			rtv = imageTable[20];	break;
		//着地硬直--------------------------------------------------------------------------
		case  Landing:		rtv = imageTable[21];	break;
		//ダメージ--------------------------------------------------------------------------
		case  Damage:		rtv = imageTable[22];	break;
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
		if (this->moveCnt % this->interval_Shot == 0)
		{
			auto shot = Shot00::Object::Create(true);
			shot->state = Shoot;
			//攻撃毎に攻撃範囲を生成時に指定
			shot->hitBase = this->range_Shot;
			shot->Set_Limit(this->limit_Shot);
			shot->Set_Erase(1);
			shot->Set_Power(this->power_Shot);
			shot->angle_LR = this->angle_LR;
			shot->tip = true;
			//右スティックが入力されていなければ一定の軌道を描く
			if (in.RStick.axis == ML::Vec2(0.0f, 0.0f))
			{
				shot->Set_Angle(0.0f);
				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(+this->speed_Shot, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(-this->speed_Shot, 0);
				}
			}
			//スティックの入力方向へ発射
			else
			{
				shot->pos = this->pos + ML::Vec2(cos(angle),sin(angle)) * this->reach;
				shot->moveVec = ML::Vec2(cos(angle), sin(angle)) * this->speed_Shot;
				shot->Set_Angle(angle);
			}
		}
	}
	//行動ショット中
	void Object::Move_Shot()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//射撃中は後退できるが移動速度が落ちる
		if (in.LStick.L.on && this->angle_LR == Right)
		{
			this->moveVec.x = -this->maxSpeed / 2.0f;
		}
		else if (in.LStick.R.on && this->angle_LR == Left)
		{
			this->moveVec.x = +this->maxSpeed / 2.0f;
		}
		if (in.LStick.L.on&&this->angle_LR == Left)
		{
			this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
		}
		else if (in.LStick.R.on&&this->angle_LR == Right)
		{
			this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
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
		auto stompLandingRect = Shot00::Object::Create(true);
		stompLandingRect->state = StompLanding;
		//攻撃毎に攻撃範囲を生成時に指定
		stompLandingRect->hitBase = this->range_Stomp;
		stompLandingRect->pos = ML::Vec2(this->pos.x, this->pos.y + this->hitBase.h / 4);
		stompLandingRect->Set_Limit(this->limit_Stomp);
		stompLandingRect->Set_Erase(0);
		stompLandingRect->Set_Power(this->power_Stomp);
		stompLandingRect->Set_Angle(0.0f);
		//範囲攻撃のふっとび量xは+の値で指定する（符号反転は当たった際に行う）
		stompLandingRect->moveBack = this->moveBack_Stomp;
		//範囲攻撃であることを知らせるフラグをtrue
		stompLandingRect->wideRange = true;
		//画面を揺らすための設定を行う
		auto map = ge->GetTask_One_G<Map2D::Object>("フィールド");
		map->Set_Dist_Quake(this->dist_Quake);
		map->Set_Limit_Quake(this->limit_Quake);
		//エフェクトの生成
		//タスクキルはエフェクト側で行う
		auto stompLandingEffect = Effect::Object::Create(true);
		stompLandingEffect->pos = this->pos;
		stompLandingEffect->Set_Limit(this->limit_StompEffect);
		stompLandingEffect->state = StompLanding;
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