//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Retry.h"
#include  "Task_Ending.h"
#include  "Task_UI.h"
#include  "Task_Back.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Sprite.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_Item00.h"
#include  "Task_Item01.h"
#include  "Task_Item02.h"
#include  "Task_Goal.h"
#include  "Task_EnemySearch.h"
#include  "Task_Pause.h"
#include  "Task_Tutorials.h"
#include  "Task_Corpse.h"

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
		//クリアフラグ初期化
		ge->clear = false;
		//ミスフラグ初期化
		ge->failure = false;
		//ポーズフラグの初期化
		ge->pause = false;
		
		//★タスクを常駐させる
		this->shot00_Resource = Shot00::Resource::Create();

		//★タスクの生成
		//マップの生成
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		//プレイヤの生成
		auto  pl = Player::Object::Create(true);
		pl->pos.x = 770;
		pl->pos.y = 2000;
		//妖精の生成
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//デバッグ用
		//敵の生成
		auto ene0 = Enemy01::Object::Create(true);
		ene0->pos.x = 550.0f;
		ene0->pos.y = 4200.0f;
		ene0->hp = 30;
		//敵の生成
		auto ene1 = Enemy00::Object::Create(true);
		ene1->pos.x = 5535.0f;
		ene1->pos.y = 3712.0f;
		ene1->addSpeed = 0.0f;
		ene1->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene1->hp = 5;
		//敵の生成
		auto ene2 = Enemy00::Object::Create(true);
		ene2->pos.x = 4769.0f;
		ene2->pos.y = 3712.0f;
		ene2->addSpeed = 0.0f;
		ene2->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene2->hp = 5;
		//敵の生成
		auto ene3 = Enemy00::Object::Create(true);
		ene3->pos.x = 3299.0f;
		ene3->pos.y = 3712.0f;
		ene3->addSpeed = 0.0f;
		ene3->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene3->hp = 5;
		//敵の生成
		auto ene4 = Enemy00::Object::Create(true);
		ene4->pos.x = 2339.0f;
		ene4->pos.y = 3456.0f;
		ene4->addSpeed = 0.0f;
		ene4->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene4->hp = 5;
		//敵の生成
		auto ene5 = Enemy00::Object::Create(true);
		ene5->pos.x = 1340.0f;
		ene5->pos.y = 3072.0f;
		ene5->addSpeed = 0.0f;
		ene5->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene5->hp = 5;
		//敵の生成
		auto ene6 = Enemy00::Object::Create(true);
		ene6->pos.x =  836.0f;
		ene6->pos.y = 3392.0f;
		ene6->addSpeed = 0.0f;
		ene6->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene6->hp = 5;
		//敵の生成
		auto ene7 = Enemy00::Object::Create(true);
		ene7->pos.x =  966.0f;
		ene7->pos.y = 3776.0f;
		ene7->addSpeed = 0.0f;
		ene7->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene7->hp = 5;
		//UIの生成
		auto ui = UI::Object::Create(true);
		//チュートリアル01の生成
		auto tutorials01 = Tutorials::Object::Create(true);
		tutorials01->pos = ML::Vec2(770, 2624);
		tutorials01->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials01->Set_Message("左スティックを横に倒すと移動");
		//チュートリアル02の生成
		auto tutorials02 = Tutorials::Object::Create(true);
		tutorials02->pos = ML::Vec2(1857, 2624);
		tutorials02->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials02->Set_Message("×ボタンでジャンプ");
		//チュートリアル03の生成
		auto tutorials03 = Tutorials::Object::Create(true);
		tutorials03->pos = ML::Vec2(4032, 2624);
		tutorials03->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials03->Set_Message("×ボタン長押しで高くジャンプ");
		//チュートリアル04の生成
		auto tutorials04 = Tutorials::Object::Create(true);
		tutorials04->pos = ML::Vec2(5570, 1920);
		tutorials04->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials04->Set_Message("遺体に触れると回復する");
		//チュートリアル05の生成
		auto tutorials05 = Tutorials::Object::Create(true);
		tutorials05->pos = ML::Vec2(6210, 3648);
		tutorials05->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials05->Set_Message("□ボタンでパンチ");
		//チュートリアル06の生成
		auto tutorials06 = Tutorials::Object::Create(true);
		tutorials06->pos = ML::Vec2(3774, 3648);
		tutorials06->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials06->Set_Message("R1ボタンでショット");
		//チュートリアル07の生成
		auto tutorials07 = Tutorials::Object::Create(true);
		tutorials07->pos = ML::Vec2(2688, 3648);
		tutorials07->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials07->Set_Message("空中にいるとき□ボタンで空中攻撃");
		//チュートリアル08の生成
		auto tutorials08 = Tutorials::Object::Create(true);
		tutorials08->pos = ML::Vec2(1600, 3136);
		tutorials08->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials08->Set_Message("空中にいるときR1ボタンで空中ショット");
		//チュートリアル09の生成
		auto tutorials09 = Tutorials::Object::Create(true);
		tutorials09->pos = ML::Vec2(1088, 3008);
		tutorials09->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials09->Set_Message("△ボタンで踏みつけ");
		//背景の生成
		auto back = Back::Object::Create(true);
		//仮ゴールの生成
		auto goal = Goal::Object::Create(true);
		//チュートリアル用の遺体を配置
		auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(5955, 1984);
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
		ge->KillAll_G("敵オプション");
		ge->KillAll_G("弾");
		ge->KillAll_G("オプション");
		ge->KillAll_G("アイテム");
		ge->KillAll_G("UI");
		ge->KillAll_G("エフェクト");
		ge->KillAll_G("背景");
		ge->KillAll_G("ゴール");
		ge->KillAll_G("遺体");
		ge->KillAll_G("シーン");
		ge->KillAll_G("チュートリアル");
		//★リソースを常駐を解除する（書かなくても勝手に解除される）
		this->shot00_Resource.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto in = DI::GPad_GetState("P1");
			//クリア画面に移る
			if (ge->clear)
			{
				auto ending = Ending::Object::Create(true);
			}
			//リトライ画面に移る
			else if (ge->failure)
			{
				auto retry = Retry::Object::Create(true);
				//死亡地点を渡す
				retry->Set_DeadPos(this->deadPos);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (!ge->pause&&in.ST.down)
		{
			//スタートでポーズ
			ge->pause = true;
			auto pause = Pause::Object::Create(true);
		}
		//ポーズ中にスタートでポーズ解除
		else if (ge->pause&&in.ST.down)
		{
			ge->pause = false;
		}
		if (ge->clear)
		{
			this->Kill();
		}
		if (ge->failure)
		{
			this->Kill();
			//死亡地点を保存
			auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
			this->deadPos = pl->pos;
		}
		//Selectボタンでデバッグモード
		if (in.SE.down)
		{
			ge->debugMode = !ge->debugMode;
		}
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