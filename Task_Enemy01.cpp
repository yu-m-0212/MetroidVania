//-------------------------------------------------------------------
//�G00(�X���C��
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Shot00.h"
#include	"Task_Shot01.h"
#include	"Task_Enemy01.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Player.h"
#include	"Task_EnemySearch.h"

namespace  Enemy01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Enemy01Img";
		DG::Image_Create(this->imageName, "./data/image/enemy01.png");
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
		this->hitBase = ML::Box2D(-52,-48,102,96);
		this->recieveBase = this->hitBase;
		this->angle_LR = Left;
		this->state = Stand;
		this->hp = 10;
		this->maxSpeed = 2.0f;							//�ő�ړ����x(��)
		this->addSpeed = 0.7f;							//���s�����x(�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;							//�ڒn��Ԃ̎��̑��x������(���C
		this->maxFallSpeed = 10.0f;						//�ő嗎�����x
		this->gravity = ML::Gravity(32) * 5;			//�d�͉����x&���ԑ��x�ɂ����Z��
		this->interval_Caution = 60;					//�v���C�������E����O�ꂽ��A�ēx�x���ɓ���܂ł̎���
		this->interval_Attack = 120;					//�e�𐶐�����Ԋu
		this->interval_Flash = 4;						//�_�ŊԊu
		this->searchBase = ML::Box2D(-192, -96, 384, 192);
		this->shot_Init = ML::Vec2(0, -45);
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
		//���j�G�t�F�N�g�̐���
		auto DefeatEffect = Effect::Object::Create(true);
		DefeatEffect->pos = this->pos;
		DefeatEffect->Set_Limit(24);
		DefeatEffect->state = Lose;
		DefeatEffect->angle_LR = this->angle_LR;
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
		if (this->unHitTime > 0) { this->unHitTime--; }
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
		//�߂荞�܂Ȃ��ړ�
		ML::Vec2 est = this->moveVec;
		this->CheckMove(est);
		//�����蔻��
		//���g�������Ԃ̎��̓v���C���ɐڐG���Ă��U�����Ȃ�
		if (this->state != Bound)
		{
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
					(*it)->Received(this, at);
					break;
				}
			}
		}
		//HP��0����
		//���ɐG��邩��莞�Ԍo�߂ŏ���
		if (this->hp <= 0 &&
			(this->CheckFoot() || this->moveCnt >= 60))
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���G���Ԓ��͓_��
		if (this->unHitTime > 0) {
			if (this->unHitTime %this->interval_Flash == 0) {
				return;
			}
		}
		BChara::DrawInfo di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, di.draw, di.src,ML::Color(1.0f,1.0f,1.0f,1.0f));
	}
	//-------------------------------------------------------------------
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		//������
		//hp������ƍs���������Ȃ�
		this->interval_Caution -= 4;
		this->interval_Attack -= 1;
		//�U�����󂯂�����������
		if (this->pos.x - from_->pos.x > 0)
		{
			this->angle_LR = Left;
		}
		else
		{
			this->angle_LR = Right;
		}
		if (this->unHitTime > 0) {
			return;//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		}
		this->hp -= at_.power;
		//�m�b�N�o�b�N�̔������Ȃ��U���̏ꍇ�A�ȉ���ǂ܂Ȃ�
		if (from_->Get_Tip()) { return; }
		//���G����
		this->unHitTime = 30;
		//�܂��͈͍U�����ǂ����𔻒肷��
		if (!from_->Get_Range_Wide())
		{
			//������΂����
			this->moveVec = from_->moveBack;
		}
		//�͈͍U���̏ꍇ�͍U�����󂯂��u�Ԃ̈ʒu�֌W�Ŕ�ԕ��������߂�
		else
		{
			//�������E���ɂ���Ƃ�
			if (this->pos.x - from_->pos.x > 0)
			{
				this->moveVec = from_->moveBack;
			}
			//�����������ɂ���Ƃ�
			else
			{
				float x = from_->moveBack.x*(-1);
				this->moveVec = ML::Vec2(x, from_->moveBack.y);
			}
		}
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
			if ( this->Search_Player()) { nm = Caution; }
			if (this->CheckFront_LR() == true) { nm = Turn; }//�ǂɏՓ�
			if (this->CheckFoot() == false) { nm = Fall; }
			if (this->CheckFrontFoot_LR() == false) { nm = Turn; }
			break;
		case	Turn:	//����
			if (this->moveCnt >= 5) { nm = Stand;}
			if ( this->Search_Player()) { nm = Caution; }
			break;
		case	Jump:	//�㏸��
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
		case	Fall:	//������
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Bound:
			//�_���[�W���󂯂Đ������ł���
			//8�t���[���o�߂Ɛڒn�Łu�����Ă���v�ɖ߂�
			if (this->moveCnt >= 8 && this->CheckFoot() == true) { nm = Stand; }
			break;
		case	TakeOff://��ї���
			if (this->CheckFoot() == true) { nm = Stand; }
			break;
		case	Landing://���n
			if (this->CheckFoot() == false) { nm = Fall; }
			break;
		case Caution:
			if (! this->Search_Player()) { nm = TargetLost; }
			else if ( this->Search_Player() && this->moveCnt >= this->interval_Attack) { nm = Shoot; }
			break;
		case TargetLost:
			if ( this->Search_Player()) { nm = Caution; }
			//�������ꂸ�Ɉ�莞�Ԍo�߂���ƌx������
			if (this->moveCnt >= this->interval_Caution &&
				! this->Search_Player())
			{
				nm = Stand;
			}
			break;
		case Shoot:
			if (! this->Search_Player()) { nm = TargetLost; }
			if (this->moveCnt >= 12&&  this->Search_Player()) { nm = Caution; }
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
				this->CheckFoot() == false) 
			{
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
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
		switch (this->state) 
		{
		case  Stand:	//�����Ă���
			break;
		case  Walk:		//�����Ă���
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
		case Caution:
		{
			auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
			if (nullptr == pl) { break; }
			//�v���C���Ɍ�����ς���
			if (this->pos.x - pl->pos.x > 0)
			{
				this->angle_LR = Left;
			}
			else
			{
				this->angle_LR = Right;
			}
			break;
		}
		case Shoot:
			if (this->moveCnt == 0)
			{
				auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
				if (nullptr == pl) { break; }
				auto shot = Shot01::Object::Create(true);
				//�Ăяo���������`�Ɏv�l�����邽�ߏ�Ԃ��w��
				shot->state = Shoot;
				shot->hitBase = ML::Box2D(-16, -16, 32, 32);
				shot->Set_Limit(300);
				shot->Set_Erase(0);
				shot->Set_Power(1);
				shot->angle_LR = this->angle_LR;
				shot->pos = this->pos + this->shot_Init;
				//�v���C���Ɍ������Ēe�𔭎�
				ML::Vec2 toTarget = pl->pos - this->pos;
				float angle = atan2(toTarget.y, toTarget.x);
				shot->moveVec = ML::Vec2(cos(angle), sin(angle))*8.0f;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		ML::Box2D dd(-48, -48, 96, 96);
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw						src
			{ dd,ML::Box2D(  0,  0,192,192),dc },	//�ҋ@		[0]
			{ dd,ML::Box2D(192,  0,192,192),dc },	//���s1		[1]
			{ dd,ML::Box2D(192,192,192,192),dc },	//���s2		[2]
			{ dd,ML::Box2D(192,384,192,192),dc },	//���s3		[3]
			{ dd,ML::Box2D(192,576,192,192),dc },	//���s4		[4]
			{ dd,ML::Box2D(384,  0,192,192),dc },	//�^�[��		[5]
			{ dd,ML::Box2D(576,  0,192,192),dc },	//�ˌ�01		[6]
			{ dd,ML::Box2D(576,192,192,192),dc },	//�ˌ�02		[7]
			{ dd,ML::Box2D(768,  0,192,192),dc },	//������01	[8]
			{ dd,ML::Box2D(768,192,192,192),dc },	//������02	[9]
			{ dd,ML::Box2D(960,  0,192,192),dc },	//�x��01		[10]
			{ dd,ML::Box2D(960,192,192,192),dc }	//�x��02		[11]
		};
		BChara::DrawInfo rtv;
		int anim = 0;
		switch (this->state) {
		default:		rtv = imageTable[0];	break;
		case  Stand:
			rtv = imageTable[0];
			break;
		case  Walk:
			anim = this->animCnt / 8;
			anim %= 4;
			rtv = imageTable[anim + 1];	
			break;
		case Turn:		rtv = imageTable[5];	break;
		case Shoot:
			anim = this->animCnt / 6;
			anim %= 2;
			rtv = imageTable[anim + 6];
			break;
		case TargetLost:
			anim = this->animCnt / 24;
			anim %= 2;
			rtv = imageTable[anim+8];
			break;
		case Caution:
			anim = this->animCnt / 24;
			anim %= 2;
			rtv = imageTable[anim+10];
			break;
		}
		//�����ɉ����ĉ摜�����E���]����
		//�A���O�����ς��Ɖ摜�𔽓]
		if (false == this->angle_LR) 
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//�v���C���𔭌�������x�����[�h�ɓ���
	bool Object::Search_Player()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (pl == nullptr || this == nullptr) { return false; }
		ML::Box2D you = pl->hitBase.OffsetCopy(pl->pos);
		ML::Box2D  me = this->searchBase.OffsetCopy(this->pos);
		//���g�̍��E�ŒT�m��`�����炷
		if (this->angle_LR == Left)
		{
			me.x -= this->searchBase.w / 2;
		}
		else
		{
			me.x += this->searchBase.w / 2;
		}
		return you.Hit(me);
	}
	//�T�m��`���擾����
	ML::Box2D Object::Get_Search()
	{
		return this->searchBase;
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