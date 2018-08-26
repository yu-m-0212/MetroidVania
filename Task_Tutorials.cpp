//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorials.h"
#include  "Task_Player.h"

namespace  Tutorials
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "imageName";
		DG::Image_Create(this->imageName, "./data/image/UI.png");
		this->fontName = "tutorialsFont";
		DG::Font_Create(this->fontName, "ＭＳ ゴシック", 16,32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase(this->fontName);
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
		this->message = "このメッセージが見れるのはおかしいよ";
		
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//プレイヤと接触したら画面下にメッセージを表示
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		ML::Box2D you = pl->recieveBase;
		you.Offset(pl->pos);
		if (this->CheckHit(you))
		{
			//判定矩形の下に表示
			ML::Box2D draw(int(this->pos.x - this->message.length()*16/2),
				int(this->pos.y + this->recieveBase.h / 2), 
				this->message.length()*16, 32);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//メッセージボックスを表示
			ML::Box2D  src(32, 0, 32, 32);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
			//メッセージを表示
			DG::Font_Draw(this->res->fontName, draw, this->message,ML::Color(1.0f,1.0f,1.0f,1.0f));
		}
		if (!ge->debugMode) { return; }
		//デバッグモードの時、判定矩形を表示
		{
			ML::Box2D draw = this->recieveBase;
			ML::Box2D  src(32, 0, 32, 32);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 0.0f, 0.0f, 1.0f));
		}
	}
	//外部から生成する際、状態を指定する
	void Object::Set_Message(const string& message_)
	{
		this->message = message_;
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