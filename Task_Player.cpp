//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"

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
		this->motion = Stand;				//キャラ初期状態
		this->max_Hp = 10;					//ヘルス最大値
		this->hp = this->max_Hp;			//ヘルス初期値
		this->maxSpeed = 10.0f;				//最大移動速度（横）
		this->addSpeed = 1.0f;				//歩行加速度（地面の影響である程度打ち消される
		this->decSpeed = 0.5f;				//接地状態の時の速度減衰量（摩擦
		this->maxFallSpeed = 15.0f;			//最大落下速度
		this->jumpPow = -15.0f;				//ジャンプ力（初速）
		this->gravity = ML::Gravity(64);	//重力加速度＆時間速度による加算量
		this->reach = 100.0f;				//パンチの射程
		this->slide = 10.0f;				//パンチで前進する距離
		this->meleeCnt = 15;				//格闘攻撃判定継続時間
		this->shotSpeed = 20;

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
		if (ge->debugMode)
		{
			ML::Box2D debugBox(150, 50, 1000, 1000);
			string debugText =
				"motion = " + to_string(this->motion) + "\n" +
				"moveVec.x = " + to_string(this->moveVec.x) + "\n" +
				"moveVec.y = " + to_string(this->moveVec.y) + "\n" +
				"angle = " + to_string(this->angle_LR) + "\n"
				"moveCnt = " + to_string(this->moveCnt) + "\n" +
				"unHitTime = " + to_string(this->unHitTime) + "\n" +
				"hp=" + to_string(this->hp)+"\n"+
				"Selectボタンでデバッグモード切替";
			DG::Font_Draw("FontA", debugBox, debugText, ML::Color(1, 0, 0, 0));
		}
		//以上デバッグ----------------------------------------------------
		//8フレーム中4フレーム画像を表示しない
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
		if (this->hp <= 0) {
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
		BChara::Motion  nm = this->motion;	//とりあえず今の状態を指定
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
			if (this->moveCnt >= 30) { nm = Stand; }
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
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//重力加速
		switch (this->motion) {
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
		switch (this->motion) {
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
		switch (this->motion) {
		case  Stand:	//立っている
			break;
		case  Walk:		//歩いている
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
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
			if (this->moveCnt == 0) {
				this->moveVec.y = this->jumpPow;//初速設定
			}
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			//天井に衝突で上昇力を0に
			if (this->CheckHead() == true) { this->moveVec.y = 0; }
			break;
		case  Fall:		//落下中
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
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
			if (this->moveCnt == 0) {
				auto punch1 = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					punch1->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					punch1->moveVec = ML::Vec2(0, 0);
					punch1->Set_Limit(this->meleeCnt);
					punch1->Set_Erase(0);
					//攻撃時に前進する
					this->moveVec.x = this->slide;
				}
				else {
					punch1->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					punch1->moveVec = ML::Vec2(0, 0);
					punch1->Set_Limit(this->meleeCnt);
					punch1->Set_Erase(0);
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
				if (this->angle_LR == Right) {
					punch2->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					punch2->moveVec = ML::Vec2(0, 0);
					punch2->Set_Limit(this->meleeCnt);
					punch2->Set_Erase(0);
					//攻撃時に前進する
					this->moveVec.x = this->slide;
				}
				else {
					punch2->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					punch2->moveVec = ML::Vec2(0, 0);
					punch2->Set_Limit(this->meleeCnt);
					punch2->Set_Erase(0);
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
				for (int i = -1; i < 2; ++i)
				{
					auto stompLanding = Shot00::Object::Create(true);
					//足元に攻撃矩形を生成
					stompLanding->pos = ML::Vec2(this->pos.x + this->reach * i, this->pos.y + this->hitBase.h / 2);
					stompLanding->moveVec = ML::Vec2(0, 0);
					stompLanding->Set_Limit(this->meleeCnt);
					stompLanding->Set_Erase(0);
				}
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
					for (int i = 0; i < 3; ++i)
					{
						auto bunker = Shot00::Object::Create(true);
						bunker->pos = ML::Vec2(this->pos.x + this->reach * i, this->pos.y);
						bunker->moveVec = ML::Vec2(0, 0);
						bunker->Set_Limit(this->meleeCnt);
						bunker->Set_Erase(0);
					}
				}
				else {
					for (int i = 0; i < 3; ++i)
					{
						auto bunker = Shot00::Object::Create(true);
						bunker->pos = ML::Vec2(this->pos.x - this->reach * i, this->pos.y);
						bunker->moveVec = ML::Vec2(0, 0);
						bunker->Set_Limit(this->meleeCnt);
						bunker->Set_Erase(0);
					}
				}
			}
			break;
		case Bunker3:
				break;
		case  Shoot:	//射撃
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			if (this->moveCnt == 4) {//4フレーム目で弾を発射
				auto shot = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					shot->pos = ML::Vec2(this->pos.x + 20, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
				else {
					shot->pos = ML::Vec2(this->pos.x - 20, this->pos.y);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
			}
			break;
		case Air:
			//空中制動
			if (in.LStick.L.on) {
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) {
				this->moveVec.x = this->maxSpeed;
			}
			//空中攻撃(目の前に弾を生成)
			if (this->moveCnt == 4) {//4フレーム目で弾を発射
				auto air = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					air->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					air->moveVec = ML::Vec2(0, 0);
					air->Set_Limit(this->meleeCnt);
					air->Set_Erase(0);
				}
				else {
					air->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					air->moveVec = ML::Vec2(0, 0);
					air->Set_Limit(this->meleeCnt);
					air->Set_Erase(0);
				}
			}
			break;
		case Airshoot:
			//空中制動
			if (in.LStick.L.on) {
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) {
				this->moveVec.x = this->maxSpeed;
			}
			//空中射撃
			if (this->moveCnt == 4) {//4フレーム目で弾を発射
				auto shot = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					shot->pos = ML::Vec2(this->pos.x + 20, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
				else {
					shot->pos = ML::Vec2(this->pos.x - 20, this->pos.y);
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
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(   0,  0, 64, 128), ML::Color(1, 1, 1, 1) },//停止		[0]
			{ this->hitBase, ML::Box2D(  64,  0, 64, 128), ML::Color(1, 1, 1, 1) },//歩行1		[1]
			{ this->hitBase, ML::Box2D(  64,128, 64, 128), ML::Color(1, 1, 1, 1) },//歩行2		[2]
			{ this->hitBase, ML::Box2D(  64,256, 64, 128), ML::Color(1, 1, 1, 1) },//歩行3		[3]
			{ this->hitBase, ML::Box2D( 128,  0, 64, 128), ML::Color(1, 1, 1, 1) },//減速		[4]
			{ this->hitBase, ML::Box2D( 192,  0, 64, 128), ML::Color(1, 1, 1, 1) },//パンチ1		[5]
			{ this->hitBase, ML::Box2D( 256,  0, 64, 128), ML::Color(1, 1, 1, 1) },//パンチ2		[6]
			{ this->hitBase, ML::Box2D( 320,  0, 64, 128), ML::Color(1, 1, 1, 1) },//空中攻撃	[7]
			{ this->hitBase, ML::Box2D( 384,  0, 64, 128), ML::Color(1, 1, 1, 1) },//ストンプ	[8]
			{ this->hitBase, ML::Box2D( 448,  0, 64, 128), ML::Color(1, 1, 1, 1) },//ストンプ着地	[9]																				 //ストンプ着地									 64	 128
			{ this->hitBase, ML::Box2D( 512,  0, 64, 128), ML::Color(1, 1, 1, 1) },//射撃		[10]
			{ this->hitBase, ML::Box2D( 576,  0, 64, 128), ML::Color(1, 1, 1, 1) },//空中射撃	[11]
			{ this->hitBase, ML::Box2D( 640,  0, 64, 128), ML::Color(1, 1, 1, 1) },//バンカー1	[12]
			{ this->hitBase, ML::Box2D( 704,  0, 64, 128), ML::Color(1, 1, 1, 1) },//バンカー2	[13]
			{ this->hitBase, ML::Box2D( 768,  0, 64, 128), ML::Color(1, 1, 1, 1) },//バンカー3	[14]
			{ this->hitBase, ML::Box2D( 832,  0, 64, 128), ML::Color(1, 1, 1, 1) },//ジャンプ	[15]
			{ this->hitBase, ML::Box2D( 896,  0, 64, 128), ML::Color(1, 1, 1, 1) },//落下		[16]
			{ this->hitBase, ML::Box2D( 960,  0, 64, 128), ML::Color(1, 1, 1, 1) },//着地		[17]
			{ this->hitBase, ML::Box2D(1024,  0, 64, 128), ML::Color(1, 1, 1, 1) },//ダメージ	[18]
		};
		BChara::DrawInfo  rtv;
		int  walkAnim;
		switch (this->motion) {
		default:			rtv = imageTable[0];	break;
		//	停止----------------------------------------------------------------------------
		case  Stand:		rtv = imageTable[0];	break;
		//	歩行----------------------------------------------------------------------------
		case  Walk:
			//歩行アニメーション
			walkAnim = this->animCnt / 8;
			walkAnim %= 3;
			rtv = imageTable[walkAnim + 1];
			break;
		//	減速----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[4];	break;
		//	パンチ1-------------------------------------------------------------------------
		case Punch1:		rtv = imageTable[5];	break;
		//	パンチ2-------------------------------------------------------------------------
		case Punch2:		rtv = imageTable[6];	break;
		//	空中攻撃-------------------------------------------------------------------------
		case Air:			rtv = imageTable[7];	break;
		//	ストンプ-------------------------------------------------------------------------
		case Stomp:			rtv = imageTable[8];	break;
		//	ストンプ着地---------------------------------------------------------------------
		case StompLanding:	rtv = imageTable[9];	break;
		//	地上射撃-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[10];	break;
		//	空中射撃-------------------------------------------------------------------------
		case Airshoot:		rtv = imageTable[11];	break;
		//	バンカー1-------------------------------------------------------------------------
		case Bunker1:		rtv = imageTable[12];	break;
		//	バンカー2-------------------------------------------------------------------------
		case Bunker2:		rtv = imageTable[13];	break;
		//	バンカー3-------------------------------------------------------------------------
		case Bunker3:		rtv = imageTable[14];	break;
		//	ジャンプ------------------------------------------------------------------------
		case  Jump:			rtv = imageTable[15];	break;
		//	落下----------------------------------------------------------------------------
		case  Fall:			rtv = imageTable[16];	break;
		//着地硬直--------------------------------------------------------------------------
		case  Landing:		rtv = imageTable[17];	break;
		//ダメージ--------------------------------------------------------------------------
		case  Damage:		rtv = imageTable[18];	break;
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