//-------------------------------------------------------------------
//プレイヤの発生させる判定矩形（反射アビリティ）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Barrier.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"

namespace  Barrier
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "barrier";
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
		this->power = 0;						//攻撃力
		this->un_hit = 8;						//相手に与える無敵時間
		this->limit_Erase = 0;					//消滅するまでの時間
		this->eff = new Task_Effect::Object();	//メソッド呼び出し
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
		//ポーズ
		if (ge->pause) { return; }
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		this->moveCnt++;
		//各状態ごとの処理
		this->Move();
		//プレイヤの座標に合わせる
		this->pos = pl->pos;
		//雑魚との接触判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("敵");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//相手に接触の有無を確認させる
					if ((*it)->CheckHit(me))
					{
						//相手にダメージの処理を行わせる
						BChara::AttackInfo at = { this->power,0,0 };
						(*it)->Recieved(this, at, this->un_hit);
						//当たった瞬間のみエフェクトを生成する
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
						break;
					}
				}
			}
		}
		//ボスとの接触判定
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("ボス");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//相手に接触の有無を確認させる
					if ((*it)->CheckHit(me))
					{
						//相手にダメージの処理を行わせる
						BChara::AttackInfo at = { this->power,0,0 };
						(*it)->Recieved(this, at, this->un_hit);
						//当たった瞬間のみエフェクトを生成する
						if ((*it)->moveCnt == 0)
						{
							this->Effect_Hit((*it)->pos);
						}
						break;
					}
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->recieveBase;
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//以下攻撃範囲表示
		if (ge->debugMode)
		{
			ML::Box2D srcDebug(0, 0, 32, 32);
			DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
			DG::Image_Draw(this->res->imageName, draw, srcDebug, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
		}
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G <Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		switch (this->state)
		{
		default:
			break;
		case StompLanding:
			break;
		case Shoot:
			break;
		case Air:
			//パンチ中はプレイヤの動きに合わせて判定矩形も前進する
			this->moveVec = pl->moveVec;
			//プレイヤが壁に衝突したら移動量を0に
			if (pl->CheckFront_LR() || pl->CheckBack_LR())
			{
				this->moveVec.x = 0.0f;
			}
			//プレイヤがダメージを受ける(状態が変わる)と消滅
			if (pl->state == Damage)
			{
				this->Kill();
				return;
			}
			break;
		case Jumpshoot:
		case Fallshoot:
			break;
		}
	}
	//消滅する際、状態に応じてエフェクトを生成
	//引数	：	（エフェクトを生成する座標）
	void Object::Effect_Hit(const ML::Vec2& pos_)
	{
		switch (this->state)
		{
		default:
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			this->eff->Create_Effect(1, this->pos, this->angle, this->angle_LR);
		}
	}
	//呼び出したタスクから寿命を設定する
	void Object::Set_Limit(const int& cl_)
	{
		this->limit_Erase = cl_;
	}
	//外部から生成する際、攻撃力を指定
	//引数	：	（整数値）
	void Object::Set_Power(const int& pow_)
	{
		this->power = pow_;
	}
	//外部から描画角度を指定
	//引数	：	（角度）
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
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