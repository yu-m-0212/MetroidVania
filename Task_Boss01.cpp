//-------------------------------------------------------------------
//�{�X01
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss01.h"

namespace  Task_Boss01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "image_boss01";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->render2D_Priority[1] = 0.6f;				//�`�揇
		this->state = Boss_Floating;					//�{�X�p��ԊǗ�
		this->hitBase = ML::Box2D(-92, -46, 184, 92);	//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;				//��`�g��
		this->state = Stand;							//��ԊǗ�
		this->interval_shake = 20;						//�Ԋu���E�h��
		this->speed_shake = 15.0f;						//���x�h��
		this->std_pos_x = 700.0f;						//��l�������W
		this->gravity = ML::Gravity(SIZE_CHIP);			//�d�͉����x
		this->max_speed_fall = 2.0f;					//�ő嗎�����x

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->moveCnt++;
		this->animCnt++;
		this->Think();
		this->Move();
		//�ړ�����
		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//���^�X�N�̉�]�������Ȃ��悤�ɏC��
		DG::Image_Rotation(this->res->name_image, 0.0f, ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
		DG::Image_Draw(this->res->name_image, di.draw, di.src);
	}
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	//����	�F	(�U����,�U�����,�^���G����)
	void Object::Received(BChara* from_, AttackInfo at_, const int&)
	{
		//�_���[�W����
		this->hp -= at_.power;
		this->UpdateMotion(Damage);
	}
	//�v�l���󋵔��f(�X�e�[�^�X����j
	void  Object::Think()
	{
		BChara::State  nm = this->state;	//�Ƃ肠�������̏�Ԃ��w��

		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���鎖��ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm)
		{
		default:
			break;
		case Boss_Floating:
		{
			auto boss = ge->GetTask_One_G<Task_Boss01::Object>(Task_Boss01::defGroupName);
			if (nullptr == boss) { return; }
			ML::Box2D  me = this->recieveBase.OffsetCopy(this->pos);
			//�����Ƃ̐ڐG�𔻒肷��
			ML::Box2D you = ML::Box2D(boss->recieveBase.x, boss->recieveBase.y - 1,
				boss->recieveBase.w, 1);
			you.Offset(boss->pos);
			if (you.Hit(me))
			{
				nm = Boss_Common;
			}
			if (this->CheckFoot()) { nm = Boss_Base; }
			break;
		}
		case Boss_Common:
			if (!this->CheckFoot()) { nm = Boss_Floating; }
			break;
		case Boss_Base:
			break;
		}
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//���[�V�����ɑΉ���������
	void  Object::Move()
	{
		switch (this->state)
		{
		default:
			break;
		case Boss_Floating:
			//�d�͉���
			this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
			break;
		case Boss_Common:
			this->moveVec.y = 0.0f;
			break;
		case Boss_Base:
			this->moveVec.y = 0.0f;
			//����p������
			this->pos.x = this->std_pos_x + float(sin(this->moveCnt / this->interval_shake) * this->speed_shake);
			break;
		}
	}
	//�A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		//�f�t�H���g�̒l��p��
		int x = 184, y = 92;
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] =
		{
			{this->recieveBase,ML::Box2D(x * 0,y * 0,184,92),dc}
		};
		BChara::DrawInfo  rtv;
		//�A�j���[�V�����J�E���^�̐錾

		switch (this->state) 
		{
		default:
		case Boss_Floating:
		case Boss_Common:
		case Boss_Base:
			rtv = imageTable[0];
			break;
		}
		return rtv;
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