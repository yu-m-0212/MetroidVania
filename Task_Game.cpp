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
		ge->camera2D = ML::Box2D(-960, -540, 1920, 1080);	//�J����������
		ge->clear = false;									//�N���A�t���O������
		ge->failure = false;								//�~�X�t���O������
		ge->pause = false;									//�|�[�Y�t���O�̏�����
		this->cnt_Scenes = 0;								//�J�E���^�J�ڗp
		
		//���^�X�N���풓������
		this->shot00_Resource = Shot00::Resource::Create();
		this->shot01_Resource = Shot01::Resource::Create();
		this->effect_Resource = Effect::Resource::Create();

		//���^�X�N�̐���
		//�}�b�v�̐���
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		////�v���C���̐���
		auto  pl = Player::Object::Create(true);
		pl->pos = ML::Vec2(383.0f, 1100.0f);
		////�d���̐���
		auto  spr = Sprite::Object::Create(true);
		spr->Set_Target(pl);
		spr->pos = pl->pos;
		//�G�l�~�[�i�T�\���j�̐���
		this->Create_Enemy01(ML::Vec2(2582.0f, 1875.0f));
		this->Create_Enemy01(ML::Vec2(1155.0f, 1690.0f));
		//UI�̐���
		auto ui = UI::Object::Create(true);
		//�`���[�g���A���̐���
		this->Create_Message(ML::Vec2( 389, 1312), "���X�e�B�b�N�����ɓ|���ƈړ�");
		this->Create_Message(ML::Vec2( 929, 1312), "�~�{�^���ŃW�����v");
		this->Create_Message(ML::Vec2(2016, 1312), "�~�{�^���������ō����W�����v");
		this->Create_Message(ML::Vec2(2787,  800), "��̂ɐG���Ɖ񕜂���");
		this->Create_Message(ML::Vec2(3040, 1888), "R1�{�^���ŃV���b�g");
		this->Create_Message(ML::Vec2(1440, 1888), "�E�X�e�B�b�N�ŏe�����X����");
		this->Create_Message(ML::Vec2( 640, 1698), "���{�^���ŋߐڍU��");
		//�A�C�e���̐���
		auto item00 = Item00::Object::Create(true);
		item00->pos = ML::Vec2(1727, 2200);
		//�w�i�̐���
		auto back = Back::Object::Create(true);
		//�S�[���̐���
		auto goal = Goal::Object::Create(true);
		goal->pos = ML::Vec2(16, 1170);
		//�`���[�g���A���p�̈�̂�z�u
		auto corpse = Corpse::Object::Create(true);
		corpse->pos = ML::Vec2(2974, 820);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�G");
		ge->KillAll_G("UI");
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�w�i");
		ge->KillAll_G("���");
		ge->KillAll_G("�I�v�V����");
		ge->KillAll_G("�S�[��");
		ge->KillAll_G("�A�C�e��");
		ge->KillAll_G("�`���[�g���A��");
		ge->KillAll_G("�v���C��");
		
		//�����\�[�X���풓����������i�����Ȃ��Ă�����ɉ��������j
		this->shot00_Resource.reset();
		this->shot01_Resource.reset();
		this->effect_Resource.reset();
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			if (ge->clear)
			{
				Ending::Object::Create(true);
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
		//Select�{�^���Ńf�o�b�O���[�h
		if (in.SE.down)
		{
			ge->DebugMode(!ge->debugMode);
		}
		//
		if (ge->failure || ge->clear)
		{
			if (this->cnt_Scenes==60)
			{
				if (ge->failure)
				{
					auto pl = ge->GetTask_One_G < Player::Object>("�v���C��");
					if (nullptr != pl)
					{
						this->deadPos = pl->pos;
						auto retry = Retry::Object::Create(true);
						retry->Set_DeadPos(this->deadPos);
					}
				}
				this->Kill();
			}
			this->cnt_Scenes++;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
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
	void Object::Create_Enemy01(const ML::Vec2& pos_)
	{
		auto ene = Enemy01::Object::Create(true);
		ene->pos = pos_;
		auto es = EnemySearch::Object::Create(true);
		es->hitBase = ene->Get_Search();
		es->Set_Target(ene);
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