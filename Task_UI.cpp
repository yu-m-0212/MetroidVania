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
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->hpImageName = "hpImage";
		DG::Image_Create(this->hpImageName, "./data/image/ui.png");
		DG::Font_Create("fontUI", "lr SVbN", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->hpImageName);
		DG::Font_Erase("fontUI");
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
		this->render2D_Priority[1] = 0.2f;
		this->controllerName = "P1";
		
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("vC");
		if (nullptr == pl) { return; }
		auto es = ge->GetTask_One_G<Enemy01::Object>("G");
		auto in = DI::GPad_GetState(this->controllerName);
		//x[XΜ\¦
		{
			ML::Box2D draw(0, 0, 1920, 128);
			ML::Box2D  src(0, 64, 1920, 128);
			DG::Image_Draw(this->res->hpImageName, draw, src);
		}
		//vCHPgΜ\¦
		{
			for (int i = 0; i < pl->Get_Max_HP(); ++i)
			{
				ML::Box2D draw(128 + 64 * i, 32, 64, 64);
				if (ge->debugMode)
				{
					draw.x += 120;
				}
				ML::Box2D  src(128, 0, 64, 64);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//vCΜHP\¦
		//o[Ε\¦
		//ML::Box2D draw(32, 32, 32 * pl->Get_HP(), 16);
		//{^Ε\¦
		{
			ML::Box2D draw;
			ML::Box2D  src(0, 0, 64, 64);
			for (int i = 0; i < pl->Get_HP(); ++i)
			{
				draw = ML::Box2D(128 + 64 * i, 32, 64, 64);
				if (ge->debugMode)
				{
					draw.x += 120;
				}
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//ΘΊfobO----------------------------------------------------
		if (ge->debugMode)
		{
			ML::Box2D debugBox01(825, 0, 625, 450);
			string debugText01 =
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
				"BackSpace/Select{^ΕfobO[h";
			DG::Image_Draw(this->res->hpImageName, debugBox01, ML::Box2D(32, 0, 32, 32), ML::Color(0.7f, 0, 0, 0));
			DG::Font_Draw("fontUI", debugBox01, debugText01, ML::Color(1, 1, 1, 1));
			ML::Box2D debugBox02(1450, 75, 600, 100);
			string debugText02 =
				"RStick.axis.x = " + to_string(in.RStick.axis.x) + "\n" +
				"RStick.axis.y = " + to_string(in.RStick.axis.y) + "\n" +
				"angle = " + to_string(ML::ToDegree(atan2(in.RStick.axis.y, in.RStick.axis.x)));
			DG::Image_Draw(this->res->hpImageName, debugBox02, ML::Box2D(32, 0, 32, 32), ML::Color(0.7f, 0, 0, 0));
			DG::Font_Draw("fontUI", debugBox02, debugText02, ML::Color(1, 1, 1, 1));
		}
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