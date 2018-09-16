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
		this->render2D_Priority[1] = 0.4f;
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		this->pos = pl->pos;
		this->hitBase = ML::Box2D(-64, -32, 128, 64);
		this->controllerName = "P1";
		this->tremor = 3.0f;

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
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		auto in = DI::GPad_GetState(this->controllerName);
		this->angle_LR = pl->angle_LR;
		
		this->pos = pl->pos;
		this->Think();
		this->Move();
		this->angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
		if (this->angle_LR == Right)
		{
			if (ML::ToDegree(this->angle) > 45.0)
			{
				this->angle = ML::ToRadian(45.0f);
			}
			else if (ML::ToDegree(this->angle) < -45.0f)
			{
				this->angle = ML::ToRadian(-45.0f);
			}
		}
		else
		{
			if (in.RStick.axis != ML::Vec2(0, 0))
			{
				this->angle += ML::ToRadian(180.0f);
				if (ML::ToDegree(this->angle) > 45.0f && ML::ToDegree(this->angle)<180.0f)
				{
					this->angle = ML::ToRadian(45.0f);
				}
				else if (ML::ToDegree(this->angle) < 315.0f &&
					!(ML::ToDegree(this->angle) > 0.0f && ML::ToDegree(this->angle)<45.0f))
				{
					this->angle = ML::ToRadian(315.0f);
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D  src(1216, 0, 64, 32);
		if (this->angle_LR == Left)
		{
			src.y = 32;
		}
		DG::Image_Rotation(this->res->imageName, this->angle, 
			ML::Vec2(float(this->hitBase.w/2), float(this->hitBase.h/2)));
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//プレイヤが消滅したとき、プレイヤ側からKill
	void Object::Gun_Kill()
	{
		this->Kill();
	}
	//
	float Object::Get_Angle()
	{
		return this->angle;
	}
	//思考
	void Object::Think()
	{
		BChara::State nm = this->state; //とりあえず今の状態を指定
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
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
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		switch (this->state) 
		{
		default:
			break;
		case Stand:
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			//発砲中は上下に揺らす
			this->pos.y = float(pl->pos.y + sin(this->moveCnt) * this->tremor);
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