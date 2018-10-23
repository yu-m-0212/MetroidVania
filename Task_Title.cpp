//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Effect.h"
#include  "Task_Display_Effect.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->back_title = "back";
		DG::Image_Create(this->back_title, "./data/image/Title.png");
		this->button_title = "button";
		DG::Image_Create(this->button_title, "./data/image/ui.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->back_title);
		DG::Image_Erase(this->button_title);
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
		this->render2D_Priority[1] = 1.0f;								//描画順
		this->flag_transition = false;									//画面遷移フラグ
		this->cnt_transition = 0;										//消滅カウンタ
		this->limit_transparency = 100;									//画面遷移時の消滅時間
		this->cnt_create_bubble = 0;									//エフェクトの生成カウンタ
		this->cnt_anim_button = 0.0f;									//ボタンアニメカウンタ
		this->interval_button = 2.0f;									//ボタンアニメ周期
		this->cnt_anim_back = 0;										//背景アニメカウンタ
		this->interval_anim_back = 25;									//背景アニメ周期
		this->posY = -360.0f;											//背景Y軸座標
		this->posY_std = -240.0f;										//背景Y軸座標基準値
		this->height_anim_back = 25.0f;									//背景アニメ揺れ幅
		this->init_bubble_pos_y = float(ge->screenHeight + 96.0f);		//泡のY軸座標初期位置
		this->pos_button = ML::Vec2(float(ge->screenWidth / 2), 800);	//座標ボタン
		this->controllerName = "P1";									//コントローラー宣言

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("エフェクト");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->cnt_create_bubble++;
		this->cnt_anim_back++;
		this->cnt_anim_button += 0.1f;

		auto in = DI::GPad_GetState(this->controllerName);
		//デバッグ切り替え
		if (in.SE.down) { ge->debugMode = !ge->debugMode; }

		//エフェクトの生成
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			int num = rand() % 3;
			float ang = float(rand() % 360);
			eff->Create_Bubble(num, ML::Vec2(initX, float(this->init_bubble_pos_y)), 16, 5.0f, 3.0f, ang, 600);
		}
		//自身に消滅要請
		if (in.ST.down)
		{
			this->flag_transition = true;
			auto display_effect = Display_Effect::Object::Create(true);
		}
		//背景アニメーション
		float y = this->posY_std + float(sin(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back);
		this->posY = y;
		//消滅カウントダウン
		if (this->flag_transition)
		{
			this->cnt_transition++;
		}
		//一定時間で消滅する
		if (this->cnt_transition > this->limit_transparency)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D  draw(-640, -360, 3200, 1800);
			draw.Offset(ML::Vec2(0, this->posY));
			ML::Box2D   src(0, 0, 3200, 1800);
			DG::Image_Draw(this->res->back_title, draw, src);
		}
		{
			ML::Box2D draw(-425, -125, 850, 250);
			draw.Offset(this->pos_button);
			ML::Box2D  src(0, 250, 850, 250);
			DG::Image_Draw(this->res->button_title, draw, src,ML::Color(float(sin(this->cnt_anim_button/this->interval_button)),1,1,1));
		}
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