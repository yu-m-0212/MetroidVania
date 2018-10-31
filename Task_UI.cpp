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
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->hpImageName = "hpImage";
		DG::Image_Create(this->hpImageName, "./data/image/ui.png");
		DG::Font_Create("fontUI", "�l�r �S�V�b�N", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->hpImageName);
		DG::Font_Erase("fontUI");
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.2f;	//�`�揇
		this->controllerName = "P1";		//�R���g���[���[�錾
		this->cnt = 0;						//�A�j���[�V�����J�E���^
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->cnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (nullptr == pl) { return; }
		auto es = ge->GetTask_One_G<Enemy01::Object>("�G");
		auto in = DI::GPad_GetState(this->controllerName);
		//�x�[�X�̕\��
		{
			ML::Box2D draw(0, 0, 1920, 128);
			ML::Box2D  src(0, 64, 1920, 128);
			DG::Image_Draw(this->res->hpImageName, draw, src);
		}
		//�v���C��HP�g�̕\��
		{
			for (int i = 0; i < pl->Get_Max_HP(); ++i)
			{
				ML::Box2D draw(128 + 64 * i, 32, 64, 64);
				ML::Box2D  src(128, 0, 64, 64);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//�v���C����HP�\��
		//�o�[�ŕ\��
		//ML::Box2D draw(32, 32, 32 * pl->Get_HP(), 16);
		//�{�^���ŕ\��
		{
			ML::Box2D draw;
			ML::Box2D  src(0, 0, 64, 64);
			for (int i = 0; i < pl->Get_HP(); ++i)
			{
				draw = ML::Box2D(128 + 64 * i, 32, 64, 64);
				DG::Image_Draw(this->res->hpImageName, draw, src);
			}
		}
		//�ߐڍU���Q�[�W�̕\��
		{
			ML::Box2D draw(1589, 53, pl->Get_Gauge_Mlee() * 3, 24);
			ML::Box2D  src(64, 0, 64, 64);
			ML::Color  clr;
			//�}�b�N�X�̎��A�_�ł���
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
		//�ȉ��f�o�b�O----------------------------------------------------
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
				"time_un_hit = " + to_string(pl->time_un_hit) + "\n" +
				"hp=" + to_string(pl->hp) + "\n" +
				"ge->clear = " + to_string(ge->clear) + "\n" +
				"ge->failure = " + to_string(ge->failure) + "\n" +
				"Search_Player() = " + to_string(es->Search_Player()) + "\n" +
				"BackSpace/Select�{�^���Ńf�o�b�O���[�h";
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
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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