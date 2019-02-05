//-------------------------------------------------------------------
//アイテム00(体力上限アップ)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item00.h"
#include  "Task_Player.h"

namespace  Item00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Item00Img";
		DG::Image_Create(this->imageName, "./data/image/Item00.png");

		this->name_se_pick_up_ability_00 = "se_pick_up_ability00";
		DM::Sound_CreateSE(this->name_se_pick_up_ability_00, "./data/sound/wav/jingle_pick_up_ability_00.wav");
		this->name_se_pick_up_ability_01 = "se_pick_up_ability01";
		DM::Sound_CreateSE(this->name_se_pick_up_ability_01, "./data/sound/wav/jingle_pick_up_ability_01.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_se_pick_up_ability_00);
		DM::Sound_Erase(this->name_se_pick_up_ability_01);
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
		this->render2D_Priority[1] = 0.7f;													//描画順
		this->hitBase = ML::Box2D(-16, -16, 32, 32);										//マップとの判定矩形
		this->recieveBase = this->hitBase;													//キャラクタとの判定矩形
		this->add_Hp = 1;																	//プレイヤのHP増加量
		this->limit_message = 180;															//時間制限メッセージ
		this->time_play_second_se = 75;									//消滅までの時間
		this->time_erase = 180;											//自身の消滅タイミング
		this->center = ML::Vec2(float(ge->screenWidth / 2), float(ge->screenHeight / 2));	//画面中央
		this->tutorials = new Tutorials::Object();											//ポインタチュートリアル
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		
		delete this->tutorials;
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->time_un_hit > 0) { this->time_un_hit--; }

		switch (this->state) {
		case Stand:
			this->pos.y += float(sin(this->moveCnt/12));
			break;
		case Lose:
			//1つ目のSEを再生
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_se_pick_up_ability_00, false, VOLUME_ALL_GAME);
			}
			//2つ目のSEを再生し消滅
			if (this->moveCnt == this->time_play_second_se)
			{
				DM::Sound_Play_Volume(this->res->name_se_pick_up_ability_01, false, VOLUME_ALL_GAME);
			}
			//SW再生後、消滅
			if (this->moveCnt >= this->time_erase)
			{
				this->Kill();
			}
			break;
		}
		this->moveCnt++;
		this->animCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src, di.color);
	}
	//-------------------------------------------------------------------
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	（攻撃側のポインタ,攻撃情報,与無敵時間）
	void Object::Recieved(BChara* from_, AttackInfo at_, const int& un_hit_)
	{
		if (this->state != Stand) 
		{
			return;
		}
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//メッセージ生成
		this->tutorials->Create_Message("HPの上限が増加した", this->center, this->limit_message);
		this->UpdateMotion(Lose);
		//体力上限を増加し、回復する
		int max = pl->Get_Max_HP();
		max += this->add_Hp;
		pl->Set_Max_HP(max);
		pl->hp = pl->Get_Max_HP();
	}
	//-------------------------------------------------------------------
	//アニメーション制御
	BChara::DrawInfo Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
			//draw					src						color
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1)},	//Stand[0]
		};
		BChara::DrawInfo  rtv;
		switch (this->state) {
			//	停止----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
			//	消える・昇天---------------------------------------------------------------------
		case Lose:		rtv = imageTable[1];	break;
		}
		return rtv;
	}
	//アイテム00の生成
	//引数	：	（初期座標）
	void Object::Create_Item00(const ML::Vec2& pos_)
	{
		auto item = Item00::Object::Create(true);
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