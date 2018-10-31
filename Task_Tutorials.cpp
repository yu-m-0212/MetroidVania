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
		DG::Font_Create(this->fontName, "ＭＳ ゴシック", FONT_WIDTH,FONT_HIGHT);
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
		this->render2D_Priority[1] = 0.2f;						//描画順
		this->message = "このメッセージが見れるのはおかしいよ";		//メッセージ
		this->recieveBase = ML::Box2D(-128, -64, 256, 128);		//矩形
		this->limit = -1;										//消滅までの時間
		
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
		//時間制限付きで呼ばれた場合、一度だけポーズをかける
		if (this->limit != -1)
		{
			if (!ge->pause)
			{
				ge->pause = true;
			}
		}
		//外部から表示時間を指定した場合、消滅する
		if (this->limit != -1)
		{
			this->limit--;
			if (this->limit == 0)
			{
				//ポーズ解除
				ge->pause = false;
				//消滅
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->debugMode)
		{
			//デバッグモードの時、判定矩形を表示
			ML::Box2D draw = this->recieveBase;
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 0.0f, 0.0f, 1.0f));
		}
		//プレイヤと接触したら画面下にメッセージを表示
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (nullptr != pl)
		{
			//リソースの指定
			ML::Box2D  src(64, 0, 64, 64);
			//プレイヤの判定矩形
			ML::Box2D you = pl->recieveBase;
			you.Offset(pl->pos);
			//消滅時間無制限の場合
			if (this->limit == -1)
			{
				if (this->CheckHit(you))
				{
					//判定矩形の下に表示
					ML::Box2D draw(int(this->pos.x - this->message.length()*FONT_WIDTH / 2),
						int(this->pos.y + this->recieveBase.h / 2),
						this->message.length()*FONT_WIDTH, FONT_HIGHT);
					draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
					//メッセージボックスを表示
					DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
					//メッセージを表示
					DG::Font_Draw(this->res->fontName, draw, this->message, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			//時間制限付きの場合、常に表示
			else
			{
				//プレイヤの上に表示
				ML::Box2D draw(int(pl->pos.x - this->message.length()*FONT_WIDTH / 2),
					int(pl->pos.y), this->message.length()*FONT_WIDTH, FONT_HIGHT);
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				//メッセージボックスの表示
				DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
				//メッセージを表示
				DG::Font_Draw(this->res->fontName, draw, this->message, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	//外部から生成する際、状態を指定する
	void Object::Set_Message(const string& message_)
	{
		this->message = message_;
	}
	//消滅時間を指定する
	//引数	：	（消滅までの時間）
	void Object::Set_Limit(const int& limit_)
	{
		this->limit = limit_;
	}
	//メッセージを生成する
	//引数	：	（メッセージ,座標,消滅時間）
	void Object::Create_Message(const string& message_,const ML::Vec2& pos_, const int& limit_)
	{
		auto message = Tutorials::Object::Create(true);
		message->Set_Message(message_);
		message->pos = pos_;
		message->Set_Limit(limit_);
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