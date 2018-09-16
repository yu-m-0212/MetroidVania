//-------------------------------------------------------------------
//�K��
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Gun.h"
#include	"Task_Player.h"

namespace  Gun
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "GunImage";
		DG::Image_Create(this->imageName, "./data/image/chara(��)01.png");
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
		this->render2D_Priority[1] = 0.4f;
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		this->pos = pl->pos;
		this->hitBase = ML::Box2D(-64, -32, 128, 64);
		this->controllerName = "P1";
		this->tremor = 3.0f;

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
		this->moveCnt++;
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		auto in = DI::GPad_GetState(this->controllerName);
		this->angle_LR = pl->angle_LR;
		
		this->pos = pl->pos;
		this->Think();
		this->Move();
		this->angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
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
				if (ML::ToDegree(this->angle) > 45.0f && ML::ToDegree(this->angle)<180.0f)
				{
					this->angle = ML::ToRadian(45.0f);
				}
				else if (ML::ToDegree(this->angle) < 315.0f &&
					!(ML::ToDegree(this->angle) > 0.0f && ML::ToDegree(this->angle)<45.0f))
				{
					this->angle = ML::ToRadian(315.0f);
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D  src(1216, 0, 64, 32);
		if (this->angle_LR == Left)
		{
			src.y = 32;
		}
		DG::Image_Rotation(this->res->imageName, this->angle, 
			ML::Vec2(float(this->hitBase.w/2), float(this->hitBase.h/2)));
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//�v���C�������ł����Ƃ��A�v���C��������Kill
	void Object::Gun_Kill()
	{
		this->Kill();
	}
	//
	float Object::Get_Angle()
	{
		return this->angle;
	}
	//�v�l
	void Object::Think()
	{
		BChara::State nm = this->state; //�Ƃ肠�������̏�Ԃ��w��
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���邱�Ƃ�ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		//���[�V�����X�V
		switch (nm)
		{
		default:
			break;
		case Stand:
			if (pl->state == Shoot) { nm = Shoot; }
			if (pl->state == Jumpshoot) { nm = Jumpshoot; }
			if (pl->state == Fallshoot) { nm = Fallshoot; }
			break;
		case Shoot:
			if (!(pl->state==Shoot)) { nm = Stand; }
			break;
		case Jumpshoot:
			if (!(pl->state==Jumpshoot)) { nm = Stand; }
			break;
		case Fallshoot:
			if (!(pl->state==Fallshoot)) { nm = Stand; }
			break;
		}
		this->UpdateMotion(nm);
	}
	//���[�V�����ɑΉ���������
	//(���[�V�����͕ύX���Ȃ�)
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		switch (this->state) 
		{
		default:
			break;
		case Stand:
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			//���C���͏㉺�ɗh�炷
			this->pos.y = float(pl->pos.y + sin(this->moveCnt) * this->tremor);
			break;
		}
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