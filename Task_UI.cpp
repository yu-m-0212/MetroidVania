//-------------------------------------------------------------------
//UI
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_UI.h"
#include	"Task_Player.h"
#include	"Task_Enemy01.h"
#include	"Task_Display_Effect.h"
#include	"Task_Boss_Head.h"

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
		this->render2D_Priority[1] = 0.2f;						//描画順
		this->controllerName = "P1";							//コントローラー宣言
		this->cnt = 0;											//アニメーションカウンタ
		this->cnt_highllight_bar = 0;							//ボスHPバー表示時間カウンタ
		this->limit_highlight_bar = 30;							//ボスHPバー強調表示時間
		this->pos_hp_boss = ML::Vec2(64.0f, 1002.0f);			//ボスHPバー座標
		this->max_range_hp_bar = 1792;							//ボスHpの最大表示幅
		this->flag_highlight_bar = false;						//ボスHPバー強調表示フラグ
		this->eff = new Effect();								//エフェクトクラスインスタンス
		this->max_dist_quake_x_bar = 15;						//HPバーX軸揺れ最大値
		this->max_dist_quake_y_bar = 15;						//HPバーy軸揺れ最大値
		this->interval_quake_bar = 5;							//HPバー揺れ間隔
		
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
		this->cnt++;
		//HPバーハイライト
		if (this->flag_highlight_bar)
		{
			this->cnt_highllight_bar = this->limit_highlight_bar;
			this->flag_highlight_bar = false;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//画面効果中は表示しない
		auto display_effect =
			ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
		if (nullptr != display_effect) { return; }
		//不正アクセス防止
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//コントローラー宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//ベースの表示
		{
			ML::Box2D draw(0, 0, 1920, 128);
			ML::Box2D  src(0, 64, 1920, 128);
			DG::Image_Draw(this->res->hpImageName, draw, src);
		}
		//プレイヤHP枠の表示
		{
			for (int i = 0; i < pl->Get_Max_HP(); ++i)
			{
				ML::Box2D draw(128 + 64 * i, 32, 64, 64);
				ML::Box2D  src(128, 0, 64, 64);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//プレイヤのHP表示
		//バーで表示
		//ML::Box2D draw(32, 32, 32 * pl->Get_HP(), 16);
		//ボタンで表示
		{
			ML::Box2D draw;
			ML::Box2D  src(0, 0, 64, 64);
			for (int i = 0; i < pl->Get_HP(); ++i)
			{
				draw = ML::Box2D(128 + 64 * i, 32, 64, 64);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//近接攻撃ゲージの表示
		if(pl->Get_Barrier())
		{
			//バー
			{
				ML::Box2D draw(1589, 53, pl->Get_Gauge_Mlee() * 3, 24);
				ML::Box2D  src(64, 0, 64, 64);
				ML::Color  clr;
				//マックスの時、点滅する
				if (pl->Get_Gauge_Mlee() == pl->Get_Gauge_Melle_Max())
				{
					if (this->cnt % 12 == 0)
					{
						clr = ML::Color(1, 1, 1, 1);
					}
					else
					{
						clr = ML::Color(1, 0.25f, 0.5f, 1);
					}
				}
				else
				{
					clr = ML::Color(1, 0.25f, 0.5f, 1);
				}
				DG::Image_Draw(this->res->hpImageName, draw, src, clr);
			}
			//フレーム
			{
				ML::Box2D draw(0, 0, 1920, 128);
				ML::Box2D  src(0, 576, 1920, 128);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//ボスHP表示
		{
			auto boss_head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			if (nullptr != boss_head)
			{
				//フレーム表示
				ML::Box2D draw(0, 0, 1920, 1080);
				ML::Box2D  src(0, 1377, 1920, 1080);
				//揺れ効果
				if (boss_head->time_un_hit)
				{
					ML::Vec2 quake_bar = 
						this->eff->Quake
						(
							this->cnt,
							this->max_dist_quake_x_bar,
							this->max_dist_quake_y_bar, 
							this->interval_quake_bar
						);
					draw.x = int(0 + quake_bar.x);
					draw.y = int(0 + quake_bar.y);
				}
				DG::Image_Draw(this->res->hpImageName, draw, src);
				//反射に対応したフレーム効果の表示
				if (this->cnt_highllight_bar)
				{
					//表示用カウンタデクリメント
					this->cnt_highllight_bar--;
					ML::Box2D  src(0, 2457, 1920, 1080);
					//グラデーション
					this->cnt_gradation_bar += 0.1f;
					float highlight_red_green =
						sinf(this->cnt_gradation_bar);
					if (highlight_red_green < 0.0f)
					{
						highlight_red_green = 0.0f;
					}
					ML::Color gradation(1.0f, highlight_red_green, highlight_red_green, 1.0f);
					DG::Image_Draw(this->res->hpImageName, draw, src, gradation);
				}
				//HPバー本体表示
				{
					ML::Box2D draw
					(
						+ 0,
						-16,
						+(boss_head->hp+1)*(this->max_range_hp_bar / HP_BOSS_HEAD),//現在のHP*(最大表示幅/ボスHP最大値)
						+32
					);
					draw.Offset(this->pos_hp_boss);
					ML::Box2D  src(64, 0, 64, 64);
					//色は仮処理
					//長さ全体のリソースを用意しそこでアニメーション
					//表示幅のみをプログラム側から調整すること
					DG::Image_Draw(this->res->hpImageName, draw, src,ML::Color(1,1,0,0));
				}
			}
		}
		//以下デバッグ----------------------------------------------------
		if (ge->debugMode)
		{
			//共通背景リソース
			ML::Box2D  src(64, 0, 64, 64);
			auto es = ge->GetTask_One_G<Enemy01::Object>(Enemy01::defGroupName);
			ML::Box2D debugBox01(825, 75, 625, 450);
			string debugText01 =
				"state = " + to_string(pl->state) + "\n" +
				"pos.x = " + to_string(pl->pos.x) + "\n" +
				"pos.y = " + to_string(pl->pos.y) + "\n" +
				"moveVec.x = " + to_string(pl->moveVec.x) + "\n" +
				"moveVec.y = " + to_string(pl->moveVec.y) + "\n" +
				"angle = " + to_string(pl->angle_LR) + "\n"
				"moveCnt = " + to_string(pl->moveCnt) + "\n" +
				"time_un_hit = " + to_string(pl->time_un_hit) + "\n" +
				"hp=" + to_string(pl->hp) + "\n" +
				"ge->clear = " + to_string(ge->clear) + "\n" +
				"ge->failure = " + to_string(ge->failure) + "\n" +
				"Search_Player() = " + to_string(es->Search_Player()) + "\n" +
				"BackSpace/Selectボタンでデバッグモード";
			DG::Image_Draw(this->res->hpImageName, debugBox01, src, ML::Color(0.7f, 0, 0, 0));
			DG::Font_Draw("fontUI", debugBox01, debugText01, ML::Color(1, 1, 1, 1));
			ML::Box2D debugBox02(1450, 75, 600, 100);
			string debugText02 =
				"RStick.axis.x = " + to_string(in.RStick.axis.x) + "\n" +
				"RStick.axis.y = " + to_string(in.RStick.axis.y) + "\n" +
				"angle = " + to_string(ML::ToDegree(atan2(in.RStick.axis.y, in.RStick.axis.x)));
			DG::Image_Draw(this->res->hpImageName, debugBox02, src, ML::Color(0.7f, 0, 0, 0));
			DG::Font_Draw("fontUI", debugBox02, debugText02, ML::Color(1, 1, 1, 1));
			//タスク一覧の背景
			{
				ML::Box2D draw_back_tasks(0, 0, 150, ge->screenHeight);
				DG::Image_Draw(this->res->hpImageName, draw_back_tasks, src, ML::Color(0.7f,0.0f,0.0f,0.0f));
			}
		}
	}
	//HPバーの強調表示を外部から指定する
	void Object::Set_Highlight_Bar()
	{
		this->flag_highlight_bar = true;
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