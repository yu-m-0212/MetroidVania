//-------------------------------------------------------------------
//リトライ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Retry.h"
#include  "Task_Game.h"
#include  "Task_Title.h"
#include  "Task_Corpse.h"
using namespace ML;

namespace  Retry
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "retryImage";
		DG::Font_Create("fontRetry", "HG丸ｺﾞｼｯｸM-PRO", 8, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("fontRetry");
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
		this->controllerName = "P1";
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
			//リトライ
			if (in.ST.down)
			{
				auto nextTask = Game::Object::Create(true);
				//リトライする場合は前回の死亡地点に遺体を設置
				auto corpse = Corpse::Object::Create(true);
				corpse->pos = this->deadPos;
			}
			//タイトルに戻る
			else if (in.SE.down)
			{
				auto nextTask = Title::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//スタートセレクトボタンで自身を消滅
		if (in.ST.down || in.SE.down)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		Box2D textBox(ge->screenWidth / 2 - 125, ge->screenHeight / 2, 250, 250);
		string	 textJp = "スタートボタンで続行\nセレクトボタンでタイトルへ戻る";
		DG::Font_Draw("fontRetry", textBox, textJp, Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//死亡した座標を保存する
	//引数	：	（Vec2)
	void Object::Set_DeadPos(const Vec2& dead_)
	{
		this->deadPos = dead_;
	}
	//前回死亡した座標を返す
	ML::Vec2 Object::Get_DeadPos()
	{
		return this->deadPos;
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