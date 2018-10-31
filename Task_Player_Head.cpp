//-------------------------------------------------------------------
//�v���C�������\���p
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Player_Head.h"

namespace  Player_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerHeadImg";
		DG::Image_Create(this->imageName, "./data/image/player.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.5f;				//�`�揇
		this->interval_anim_def = 16;					//�A�j���[�V�����Ԋu�f�t�H���g
		this->interval_anim_shot = 4;					//�A�j���[�V�����Ԋu�V���b�g
		this->angle = 0.0f;								//�p�x
		this->hitBase= ML::Box2D(-69, -46, 138, 92);	//�����`
		this->center_Rotate = ML::Vec2(69, 36);			//��]��
		this->controllerName = "P1";					//�R���g���[���[�錾
		
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
		this->animCnt++;
		if (ge->pause) { return; }
		this->moveCnt++;
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (nullptr == pl) { return; }
		auto in = DI::GPad_GetState(this->controllerName);
		this->angle_LR = pl->angle_LR;
		this->pos = pl->pos;
		//���ʂ̏���
		switch (pl->state)
		{
		default:
			this->angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
			//�p�x�ɏ㉺����݂���ꍇ
			if (this->angle_LR == Right)
			{
				if (ML::ToDegree(this->angle) > 45.0)
				{
					this->angle = ML::ToRadian(45.0f);
				}
				else if (ML::ToDegree(this->angle) < -45.0f)
				{
					this->angle = ML::ToRadian(-45.0f);
				}
			}
			else
			{
				if (in.RStick.axis != ML::Vec2(0, 0))
				{
					this->angle += ML::ToRadian(180.0f);
					if (ML::ToDegree(this->angle) > 45.0f && ML::ToDegree(this->angle) < 180.0f)
					{
						this->angle = ML::ToRadian(45.0f);
					}
					else if (ML::ToDegree(this->angle) < 315.0f &&
						!(ML::ToDegree(this->angle) > 0.0f && ML::ToDegree(this->angle) < 45.0f))
					{
						this->angle = ML::ToRadian(315.0f);
					}
				}
			}
			break;
			//��O�����i���𓮂����Ȃ��j
		case SlowDown:
		case Landing:
		case PreStomp:
		case LandStomp:
		case AirStomp:
		case StompLanding:
		case Damage:
			break;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (nullptr == pl) { return; }
		//���G����8�t���[����4�t���[���摜��\�����Ȃ��i�_�ł���j
		if (pl->time_un_hit > 0) {
			if ((pl->time_un_hit / 4) % 2 == 0) {
				return;
			}
		}
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		//�f�t�H���g�̒l��p��
		int wide = 138, height = 92;
		//�A�j���[�V����
		int anim = 0;
		switch (pl->state)
		{
		default:
			anim = this->animCnt / this->interval_anim_def;
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			anim = this->animCnt / this->interval_anim_shot;
		}
		anim %= 4;
		ML::Box2D  src(wide * 16, anim*height, wide, height);
		//	�����ɉ����ĉ摜�����E���]����
		if (false == this->angle_LR) {
			src.y = src.y + height * 4;
		}
		//�_���[�W
		if (pl->time_un_hit>0)
		{
			src.x += wide;
		}
		DG::Image_Rotation(this->res->imageName, this->angle,
			this->center_Rotate);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
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