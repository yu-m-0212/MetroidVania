//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_Sprite.h"
#include  "Task_Item00.h"
#include  "Task_Item01.h"
#include  "Task_Item02.h"
#include  "Task_UI.h"
#include  "Task_Back.h"
#include  "Task_Goal.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		ge->camera2D = ML::Box2D(-960, -540, 1920, 1080);//��肠���������l�ݒ�
		
		//���^�X�N���풓������
		this->shot00_Resource = Shot00::Resource::Create();

		//���^�X�N�̐���
		//�}�b�v�̐���
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		//�v���C���̐���
		auto  pl = Player::Object::Create(true);
		pl->pos.x = 770;
		pl->pos.y = 2200;
		//�d���̐���
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//�G�̐���
		auto ene1 = Enemy01::Object::Create(true);
		ene1->pos.x = 5535.0f;
		ene1->pos.y = 3712.0f;
		ene1->hp = 5;
		//�G�̐���
		auto ene2 = Enemy01::Object::Create(true);
		ene2->pos.x = 4769.0f;
		ene2->pos.y = 3712.0f;
		ene2->hp = 5;
		//�G�̐���
		auto ene3 = Enemy01::Object::Create(true);
		ene3->pos.x = 3299.0f;
		ene3->pos.y = 3712.0f;
		ene3->hp = 5;
		//�G�̐���
		auto ene4 = Enemy01::Object::Create(true);
		ene4->pos.x = 2339.0f;
		ene4->pos.y = 3456.0f;
		ene4->hp = 5;
		//�G�̐���
		auto ene5 = Enemy01::Object::Create(true);
		ene5->pos.x = 1340.0f;
		ene5->pos.y = 3072.0f;
		ene5->hp = 5;
		//�G�̐���
		auto ene6 = Enemy01::Object::Create(true);
		ene6->pos.x =  836.0f;
		ene6->pos.y = 3392.0f;
		ene6->hp = 5;
		//�G�̐���
		auto ene7 = Enemy01::Object::Create(true);
		ene7->pos.x =  966.0f;
		ene7->pos.y = 3776.0f;
		ene7->hp = 5;
		//UI�̐���
		auto ui = UI::Object::Create(true);
		//�w�i�̐���
		auto back = Back::Object::Create(true);
		//���S�[���̐���
		auto goal = Goal::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�G");
		ge->KillAll_G("�I�v�V����");
		ge->KillAll_G("�A�C�e��");
		ge->KillAll_G("UI");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�w�i");
		ge->KillAll_G("�S�[��");
		//�����\�[�X���풓����������i�����Ȃ��Ă�����ɉ��������j
		this->shot00_Resource.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (in.ST.down) {
			//���g�ɏ��ŗv��
			this->Kill();
		}
		//Select�{�^���Ńf�o�b�O���[�h
		if (in.SE.down)
		{
			ge->debugMode = !ge->debugMode;
		}
		//�v���C�����S�ŃG���f�B���O

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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