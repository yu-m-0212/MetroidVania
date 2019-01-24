//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include  "Task_Display_Effect.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "name_image";
		DG::Image_Create(this->name_image, "./data/image/Staff_Roll.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->render2D_Priority[1] = 0.1f;												//描画順
		this->transition = false;														//フラグ画面遷移
		this->cnt_transition = 0;														//カウンタ画面遷移
		this->time_kill_this = 150;														//時間自身を消滅させる
		this->size_image_w = int(DG::Image_Size(this->res->name_image).x);				//サイズリソース幅
		this->size_image_h = int(DG::Image_Size(this->res->name_image).y);				//サイズリソース高さ
		this->pos_staff_roll = 0.0f;													//表示位置スタッフロール
		this->add_pos_staff_roll = 3.0f;												//加算量表示位置スタッフロール
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		//★引き継ぎタスクの生成
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			auto  nextTask = Title::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//画面が停止するまでの処理
		if (this->pos_staff_roll > int(-this->size_image_h + ge->screenHeight))
		{
			this->pos_staff_roll -= this->add_pos_staff_roll;
		}
		//画面遷移
		if (in.ST.down)
		{
			this->transition = true;
		}
		if (this->transition)
		{
			if (this->cnt_transition == 0)
			{
				auto display_effect =
					ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				display_effect->Create_Display_Effect(0);
			}
			this->cnt_transition++;
		}
		if (this->cnt_transition >= this->time_kill_this)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0,int(this->size_image_w),int(this->size_image_h));
		draw.Offset(ML::Vec2(0, this->pos_staff_roll));
		ML::Box2D  src(0, 0, int(this->size_image_w), int(this->size_image_h));
		DG::Image_Draw(this->res->name_image, draw, src);
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