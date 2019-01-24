//-------------------------------------------------------------------
//アイテム01(バリアアビリティ)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item01.h"
#include  "Task_Player.h"

namespace  Item01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "image_item01";
		DG::Image_Create(this->name_image, "./data/image/Item01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->render2D_Priority[1] = 0.7f;						//描画順
		this->limit_message = 180;								//メッセージの時間制限
		this->hitBase = ML::Box2D(-16, -16, 32, 32);			//マップとの判定矩形
		this->recieveBase = this->hitBase;						//キャラクタとの判定矩形
		this->center = ML::Vec2(float(ge->screenWidth / 2.0f),
			float(ge->screenHeight / 2.0f));					//画面の中心座標
		this->tutorials = new Tutorials::Object();				//メソッド呼び出し
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->tutorials;

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
		if (this->time_un_hit > 0) { this->time_un_hit--; }

		switch (this->state) {
		case Stand:
			this->pos.y += float(sin(this->moveCnt / 12));
			break;
		case Lose:
			this->Kill();
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->name_image, di.draw, di.src, di.color);
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Recieved(BChara* from_, AttackInfo at_,const int& un_hit_)
	{
		if (this->state != Stand)
		{
			return;
		}
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//メッセージ生成
		this->tutorials->Create_Message("バリアを取得した[□を押す]", this->center, this->limit_message);
		this->UpdateMotion(Lose);
		//バリアを有効にする
		pl->Set_Barrier(true);
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
			//draw					src						color
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1) },	//Stand[0]
		};
		BChara::DrawInfo  rtv;
		switch (this->state) 
		{
		//停止----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
		}
		return rtv;
	}
	//アイテム01の生成
	//引数	：	（初期座標）
	void Object::Create_Item01(const ML::Vec2& pos_)
	{
		auto item = Item01::Object::Create(true);
		item->pos = pos_;
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