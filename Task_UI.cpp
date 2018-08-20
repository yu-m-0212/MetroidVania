//-------------------------------------------------------------------
//UI
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_UI.h"
#include	"Task_Player.h"
#include	"Task_Enemy01.h"

namespace  UI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->hpImageName = "hpImage";
		DG::Image_Create(this->hpImageName, "./data/image/ui.png");
		DG::Font_Create("fontUI", "ＭＳ ゴシック", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->hpImageName);
		DG::Font_Erase("fontUI");
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
		//HPボタン表示用
		/*for (int i = 0; i < 10; ++i)
		{
			this->playerHp[i].active = true;
			this->playerHp[i].x = 0;
			this->playerHp[i].y = 32;
		}*/
		
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
		//HPのボタン表示
		//auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//for (int i = 0; i < pl->Get_HP(); ++i)
		//{
		//	this->playerHp[i].active = true;
		//}
		//for (int j = pl->Get_Max_HP(); j > pl->Get_HP(); --j)
		//{
		//	this->playerHp[j].active = false;
		//}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		auto es = ge->GetTask_One_G<Enemy01::Object>("敵");
		if (ge->debugMode)
		{
			ML::Box2D debugBox(200, 100, 600, 600);
			string debugText =
				"state = " + to_string(pl->state) + "\n" +
				"pos.x = " + to_string(pl->pos.x) + "\n" +
				"pos.y = " + to_string(pl->pos.y) + "\n" +
				"moveVec.x = " + to_string(pl->moveVec.x) + "\n" +
				"moveVec.y = " + to_string(pl->moveVec.y) + "\n" +
				"angle = " + to_string(pl->angle_LR) + "\n"
				"moveCnt = " + to_string(pl->moveCnt) + "\n" +
				"unHitTime = " + to_string(pl->unHitTime) + "\n" +
				"hp=" + to_string(pl->hp) + "\n" +
				"ge->clear = " + to_string(ge->clear) + "\n" +
				"ge->failure = " + to_string(ge->failure) + "\n" +
				"Search_Player() = " + to_string(es->Search_Player()) + "\n" +
				"BackSpace/Selectボタンでデバッグモード切替";
			DG::Font_Draw("fontUI", debugBox, debugText, ML::Color(1, 1, 1, 1));
		}
		//以上デバッグ----------------------------------------------------
		//プレイヤのHP表示
		//ボタン表示板
		//for (int i = 0; i < pl->Get_HP(); ++i)
		//{
		//	if (this->playerHp[i].active)
		//	{
		//		ML::Box2D draw(32 + 32 * i, 32, 32, 32);
		//		//デバッグ時は表示をずらす
		//		if (ge->debugMode)
		//		{
		//			draw.x += 100;
		//		}
		//		ML::Box2D src(0, 0, 32, 32);
		//		//残り体力によって色を指定する
		//		float red = 1.0f - 0.1f * i;
		//		float blue = 0.1f + 0.1f * i;
		//		DG::Image_Draw(this->res->hpImageName, draw, src, ML::Color(1.0f, red, 0.0f, blue));
		//	}
		//}
		//プレイヤのHP表示
		//バーで表示
		ML::Box2D draw(32, 32, 32 * pl->Get_HP(), 16);
		//デバッグ時は表示をずらす
		if (ge->debugMode)
		{
			draw.x += 100;
		}
		ML::Box2D  src(32, 0, 32, 32);
		DG::Image_Draw(this->res->hpImageName, draw, src);
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