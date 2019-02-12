//-------------------------------------------------------------------
//ゲームパッド注意
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Caution_GamePad.h"
#include  "Task_Title.h"

namespace  Caution
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image_caution = "image_caution";
		DG::Image_Create(this->name_image_caution, "./data/image/caution_game_pad.png");
		this->name_font = "font_skip";
		DG::Font_Create(this->name_font, "HG丸ｺﾞｼｯｸM - PRO", 12, 24);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image_caution);
		DG::Font_Erase(this->name_font);
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
		this->transition_scene = false;
		this->pos = ge->center;						//描画座標
		this->rate_close_up = 1;					//描画拡大率
		this->speed_rate_close_up = 2;				//描画拡大速度
		this->add_rate_close_up = 1;				//描画拡大率増加量
		this->time_flag = 300;						//タイムフラグを反転させる時間
		this->time_erase = 90;						//フラグ反転後、消滅までの時間
		this->name_pad = "P1";						//パッド宣言
		this->text_skip = "STARTボタンでスキップ";	//スキップ表示
		this->pos_font = ML::Vec2(1750, 1040);		//フォント座標
		this->color_font = ML::Color(1, 0, 0, 0);	//フォント色

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
			Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//カウンタインクリメント
		this->moveCnt++;
		//フラグ管理
		this->Transition_Scene();
		//一定条件で画面遷移
		if (this->transition_scene)
		{
			//拡大速度
			if (this->moveCnt%this->speed_rate_close_up == 0)
			{
				this->rate_close_up += this->add_rate_close_up;
			}
			//消滅までの時間
			if (this->moveCnt > this->time_erase)
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			//リソースのサイズを取得
			POINT size =
				DG::Image_Size(this->res->name_image_caution);
			ML::Box2D draw
			(
				-size.x / 2,
				-size.y / 2,
				size.x,
				size.y
			);
			//拡大率を反映する
			draw.x *= this->rate_close_up;
			draw.y *= this->rate_close_up;
			draw.w *= this->rate_close_up;
			draw.h *= this->rate_close_up;
			draw.Offset(this->pos);
			ML::Box2D src(0, 0, size.x, size.y);
			DG::Image_Draw(this->res->name_image_caution, draw, src);
		}
		//フォント表示
		{
			//遷移フラグ判定
			if (!this->transition_scene)
			{
				ML::Box2D text_box(-256,-64,512,128);
				text_box.Offset(this->pos_font);
				DG::Font_Draw(this->res->name_font, text_box, this->text_skip, this->color_font);
			}
		}
	}
	//画面切り替えフラグ管理
	bool Object::Transition_Scene()
	{
		//時間経過で成立
		if (this->moveCnt > this->time_flag)
		{
			this->transition_scene = true;
			this->moveCnt = 0;
			return true;
		}
		//STARTボタンタップで成立
		auto in =
			DI::GPad_GetState(this->name_pad);
		if (in.ST.down)
		{
			this->transition_scene = true;
			this->moveCnt = 0;
			return true;
		}
		return false;
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