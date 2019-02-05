//-------------------------------------------------------------------
//�A�C�e��01(�o���A�A�r���e�B)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item01.h"
#include  "Task_Player.h"

namespace  Item01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "image_item01";
		DG::Image_Create(this->name_image, "./data/image/Item01.png");

		this->name_se_pick_up_item = "se_pick_up_item";
		DM::Sound_CreateSE(this->name_se_pick_up_item, "./data/sound/wav/pick_up_life_item.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DM::Sound_Erase(this->name_se_pick_up_item);
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
		this->render2D_Priority[1] = 0.7f;								//�`�揇
		this->limit_message = 180;										//���b�Z�[�W�̎��Ԑ���
		this->time_erase = 120;											//���g�̏��Ń^�C�~���O
		this->hitBase = ML::Box2D(-16, -16, 32, 32);					//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;								//�L�����N�^�Ƃ̔����`
		this->center = ML::Vec2(float(ge->screenWidth / 2.0f),
			float(ge->screenHeight / 2.0f));							//��ʂ̒��S���W
		this->init_pos_create_tutorial = ML::Vec2(7228.0f, 5120.0f);	//���˃`���[�g���A���̐������W
		this->tutorials = new Tutorials::Object();						//���\�b�h�Ăяo��
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->tutorials;
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//���G���Ԃ̌���
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		switch (this->state)
		{
		case Stand:
			this->pos.y += float(sin(this->moveCnt / 12));
			break;
		case Lose:
			//SE�Đ�
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_se_pick_up_item, false, VOLUME_ALL_GAME);
				//���˃`���[�g���A���̐���
				this->tutorials->Create_Message("�o���A�͓G�̍U���𒵂˕Ԃ����Ƃ��ł���", this->init_pos_create_tutorial, -1);
			}
			//����
			if (this->moveCnt > this->time_erase)
			{
				this->Kill();
			}
			break;
		}
		//�J�E���^�C���N�������g
		this->moveCnt++;
		this->animCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->name_image, di.draw, di.src, di.color);
	}
	//-------------------------------------------------------------------
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	void Object::Recieved(BChara* from_, AttackInfo at_,const int& un_hit_)
	{
		if (this->state != Stand)
		{
			return;
		}
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//���b�Z�[�W����
		this->tutorials->Create_Message("�o���A���擾����[��������]", this->center, this->limit_message);
		this->UpdateMotion(Lose);
		//�o���A��L���ɂ���
		pl->Set_Barrier(true);
		pl->Set_Barrier_Recharge(true);
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
			//draw					src						color
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1) },	//Stand[0]
		};
		BChara::DrawInfo  rtv;
		switch (this->state) 
		{
		//��~----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
		}
		return rtv;
	}
	//�A�C�e��01�̐���
	//����	�F	�i�������W�j
	void Object::Create_Item01(const ML::Vec2& pos_)
	{
		auto item = Item01::Object::Create(true);
		item->pos = pos_;
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