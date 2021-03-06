//-------------------------------------------------------------------
//エネミーの探知矩形表示用
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_EnemySearch.h"
#include	"Task_Enemy01.h"
namespace  EnemySearch
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "EnemySearch";
		DG::Image_Create(this->imageName, "./data/image/effect.png");
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
		this->render2D_Priority[1] = 0.3f;
		this->recieveBase = this->hitBase;
		
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
		//追従対象がいればアクセスする
		if (auto tg = this->target.lock())
		{
			this->pos = tg->pos;
			this->state = tg->state;
			this->angle_LR = tg->angle_LR;
			//エネミーの向きで座標を更新
			if (this->angle_LR == Left)
			{
				this->pos.x -= this->hitBase.w / 2;
			}
			else
			{
				this->pos.x += this->hitBase.w / 2;
			}
		}
		else
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		ML::Box2D src(960, 0, 384, 192);
		//状態によって色が変わる
		switch (this->state)
		{
		default:
			src.y = 0;
			break;
		case TargetLost:
			src.y = 192;
			break;
		case Caution:
			src.y = 384;
			break;
		case Bound:
			src.y = 384;
			break;
		}
		//左右反転
		if (this->angle_LR==Left)
		{
			draw.x = -draw.x;
			draw.w = -draw.w;
		}
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, 0.0f, ML::Vec2(float(draw.w / 2), float(draw.h / 2)));
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1.0f, 1.0f, 1.0f));
	}
	//追従対象を初期化
	void Object::Set_Target(const weak_ptr<BChara> target_)
	{
		this->target = target_;
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