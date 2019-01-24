//-------------------------------------------------------------------
//X|i[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Spawner.h"

#include  "Task_Boss_Head.h"
#include  "Task_Boss_Upper.h"
#include  "Task_Boss_Upper_Middle.h"
#include  "Task_Boss_Center.h"
#include  "Task_Boss_Lower.h"

#include  "Task_Display_Effect.h"

namespace  Spawner
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		this->name_image = "image";
		DG::Image_Create(this->name_image, "./data/image/UI.png");

		this->name_bgm_boss = "bgm_boss";
		DM::Sound_CreateStream(this->name_bgm_boss, "./data/sound/wav/bgm_boss.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DM::Sound_Erase(this->name_bgm_boss);
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		this->hitBase = ML::Box2D(-512, -1, 1024, 2);		//\ฆ้`
		this->recieveBase = this->hitBase;					//ป่้`
		this->time_create_boss = 180;						//{Xฬถฌ^C~O
		this->time_bgm = 240;								//{XBGMฬฤถJnิ
		this->flag_spawn_boss = false;						//{XoปtO
		this->init_pos_boss = ML::Vec2(8382.0f, 7800.0f);	//{Xฬ๚ภW
		
		//^XNฬถฌ

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		//vCฦฺGลtOฌง
		auto pl =
			ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//ป่้`๐pำ
		ML::Box2D pl_base = pl->hitBase;
		pl_base.Offset(pl->pos);
		if (this->flag_spawn_boss == false &&
			this->CheckHit(pl_base))
		{
			this->flag_spawn_boss = true;
			//^[{bNXฬถฌ
			auto display_effect =
				ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr != display_effect) { return; }
			display_effect->Create_Display_Effect(1);
		}
		//tOฌงใฬ
		if (!this->flag_spawn_boss) { return; }
		//sฎJE^ฬCNg
		if (this->moveCnt <= this->time_bgm)
		{
			this->moveCnt++;
		}
		//{Xฬถฌ
		if (this->moveCnt == this->time_create_boss)
		{
			//{Xฬถฌ
			auto boss_head = Boss_Head::Object::Create(true);
			boss_head->pos = this->init_pos_boss;
			auto boss_upper = Boss_Upper::Object::Create(true);
			boss_upper->pos = ML::Vec2(boss_head->pos.x, boss_head->pos.y + float(boss_upper->hitBase.h));
			auto boss_upper_middle = Boss_Upper_Middle::Object::Create(true);
			boss_upper_middle->pos = ML::Vec2(boss_upper->pos.x, boss_upper->pos.y + float(boss_upper_middle->hitBase.h));
			auto boss_center = Boss_Center::Object::Create(true);
			boss_center->pos = ML::Vec2(boss_upper_middle->pos.x, boss_upper_middle->pos.y + float(boss_center->hitBase.h));
			auto boss_lower = Boss_Lower::Object::Create(true);
			boss_lower->pos = ML::Vec2(boss_center->pos.x, boss_center->pos.y + float(boss_lower->hitBase.h));
		}
		//{XBGMฬฤถ
		if (this->moveCnt == this->time_bgm)
		{
			DM::Sound_Play_Volume(this->res->name_bgm_boss,true,VOLUME_BGM_BOSS);
		}
		//Q[NAฉgCลBGMtF[hAEg
		if (ge->clear)
		{
			DM::Sound_FadeOut(this->res->name_bgm_boss);
		}
		if (ge->failure)
		{
			DM::Sound_FadeOut(this->res->name_bgm_boss);
		}
	}
	//-------------------------------------------------------------------
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		//fobO[hฬAป่้`๐\ฆ
		if (ge->debugMode)
		{
			ML::Box2D draw = this->hitBase;
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->name_image, draw, src, ML::Color(0.5f, 0.0f, 1.0f, 0.0f));
		}
	}
	//{XฬoปtO๐ิท
	bool Object::Get_Flag_Spawn_Boss()
	{
		return this->flag_spawn_boss;
	}
	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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