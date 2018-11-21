//-------------------------------------------------------------------
//ガン
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Gun.h"
#include	"Task_Player.h"

namespace  Gun
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "GunImage";
		DG::Image_Create(this->imageName, "./data/image/player.png");
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
		this->render2D_Priority[1] = 0.4f;				//描画順
		this->hitBase = ML::Box2D(-69, -46, 138, 92);	//表示矩形
		this->controllerName = "P1";					//コントローラ宣言
		this->tremor = 1.0f;							//揺れ幅
		this->angle = 0.0f;								//角度

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
		if (ge->pause) { return; }
		this->moveCnt++;
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		auto in = DI::GPad_GetState(this->controllerName);
		this->angle_LR = pl->angle_LR;
		this->pos = pl->pos;
		this->Think();
		this->Move();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//無敵中は8フレーム中4フレーム画像を表示しない（点滅する）
		if (pl->time_un_hit > 0) {
			if ((pl->time_un_hit / 4) % 2 == 0) {
				return;
			}
		}
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		//デフォルトの値を用意
		int wide = 138, height = 92;
		ML::Box2D  src(wide * 15, 0, wide, height);
		if (this->angle_LR == Left)
		{
			src.y = height * 2;
		}
		DG::Image_Rotation(this->res->imageName, this->angle,
			ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//角度を返す
	float Object::Get_Angle()
	{
		return this->angle;
	}
	//思考
	void Object::Think()
	{
		BChara::State nm = this->state; //とりあえず今の状態を指定
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//思考（入力）や状況に応じてモーションを変更することを目的としている。
		//モーションの変更以外の処理は行わない
		//モーション更新
		switch (nm)
		{
		default:
			break;
		case Stand:
			if (pl->state == Shoot) { nm = Shoot; }
			if (pl->state == Jumpshoot) { nm = Jumpshoot; }
			if (pl->state == Fallshoot) { nm = Fallshoot; }
			break;
		case Shoot:
			if (!(pl->state==Shoot)) { nm = Stand; }
			break;
		case Jumpshoot:
			if (!(pl->state==Jumpshoot)) { nm = Stand; }
			break;
		case Fallshoot:
			if (!(pl->state==Fallshoot)) { nm = Stand; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//モーションに対応した処理
	//(モーションは変更しない)
	void Object::Move()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		//共通の処理
		switch (pl->state)
		{
		default:
			this->angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
			//左向きの時は角度を加算する
			if (this->angle_LR == Left)
			{
				if (in.RStick.axis != ML::Vec2(0, 0))
				{
					this->angle += ML::ToRadian(180.0f);
				}
			}
			break;
		//例外処理（銃口を動かさない）
		case SlowDown:
		case Landing:
		case PreStomp:
		case LandStomp:
		case AirStomp:
		case StompLanding:
		case Damage:
			break;
		}
		if (nullptr == pl) { return; }
		switch (this->state)
		{
		default:
			break;
		case Stand:
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			//発砲中は上下に揺らす（発射と同じ間隔でアニメーションする）
			this->pos.y = float(pl->pos.y + sin(this->moveCnt*pl->Get_Interval_Shot()) * this->tremor);
			break;
		}
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