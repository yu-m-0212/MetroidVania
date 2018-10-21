//-------------------------------------------------------------------
//^CgζΚ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Effect.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		DG::Image_Create(this->imageName, "./data/image/Title.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		this->render2D_Priority[1] = 1.0f;	//`ζ
		this->cnt_create_bubble = 0;		//GtFNgΜΆ¬JE^
		this->cnt_anim_back = 0;			//wiAjJE^
		this->interval_anim_back = 25;		//wiAjόϊ
		this->posY = -360.0f;				//wiY²ΐW
		this->posY_std = -240.0f;			//wiY²ΐWξl
		this->height_anim_back = 25.0f;		//wiAjhκ

		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->KillAll_G("GtFNg");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
			auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		this->cnt_create_bubble++;
		this->cnt_anim_back++;

		auto in = DI::GPad_GetState("P1");
		//fobOΨθΦ¦
		if (in.SE.down) { ge->debugMode = !ge->debugMode; }

		//GtFNgΜΆ¬
		if (this->cnt_create_bubble % 30 == 0)
		{
			float initX = float(rand() % (ge->screenWidth - 96));
			int num = rand() % 3;
			float ang = float(rand() % 360);
			eff->Create_Bubble(num, ML::Vec2(initX, float(ge->screenHeight + 96)), 16, 5.0f, 2.0f, ang, 600);
		}
		//©gΙΑΕvΏ
		if (in.ST.down)
		{
			this->Kill();
		}
		//wiAj[V
		float y = this->posY_std + float(sin(this->cnt_anim_back / this->interval_anim_back)*this->height_anim_back);
		this->posY = y;
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(-640, -360, 3200, 1800);
		draw.Offset(ML::Vec2(0, this->posY));
		ML::Box2D   src(   0,    0, 3200, 1800);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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