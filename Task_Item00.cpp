//-------------------------------------------------------------------
//�A�C�e��00(�̗͏���A�b�v)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item00.h"
#include  "Task_Player.h"

namespace  Item00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Item00Img";
		DG::Image_Create(this->imageName, "./data/image/Item00.png");

		this->name_se_pick_up_ability_00 = "se_pick_up_ability00";
		DM::Sound_CreateSE(this->name_se_pick_up_ability_00, "./data/sound/wav/jingle_pick_up_ability_00.wav");
		this->name_se_pick_up_ability_01 = "se_pick_up_ability01";
		DM::Sound_CreateSE(this->name_se_pick_up_ability_01, "./data/sound/wav/jingle_pick_up_ability_01.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_se_pick_up_ability_00);
		DM::Sound_Erase(this->name_se_pick_up_ability_01);
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
		this->render2D_Priority[1] = 0.7f;													//�`�揇
		this->hitBase = ML::Box2D(-16, -16, 32, 32);										//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;													//�L�����N�^�Ƃ̔����`
		this->add_Hp = 1;																	//�v���C����HP������
		this->limit_message = 180;															//���Ԑ������b�Z�[�W
		this->time_play_second_se = 75;									//���ł܂ł̎���
		this->time_erase = 180;											//���g�̏��Ń^�C�~���O
		this->center = ML::Vec2(float(ge->screenWidth / 2), float(ge->screenHeight / 2));	//��ʒ���
		this->tutorials = new Tutorials::Object();											//�|�C���^�`���[�g���A��
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		
		delete this->tutorials;
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (this->time_un_hit > 0) { this->time_un_hit--; }

		switch (this->state) {
		case Stand:
			this->pos.y += float(sin(this->moveCnt/12));
			break;
		case Lose:
			//1�ڂ�SE���Đ�
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_se_pick_up_ability_00, false, VOLUME_ALL_GAME);
			}
			//2�ڂ�SE���Đ�������
			if (this->moveCnt == this->time_play_second_se)
			{
				DM::Sound_Play_Volume(this->res->name_se_pick_up_ability_01, false, VOLUME_ALL_GAME);
			}
			//SW�Đ���A����
			if (this->moveCnt >= this->time_erase)
			{
				this->Kill();
			}
			break;
		}
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
		DG::Image_Draw(this->res->imageName, di.draw, di.src, di.color);
	}
	//-------------------------------------------------------------------
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
	void Object::Recieved(BChara* from_, AttackInfo at_, const int& un_hit_)
	{
		if (this->state != Stand) 
		{
			return;
		}
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr == pl) { return; }
		//���b�Z�[�W����
		this->tutorials->Create_Message("HP�̏������������", this->center, this->limit_message);
		this->UpdateMotion(Lose);
		//�̗͏���𑝉����A�񕜂���
		int max = pl->Get_Max_HP();
		max += this->add_Hp;
		pl->Set_Max_HP(max);
		pl->hp = pl->Get_Max_HP();
	}
	//-------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		BChara::DrawInfo imageTable[] = {
			//draw					src						color
			{ ML::Box2D(-16,-16,32,32),ML::Box2D(0,0,64,64),ML::Color(1,1,1,1)},	//Stand[0]
		};
		BChara::DrawInfo  rtv;
		switch (this->state) {
			//	��~----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
			//	������E���V---------------------------------------------------------------------
		case Lose:		rtv = imageTable[1];	break;
		}
		return rtv;
	}
	//�A�C�e��00�̐���
	//����	�F	�i�������W�j
	void Object::Create_Item00(const ML::Vec2& pos_)
	{
		auto item = Item00::Object::Create(true);
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