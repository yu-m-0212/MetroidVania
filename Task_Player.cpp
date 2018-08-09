//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Sprite.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/chara(仮)01.png");
		DG::Font_Create("FontA", "ＭＳ ゴシック", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontA");
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
		this->hitBase = ML::Box2D(-32, -64, 64, 128);
		this->angle_LR = Right;
		this->controllerName = "P1";
		this->state = Stand;				//キャラ初期状態
		this->max_Hp = 10;					//ヘルス最大値
		this->hp = this->max_Hp;			//ヘルス初期値
		this->maxSpeed = 10.0f;				//最大移動速度（横）
		this->addSpeed = 1.0f;				//歩行加速度（地面の影響である程度打ち消される
		this->decSpeed = 0.5f;				//接地状態の時の速度減衰量（摩擦
		this->maxFallSpeed = 15.0f;			//最大落下速度
		this->jumpPow = -15.0f;				//ジャンプ力（初速）
		this->gravity = ML::Gravity(64);	//重力加速度＆時間速度による加算量
		this->reach = 64.0;					//パンチの射程
		this->slide = 10.0f;				//パンチで前進する距離
		this->meleeCnt = 15;				//格闘攻撃判定継続時間
		this->shotSpeed = 20;				//ショット速度
		this->stompHoldTime = 30;			//ストンプ着地時の硬直時間

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

		//当たり判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("アイテム");
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
		//カメラの位置を再調整
		{
			//自分を画面の何処に置くか（今回は画面中央）
			int  px = ge->camera2D.w / 2;
			int  py = ge->camera2D.h / 2;
			//自分を画面中央に置いた時のカメラの左上座標を求める
			int  cpx = int(this->pos.x) - px;
			int  cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			//揺れ表現がある場合はここで加算する
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//マップ外を写さないようにする調整処理
		auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
		if (nullptr != map) {
			map->AjastCameraPos();
		}
		//hp0でタスクキル
		if (this->hp <= 0)
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
		DG::Image_Draw(this->res->imageName, di.draw, di.src);
	}
	//-----------------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		if (this->unHitTime > 0) {
			return;//無敵時間中はダメージを受けない
		}
		this->unHitTime = 180;
		this->hp -= at_.power;	//仮処理
		if (this->hp <= 0)
		{
			this->Kill();
		}
		//吹き飛ばされる
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+4, -9); }
		else							{ this->moveVec = ML::Vec2(-4, -9); }
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
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (in.R2.down) { nm = Bunker1; }
			if (!this->CheckFoot()) { nm = Fall; }//足元 障害　無し
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (in.R2.down) { nm = Bunker1; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			break;
		case SlowDown:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.down) { nm = Punch1; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R2.down) { nm = Bunker1; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt >= 12) { nm = Stand; }
			break;
		case  TakeOff:	//飛び立ち
			if (this->moveCnt >= 3) { nm = Jump; }
			break;
		case  Jump:		//上昇中
			if (this->moveVec.y >= 0) { nm = Fall; }
			if (in.R1.down) { nm = Air; }//空中攻撃
			if (in.L1.down) { nm = Airshoot; }
			if (in.R2.down) { nm = Stomp; }
			break;
		case  Fall:		//落下中1
			if (this->CheckFoot() == true) { nm = Landing; }//足元　障害　有り
			if (in.R1.down) { nm = Air; }//空中攻撃
			if (in.L1.down) { nm = Airshoot; }
			if (in.R2.down) { nm = Stomp; }
			break;
		case  Landing:	//着地
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = Jump; }
			if (this->CheckFoot() == false) { nm = Fall; }//足元 障害　無し
			if (this->moveCnt >= 3) { nm = Stand; }
			break;
		case Punch1:
			if (this->moveCnt >= 20) { nm = Stand; }
			if (in.B2.down) { nm = Jump; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (in.R1.down) { nm = Punch2; }
			break;
		case Punch2:
			if (this->moveCnt >= 20) { nm = Stand; }
			if (in.B2.down) { nm = Jump; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case  Shoot:
			if (this->moveCnt >= 8) {nm = Stand;}
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Air:
			if (this->moveCnt >= 8 && this->moveVec.y >= 0) { nm = Fall; }
			break;
		case Airshoot:
			if (this->moveCnt >= 8 && this->moveVec.y >= 0) { nm = Fall; }
			break;
		case Stomp:
			if (this->CheckFoot()) { nm = StompLanding; }
			if (!this->CheckFoot() && this->moveCnt >= 30) { nm = Fall; }
			break;
		case StompLanding:
			if (this->moveCnt >= this->stompHoldTime) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Bunker1:
			if (this->moveCnt >= 8 && in.R2.off) { nm = Bunker2; }
			if (in.B2.down) { nm = Jump; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt < 30 && in.R2.off) { nm = Stand; }
			break;
		case Bunker2:
			if (this->moveCnt >= 20) { nm = Bunker3; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Bunker3:
			if (this->moveCnt >= 20) { nm = Stand; }
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
	//	モーションに対応した処理
	//(モーションの変更はThinkで行うこと）
	void  Object::Move()
	{
		auto cam = ge->GetTask_One_G<Sprite::Object>("オプション");
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//重力加速
		switch (this->state) {
		default:
			//ジャンプボタンを押す長さでジャンプの高さが変わる
			if (in.B2.on)
			{
				this->gravity = ML::Gravity(32) * 4;
			}
			else
			{
				this->gravity = ML::Gravity(32) * 10;
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
			//未実装
			break;
			//移動速度減衰を無効化する必要があるモーションは下にcaseを書く
		case Damage: break;
		case Unnon:	break;
		}
		//-----------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state) 
		{
		case  Stand:	//立っている
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
			break;
		case  Jump:		//上昇中
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumpPow;//初速設定
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
		case Punch1:
			//目の前にパンチ矩形を生成
			if (this->moveCnt == 0) 
			{
				auto punch1 = Shot00::Object::Create(true);
				//呼び出した判定矩形に思考させるため状態を指定
				punch1->state = Punch1;
				//攻撃毎に攻撃範囲を生成時に指定
				punch1->hitBase = ML::Box2D(-64, -32, 128, 64);
				punch1->Set_Limit(this->meleeCnt);
				punch1->Set_Erase(0);
				//エフェクトの呼び出し
				auto punch1Effect = Effect::Object::Create(true);
				punch1Effect->state = Punch1;
				punch1Effect->Set_Limit(18);
				punch1Effect->pos = this->pos;
				//以下プレイヤの左右によって変化する
				if (this->angle_LR == Right)
				{
					//初期座標をプレイヤの目の前に指定
					punch1->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					//攻撃時に前進する
					this->moveVec.x = this->slide;
				}
				else 
				{
					//初期座標をプレイヤの目の前に指定
					punch1->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					//攻撃時に前進する
					this->moveVec.x = -this->slide;
				}
			}
			//目の前に足場がなければ前進をやめる(足場から落下しない)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Punch2:
			//目の前にパンチ矩形を生成
			if (this->moveCnt == 0) {
				auto punch2 = Shot00::Object::Create(true);
				//呼び出した判定矩形に思考させるため状態を指定
				punch2->state = Punch2;
				//攻撃毎に攻撃範囲を生成時に指定
				punch2->hitBase = ML::Box2D(-64, -32, 128, 64);
				punch2->Set_Limit(this->meleeCnt);
				punch2->Set_Erase(0);
				//以下プレイヤの左右によって変化する
				if (this->angle_LR == Right)
				{
					//初期座標をプレイヤの目の前に指定
					punch2->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					//攻撃時に前進する
					this->moveVec.x = this->slide;
				}
				else
				{
					//初期座標をプレイヤの目の前に指定
					punch2->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					//攻撃時に前進する
					this->moveVec.x = -this->slide;
				}
			}
			//目の前に足場がなければ前進をやめる(足場から落下しない)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Stomp:
			//出だしだけ少し浮かせる
			if (this->moveCnt == 0)
			{
				stompFallSpeed = -3.5f;
				//無敵時間あり
				this->unHitTime = 60;
			}
			this->stompFallSpeed+=0.5f;
			this->moveVec.y += stompFallSpeed;
			break;
		case StompLanding:
			//着地の際、自身の左中右に攻撃
			if (this->moveCnt == 0)
			{
				//足元に攻撃矩形を生成
				auto stompLandingRect = Shot00::Object::Create(true);
				//攻撃毎に攻撃範囲を生成時に指定
				stompLandingRect->hitBase = ML::Box2D(-96, -32, 192, 64);
				stompLandingRect->pos = ML::Vec2(this->pos.x, this->pos.y + this->hitBase.h / 2);
				stompLandingRect->Set_Limit(this->meleeCnt);
				stompLandingRect->Set_Erase(0);
				//エフェクトの生成
				//タスクキルはエフェクト側で行う
				auto stompLandingEffect = Effect::Object::Create(true);
				stompLandingEffect->pos = this->pos;
				stompLandingEffect->Set_Limit(18);
				stompLandingEffect->state = StompLanding;
			}
			break;
		case Bunker1:
			//溜め中に向きを変えられる
			if (in.LStick.L.down) { this->angle_LR = Left; }
			if (in.LStick.R.down) { this->angle_LR = Right; }
			break;
		case Bunker2:
			//離して発射
			if (this->moveCnt == 4) {
				if (this->angle_LR == Right) {
					//弾を正面に3発出す
					auto bunker = Shot00::Object::Create(true);
					//攻撃毎に攻撃範囲を生成時に指定
					bunker->hitBase = ML::Box2D(-96, -32, 192, 64);
					bunker->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					bunker->moveVec = ML::Vec2(0, 0);
					bunker->Set_Limit(this->meleeCnt);
					bunker->Set_Erase(0);
				}
				else {
					auto bunker = Shot00::Object::Create(true);
					//攻撃毎に攻撃範囲を生成時に指定
					bunker->hitBase = ML::Box2D(-96, -32, 192, 64);
					bunker->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					bunker->moveVec = ML::Vec2(0, 0);
					bunker->Set_Limit(this->meleeCnt);
					bunker->Set_Erase(0);
				}
			}
			break;
		case Bunker3:
			break;
		case  Shoot:	//射撃
			//射撃中も移動できる
			if (in.LStick.L.on) 
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//4フレーム目で弾を発射
			if (this->moveCnt == 4)
			{
				auto shot = Shot00::Object::Create(true);
				shot->state = Shoot;
				//攻撃毎に攻撃範囲を生成時に指定
				shot->hitBase = ML::Box2D(-32, -32, 64, 64);
				shot->Set_Limit(30);
				shot->Set_Erase(1);

				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
				}
			}
			break;
		case Air:
			//空中制動
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//空中攻撃(目の前に弾を生成)
			if (this->moveCnt == 4) {//4フレーム目で弾を発射
				auto air = Shot00::Object::Create(true);
				air->state = Air;
				//攻撃毎に攻撃範囲を生成時に指定
				air->hitBase = ML::Box2D(-64, -32, 128, 64);
				air->Set_Limit(this->meleeCnt);
				air->Set_Erase(0);
				//以下プレイヤの左右によって変化する
				if (this->angle_LR == Right) 
				{
					air->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
				}
				else
				{
					air->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
				}
			}
			break;
		case Airshoot:
			//空中制動
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = this->maxSpeed;
			}
			//空中射撃
			//4フレーム目で弾を発射
			if (this->moveCnt == 4) 
			{
				auto shot = Shot00::Object::Create(true);
				if (this->angle_LR == Right) 
				{
					shot->state = Airshoot;
					//攻撃毎に攻撃範囲を生成時に指定
					shot->hitBase = ML::Box2D(-32, -32, 64, 64);
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
				else
				{
					shot->state = Airshoot;
					//攻撃毎に攻撃範囲を生成時に指定
					shot->hitBase = ML::Box2D(-32, -32, 64, 64);
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
			}
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
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo  Object::Anim()
	{
		//デフォルトカラーを宣言
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(   0,   0, 64, 128), dc },//停止1			[0]
			{ this->hitBase, ML::Box2D(   0, 128, 64, 128), dc },//停止2			[1]
			{ this->hitBase, ML::Box2D(   0, 256, 64, 128), dc },//停止3			[2]
			{ this->hitBase, ML::Box2D(   0, 384, 64, 128), dc },//停止4			[3]
			{ this->hitBase, ML::Box2D(   0, 512, 64, 128), dc },//停止5			[4]
			{ this->hitBase, ML::Box2D(  64,   0, 64, 128), dc },//歩行1			[5]
			{ this->hitBase, ML::Box2D(  64, 128, 64, 128), dc },//歩行2			[6]
			{ this->hitBase, ML::Box2D(  64, 256, 64, 128), dc },//歩行3			[7]
			{ this->hitBase, ML::Box2D( 128,   0, 64, 128), dc },//減速			[8]
			{ this->hitBase, ML::Box2D( 192,   0, 64, 128), dc },//パンチ1		[9]
			{ this->hitBase, ML::Box2D( 256,   0, 64, 128), dc },//パンチ2		[10]
			{ this->hitBase, ML::Box2D( 320,   0, 64, 128), dc },//空中攻撃		[11]
			{ this->hitBase, ML::Box2D( 384,   0, 64, 128), dc },//ストンプ		[12]
			{ this->hitBase, ML::Box2D( 448,   0, 64, 128), dc },//ストンプ着地	[13]
			{ this->hitBase, ML::Box2D( 512,   0, 64, 128), dc },//射撃			[14]
			{ this->hitBase, ML::Box2D( 576,   0, 64, 128), dc },//空中射撃		[15]
			{ this->hitBase, ML::Box2D( 640,   0, 64, 128), dc },//バンカー1		[16]
			{ this->hitBase, ML::Box2D( 704,   0, 64, 128), dc },//バンカー2		[17]
			{ this->hitBase, ML::Box2D( 768,   0, 64, 128), dc },//バンカー3		[18]
			{ this->hitBase, ML::Box2D( 832,   0, 64, 128), dc },//ジャンプ		[19]
			{ this->hitBase, ML::Box2D( 896,   0, 64, 128), dc },//落下			[20]
			{ this->hitBase, ML::Box2D( 960,   0, 64, 128), dc },//着地			[21]
			{ this->hitBase, ML::Box2D(1024,   0, 64, 128), dc },//ダメージ		[22]
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
			//切り替わるフレーム数
			walkAnim = this->animCnt / 8;
			//パターン数
			walkAnim %= 3;
			//出た値に一枚目の要素番号を足す
			rtv = imageTable[walkAnim + 5];
			break;
		//	減速----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[8];	break;
		//	パンチ1-------------------------------------------------------------------------
		case Punch1:		rtv = imageTable[9];	break;
		//	パンチ2-------------------------------------------------------------------------
		case Punch2:		rtv = imageTable[10];	break;
		//	空中攻撃-------------------------------------------------------------------------
		case Air:			rtv = imageTable[11];	break;
		//	ストンプ-------------------------------------------------------------------------
		case Stomp:			rtv = imageTable[12];	break;
		//	ストンプ着地---------------------------------------------------------------------
		case StompLanding:	rtv = imageTable[13];	break;
		//	地上射撃-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[14];	break;
		//	空中射撃-------------------------------------------------------------------------
		case Airshoot:		rtv = imageTable[15];	break;
		//	バンカー1-------------------------------------------------------------------------
		case Bunker1:		rtv = imageTable[16];	break;
		//	バンカー2-------------------------------------------------------------------------
		case Bunker2:		rtv = imageTable[17];	break;
		//	バンカー3-------------------------------------------------------------------------
		case Bunker3:		rtv = imageTable[18];	break;
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