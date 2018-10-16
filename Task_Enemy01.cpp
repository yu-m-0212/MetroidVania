//-------------------------------------------------------------------
//敵00(スライム
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Shot00.h"
#include	"Task_Shot01.h"
#include	"Task_Enemy01.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Player.h"
#include	"Task_EnemySearch.h"

namespace  Enemy01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Enemy01Img";
		DG::Image_Create(this->imageName, "./data/image/enemy01.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = ML::Box2D(-52,-48,102,96);
		this->recieveBase = this->hitBase;
		this->angle_LR = Left;
		this->state = Stand;
		this->hp = 10;
		this->maxSpeed = 2.0f;							//最大移動速度(横)
		this->addSpeed = 0.7f;							//歩行加速度(地面の影響である程度打ち消される
		this->decSpeed = 0.5f;							//接地状態の時の速度減衰量(摩擦
		this->maxFallSpeed = 10.0f;						//最大落下速度
		this->gravity = ML::Gravity(32) * 5;			//重力加速度&時間速度による加算量
		this->interval_Caution = 60;					//プレイヤが視界から外れた後、再度警戒に入るまでの時間
		this->interval_Attack = 120;					//弾を生成する間隔
		this->interval_Flash = 4;						//点滅間隔
		this->searchBase = ML::Box2D(-192, -96, 384, 192);
		this->shot_Init = ML::Vec2(0, -45);
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
		//撃破エフェクトの生成
		auto DefeatEffect = Effect::Object::Create(true);
		DefeatEffect->pos = this->pos;
		DefeatEffect->Set_Limit(24);
		DefeatEffect->state = Lose;
		DefeatEffect->angle_LR = this->angle_LR;
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
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
		ML::Vec2 est = this->moveVec;
		this->CheckMove(est);
		//当たり判定
		//自身がやられ状態の時はプレイヤに接触しても攻撃しない
		if (this->state != Bound)
		{
			ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
			if (nullptr == targets) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { 1,0,0 };
					(*it)->Received(this, at);
					break;
				}
			}
		}
		//HPが0かつ
		//床に触れるか一定時間経過で消滅
		if (this->hp <= 0 &&
			(this->CheckFoot() || this->moveCnt >= 60))
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//無敵時間中は点滅
		if (this->unHitTime > 0) {
			if (this->unHitTime %this->interval_Flash == 0) {
				return;
			}
		}
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src,ML::Color(1.0f,1.0f,1.0f,1.0f));
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		//仮処理
		//hpが減ると行動が早くなる
		this->interval_Caution -= 4;
		this->interval_Attack -= 1;
		//攻撃を受けた方向を向く
		if (this->pos.x - from_->pos.x > 0)
		{
			this->angle_LR = Left;
		}
		else
		{
			this->angle_LR = Right;
		}
		if (this->unHitTime > 0) {
			return;//無敵時間中はダメージを受けない
		}
		this->hp -= at_.power;
		//ノックバックの発生しない攻撃の場合、以下を読まない
		if (from_->Get_Tip()) { return; }
		//無敵時間
		this->unHitTime = 30;
		//まず範囲攻撃かどうかを判定する
		if (!from_->Get_Range_Wide())
		{
			//吹き飛ばされる
			this->moveVec = from_->moveBack;
		}
		//範囲攻撃の場合は攻撃を受けた瞬間の位置関係で飛ぶ方向を決める
		else
		{
			//自分が右側にいるとき
			if (this->pos.x - from_->pos.x > 0)
			{
				this->moveVec = from_->moveBack;
			}
			//自分が左側にいるとき
			else
			{
				float x = from_->moveBack.x*(-1);
				this->moveVec = ML::Vec2(x, from_->moveBack.y);
			}
		}
		this->UpdateMotion(Bound);
		//from_は攻撃してきた相手、カウンターなどで逆にダメージを与えたいときに使う
	}
	//-------------------------------------------------------------------
	//思考&状況判断　モーション決定
	void Object::Think()
	{
		BChara::State nm = this->state; //とりあえず今の状態を指定
		//思考（入力）や状況に応じてモーションを変更することを目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm) {
		case	Stand:	//立っている
			if (this->CheckFoot() == false) { nm = Fall; }
			else							{ nm = Walk; }//接地で歩きに移行
			break;
		case	Walk:	//歩いている
			if ( this->Search_Player()) { nm = Caution; }
			if (this->CheckFront_LR() == true) { nm = Turn; }//壁に衝突
			if (this->CheckFoot() == false) { nm = Fall; }
			if (this->CheckFrontFoot_LR() == false) { nm = Turn; }
			break;
		case	Turn:	//旋回中
			if (this->moveCnt >= 5) { nm = Stand;}
			if ( this->Search_Player()) { nm = Caution; }
			break;
		case	Jump:	//上昇中
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
		case	Fall:	//落下中
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Bound:
			//ダメージを受けて吹き飛んでいる
			//8フレーム経過と接地で「立っている」に戻る
			if (this->moveCnt >= 8 && this->CheckFoot() == true) { nm = Stand; }
			break;
		case	TakeOff://飛び立ち
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Landing://着地
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
		case Caution:
			if (! this->Search_Player()) { nm = TargetLost; }
			else if ( this->Search_Player() && this->moveCnt >= this->interval_Attack) { nm = Shoot; }
			break;
		case TargetLost:
			if ( this->Search_Player()) { nm = Caution; }
			//発見されずに一定時間経過すると警戒解除
			if (this->moveCnt >= this->interval_Caution &&
				! this->Search_Player())
			{
				nm = Stand;
			}
			break;
		case Shoot:
			if (! this->Search_Player()) { nm = TargetLost; }
			if (this->moveCnt >= 12&&  this->Search_Player()) { nm = Caution; }
			break;
		}
		//モーション更新
		this->UpdateMotion(nm);
	}
	//-------------------------------------------------------------------
	//モーションに対応した処理
	//(モーションは変更しない)
	void Object::Move()
	{
		//重力加速
		switch (this->state) {
		default:
			//上昇中もしくは足元に地面が無い
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) 
			{
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
			}
			//地面に接触している
			else {
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
		case Bound:
		case Unnon:	break;
		}
		//-------------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state) 
		{
		case  Stand:	//立っている
			break;
		case  Walk:		//歩いている
			if (this->angle_LR == Left) {//横軸移動(徐々に加速)
				this->moveVec.x =
					max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (this->angle_LR == Right) {
				this->moveVec.x =
					min(+this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case	Turn:	//旋回中
			if (this->moveCnt == 3) {
				if (this->angle_LR == Left) { this->angle_LR = Right; }
				else {						  this->angle_LR = Left; }
			}
			break;
		case  Fall:		//落下中
			if (this->angle_LR == Left) {//横軸移動(徐々に加速)
				this->moveVec.x =
					max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (this->angle_LR == Right) {
				this->moveVec.x =
					min(+this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case  Jump:		//上昇中
			break;
		case Caution:
		{
			auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
			if (nullptr == pl) { break; }
			//プレイヤに向きを変える
			if (this->pos.x - pl->pos.x > 0)
			{
				this->angle_LR = Left;
			}
			else
			{
				this->angle_LR = Right;
			}
			break;
		}
		case Shoot:
			if (this->moveCnt == 0)
			{
				auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
				if (nullptr == pl) { break; }
				auto shot = Shot01::Object::Create(true);
				//呼び出した判定矩形に思考させるため状態を指定
				shot->state = Shoot;
				shot->hitBase = ML::Box2D(-16, -16, 32, 32);
				shot->Set_Limit(300);
				shot->Set_Erase(0);
				shot->Set_Power(1);
				shot->angle_LR = this->angle_LR;
				shot->pos = this->pos + this->shot_Init;
				//プレイヤに向かって弾を発射
				ML::Vec2 toTarget = pl->pos - this->pos;
				float angle = atan2(toTarget.y, toTarget.x);
				shot->moveVec = ML::Vec2(cos(angle), sin(angle))*8.0f;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		ML::Box2D dd(-48, -48, 96, 96);
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw						src
			{ dd,ML::Box2D(  0,  0,192,192),dc },	//待機		[0]
			{ dd,ML::Box2D(192,  0,192,192),dc },	//歩行1		[1]
			{ dd,ML::Box2D(192,192,192,192),dc },	//歩行2		[2]
			{ dd,ML::Box2D(192,384,192,192),dc },	//歩行3		[3]
			{ dd,ML::Box2D(192,576,192,192),dc },	//歩行4		[4]
			{ dd,ML::Box2D(384,  0,192,192),dc },	//ターン		[5]
			{ dd,ML::Box2D(576,  0,192,192),dc },	//射撃01		[6]
			{ dd,ML::Box2D(576,192,192,192),dc },	//射撃02		[7]
			{ dd,ML::Box2D(768,  0,192,192),dc },	//見失う01	[8]
			{ dd,ML::Box2D(768,192,192,192),dc },	//見失う02	[9]
			{ dd,ML::Box2D(960,  0,192,192),dc },	//警戒01		[10]
			{ dd,ML::Box2D(960,192,192,192),dc }	//警戒02		[11]
		};
		BChara::DrawInfo rtv;
		int anim = 0;
		switch (this->state) {
		default:		rtv = imageTable[0];	break;
		case  Stand:
			rtv = imageTable[0];
			break;
		case  Walk:
			anim = this->animCnt / 8;
			anim %= 4;
			rtv = imageTable[anim + 1];	
			break;
		case Turn:		rtv = imageTable[5];	break;
		case Shoot:
			anim = this->animCnt / 6;
			anim %= 2;
			rtv = imageTable[anim + 6];
			break;
		case TargetLost:
			anim = this->animCnt / 24;
			anim %= 2;
			rtv = imageTable[anim+8];
			break;
		case Caution:
			anim = this->animCnt / 24;
			anim %= 2;
			rtv = imageTable[anim+10];
			break;
		}
		//向きに応じて画像を左右反転する
		//アングルが変わると画像を反転
		if (false == this->angle_LR) 
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//プレイヤを発見したら警戒モードに入る
	bool Object::Search_Player()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (pl == nullptr || this == nullptr) { return false; }
		ML::Box2D you = pl->hitBase.OffsetCopy(pl->pos);
		ML::Box2D  me = this->searchBase.OffsetCopy(this->pos);
		//自身の左右で探知矩形をずらす
		if (this->angle_LR == Left)
		{
			me.x -= this->searchBase.w / 2;
		}
		else
		{
			me.x += this->searchBase.w / 2;
		}
		return you.Hit(me);
	}
	//探知矩形を取得する
	ML::Box2D Object::Get_Search()
	{
		return this->searchBase;
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