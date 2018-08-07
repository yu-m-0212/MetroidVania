//-------------------------------------------------------------------
//�G00(�X���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Enemy01.h"

namespace  Enemy01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Enemy01Img";
		DG::Image_Create(this->imageName, "./data/image/Enemy01.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = ML::Box2D(-28, -22, 56, 45);
		this->angle_LR = Left;
		this->state = Stand;
		this->hp = 20;				//hp�����l
		this->maxSpeed = 2.0f;		//�ő�ړ����x(��)
		this->addSpeed = 0.7f;		//���s�����x(�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;		//�ڒn��Ԃ̎��̑��x������(���C
		this->maxFallSpeed = 10.0f;	//�ő嗎�����x
		this->jumpPow = -6.0f;		//�W�����v��(����)
		this->gravity = ML::Gravity(32) * 5;	//�d�͉����x&���ԑ��x�ɂ����Z��
		
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
		this->animCnt++;
		//���G���Ԃ̌���
		if (this->unHitTime > 0) { this->unHitTime--; }
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
		//�߂荞�܂Ȃ��ړ�
		ML::Vec2 est = this->moveVec;
		this->CheckMove(est);

		//�����蔻��
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�v���C��");
			for (auto it = targets->begin();
				it != targets->end();
				++it) {
				//����ɐڐG�̗L�����m�F������
				if ((*it)->CheckHit(me)) {
					//����Ƀ_���[�W�̏������s�킹��
					BChara::AttackInfo at = { 1,0,0 };
					(*it)->Received(this, at);
					break;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���G���Ԓ��͓_��
		//8�t���[����4�t���[���摜��\�����Ȃ�
		if (this->unHitTime > 0) {
			if ((this->unHitTime / 4) % 2 == 0) {
				return;
			}
		}
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		if (this->unHitTime > 0) {
			return;//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		}
		this->unHitTime = 30;//���G����
		this->hp -= at_.power;//�e��hp
		if (this->hp <= 0) {
			this->Kill();
		}
		//������΂����
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+4, -9); }
		else { this->moveVec = ML::Vec2(-4, -9); }
		this->UpdateMotion(Bound);
		//from_�͍U�����Ă�������A�J�E���^�[�Ȃǂŋt�Ƀ_���[�W��^�������Ƃ��Ɏg��
	}
	//-------------------------------------------------------------------
	//�v�l&�󋵔��f�@���[�V��������
	void Object::Think()
	{
		BChara::State nm = this->state; //�Ƃ肠�������̏�Ԃ��w��

		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���邱�Ƃ�ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm) {
		case	Stand:	//�����Ă���
			if (this->CheckFoot() == false) { nm = Fall; }
			else							{ nm = Walk; }//�ڒn�ŕ����Ɉڍs
			break;
		case	Walk:	//�����Ă���
			if (this->CheckFront_LR() == true) { nm = Turn; }//�ǂɏՓ�
			if (this->CheckFoot() == false) { nm = Fall; }
			if (this->CheckFrontFoot_LR() == false) { nm = Turn; }
			break;
		case	Turn:	//����
			if (this->moveCnt >= 5) { nm = Stand;}
			break;
		case	Jump:	//�㏸��
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
		case	Fall:	//������
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Bound:	//�_���[�W���󂯂Đ������ł���
						//8�t���[���o�߂Ɛڒn�Łu�����Ă���v�ɖ߂�
			if (this->moveCnt >= 8 && this->CheckFoot() == true) { nm = Stand; }
			break;
		case	TakeOff://��ї���
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Landing://���n
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
		}
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//-------------------------------------------------------------------
	//���[�V�����ɑΉ���������
	//(���[�V�����͕ύX���Ȃ�)
	void Object::Move()
	{
		//�d�͉���
		switch (this->state) {
		default:
			//�㏸���������͑����ɒn�ʂ�����
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) {
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);//min( , )
			}
			//�n�ʂɐڐG���Ă���
			else {
				this->moveVec.y = 0.0f;
			}
			break;
			//�d�͉����𖳌�������K�v�����郂�[�V�����͉���case�������i���ݑΏۖ����j
		case Unnon:	break;
		}

		//�ړ����x����
		switch (this->state) {
		default:
			if (this->moveVec.x < 0) {
				this->moveVec.x = min(this->moveVec.x + this->decSpeed, 0);
			}
			else {
				this->moveVec.x = max(this->moveVec.x - this->decSpeed, 0);
			}
			//������
			break;
			//�ړ����x�����𖳌�������K�v�����郂�[�V�����͉���case������
		case Bound:
		case Unnon:	break;
		}
		//-------------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->state) {
		case  Stand:	//�����Ă���
			break;
		case  Walk:		//�����Ă���
			/*this->maxSpeed = this->maxSpeed * 3;*/
			if (this->angle_LR == Left) {//�����ړ�(���X�ɉ���)
				this->moveVec.x =
					max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (this->angle_LR == Right) {
				this->moveVec.x =
					min(+this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case	Turn:	//����
			if (this->moveCnt == 3) {
				if (this->angle_LR == Left) { this->angle_LR = Right; }
				else {						  this->angle_LR = Left; }
			}
			break;
		case  Fall:		//������
			if (this->angle_LR == Left) {//�����ړ�(���X�ɉ���)
				this->moveVec.x =
					max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (this->angle_LR == Right) {
				this->moveVec.x =
					min(+this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case  Jump:		//�㏸��
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw						src
			{ ML::Box2D(-32,-24,64,48),ML::Box2D(0,0,64,48),dc },//��~
		{ ML::Box2D(-32,-32,64,64),ML::Box2D(128,48,64,64),dc },//����
		{ ML::Box2D(-32,-32,64,64),ML::Box2D(0,116,64,64),dc }//�_���[�W
		};
		BChara::DrawInfo rtv;
		/*int work;*/
		switch (this->state) {
		default:	rtv = imageTable[0];	break;
			//	�W�����v------------------------------------------------------------------------
		case  Jump:		rtv = imageTable[0];	break;
			//	��~----------------------------------------------------------------------------
		case  Stand:	rtv = imageTable[0];	break;
			//	���s----------------------------------------------------------------------------
		case  Walk:		rtv = imageTable[0];	break;
			//	����----------------------------------------------------------------------------
		case  Fall:		rtv = imageTable[1];	break;
			//	�_���[�W------------------------------------------------------------------------
		case  Bound:	rtv = imageTable[2];	break;
		}
		//�����ɉ����ĉ摜�����E���]����
		//���[�V������Bound�̎�
		if (this->state == Bound)
		{
			auto shot = ge->GetTask_One_G<Shot00::Object>("�v���C��");
			//�ʒu�֌W		�e	| �G�l�~�[
			if (shot->pos.x < this->pos.x)
			{
				rtv.src = ML::Box2D(64, 116, -64, 64);
			}
			//�ʒu�֌W	�G�l�~�[ |	�e
			else
			{
				rtv.src = ML::Box2D(0, 116, 64, 64);
			}
		}
		//���[�V������Bound�ȊO�̎�
		//�A���O�����ς��Ɖ摜�𔽓]
		else
		{
			if (false == this->angle_LR) {
				rtv.draw.x = -rtv.draw.x;
				rtv.draw.w = -rtv.draw.w;
			}
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