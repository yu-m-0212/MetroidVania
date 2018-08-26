//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Retry.h"
#include  "Task_Ending.h"
#include  "Task_UI.h"
#include  "Task_Back.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Sprite.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_Item00.h"
#include  "Task_Item01.h"
#include  "Task_Item02.h"
#include  "Task_Goal.h"
#include  "Task_EnemySearch.h"
#include  "Task_Pause.h"
#include  "Task_Tutorials.h"
#include  "Task_Corpse.h"

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
		//�N���A�t���O������
		ge->clear = false;
		//�~�X�t���O������
		ge->failure = false;
		//�|�[�Y�t���O�̏�����
		ge->pause = false;
		
		//���^�X�N���풓������
		this->shot00_Resource = Shot00::Resource::Create();

		//���^�X�N�̐���
		//�}�b�v�̐���
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		//�v���C���̐���
		auto  pl = Player::Object::Create(true);
		pl->pos.x = 770;
		pl->pos.y = 2000;
		//�d���̐���
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//�f�o�b�O�p
		//�G�̐���
		auto ene0 = Enemy01::Object::Create(true);
		ene0->pos.x = 550.0f;
		ene0->pos.y = 4200.0f;
		ene0->hp = 30;
		//�G�̐���
		auto ene1 = Enemy00::Object::Create(true);
		ene1->pos.x = 5535.0f;
		ene1->pos.y = 3712.0f;
		ene1->addSpeed = 0.0f;
		ene1->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene1->hp = 5;
		//�G�̐���
		auto ene2 = Enemy00::Object::Create(true);
		ene2->pos.x = 4769.0f;
		ene2->pos.y = 3712.0f;
		ene2->addSpeed = 0.0f;
		ene2->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene2->hp = 5;
		//�G�̐���
		auto ene3 = Enemy00::Object::Create(true);
		ene3->pos.x = 3299.0f;
		ene3->pos.y = 3712.0f;
		ene3->addSpeed = 0.0f;
		ene3->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene3->hp = 5;
		//�G�̐���
		auto ene4 = Enemy00::Object::Create(true);
		ene4->pos.x = 2339.0f;
		ene4->pos.y = 3456.0f;
		ene4->addSpeed = 0.0f;
		ene4->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene4->hp = 5;
		//�G�̐���
		auto ene5 = Enemy00::Object::Create(true);
		ene5->pos.x = 1340.0f;
		ene5->pos.y = 3072.0f;
		ene5->addSpeed = 0.0f;
		ene5->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene5->hp = 5;
		//�G�̐���
		auto ene6 = Enemy00::Object::Create(true);
		ene6->pos.x =  836.0f;
		ene6->pos.y = 3392.0f;
		ene6->addSpeed = 0.0f;
		ene6->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene6->hp = 5;
		//�G�̐���
		auto ene7 = Enemy00::Object::Create(true);
		ene7->pos.x =  966.0f;
		ene7->pos.y = 3776.0f;
		ene7->addSpeed = 0.0f;
		ene7->hitBase = ML::Box2D(-56, -48, 102, 96);
		ene7->hp = 5;
		//UI�̐���
		auto ui = UI::Object::Create(true);
		//�`���[�g���A��01�̐���
		auto tutorials01 = Tutorials::Object::Create(true);
		tutorials01->pos = ML::Vec2(770, 2624);
		tutorials01->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials01->Set_Message("���X�e�B�b�N�����ɓ|���ƈړ�");
		//�`���[�g���A��02�̐���
		auto tutorials02 = Tutorials::Object::Create(true);
		tutorials02->pos = ML::Vec2(1857, 2624);
		tutorials02->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials02->Set_Message("�~�{�^���ŃW�����v");
		//�`���[�g���A��03�̐���
		auto tutorials03 = Tutorials::Object::Create(true);
		tutorials03->pos = ML::Vec2(4032, 2624);
		tutorials03->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials03->Set_Message("�~�{�^���������ō����W�����v");
		//�`���[�g���A��04�̐���
		auto tutorials04 = Tutorials::Object::Create(true);
		tutorials04->pos = ML::Vec2(5570, 1920);
		tutorials04->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials04->Set_Message("��̂ɐG���Ɖ񕜂���");
		//�`���[�g���A��05�̐���
		auto tutorials05 = Tutorials::Object::Create(true);
		tutorials05->pos = ML::Vec2(6210, 3648);
		tutorials05->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials05->Set_Message("���{�^���Ńp���`");
		//�`���[�g���A��06�̐���
		auto tutorials06 = Tutorials::Object::Create(true);
		tutorials06->pos = ML::Vec2(3774, 3648);
		tutorials06->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials06->Set_Message("R1�{�^���ŃV���b�g");
		//�`���[�g���A��07�̐���
		auto tutorials07 = Tutorials::Object::Create(true);
		tutorials07->pos = ML::Vec2(2688, 3648);
		tutorials07->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials07->Set_Message("�󒆂ɂ���Ƃ����{�^���ŋ󒆍U��");
		//�`���[�g���A��08�̐���
		auto tutorials08 = Tutorials::Object::Create(true);
		tutorials08->pos = ML::Vec2(1600, 3136);
		tutorials08->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials08->Set_Message("�󒆂ɂ���Ƃ�R1�{�^���ŋ󒆃V���b�g");
		//�`���[�g���A��09�̐���
		auto tutorials09 = Tutorials::Object::Create(true);
		tutorials09->pos = ML::Vec2(1088, 3008);
		tutorials09->recieveBase = ML::Box2D(-256, -128, 512, 256);
		tutorials09->Set_Message("���{�^���œ��݂�");
		//�w�i�̐���
		auto back = Back::Object::Create(true);
		//���S�[���̐���
		auto goal = Goal::Object::Create(true);
		//�`���[�g���A���p�̈�̂�z�u
		auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(5955, 1984);
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
		ge->KillAll_G("�G�I�v�V����");
		ge->KillAll_G("�e");
		ge->KillAll_G("�I�v�V����");
		ge->KillAll_G("�A�C�e��");
		ge->KillAll_G("UI");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�w�i");
		ge->KillAll_G("�S�[��");
		ge->KillAll_G("���");
		ge->KillAll_G("�V�[��");
		ge->KillAll_G("�`���[�g���A��");
		//�����\�[�X���풓����������i�����Ȃ��Ă�����ɉ��������j
		this->shot00_Resource.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto in = DI::GPad_GetState("P1");
			//�N���A��ʂɈڂ�
			if (ge->clear)
			{
				auto ending = Ending::Object::Create(true);
			}
			//���g���C��ʂɈڂ�
			else if (ge->failure)
			{
				auto retry = Retry::Object::Create(true);
				//���S�n�_��n��
				retry->Set_DeadPos(this->deadPos);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		if (!ge->pause&&in.ST.down)
		{
			//�X�^�[�g�Ń|�[�Y
			ge->pause = true;
			auto pause = Pause::Object::Create(true);
		}
		//�|�[�Y���ɃX�^�[�g�Ń|�[�Y����
		else if (ge->pause&&in.ST.down)
		{
			ge->pause = false;
		}
		if (ge->clear)
		{
			this->Kill();
		}
		if (ge->failure)
		{
			this->Kill();
			//���S�n�_��ۑ�
			auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
			this->deadPos = pl->pos;
		}
		//Select�{�^���Ńf�o�b�O���[�h
		if (in.SE.down)
		{
			ge->debugMode = !ge->debugMode;
		}
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