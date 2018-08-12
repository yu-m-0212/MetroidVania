//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_Sprite.h"
#include  "Task_Item00.h"
#include  "Task_Item01.h"
#include  "Task_Item02.h"
#include  "Task_UI.h"
#include  "Task_Back.h"
#include  "Task_Goal.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		ge->camera2D = ML::Box2D(-960, -540, 1920, 1080);//取りあえず初期値設定
		
		//★タスクを常駐させる
		this->shot00_Resource = Shot00::Resource::Create();

		//★タスクの生成
		//マップの生成
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		//プレイヤの生成
		auto  pl = Player::Object::Create(true);
		pl->pos.x = 770;
		pl->pos.y = 2200;
		//妖精の生成
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//敵の生成
		auto ene1 = Enemy01::Object::Create(true);
		ene1->pos.x = 5535.0f;
		ene1->pos.y = 3712.0f;
		ene1->hp = 5;
		//敵の生成
		auto ene2 = Enemy01::Object::Create(true);
		ene2->pos.x = 4769.0f;
		ene2->pos.y = 3712.0f;
		ene2->hp = 5;
		//敵の生成
		auto ene3 = Enemy01::Object::Create(true);
		ene3->pos.x = 3299.0f;
		ene3->pos.y = 3712.0f;
		ene3->hp = 5;
		//敵の生成
		auto ene4 = Enemy01::Object::Create(true);
		ene4->pos.x = 2339.0f;
		ene4->pos.y = 3456.0f;
		ene4->hp = 5;
		//敵の生成
		auto ene5 = Enemy01::Object::Create(true);
		ene5->pos.x = 1340.0f;
		ene5->pos.y = 3072.0f;
		ene5->hp = 5;
		//敵の生成
		auto ene6 = Enemy01::Object::Create(true);
		ene6->pos.x =  836.0f;
		ene6->pos.y = 3392.0f;
		ene6->hp = 5;
		//敵の生成
		auto ene7 = Enemy01::Object::Create(true);
		ene7->pos.x =  966.0f;
		ene7->pos.y = 3776.0f;
		ene7->hp = 5;
		//UIの生成
		auto ui = UI::Object::Create(true);
		//背景の生成
		auto back = Back::Object::Create(true);
		//仮ゴールの生成
		auto goal = Goal::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("フィールド");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("敵");
		ge->KillAll_G("オプション");
		ge->KillAll_G("アイテム");
		ge->KillAll_G("UI");
		ge->KillAll_G("エフェクト");
		ge->KillAll_G("背景");
		ge->KillAll_G("ゴール");
		//★リソースを常駐を解除する（書かなくても勝手に解除される）
		this->shot00_Resource.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto  nextTask = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (in.ST.down) {
			//自身に消滅要請
			this->Kill();
		}
		//Selectボタンでデバッグモード
		if (in.SE.down)
		{
			ge->debugMode = !ge->debugMode;
		}
		//プレイヤ死亡でエンディング

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
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