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
		this->render2D_Priority[1] = 0.2f;
		//HP�{�^���\���p
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		for (int i = 0; i < pl->max_Hp; ++i)
		{
			this->playerHp[i].active = true;
			this->playerHp[i].x = 0;
			this->playerHp[i].y = 32;
		}
		
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
		//HP�̃{�^���\��
		//auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//for (int i = 0; i < pl->Get_HP(); ++i)
		//{
		//	this->playerHp[i].active = true;
		//}
		//for (int j = pl->Get_Max_HP(); j > pl->Get_HP(); --j)
		//{
		//	this->playerHp[j].active = false;
		//}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		auto es = ge->GetTask_One_G<Enemy01::Object>("�G");
		if (ge->debugMode)
		{
			ML::Box2D debugBox(850, 50, 600, 600);
			string debugText =
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
				"BackSpace/Select�{�^���Ńf�o�b�O���[�h�ؑ�";
			DG::Font_Draw("fontUI", debugBox, debugText, ML::Color(1, 1, 1, 1));
		}
		//�ȏ�f�o�b�O----------------------------------------------------
		//�v���C����HP�\��
		//�{�^���\��
		for (int i = 0; i < pl->Get_HP(); ++i)
		{
			if (this->playerHp[i].active)
			{
				ML::Box2D draw(64 + 64 * i, 64, 64, 64);
				//�f�o�b�O���͕\�������炷
				if (ge->debugMode)
				{
					draw.x += 100;
				}
				ML::Box2D src(0, 0, 32, 32);
				//�c��̗͂ɂ���ĐF���w�肷��
				float red = 1.0f - 0.1f * i;
				float blue = 0.1f + 0.1f * i;
				DG::Image_Draw(this->res->hpImageName, draw, src, ML::Color(1.0f, red, 0.0f, blue));
			}
		}
		//�v���C����HP�\��
		//�o�[�ŕ\��
		//ML::Box2D draw(32, 32, 32 * pl->Get_HP(), 16);
		////�f�o�b�O���͕\�������炷
		//if (ge->debugMode)
		//{
		//	draw.x += 100;
		//}
		//ML::Box2D  src(32, 0, 32, 32);
		//DG::Image_Draw(this->res->hpImageName, draw, src);
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