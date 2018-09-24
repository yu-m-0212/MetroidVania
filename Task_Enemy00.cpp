//-------------------------------------------------------------------
//敵00(スライム
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Enemy00.h"
#include  "Task_Effect.h"

namespace  Enemy00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Enemy00Img";
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
		this->hitBase = ML::Box2D(-56, -48, 102, 96);
		this->recieveBase = this->hitBase;
		this->state = Stand;
		this->hp = 5;							//hp初期値
		this->maxSpeed = 2.0f;					//最大移動速度(横)
		this->addSpeed = 0.7f;					//歩行加速度(地面の影響である程度打ち消される
		this->decSpeed = 0.5f;					//接地状態の時の速度減衰量(摩擦
		this->maxFallSpeed = 10.0f;				//最大落下速度
		this->jumpPow = -6.0f;					//ジャンプ力(初速)
		this->gravity = ML::Gravity(32) * 5;	//重力加速度&時間速度による加算量
		this->interval_Flash = 4;				//点滅間隔
		
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
		if(this->state!=Bound)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
			if (nullptr == targets) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) 
			{
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me))
				{
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
		DG::Image_Draw(this->res->imageName, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Received(BChara* from_, AttackInfo at_)
	{
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
		if (from_->tip) { return; }
		//無敵時間
		this->unHitTime = 30;
		//まず範囲攻撃かどうかを判定する
		if (!from_->wideRange)
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
			if (this->CheckFront_LR() == true) { nm = Turn; }//壁に衝突
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
		case	Turn:	//旋回中
			if (this->moveCnt >= 5) { nm = Stand;}
			break;
		case	Jump:	//上昇中
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
		case	Fall:	//落下中
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Bound:	//ダメージを受けて吹き飛んでいる
						//8フレーム経過と接地で「立っている」に戻る
			if (this->moveCnt >= 8 && this->CheckFoot() == true) { nm = Stand; }
			break;
		case	TakeOff://飛び立ち
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Landing://着地
			if (this->CheckFoot() == false) { nm = Fall; }
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
				this->CheckFoot() == false) {
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);//min( , )
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
		switch (this->state) {
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
		}
	}
	//-----------------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw						src
			{ this->hitBase,ML::Box2D(0,  0,192,192),dc },//待機01[0]
			{ this->hitBase,ML::Box2D(0,192,192,192),dc },//待機02[1]
		};
		BChara::DrawInfo rtv;
		int anim = 0;
		switch (this->state) {
		default:
			rtv = imageTable[0];
			break;
		//直立
		case  Stand:
			anim = this->animCnt / 24;
			anim %= 2;
			rtv = imageTable[anim + 0];
			break;
		}
		//アングルが変わると画像を反転
		if (false == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
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