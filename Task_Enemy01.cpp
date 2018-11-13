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

		this->base_file_path_sound = "./data/sound/wav/";
		this->name_sound_defeat = "sound_defeat";
		//wavファイルの中でも再生できないものあり
		DM::Sound_CreateSE(this->name_sound_defeat, this->base_file_path_sound + "explosion_enemy01.wav");
		DM::Sound_Volume(this->name_sound_defeat, 1000);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_sound_defeat);
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
		this->hp = 7;
		this->maxSpeed = 2.0f;								//最大移動速度(横)
		this->addSpeed = 0.7f;								//歩行加速度(地面の影響である程度打ち消される
		this->decSpeed = 0.5f;								//接地状態の時の速度減衰量(摩擦
		this->max_speed_fall = 10.0f;						//最大落下速度
		this->gravity = ML::Gravity(SIZE_CHIP) * 5;			//重力加速度&時間速度による加算量
		this->interval_Caution = 60;						//プレイヤが視界から外れた後、再度警戒に入るまでの時間
		this->interval_Attack = 120;						//弾を生成する間隔
		this->interval_Flash = 4;							//点滅間隔
		this->add_un_hit = 60;								//プレイヤに与える無敵時間
		this->size_h_resource = 192;						//被弾時、ホワイトアウトする際の基準値
		this->searchBase = ML::Box2D(-192, -96, 384, 192);	//探知矩形
		this->shot_Init = ML::Vec2(0, -45);					//生成位置補正ショット
		this->eff = new Task_Effect::Object();				//メソッド呼び出し
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
		//ゲームが進行中の時のみ撃破エフェクトの生成
		if (!ge->failure && !ge->clear)
		{
			//画面エフェクトの生成
			this->eff->Create_Effect(3, this->pos);
			for (int i = 0; i < 5; ++i)
			{
				this->eff->Create_Effect(7, this->pos, float(rand() % 360), this->angle_LR);
			}
			//SEの再生
			DM::Sound_Play(this->res->name_sound_defeat, false);
		}
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
		if (this->time_un_hit > 0) { this->time_un_hit--; }
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
					(*it)->Received(this, at,this->add_un_hit);
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
		
		BChara::DrawInfo di = this->Anim();
		//無敵時間中は点滅
		if (this->time_un_hit > 0) 
		{
			if (this->time_un_hit %this->interval_Flash == 0) 
			{
				//リソース縦サイズ*最大アニメーション数
				//白抜きリソースの呼び出し
				di.src.y = di.src.y + this->size_h_resource * 4;
			}
		}
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src,ML::Color(1.0f,1.0f,1.0f,1.0f));
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
	void Object::Received(BChara* from_, AttackInfo at_,const int& un_hit_)
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
		if (this->time_un_hit > 0) {
			return;//無敵時間中はダメージを受けない
		}
		this->hp -= at_.power;
		//無敵時間
		this->time_un_hit = un_hit_;
		//まず範囲攻撃かどうかを判定する
		if (!from_->Get_Range_Wide())
		{
			//吹き飛ばされる
			this->moveVec = from_->Get_Move_Back();
		}
		//範囲攻撃の場合は攻撃を受けた瞬間の位置関係で飛ぶ方向を決める
		else
		{
			//自分が右側にいるとき
			if (this->pos.x - from_->pos.x > 0)
			{
				this->moveVec = from_->Get_Move_Back();
			}
			//自分が左側にいるとき
			else
			{
				float x = from_->Get_Move_Back().x*(-1);
				this->moveVec = ML::Vec2(x, from_->Get_Move_Back().y);
			}
		}
		//状態を遷移させない時、使用
		if (from_->Get_Tip()) { return; }
		//状態遷移
		this->UpdateMotion(Bound);
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
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
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
			auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
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
				auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
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
		//デフォルトの値を宣言
		ML::Box2D dd(-48, -48, 96, 96);
		ML::Color dc(1, 1, 1, 1);
		int size = 192;
		BChara::DrawInfo imageTable[] = {
			//draw						src
			{ dd,ML::Box2D(  0,  0,size*1,size*1),dc },	//待機		[0]
			{ dd,ML::Box2D(192,  0,size*1,size*1),dc },	//歩行1		[1]
			{ dd,ML::Box2D(192,192,size*1,size*1),dc },	//歩行2		[2]
			{ dd,ML::Box2D(192,384,size*1,size*1),dc },	//歩行3		[3]
			{ dd,ML::Box2D(192,576,size*1,size*1),dc },	//歩行4		[4]
			{ dd,ML::Box2D(384,  0,size*1,size*1),dc },	//ターン		[5]
			{ dd,ML::Box2D(576,  0,size*1,size*1),dc },	//射撃01		[6]
			{ dd,ML::Box2D(576,192,size*1,size*1),dc },	//射撃02		[7]
			{ dd,ML::Box2D(768,  0,size*1,size*1),dc },	//見失う01	[8]
			{ dd,ML::Box2D(768,192,size*1,size*1),dc },	//見失う02	[9]
			{ dd,ML::Box2D(960,  0,size*1,size*1),dc },	//警戒01		[10]
			{ dd,ML::Box2D(960,192,size*1,size*1),dc }	//警戒02		[11]
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
		//被弾時は白いリソースを使用する
		if (this->state == Damage)
		{
			rtv.src.y = rtv.src.y + size * 4;
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
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
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
	//エネミーの生成
	//引数	：	（初期座標,移動速度,HP）
	void Object::Create_Enemy01(const ML::Vec2& pos_)
	{
		auto ene = Enemy01::Object::Create(true);
		ene->pos = pos_;
		auto es = EnemySearch::Object::Create(true);
		es->hitBase = ene->Get_Search();
		es->Set_Target(ene);
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