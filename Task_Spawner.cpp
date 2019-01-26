//-------------------------------------------------------------------
//スポナー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Spawner.h"

#include  "Task_Boss_Head.h"
#include  "Task_Boss_Upper.h"
#include  "Task_Boss_Upper_Middle.h"
#include  "Task_Boss_Center.h"
#include  "Task_Boss_Lower.h"

#include  "Task_Display_Effect.h"

#include  "Task_Player.h"
#include  "Task_Sprite.h"

namespace  Spawner
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "image";
		DG::Image_Create(this->name_image, "./data/image/UI.png");

		this->name_bgm_boss = "bgm_boss";
		DM::Sound_CreateStream(this->name_bgm_boss, "./data/sound/wav/bgm_boss.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DM::Sound_Erase(this->name_bgm_boss);
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
		this->hitBase = ML::Box2D(-512, -1, 1024, 2);		//表示矩形
		this->recieveBase = this->hitBase;					//判定矩形
		this->time_create_boss = 180;						//ボスの生成タイミング
		this->time_bgm = 240;								//ボスBGMの再生開始時間
		this->flag_spawn_boss = false;						//ボス出現フラグ
		this->init_pos_boss = ML::Vec2(8382.0f, 7800.0f);	//ボスの初期座標
		this->eff = new Task_Effect::Object();				//エフェクトインスタンス
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->eff;
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//プレイヤと接触でフラグ成立
		auto pl =
			ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//判定矩形を用意
		ML::Box2D pl_base = pl->hitBase;
		pl_base.Offset(pl->pos);
		if (this->flag_spawn_boss == false &&
			this->CheckHit(pl_base))
		{
			this->flag_spawn_boss = true;
			//レターボックスの生成
			auto display_effect =
				ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr != display_effect) { return; }
			display_effect->Create_Display_Effect(1);
		}
		//フラグ成立後の処理
		if (!this->flag_spawn_boss) { return; }
		//行動カウンタのインクリメント
		if (this->moveCnt <= this->time_bgm)
		{
			this->moveCnt++;
		}
		//ボスの生成
		if (this->moveCnt == this->time_create_boss)
		{
			//ボスの生成
			auto boss_head = Boss_Head::Object::Create(true);
			boss_head->pos = this->init_pos_boss;
			auto boss_upper = Boss_Upper::Object::Create(true);
			boss_upper->pos = ML::Vec2(boss_head->pos.x, boss_head->pos.y + float(boss_upper->hitBase.h));
			auto boss_upper_middle = Boss_Upper_Middle::Object::Create(true);
			boss_upper_middle->pos = ML::Vec2(boss_upper->pos.x, boss_upper->pos.y + float(boss_upper_middle->hitBase.h));
			auto boss_center = Boss_Center::Object::Create(true);
			boss_center->pos = ML::Vec2(boss_upper_middle->pos.x, boss_upper_middle->pos.y + float(boss_center->hitBase.h));
			auto boss_lower = Boss_Lower::Object::Create(true);
			boss_lower->pos = ML::Vec2(boss_center->pos.x, boss_center->pos.y + float(boss_lower->hitBase.h));
			//演出中はカメラをボスに追従させる
			auto sp =
				ge->GetTask_One_G<Sprite::Object>(Sprite::defGroupName);
			sp->Set_Target(boss_head);
			//警告エフェクトの生成
			//生成位置を宣言
			ML::Vec2 init_pos_target_effect =
				ML::Vec2(boss_head->pos.x, boss_head->pos.y - boss_head->hitBase.h / 2);
			this->eff->Create_Effect(9, init_pos_target_effect);
			this->eff->Create_Effect(10, init_pos_target_effect);
			this->eff->Create_Effect(11, init_pos_target_effect);
		}
		//ボスBGMの再生
		if (this->moveCnt == this->time_bgm)
		{
			DM::Sound_Play_Volume(this->res->name_bgm_boss,true,VOLUME_BGM_BOSS);
			//ボスに追従させていたカメラをプレイヤに戻す
			auto sp =
				ge->GetTask_One_G<Sprite::Object>(Sprite::defGroupName);
			auto pl =
				ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			sp->Set_Target(pl);
		}
		//ゲームクリアかリトライでBGMフェードアウト
		if (ge->clear)
		{
			DM::Sound_FadeOut(this->res->name_bgm_boss);
		}
		if (ge->failure)
		{
			DM::Sound_FadeOut(this->res->name_bgm_boss);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//デバッグモードの時、判定矩形を表示
		if (ge->debugMode)
		{
			ML::Box2D draw = this->hitBase;
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->name_image, draw, src, ML::Color(0.5f, 0.0f, 1.0f, 0.0f));
		}
	}
	//ボスの出現フラグを返す
	bool Object::Get_Flag_Spawn_Boss()
	{
		return this->flag_spawn_boss;
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