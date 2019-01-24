//-------------------------------------------------------------------
//リトライ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Retry.h"
#include  "Task_Game.h"
#include  "Task_Title.h"
#include  "Task_Corpse.h"
#include  "Task_Display_Effect.h"

using namespace ML;

namespace  Retry
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->back_retry = "back_retry";
		DG::Image_Create(this->back_retry, "./data/image/Retry.png");
		this->button_retry = "button_retry";
		DG::Image_Create(this->button_retry, "./data/image/ui.png");
		DG::Font_Create("fontRetry", "HG丸ｺﾞｼｯｸM-PRO", 8, 16);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->back_retry);
		DG::Image_Erase(this->button_retry);
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
		this->render2D_Priority[1] = 0.4f;							//描画順
		this->controllerName = "P1";								//コントローラ宣言
		this->flag_transition = false;								//画面遷移用フラグ
		this->cnt_transition = 0;									//カウンタ遷移用
		this->title_or_game = 0;									//引継ぎタスクの選択フラグ(0=Game,1=Title)
		this->time_create_next_task = 100;							//引継ぎタスクの生成タイミング
		this->time_kill_game = 100;									//自身を消滅させるタイミング
		this->cnt_create_bubble = 0;								//エフェクトの生成カウンタ
		this->cnt_available_controll = 0;							//生成後、操作を受け付けるまでのカウンタ
		this->time_available_controll = 150;						//生成後、操作を受け付けるまでの時間
		this->cnt_anim_back = 0.0f;									//背景アニメカウンタ
		this->interval_anim_back = 25.0f;							//背景アニメ周期
		this->posY = -360.0f;										//背景Y軸座標
		this->posY_std = -240.0f;									//背景Y軸座標基準値
		this->height_anim_back = 30.0f;								//背景アニメ揺れ幅
		this->init_bubble_pos_y = float(ge->screenHeight + 96.0f);	//泡のY軸座標初期位置
		this->eff = new Task_Effect::Object();						//メソッド呼び出し
		ge->pause = true;											//生成時、操作を受け付けない

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		ge->KillAll_G("エフェクト");
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
		this->cnt_create_bubble++;
		this->cnt_anim_back+=1.0f;
		//一定時間、操作を受け付けない
		if (this->cnt_available_controll < this->time_available_controll)
		{
			this->cnt_available_controll++;
		}
		else
		{
			ge->pause = false;
		}

		auto in = DI::GPad_GetState(this->controllerName);

		//エフェクトの生成
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			this->eff->Create_Effect(5, ML::Vec2(initX, this->init_bubble_pos_y));
		}
		//背景アニメーション
		float y = this->posY_std + sinf(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back;
		this->posY = y;
		//生成後、フェードアウトが終わってから操作受付け
		if (ge->pause) { return; }
		//画面遷移
		if (!this->flag_transition)
		{
			//本編を再開する場合
			if (in.ST.down)
			{
				this->flag_transition = true;
				auto display_effect = ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				if (nullptr == display_effect)
				{
					display_effect->Create_Display_Effect(0);
				}
				this->title_or_game = 0;
			}
			//タイトルに戻る場合
			else if (in.SE.down)
			{
				this->flag_transition = true;
				auto display_effect = ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				if (nullptr == display_effect)
				{
					display_effect->Create_Display_Effect(0);
				}
				this->title_or_game = 1;
			}
		}
		//消滅カウントダウン
		else
		{
			this->cnt_transition++;
			if (this->title_or_game == 0)
			{
				//画面遷移時間になったら本編生成
				if (this->cnt_transition == this->time_create_next_task)
				{
					Game::Object::Create(true);
					//リトライする場合は前回の死亡地点に遺体を設置
					auto corpse = Corpse::Object::Create(true);
					corpse->pos = this->pos_dead;
					corpse->angle_LR = this->angle_dead;
				}
			}
			//タイトルに戻る
			else if (this->title_or_game == 1)
			{
				//画面遷移時間になったらタイトル生成
				if (this->cnt_transition == this->time_create_next_task)
				{
					Title::Object::Create(true);
				}
			}
		}
		//一定時間で消滅する
		if (this->cnt_transition > this->time_kill_game)
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
			DG::Image_Draw(this->res->back_retry, draw, src);
		}
		Box2D textBox(ge->screenWidth / 2 - 125, ge->screenHeight / 2, 250, 250);
		string	 textJp = "スタートボタンで続行\nセレクトボタンでタイトルへ戻る";
		DG::Font_Draw("fontRetry", textBox, textJp, Color(1.0f, 0.0f, 0.0f, 0.0f));
	}
	//死亡した座標を保存する
	//引数	：	（Vec2)
	void Object::Set_Pos_Dead(const Vec2& dead_)
	{
		this->pos_dead = dead_;
	}
	//前回死亡した座標を返す
	ML::Vec2 Object::Get_Pos_Dead()
	{
		return this->pos_dead;
	}
	//死亡時の向きを指定する
	//引数	：	（BChara::Angle_LR)
	void Object::Set_Angle_Dead(const BChara::Angle_LR& angle_)
	{
		this->angle_dead = angle_;
	}
	//死亡時の向きを取得する
	BChara::Angle_LR Object::Get_Angle_Dead()
	{
		return this->angle_dead;
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