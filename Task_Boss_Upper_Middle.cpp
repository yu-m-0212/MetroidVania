//-------------------------------------------------------------------
//�{�X�i�A�b�p�[�~�h���j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Upper_Middle.h"
#include  "Task_Boss_Upper.h"

namespace  Boss_Upper_Middle
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "upper_middle";
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
		this->state = Stand;								//��ԊǗ�
		this->render2D_Priority[1] = 0.6f;					//�`�揇
		this->hitBase = ML::Box2D(-92, -46, 184, 92);		//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;					//�L�����N�^�Ƃ̔����`
		this->speed_chase = 0.075f;							//���x�Ǐ]
		this->shot = new Shot01::Object();					//���\�b�h�Ăяo��
		this->boss = new Boss();							//���\�b�h�Ăяo��
		this->vec_shot = ML::Vec2(-SPEED_SHOT, 0.0f);		//�ړ��ʃV���b�g
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);			//��`�V���b�g
		this->cnt_move = 0;									//�J�E���^�s��
		this->interval_shot = -1;							//�������ԃV���b�g
		this->hp = HP_BOSS;									//HP�{�X
		this->add_un_hit = 60;								//�v���C���ɗ^���閳�G����
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->shot;
		delete this->boss;
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
		//�|�[�Y
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//���G���Ԃ̌���
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->name_image, di.draw, di.src, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	//����	�F	(�U����,�U�����,�^���G����)
	void Object::Received(BChara* from_, BChara::AttackInfo at_, const int& un_hit_)
	{
		//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		if (this->time_un_hit > 0)
		{
			return;
		}
		this->hp -= at_.power;
		//���G����
		this->time_un_hit = un_hit_;
		//��ԑJ��
		this->UpdateMotion(Damage);
	}
	//-------------------------------------------------------------------
	//�v�l&�󋵔��f�@���[�V��������
	void Object::Think()
	{
		BChara::State nm = this->state; //�Ƃ肠�������̏�Ԃ��w��
										//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���邱�Ƃ�ړI�Ƃ��Ă���B
										//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm)
		{
		case Stand:		//�����Ă���
			break;
		case Damage:	//��e
			if (this->time_un_hit == 0) { nm = Stand; }
			break;
		case Lose:		//���S
			break;
		}
		//���S����
		if (this->hp <= 0) { nm = Lose; }
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//-------------------------------------------------------------------
	//���[�V�����ɑΉ���������
	//(���[�V�����͕ύX���Ȃ�)
	void Object::Move()
	{
		//�����̏�Ԃŋ��ʂ��čs�������͂����ɋL�q����
		//��O������ꍇ��switch���̉����ɋL�q����

		//��ƂȂ�^�X�N�ɒǏ]����
		auto upper = 
			ge->GetTask_One_GN<Boss_Upper::Object>(Boss_Upper::defGroupName,Boss_Upper::defName);
		//���݂��邩�m�F
		if (nullptr == upper) { return; }
		//�ڕW�Ɍ������Ĉړ�����
		this->pos.x += this->boss->Chase_Target(this->pos.x, upper->pos.x, this->speed_chase);
		this->pos.y = upper->pos.y + float(this->hitBase.h);
		//��Ԃ��󂯎��
		this->state = upper->state;

		//�����蔻��
		ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
		auto targets = ge->GetTask_Group_G<BChara>("�v���C��");
		if (nullptr == targets) { return; }
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			//����ɐڐG�̗L�����m�F������
			if ((*it)->CheckHit(me)) {
				//����Ƀ_���[�W�̏������s�킹��
				BChara::AttackInfo at = { 1,0,0 };
				(*it)->Received(this, at, this->add_un_hit);
				break;
			}
		}

		//��ƂȂ�^�X�N�̏�Ԃɉ����čU������
		switch (this->state)
		{
		default:
			break;
		case Wait_Under:
			//�V���b�g�����p�J�E���^��i�߂�
			this->cnt_move++;
			//�V���b�g�̐������Ԃ������l�Ȃ�l������
			if (this->interval_shot == -1)
			{
				this->interval_shot = rand() % MAX_INTERVAL_SHOT;
			}
			//�������ԂɂȂ�����V���b�g�𐶐�����
			if (this->cnt_move == this->interval_shot)
			{
				this->shot->Create_Shot(this->pos, this->vec_shot, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
				//�J�E���^�Ɛ������Ԃ����Z�b�g����
				this->cnt_move = 0;
				this->interval_shot = -1;
			}
			break;
		}
		//-------------------------------------------------------------------
		//���[�V�������ɌŗL�̏���

	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		//�f�t�H���g�̒l��錾
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] =
		{
			//draw		   src			 color
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 0,this->hitBase.w,this->hitBase.h),dc },//default	[0]
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 1,this->hitBase.w,this->hitBase.h),dc },//Damage	[1]
			{ this->hitBase,ML::Box2D(0,this->hitBase.h * 2,this->hitBase.w,this->hitBase.h),dc } //Lose	[2]
		};
		BChara::DrawInfo rtv;
		//�A�j���[�V�����p�ϐ�

		switch (this->state)
		{
		default:
			rtv = imageTable[0];
			break;
		case Stand:
			rtv = imageTable[0];
			break;
		case Damage:
			rtv = imageTable[1];
			break;
		case Lose:
			rtv = imageTable[2];
			break;
		}
		return rtv;
		/*return imageTable[int(this->state)];*/
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