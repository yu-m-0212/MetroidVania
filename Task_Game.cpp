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
		pl->pos = ML::Vec2(383.0f, 1100.0f);
		//�d���̐���
		auto  spr = Sprite::Object::Create(true);
		spr->pos = pl->pos;
		spr->target = pl;
		//�G�l�~�[00�̐���
		/*this->Create_Enemy(ML::Vec2(5535.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(4769.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(3299.0f, 3712.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(2339.0f, 3456.0f), 0.0f, 5);
		this->Create_Enemy(ML::Vec2(1340.0f, 3072.0f), 0.0f, 5);*/
		//�G�l�~�[01�̐���
		/*auto ene0 = Enemy01::Object::Create(true);
		ene0->pos.x = 550.0f;
		ene0->pos.y = 4200.0f;
		ene0->hp = 30;*/
		//UI�̐���
		auto ui = UI::Object::Create(true);
		//�`���[�g���A���̐���
		this->Create_Message(ML::Vec2( 389, 1312), "���X�e�B�b�N�����ɓ|���ƈړ�");
		this->Create_Message(ML::Vec2( 929, 1312), "�~�{�^���ŃW�����v");
		this->Create_Message(ML::Vec2(2016, 1312), "�~�{�^���������ō����W�����v");
		this->Create_Message(ML::Vec2(2787,  800), "��̂ɐG���Ɖ񕜂���");
		this->Create_Message(ML::Vec2(3040, 1888), "R1�{�^���ŃV���b�g");
		this->Create_Message(ML::Vec2(1440, 1888), "�E�X�e�B�b�N�ŏe�����X����");
		this->Create_Message(ML::Vec2( 384, 2272), "���{�^���ŋߐڍU��");

		//�w�i�̐���
		auto back = Back::Object::Create(true);
		//�S�[���̐���
		auto goal = Goal::Object::Create(true);
		//�`���[�g���A���p�̈�̂�z�u
		/*auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(5955, 1984);*/
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
	//�`���[�g���A���\���L�����N�^�̐���
	//����	�F	�i���W,���b�Z�[�W�j
	void Object::Create_Message(const ML::Vec2& pos_, const string& message_)
	{
		auto tutorials = Tutorials::Object::Create(true);
		tutorials->pos = pos_;
		tutorials->Set_Message(message_);
	}
	//�G�l�~�[�̐���
	//����	�F	�i�������W,�ړ����x,HP�j
	void Object::Create_Enemy(const ML::Vec2& pos_, const float& addSpeed_, const int&hp_)
	{
		auto ene = Enemy00::Object::Create(true);
		ene->pos = pos_;
		ene->addSpeed = addSpeed_;
		ene->hp = hp_;
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