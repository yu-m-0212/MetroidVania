//-------------------------------------------------------------------
//画面効果
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Display_Effect.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Display_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Display_Effect";
		DG::Image_Create(this->imageName, "./data/image/Display_Effect.png");
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
		this->render2D_Priority[1] = 0.1f;	//描画順
		this->transparency = 0.0f;			//透明度
		this->flag_in_out = 0;				//0=in 1=out
		this->cnt_transition = 0;			//画面遷移カウンタ
		this->time_create_next_task = 100;	//次のタスクを生成するタイミング
		this->time_start_fade_out = 200;	//フェードアウトを始めるタイミング
		this->next_scene = 0;				//引継ぎタスクを指定する変数
		
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

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->cnt_transition++;
		//フェードイン
		if (!this->flag_in_out)
		{
			this->transparency += 0.01f;
		}
		//一定時間でフラグを反転し、引継ぎタスクを生成する
		if (this->cnt_transition == this->time_create_next_task)
		{
			this->flag_in_out = !this->flag_in_out;
			switch (this->next_scene)
			{
			default:
				break;
			//Game
			case 0:
				Game::Object::Create(true);
				break;
			//Retry
			case 1:
				break;
			//Ending
			case 2:
				Ending::Object::Create(true);
				break;
			//Title
			case 3:
				Title::Object::Create(true);
				break;
			}
		}
		//一定時間でフェードアウトを始める
		if (this->flag_in_out && this->cnt_transition>this->time_start_fade_out)
		{
			this->transparency -= 0.01f;
		}
		//透明になったら自身を消滅させる
		if (this->transparency < 0.0f)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 1920, 1080);
		ML::Box2D  src(0, 0, 1920, 1080);
		DG::Image_Draw(this->res->imageName, draw, src,ML::Color(this->transparency,1,1,1));
	}
	//引継ぎタスクを指定する
	//引数	：	（整数）
	//0=Game	1=Retry		2=Ending	3=Title
	void Object::Set_Next_Scene(const int& next_)
	{
		this->next_scene = next_;
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