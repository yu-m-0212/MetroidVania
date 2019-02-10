//-------------------------------------------------------------------
//画面効果
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Display_Effect.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Display_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Display_Effect";
		DG::Image_Create(this->imageName, "./data/image/Display_Effect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.1f;					//描画順
		this->transparency = 0.0f;							//透明度
		this->flag_in_out = 0;								//0=in 1=out
		this->cnt_transition = 0;							//画面遷移カウンタ
		this->time_create_next_task = 100;					//次のタスクを生成するタイミング
		this->time_start_fade_out = 150;					//フェードアウトを始めるタイミング
		this->state_display = Non;							//状態管理
		this->num_state = -1;								//状態管理番号
		this->flag_switching_slide = false;					//スライドイン/アウトフラグ
		this->move_letter=0;								//レターボックスアニメカウンタ
		this->add_move_letter = 2;							//レターボックススライド速度
		this->limit_move_letter=192;						//レターの移動量上限
		this->cnt_switch_letter = 0;						//スライドインアウトの切り替えカウンタ
		this->time_switch_letter = 180;						//スライドインアウトの切り替えタイミング

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//外部から一度だけ状態を指定し、初期化
		if (this->state_display == Non)
		{
			switch (this->num_state)
			{
			default:
				break;
				//フェードイン/アウト
			case 0:
				this->state_display = Fade;
				break;
				//レターボックス
			case 1:
				this->state_display = Letter_Box;
				break;
				//ホワイトアウト
			case 2:
				this->state_display = White_Out;
				break;
			}
		}
		//汎用カウンタ
		this->cnt_transition++;
		//状態ごとの処理
		switch (this->state_display)
		{
		default:
			break;
		case Fade:
			//フェードイン
			if (!this->flag_in_out)
			{
				this->transparency += 0.01f;
			}
			//一定時間でフラグを反転
			if (this->cnt_transition == this->time_create_next_task)
			{
				this->flag_in_out = !this->flag_in_out;
			}
			//一定時間でフェードアウトを始める
			if (this->flag_in_out &&
				this->cnt_transition > this->time_start_fade_out)
			{
				this->transparency -= 0.01f;
			}
			//透明になったら自身を消滅させる
			if (this->transparency < 0.0f)
			{
				this->Kill();
			}
			break;
		case Letter_Box:
			//スライドイン
			if (!this->flag_switching_slide)
			{
				this->move_letter+=this->add_move_letter;
				//規定量移動したらフラグ反転
				if (this->move_letter == this->limit_move_letter)
				{
					this->flag_switching_slide=true;
				}
			}
			//スライドアウト
			if(this->flag_switching_slide)
			{
				this->cnt_switch_letter++;
				//スライドアウトまでの間隔
				if (this->cnt_switch_letter >= this->time_switch_letter)
				{
					this->move_letter-=this->add_move_letter;
					//フラグ反転後、規定量移動で自身を消滅させる
					if (this->move_letter == 0)
					{
						this->Kill();
					}
				}
			}
			break;
		case White_Out:
			//フェードイン
			if (!this->flag_in_out)
			{
				this->transparency += 0.01f;
			}
			//一定時間でフラグを反転
			if (this->cnt_transition == this->time_create_next_task)
			{
				this->flag_in_out = !this->flag_in_out;
			}
			//一定時間でフェードアウトを始める
			if (this->flag_in_out &&
				this->cnt_transition > this->time_start_fade_out)
			{
				this->transparency -= 0.01f;
			}
			//透明になったら自身を消滅させる
			if (this->transparency < 0.0f)
			{
				this->Kill();
			}
			break;
		}	
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		switch (this->state_display)
		{
		default:
			break;
		case Fade:
		{
			ML::Box2D draw(0, 0, 1920, 1080);
			ML::Box2D src(0, 0, 1920, 1080);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->transparency, 1, 1, 1));
			break;
		}
		case White_Out:
		{
			ML::Box2D draw(0, 0, 1920, 1080);
			ML::Box2D src(0, 1700, 1920, 1080);
			DG::Image_Draw(this->res->imageName, draw, src, ML::Color(this->transparency, 1, 1, 1));
			break;
		}
		//画面外からレターボックスが現れる/画面外に消える
		case Letter_Box:
		{
			ML::Box2D  src(0, 1245, 1920, 192);
			//X軸中心を宣言
			float center_x = float(ge->screenWidth / 2);
			//上部のY軸座標
			float top_y = float(-src.h + this->move_letter);
			ML::Box2D draw_top(-960, 0, 1920, 192);
			draw_top.Offset(ML::Vec2(center_x, top_y));
			DG::Image_Draw(this->res->imageName, draw_top, src, ML::Color(1, 1, 1, 1));
			//下部のY軸座標
			float bottom_y = float(ge->screenHeight - this->move_letter);
			ML::Box2D draw_bottom(-960, 0, 1920, 192);
			draw_bottom.Offset(ML::Vec2(center_x, bottom_y));
			DG::Image_Draw(this->res->imageName, draw_bottom, src, ML::Color(1, 1, 1, 1));
			break;
		}
		}
	}
	//外部から状態を指定する
	//引数	：	（整数）
	//0：フェードイン/アウト
	//1：レターボックス
	void Object::Set_Num_State(const int& num_)
	{
		this->num_state = num_;
	}
	//画面全体の効果を生成し、状態を指定する
	//引数	：	（状態番号）
	//0：フェードイン/アウト
	//1：レターボックス
	//2: ホワイトアウト
	void Object::Create_Display_Effect(const int& num_)
	{
		auto display_effect =
			Display_Effect::Object::Create(true);
		display_effect->Set_Num_State(num_);
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