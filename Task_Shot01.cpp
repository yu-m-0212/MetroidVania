//-------------------------------------------------------------------
//プレイヤの出す弾
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Shot01.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Barrier.h"
#include  "Task_Effect.h"
#include  "Task_Boss_Head.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Shot01Img";
		DG::Image_Create(this->imageName, "./data/image/Shot00.png");
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
		this->render2D_Priority[1] = 0.4f;		//描画順
		this->recieveBase = this->hitBase;		//キャラクタとの接触矩形
		this->flag_Erase = true;				//接触時消えるか
		this->flag_reflect = false;				//反射した弾か否か
		this->power = 0;						//攻撃力（生成時に指定する）
		this->rate_reflect_power = 3;			//反射した弾に与える攻撃力倍率
		this->rate_speed_reflect = 2.0f;		//反射したときに与える加速倍率
		this->limit_Erase = 0;					//消滅するまでの時間
		this->add_un_hit_player = 60;			//プレイヤに与える無敵時間
		this->add_un_hit_boss = 8;				//ボスに与える無敵時間
		this->angle = 0.0f;						//角度
		this->eff = new Task_Effect::Object();	//メソッド呼び出し
		this->cnt_anim = 0;						//アニメカウンタ
		this->speed_anim = 15;					//アニメスピード
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//アニメーションはポーズ中も行う
		this->cnt_anim++;
		//ポーズ
		if (ge->pause) { return; }
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		this->moveCnt++;
		//各状態ごとの処理
		this->Move();

		//移動
		this->pos += this->moveVec;
		//プレイヤとの判定
		if(!this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("プレイヤ");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_player);
					//ショットのみ消滅
					//格闘は複数体にあたる
					if (this->flag_Erase)
					{
						//対応したヒット時のエフェクトを生成
						//現状、引数には対象の敵の座標をいれる
						this->Effect_Hit((*it)->pos);
						this->Kill();
					}
					else
					{
						//対応したヒット時のエフェクトを生成
						//現状、引数には対象の敵の座標をいれる
						//格闘は矩形が残る為、当たった瞬間のみエフェクトを生成する
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
					}
					break;
				}
			}
		}
		//反射処理
		auto barrier = ge->GetTask_One_GN<Barrier::Object>(Barrier::defGroupName,Barrier::defName);
		if (nullptr != barrier)
		{
			ML::Box2D  me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = barrier->recieveBase.OffsetCopy(barrier->pos);
			//自身がまだ反射弾ではないとき
			if (!this->flag_reflect)
			{
				if (you.Hit(me))
				{
					if (barrier->state==LandStomp||
						barrier->state==AirStomp||
						barrier->state==StompLanding)
					{
						this->moveVec = ML::Vec2(-this->moveVec.x, -this->moveVec.y);
						//反射フラグ反転
						this->flag_reflect = true;
						//反射した弾攻撃力補正をかける
						this->power *= this->rate_reflect_power;
						//反射した弾の速度を上げる
						this->moveVec *= this->rate_speed_reflect;
					}
				}
			}
		}
		//射撃は壁に当たると消滅する
		if (this->flag_Erase)
		{
			if (auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName))
			{
				ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
				if (true == map->CheckHit(hit))
				{
					//壁に当たったときはエフェクトを生成しない
					this->Kill();
				}
			}
		}
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->limit_Erase)
		{
			//消滅申請
			this->Kill();
			return;
		}
		//反射した弾はエネミーとも判定を行う
		if (this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("敵");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_player);
					//対応したヒット時のエフェクトを生成
					//現状、引数には対象の敵の座標をいれる
					this->Effect_Hit((*it)->pos);
					this->Kill();
					break;
				}
			}
		}
		//反射した弾はボスとも判定を行う
		if (this->flag_reflect)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("ボス");
			if (nullptr == pl) { return; }
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//相手に接触の有無を確認させる
				if ((*it)->CheckHit(me)) {
					//相手にダメージの処理を行わせる
					BChara::AttackInfo at = { this->power,0,0 };
					(*it)->Recieved(this, at, this->add_un_hit_boss);
					//揺れ速度が一定時間上がる
					auto head =
						ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
					head->limit_hit_reflect = LIMIT_HIT_REFLECT;
					//対応したヒット時のエフェクトを生成
					//現状、引数には対象の敵の座標をいれる
					this->Effect_Hit((*it)->pos);
					this->Kill();
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 192, 16, 16);
		//反射された弾はプレイヤの色に変更する
		if (this->flag_reflect)
		{
			src.y = 208;
		}
		//アニメーション
		int anim;
		anim = this->cnt_anim / this->speed_anim;
		anim %= 2;
		src.x = this->hitBase.w*anim;
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//呼び出したタスクから寿命を設定する
	void Object::Set_Limit(const int& cl_)
	{
		this->limit_Erase = cl_;
	}
	//壁や敵に衝突したとき、消えるか否かを指定する
	void Object::Set_Erase(const int& erase_)
	{
		this->flag_Erase = erase_;
	}
	//外部から生成する際、攻撃力を指定
	//引数	：	（整数値）
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//角度を指定する
	//引数	：	（角度）
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			//敵に衝突したとき消えるか否か
			this->flag_Erase = true;
			break;
		}
	}
	//消滅する際、状態に応じてエフェクトを生成
	//引数	：	（エフェクトを生成する座標）
	//caseの中で宣言したローカル変数のスコープがswitch文全体に及ぶ恐れがある為
	//意図的にブロック{}を設けること
	void Object::Effect_Hit(const ML::Vec2& pos_)
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
			this->eff->Create_Effect(1, this->pos, this->angle,this->angle_LR);
			break;
		}
	}
	//ショットを生成する
	//引数	：	（座標,移動量,矩形,消滅時間,攻撃力,壁に当たった時消えるか否か）
	void Object::Create_Shot(const ML::Vec2& pos_, const ML::Vec2& vec_, const ML::Box2D& hit_, const int& time_, const int& power_, const bool& erase_)
	{
		auto shot = Shot01::Object::Create(true);
		shot->pos = pos_;
		shot->moveVec = vec_;
		shot->hitBase = hit_;
		shot->limit_Erase = time_;
		shot->power = power_;
		shot->flag_Erase = erase_;
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