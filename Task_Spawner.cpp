//-------------------------------------------------------------------
//�X�|�i�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Spawner.h"

#include  "Task_Boss_Head.h"
#include  "Task_Boss_Upper.h"
#include  "Task_Boss_Upper_Middle.h"
#include  "Task_Boss_Center.h"
#include  "Task_Boss_Lower.h"

#include  "Task_Display_Effect.h"

#include  "Task_Player.h"
#include  "Task_Sprite.h"

namespace  Spawner
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "image";
		DG::Image_Create(this->name_image, "./data/image/UI.png");

		this->name_bgm_boss = "bgm_boss";
		DM::Sound_CreateStream(this->name_bgm_boss, "./data/sound/wav/bgm_boss.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DM::Sound_Erase(this->name_bgm_boss);
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
		this->hitBase = ML::Box2D(-512, -1, 1024, 2);		//�\����`
		this->recieveBase = this->hitBase;					//�����`
		this->time_create_boss = 180;						//�{�X�̐����^�C�~���O
		this->time_bgm = 240;								//�{�XBGM�̍Đ��J�n����
		this->flag_spawn_boss = false;						//�{�X�o���t���O
		this->init_pos_boss = ML::Vec2(8382.0f, 7800.0f);	//�{�X�̏������W
		this->eff = new Task_Effect::Object();				//�G�t�F�N�g�C���X�^���X
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->eff;
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�v���C���ƐڐG�Ńt���O����
		auto pl =
			ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//�����`��p��
		ML::Box2D pl_base = pl->hitBase;
		pl_base.Offset(pl->pos);
		if (this->flag_spawn_boss == false &&
			this->CheckHit(pl_base))
		{
			this->flag_spawn_boss = true;
			//���^�[�{�b�N�X�̐���
			auto display_effect =
				ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr != display_effect) { return; }
			display_effect->Create_Display_Effect(1);
		}
		//�t���O������̏���
		if (!this->flag_spawn_boss) { return; }
		//�s���J�E���^�̃C���N�������g
		if (this->moveCnt <= this->time_bgm)
		{
			this->moveCnt++;
		}
		//�{�X�̐���
		if (this->moveCnt == this->time_create_boss)
		{
			//�{�X�̐���
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
			//���o���̓J�������{�X�ɒǏ]������
			auto sp =
				ge->GetTask_One_G<Sprite::Object>(Sprite::defGroupName);
			sp->Set_Target(boss_head);
			//�x���G�t�F�N�g�̐���
			//�����ʒu��錾
			ML::Vec2 init_pos_target_effect =
				ML::Vec2(boss_head->pos.x, boss_head->pos.y - boss_head->hitBase.h / 2);
			this->eff->Create_Effect(9, init_pos_target_effect);
			this->eff->Create_Effect(10, init_pos_target_effect);
			this->eff->Create_Effect(11, init_pos_target_effect);
		}
		//�{�XBGM�̍Đ�
		if (this->moveCnt == this->time_bgm)
		{
			DM::Sound_Play_Volume(this->res->name_bgm_boss,true,VOLUME_BGM_BOSS);
			//�{�X�ɒǏ]�����Ă����J�������v���C���ɖ߂�
			auto sp =
				ge->GetTask_One_G<Sprite::Object>(Sprite::defGroupName);
			auto pl =
				ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			sp->Set_Target(pl);
		}
		//�Q�[���N���A�����g���C��BGM�t�F�[�h�A�E�g
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
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�f�o�b�O���[�h�̎��A�����`��\��
		if (ge->debugMode)
		{
			ML::Box2D draw = this->hitBase;
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw(this->res->name_image, draw, src, ML::Color(0.5f, 0.0f, 1.0f, 0.0f));
		}
	}
	//�{�X�̏o���t���O��Ԃ�
	bool Object::Get_Flag_Spawn_Boss()
	{
		return this->flag_spawn_boss;
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