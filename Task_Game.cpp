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
		pl->pos = ML::Vec2(383.0f, 1100.0f);
		//妖精の生成
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//エネミー00の生成
		/*this->Create_Enemy(ML::Vec2(5535.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(4769.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(3299.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(2339.0f, 3456.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(1340.0f, 3072.0f), 0.0f, 5);*/
		//エネミー01の生成
		/*auto ene0 = Enemy01::Object::Create(true);
		ene0->pos.x = 550.0f;
		ene0->pos.y = 4200.0f;
		ene0->hp = 30;*/
		//UIの生成
		auto ui = UI::Object::Create(true);
		//チュートリアルの生成
		this->Create_Message(ML::Vec2( 389, 1312), "左スティックを横に倒すと移動");
		this->Create_Message(ML::Vec2( 929, 1312), "×ボタンでジャンプ");
		this->Create_Message(ML::Vec2(2016, 1312), "×ボタン長押しで高くジャンプ");
		this->Create_Message(ML::Vec2(2787,  800), "遺体に触れると回復する");
		this->Create_Message(ML::Vec2(3040, 1888), "R1ボタンでショット");
		this->Create_Message(ML::Vec2(1440, 1888), "右スティックで銃口を傾ける");
		this->Create_Message(ML::Vec2( 384, 2272), "□ボタンで近接攻撃");

		//背景の生成
		auto back = Back::Object::Create(true);
		//ゴールの生成
		auto goal = Goal::Object::Create(true);
		//チュートリアル用の遺体を配置
		/*auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(5955, 1984);*/
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
	//チュートリアル表示キャラクタの生成
	//引数	：	（座標,メッセージ）
	void Object::Create_Message(const ML::Vec2& pos_, const string& message_)
	{
		auto tutorials = Tutorials::Object::Create(true);
		tutorials->pos = pos_;
		tutorials->Set_Message(message_);
	}
	//エネミーの生成
	//引数	：	（初期座標,移動速度,HP）
	void Object::Create_Enemy(const ML::Vec2& pos_, const float& addSpeed_, const int&hp_)
	{
		auto ene = Enemy00::Object::Create(true);
		ene->pos = pos_;
		ene->addSpeed = addSpeed_;
		ene->hp = hp_;
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