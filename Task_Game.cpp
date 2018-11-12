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
#include  "Task_Gun.h"
#include  "Task_Display_Effect.h"

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
		this->cnt_transition = 0;							//�J�E���^�J�ڗp
		this->time_create_next_task = 100;					//���p���^�X�N�̐����^�C�~���O
		this->time_kill_game = 150;							//���g�����ł�����^�C�~���O
		this->tutorials = new Tutorials::Object();			//�|�C���^���b�Z�[�W
		this->eff = new Task_Effect::Object();				//�|�C���^�G�t�F�N�g
		
		//���^�X�N���풓������
		this->shot00_Resource = Shot00::Resource::Create();
		this->shot01_Resource = Shot01::Resource::Create();
		this->effect_Resource = Task_Effect::Resource::Create();

		//���^�X�N�̐���
		//�}�b�v�̐���
		auto  m = Map2D::Object::Create(true);
		m->Load("./data/Map/map0.txt");
		//�v���C���̐���
		auto  pl = Player::Object::Create(true);
		pl->pos = ML::Vec2(224.0f, 4460.0f);
		this->eff->Create_Effect(6, pl->pos);
		//�J�����}���̐���
		auto  spr = Sprite::Object::Create(true);
		spr->Set_Target(pl);
		spr->pos = pl->pos;
		//UI�̐���
		UI::Object::Create(true);
		//�`���[�g���A���̐���
		tutorials->Create_Message("���X�e�B�b�N�����ɓ|���ƈړ�", ML::Vec2(224, 4482), -1);
		tutorials->Create_Message("�~�{�^���ŃW�����v", ML::Vec2(1180, 4482), -1);
		tutorials->Create_Message("R1�{�^���ŃV���b�g", ML::Vec2(2130, 3970), -1);
		tutorials->Create_Message("�E�X�e�B�b�N�ŏe�����X����", ML::Vec2(6792, 3650), -1);
		tutorials->Create_Message("���{�^���œG��G�̃V���b�g��e��", ML::Vec2(7192, 5122), -1);
		tutorials->Create_Message("��̂ɐG���Ɖ񕜂���", ML::Vec2(4823, 5378), -1);
		//�w�i�̐���
		Back::Object::Create(true);
		//���S�[���L�����N�^�̐���
		auto goal = Goal::Object::Create(true);
		goal->pos = ML::Vec2(7870, 5106);
		/*goal->pos = ML::Vec2(300.0f, 4460.0f);*/
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		ge->KillAll_G("�^�C�g��"); 

		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�v���C������");
		ge->KillAll_G("�K��");
		ge->KillAll_G("�e(�v���C��)");
		ge->KillAll_G("�I�v�V����");

		ge->KillAll_G("�G");
		ge->KillAll_G("�G�I�v�V����");
		ge->KillAll_G("�e(�G�l�~�[)");

		ge->KillAll_G("�t�B�[���h");

		ge->KillAll_G("���");
		ge->KillAll_G("�S�[��");
		ge->KillAll_G("�A�C�e��");
		ge->KillAll_G("�`���[�g���A��");

		ge->KillAll_G("UI");
		ge->KillAll_G("�w�i");
		ge->KillAll_G("�|�[�Y");
		//�����\�[�X���풓����������i�����Ȃ��Ă�����ɉ��������j
		this->shot00_Resource.reset();
		this->shot01_Resource.reset();
		this->effect_Resource.reset();
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�N���A��
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
		//�Q�[���I�[�o�[
		if (ge->failure)
		{
			//�t�F�[�h�C��
			if (this->cnt_transition == 0)
			{
				auto display_effect = Display_Effect::Object::Create(true);
			}
			this->cnt_transition++;
			//���g���C����
			if (this->cnt_transition == this->time_create_next_task)
			{
				auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
				if (nullptr != pl)
				{
					auto retry = Retry::Object::Create(true);
					//���S���̍��W
					this->pos_dead = pl->pos;
					retry->Set_Pos_Dead(this->pos_dead);
					//���S���̌���
					this->angle_dead = pl->angle_LR;
					retry->Set_Angle_Dead(this->angle_dead);
					//����
					this->Kill();
				}
			}
		}
		//�Q�[���N���A
		if (ge->clear)
		{
			//�t�F�[�h�C��
			if (this->cnt_transition == 0)
			{
				Display_Effect::Object::Create(true);
			}
			this->cnt_transition++;
			//��莞�ԂŎ��g�����ł�����
			if (this->cnt_transition > this->time_kill_game)
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
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