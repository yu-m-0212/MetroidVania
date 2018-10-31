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
#include  "Task_Gun.h"
#include  "Task_Display_Effect.h"

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
		ge->camera2D = ML::Box2D(-960, -540, 1920, 1080);	//カメラ初期化
		ge->clear = false;									//クリアフラグ初期化
		ge->failure = false;								//ミスフラグ初期化
		ge->pause = false;									//ポーズフラグの初期化
		this->cnt_transition = 0;							//カウンタ遷移用
		this->time_create_next_task = 100;					//引継ぎタスクの生成タイミング
		this->time_kill_game = 200;							//自身を消滅させるタイミング
		
		//★タスクを常駐させる
		this->shot00_Resource = Shot00::Resource::Create();
		this->shot01_Resource = Shot01::Resource::Create();
		this->effect_Resource = Task_Effect::Resource::Create();

		//★タスクの生成
		//マップの生成
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		////プレイヤの生成
		auto  pl = Player::Object::Create(true);
		pl->pos = ML::Vec2(224.0f, 4400.0f);
		////妖精の生成
		auto  spr = Sprite::Object::Create(true);
		spr->Set_Target(pl);
		spr->pos = pl->pos;
		//UIの生成
		auto ui = UI::Object::Create(true);
		//チュートリアルの生成
		this->Create_Message(ML::Vec2( 224, 4482), "左スティックを横に倒すと移動");
		this->Create_Message(ML::Vec2(1180, 4482), "×ボタンでジャンプ");
		this->Create_Message(ML::Vec2(2130, 3970), "R1ボタンでショット");
		this->Create_Message(ML::Vec2(6792, 3650), "右スティックで銃口を傾ける");
		this->Create_Message(ML::Vec2(7192, 5122), "□ボタンで敵や敵のショットを弾く");
		this->Create_Message(ML::Vec2(4823, 5378), "遺体に触れると回復する");
		//背景の生成
		auto back = Back::Object::Create(true);
		//チュートリアル用の遺体を配置
		auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(4730,5394);
		corpse->angle_LR = BChara::Angle_LR::Left;
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("タイトル"); 

		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("プレイヤ頭部");
		ge->KillAll_G("ガン");
		ge->KillAll_G("弾(プレイヤ)");
		ge->KillAll_G("オプション");

		ge->KillAll_G("敵");
		ge->KillAll_G("敵オプション");
		ge->KillAll_G("弾(エネミー)");

		ge->KillAll_G("フィールド");

		ge->KillAll_G("遺体");
		ge->KillAll_G("ゴール");
		ge->KillAll_G("アイテム");
		ge->KillAll_G("チュートリアル");

		ge->KillAll_G("UI");
		ge->KillAll_G("背景");
		ge->KillAll_G("ポーズ");
		//★リソースを常駐を解除する（書かなくても勝手に解除される）
		this->shot00_Resource.reset();
		this->shot01_Resource.reset();
		this->effect_Resource.reset();
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
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
		//Selectボタンでデバッグモード
		if (in.SE.down)
		{
			ge->DebugMode(!ge->debugMode);
		}
		//ゲームオーバー
		if (ge->failure)
		{
			//フェードイン
			if (this->cnt_transition == 0)
			{
				auto display_effect = Display_Effect::Object::Create(true);
				display_effect->Set_Next_Scene(1);
			}
			this->cnt_transition++;
			//リトライ生成
			if (this->cnt_transition == this->time_create_next_task)
			{
				auto pl = ge->GetTask_One_G < Player::Object>("プレイヤ");
				if (nullptr != pl)
				{
					auto retry = Retry::Object::Create(true);
					//死亡時の座標
					this->pos_dead = pl->pos;
					retry->Set_Pos_Dead(this->pos_dead);
					//死亡時の向き
					this->angle_dead = pl->angle_LR;
					retry->Set_Angle_Dead(this->angle_dead);
				}
			}
			//一定時間で自身を消滅させる
			else if (this->cnt_transition > this->time_kill_game)
			{
				this->Kill();
			}
		}
		//ゲームクリア
		if (ge->clear)
		{
			//フェードイン
			if (this->cnt_transition == 0)
			{
				auto display_effect = Display_Effect::Object::Create(true);
				display_effect->Set_Next_Scene(2);
			}
			this->cnt_transition++;
			//一定時間で自身を消滅させる
			if (this->cnt_transition > this->time_kill_game)
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//チュートリアル表示キャラクタの生成
	//引数	：	（座標,メッセージ）
	void Object::Create_Message(const ML::Vec2& pos_, const string& message_)
	{
		auto tutorials = Tutorials::Object::Create(true);
		tutorials->pos = pos_;
		tutorials->Set_Message(message_);
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