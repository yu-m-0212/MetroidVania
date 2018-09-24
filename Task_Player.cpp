//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Player.h"
#include	"Task_Sprite.h"
#include	"Task_Shot00.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Goal.h"
#include	"Task_Retry.h"
#include	"Task_Corpse.h"
#include	"Task_Gun.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
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
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(-23, -46, 46, 92);
		this->recieveBase = this->hitBase;
		this->angle_LR = Right;
		this->controllerName = "P1";
		this->state = Stand;								//�L�����������
		this->max_Hp = 10;									//HP�ő�l
		this->hp = this->max_Hp;							//HP�����l
		this->maxSpeed = 7.5f;								//�ő�ړ����x�i���j
		this->addSpeed = 0.75f;								//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;								//�ڒn��Ԃ̎��̑��x�����ʁi���C
		this->maxFallSpeed = 15.0f;							//�ő嗎�����x
		this->max_StompFallSpeed = 17.5f;					//�X�g���v�̍ő�~�����x
		this->height_Jump = -10.0f;							//�W�����v�́i�����j
		this->gravity = ML::Gravity(32);					//�d�͉����x�����ԑ��x�ɂ����Z��
		this->reach = 32.0;									//�˒�
		this->speed_Shot = 10;								//�V���b�g���x
		this->limit_StompHoldTime = 30;						//�X�g���v���n���̍d������
		this->limit_Stomp = 15;								//�X�g���v�p������
		this->limit_StompEffect = 18;						//�p�����ԃX�g���v����
		this->limit_Quake = 15;								//��ʗh�ꎞ��
		this->limit_Shot = 40;								//�p�����ԃV���b�g
		this->limit_JumpAngleChange = 24;					//�W�����v�����莞�ԓ��Ȃ������ς�����
		this->limit_HealEffect = 24;						//�p�����ԉ񕜃G�t�F�N�g
		this->dist_Quake = 5;								//��ʗh�ꕝ
		this->lv_Stomp = 1;									//�X�g���v�A�b�v�O���[�h���x��
		this->addUnHitTime = 30;							//��e���ɓ����閳�G����
		this->interval_Shot = 12;							//�ˌ��̔��ˊԊu�i�t���[���j
		this->range_Stomp = ML::Box2D(-96, -24, 192, 46);	//�͈̓X�g���v
		this->range_Shot = ML::Box2D(-8, -8, 16, 16);		//�͈̓V���b�g
		this->power_Stomp = 3;								//�U���̓X�g���v
		this->power_Shot = 1;								//�U���̓V���b�g
		this->moveBack_Stomp = ML::Vec2(12, -4);			//�X�g���v�ӂ��Ƃї�

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
		//�N���A�A�s�k��
		if (ge->clear || ge->failure) { return; }
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
		ML::Vec2  est = this->moveVec;
		this->CheckMove(est);
		//�A�C�e���Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("�A�C�e��");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//����ɐڐG�̗L�����m�F������
					if ((*it)->CheckHit(me)) {
						//����Ƀ_���[�W�̏������s�킹��
						BChara::AttackInfo at = { 0,0,0 };
						(*it)->Received(this, at);
						break;
					}
				}
			}
		}
		//��̂Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("���");
			if (nullptr != targets)
			{
				for (auto it = targets->begin();
					it != targets->end();
					++it) {
					//����ɐڐG�̗L�����m�F������
					if ((*it)->CheckHit(me)) {
						//����Ƀ_���[�W�̏������s�킹��
						BChara::AttackInfo at = { 1,0,0 };
						(*it)->Received(this, at);
						//�񕜃G�t�F�N�g�𐶐�
						auto healEffect = Effect::Object::Create(true);
						healEffect->pos = this->pos;
						healEffect->Set_Limit(this->limit_HealEffect);
						healEffect->state = Heal;
						break;
					}
				}
			}
		}
		//�J�����̈ʒu���Ē���
		{
			//��������ʂ̉����ɒu�����i����͉�ʒ����j
			int  px = ge->camera2D.w / 2;
			int  py = ge->camera2D.h / 2;
			//��������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int  cpx = int(this->pos.x) - px;
			int  cpy = int(this->pos.y) - py;
			//�J�����̍��W���X�V
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//�}�b�v�O���ʂ��Ȃ��悤�ɂ��钲������
		auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
		if (nullptr != map) {
			map->AjastCameraPos();
		}
		//hp0�ŃQ�[���I�[�o�[
		if (this->hp <= 0)
		{
			//�Q�[���I�[�o�[�t���O����
			ge->failure = true;
		}
		//���S�[���Ƃ̐ڐG����
		auto goal = ge->GetTask_One_G<Goal::Object>("�S�[��");
		if (nullptr != goal)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = goal->hitBase.OffsetCopy(goal->pos);
			if (you.Hit(me))
			{
				ge->clear = true;
			}
		}
		auto in = DI::GPad_GetState(this->controllerName);
		if (in.R3.on&&in.L3.on)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���G����8�t���[����4�t���[���摜��\�����Ȃ��i�_�ł���j
		if (this->unHitTime > 0) {
			if ((this->unHitTime / 4) % 2 == 0) {
				return;
			}
		}
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//���^�X�N�̉�]�������Ȃ��悤�ɏC��
		DG::Image_Rotation(this->res->imageName,0.0f,ML::Vec2(float(this->hitBase.w/2),float(this->hitBase.h/2)));
		DG::Image_Draw(this->res->imageName, di.draw, di.src);
	}
	//-----------------------------------------------------------------------------
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		if (this->unHitTime > 0) {
			return;//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		}
		this->unHitTime = this->addUnHitTime;
		this->hp -= at_.power;	//������
		//������΂����
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+2.0f, -4.5f); }
		else							{ this->moveVec = ML::Vec2(-2.0f, -4.5f); }
		this->UpdateMotion(Damage);
		//from_�͍U�����Ă�������A�J�E���^�[�Ȃǂŋt�Ƀ_���[�W��^�������Ƃ��Ɏg��
	}
	//-----------------------------------------------------------------------------
	//�v�l���󋵔��f�@���[�V��������
	void  Object::Think()
	{
		BChara::State  nm = this->state;	//�Ƃ肠�������̏�Ԃ��w��
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���鎖��ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm) {
		case  Stand:	//�����Ă���
			//�������u�Ԃ͕����]���̂ݍs��
			if (in.LStick.L.on && this->moveCnt >= 6) { nm = Walk; }
			if (in.LStick.R.on && this->moveCnt >= 6) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (in.B1.down) { nm = PreStomp; }
			if (!this->CheckFoot()) { nm = Fall; }//���� ��Q�@����
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (in.B1.down) { nm = PreStomp; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			break;
		case SlowDown:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.B1.down) { nm = PreStomp; }
			if (in.R1.on) { nm = Shoot; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt >= 12) { nm = Stand; }
			break;
		case  TakeOff:	//��ї���
			if (this->moveCnt >= 3) { nm = Jump; }
			break;
		case  Jump:		//�㏸��
			if (this->moveVec.y >= 0) { nm = Fall; }
			if (in.R1.on) { nm = Jumpshoot; }
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Fall:		//������1
			if (this->CheckFoot() == true) { nm = Landing; }//�����@��Q�@�L��
			if (in.R1.on) { nm = Fallshoot; }
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Landing:	//���n
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = Jump; }
			if (in.B1.down) { nm = PreStomp; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			if (this->moveCnt >= 6) { nm = Stand; }
			break;
		case  Shoot:
			if (in.R1.off) {nm = Stand;}
			if (in.B2.down) { nm = TakeOff; }
			if (!this->CheckFoot()) { nm = Fallshoot; }
			break;
		case Jumpshoot:
			if (in.R1.off) { nm = Fall; }
			if (this->moveVec.y >= 0) { nm = Fallshoot; }
			break;
		case Fallshoot:
			if (in.R1.off) { nm = Fall; }
			if (this->CheckFoot()) { nm = Shoot; }
			break;
		case PreStomp:
			if (this->moveCnt > 24) { nm = LandStomp; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case LandStomp:
			if (this->moveCnt > 12) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case AirStomp:
			if (this->CheckFoot()) { nm = StompLanding; }
			if (!this->CheckFoot() && this->moveCnt >= 60) { nm = Fall; }
			break;
		case StompLanding:
			if (this->moveCnt >= this->limit_StompHoldTime) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case	Damage:	//�_���[�W���󂯂Đ������ł���
			if (this->moveCnt >= 12 && this->CheckFoot()) 
			{
				nm = Stand;
			}
			else if (this->moveCnt >= 12 && !this->CheckFoot())
			{
				nm = Fall;
			}
			break;
		}
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//-----------------------------------------------------------------------------
	//���[�V�����ɑΉ���������
	//(���[�V�����̕ύX��Think�ōs�����Ɓj
	void  Object::Move()
	{
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//�d�͉���
		switch (this->state) {
		default:
			//�W�����v�{�^�������������ŃW�����v�̍������ς��
			if (in.B2.on)
			{
				this->gravity = ML::Gravity(32) * 2;
			}
			else
			{
				this->gravity = ML::Gravity(32) * 5;
			}
			//�㏸���������͑����ɒn�ʂ�����
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) {
				this->moveVec.y=min(this->moveVec.y + this->gravity, this->maxFallSpeed);
			}
			//�n�ʂɐڐG���Ă���
			else 
			{
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
			break;
			//�ړ����x�����𖳌�������K�v�����郂�[�V�����͉���case������
		case Damage:
		case Unnon:
			break;
		}
		//-----------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->state) 
		{
		case  Stand:	//�����Ă���
			//���������ς���
			if (in.LStick.L.down)
			{
				this->moveCnt = 0;
				this->angle_LR = Left;
			}
			if (in.LStick.R.down)
			{
				this->moveCnt = 0;
				this->angle_LR = Right;
			}
			//�E�X�e�B�b�N�ɂ���Č�����ς���
			if (in.RStick.axis.x > 0)
			{
				this->angle_LR = Right;
			}
			else if (in.RStick.axis.x<0)
			{
				this->angle_LR = Left;
			}
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.on)
			{
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on)
			{
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			break;
		case SlowDown:
			//�ڂ̑O�ɑ��ꂪ�Ȃ���ΑO�i����߂�(���ꂩ�痎�����Ȃ�)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			//�E�X�e�B�b�N�ɂ���Č�����ς���
			if (in.RStick.axis.x > 0)
			{
				this->angle_LR = Right;
			}
			else if (in.RStick.axis.x<0)
			{
				this->angle_LR = Left;
			}
			break;
		case  Jump:		//�㏸��
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->height_Jump;//�����ݒ�
			}
			//�����̕ύX�͓|�����u��
			if (this->moveCnt<this->limit_JumpAngleChange&&in.LStick.L.down)
			{
				this->angle_LR = Left;
			}
			if (this->moveCnt<this->limit_JumpAngleChange && in.LStick.R.down)
			{
				this->angle_LR = Right;
			}
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) 
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//�V��ɏՓ˂ŏ㏸�͂�0��
			if (this->CheckHead() == true) { this->moveVec.y = 0; }
			break;
		case  Fall:		//������
			if (in.LStick.L.on) 
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			break;
		case Landing:
			//�ړ��ʌ����ɂ���Ċ���Ƒ���̈ړ�������Ȃ̂�
			//���E�̈ړ��ʂ�0��
			if (this->moveCnt == 0)
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case PreStomp:
			break;
		case LandStomp:
			if (this->moveCnt == 0)
			{
				this->Stomp_Std();
			}
			break;
		case AirStomp:
			//�o��������������������
			if (this->moveCnt == 0)
			{
				//���G���Ԃ���
				this->unHitTime = 60;
			}
			else if (this->moveCnt <= 10)
			{
				this->moveVec.y = -0.5f;
			}
			else if (this->moveCnt > 20)
			{
				if (this->moveVec.y <= this->maxFallSpeed)
				{
					this->moveVec.y += 1.5f;
				}
			}
			break;
		case StompLanding:
			//���n�̍ہA���g�̍����E�ɍU��
			if (this->moveCnt == 0)
			{
				this->Stomp_Std();
			}
			break;
		case  Shoot:
			this->Move_Shot();
			this->Shot_Appear();
			break;
		case Jumpshoot:
			this->Move_Shot();
			this->Shot_Appear();
			if (this->CheckHead() == true) { this->moveVec.y = 0; }	
			break;
		case Fallshoot:
			this->Move_Shot();
			this->Shot_Appear();
			break;
		}
	}
	//HP�̒l���擾����
	int Object::Get_HP()
	{
		return this->hp;
	}
	//HP�̍ő�l���擾����
	int Object::Get_Max_HP()
	{
		return this->max_Hp;
	}
	//��Ԃ��擾����
	int Object::Get_State()
	{
		return this->state;
	}
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		//�f�t�H���g�J���[��錾
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(  0,  0, 48, 96), dc },//��~1			[0]
			{ this->hitBase, ML::Box2D(  0, 96, 48, 96), dc },//��~2			[1]
			{ this->hitBase, ML::Box2D(  0,192, 48, 96), dc },//��~3			[2]
			{ this->hitBase, ML::Box2D(  0,288, 48, 96), dc },//��~4			[3]
			{ this->hitBase, ML::Box2D(  0,384, 48, 96), dc },//��~5			[4]
			{ this->hitBase, ML::Box2D( 48,  0, 48, 96), dc },//���s1			[5]
			{ this->hitBase, ML::Box2D( 48, 96, 48, 96), dc },//���s2			[6]
			{ this->hitBase, ML::Box2D( 48,192, 48, 96), dc },//���s3			[7]
			{ this->hitBase, ML::Box2D(96,   0, 48, 96), dc },//����				[8]
			{ this->hitBase, ML::Box2D(144,  0, 48, 96), dc },//�p���`1			[9]
			{ this->hitBase, ML::Box2D(192,  0, 48, 96), dc },//�p���`2			[10]
			{ this->hitBase, ML::Box2D(240,  0, 48, 96), dc },//�󒆍U��			[11]
			{ this->hitBase, ML::Box2D(288,  0, 48, 96), dc },//�X�g���v			[12]
			{ this->hitBase, ML::Box2D(336,  0, 48, 96), dc },//�X�g���v���n		[13]
			{ this->hitBase, ML::Box2D(384,  0, 48, 96), dc },//�ˌ�				[14]
			{ this->hitBase, ML::Box2D(432,  0, 48, 96), dc },//�󒆎ˌ�			[15]
			{ this->hitBase, ML::Box2D(480,  0, 48, 96), dc },//�o���J�[1		[16]
			{ this->hitBase, ML::Box2D(528,  0, 48, 96), dc },//�o���J�[2		[17]
			{ this->hitBase, ML::Box2D(576,  0, 48, 96), dc },//�o���J�[3		[18]
			{ this->hitBase, ML::Box2D(624,  0, 48, 96), dc },//�W�����v			[19]
			{ this->hitBase, ML::Box2D(672,  0, 48, 96), dc },//����				[20]
			{ this->hitBase, ML::Box2D(720,  0, 48, 96), dc },//���n				[21]
			{ this->hitBase, ML::Box2D(768,  0, 48, 96), dc },//�_���[�W			[22]
		};
		BChara::DrawInfo  rtv;
		int  standAnim,walkAnim;
		switch (this->state) {
		default:			rtv = imageTable[0];	break;
		//	��~----------------------------------------------------------------------------
		case  Stand:
			//�؂�ւ��t���[����
			standAnim = this->animCnt / 12;
			//�p�^�[����
			standAnim %= 5;
			//�o���l�Ɉꖇ�ڂ̗v�f�ԍ��𑫂�
			rtv = imageTable[standAnim + 0];
			break;
		//	���s----------------------------------------------------------------------------
		case  Walk:
			walkAnim = this->animCnt / 8;
			walkAnim %= 3;
			rtv = imageTable[walkAnim + 5];
			break;
		//	����----------------------------------------------------------------------------
		case SlowDown:
		case PreStomp:
			rtv = imageTable[8];
			break;
		//	�󒆍U��-------------------------------------------------------------------------
		case Air:			rtv = imageTable[11];	break;
		//	�X�g���v-------------------------------------------------------------------------
		case AirStomp:			rtv = imageTable[12];	break;
		//	�X�g���v���n---------------------------------------------------------------------
		case StompLanding:
		case LandStomp:
			rtv = imageTable[13];
			break;
		//	�n��ˌ�-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[14];	break;
		//	�󒆎ˌ�-------------------------------------------------------------------------
		case Jumpshoot:
		case Fallshoot:		rtv = imageTable[15];	break;
		//	�W�����v------------------------------------------------------------------------
		case  Jump:			rtv = imageTable[19];	break;
		//	����----------------------------------------------------------------------------
		case  Fall:			rtv = imageTable[20];	break;
		//���n�d��--------------------------------------------------------------------------
		case  Landing:		rtv = imageTable[21];	break;
		//�_���[�W--------------------------------------------------------------------------
		case  Damage:		rtv = imageTable[22];	break;
		}
		//	�����ɉ����ĉ摜�����E���]����
		if (false == this->angle_LR) {
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
			////	�\���ʒu�̋t�]
			//rtv.draw.x = -(rtv.draw.x + rtv.draw.w);
			////	�摜�ʒu�̋t�]
			//rtv.src.x = (rtv.src.x + rtv.src.w);
			//rtv.src.w = -rtv.src.w;
		}
		return rtv;
	}
	//�V���b�g�����X�^���_�[�h
	void Object::Shot_Appear()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		float angle=atan2(in.RStick.axis.y,in.RStick.axis.x);
		//���Ԋu�Œe�𐶐�
		if (this->moveCnt % this->interval_Shot == 0)
		{
			auto shot = Shot00::Object::Create(true);
			shot->state = Shoot;
			//�U�����ɍU���͈͂𐶐����Ɏw��
			shot->hitBase = this->range_Shot;
			shot->Set_Limit(this->limit_Shot);
			shot->Set_Erase(1);
			shot->Set_Power(this->power_Shot);
			shot->angle_LR = this->angle_LR;
			shot->tip = true;
			//�E�X�e�B�b�N�����͂���Ă��Ȃ���Έ��̋O����`��
			if (in.RStick.axis == ML::Vec2(0.0f, 0.0f))
			{
				shot->Set_Angle(0.0f);
				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(+this->speed_Shot, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveVec = ML::Vec2(-this->speed_Shot, 0);
				}
			}
			//�X�e�B�b�N�̓��͕����֔���
			else
			{
				shot->pos = this->pos + ML::Vec2(cos(angle),sin(angle)) * this->reach;
				shot->moveVec = ML::Vec2(cos(angle), sin(angle)) * this->speed_Shot;
				shot->Set_Angle(angle);
			}
		}
	}
	//�s���V���b�g��
	void Object::Move_Shot()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//�ˌ����͌�ނł��邪�ړ����x��������
		if (in.LStick.L.on && this->angle_LR == Right)
		{
			this->moveVec.x = -this->maxSpeed / 2.0f;
		}
		else if (in.LStick.R.on && this->angle_LR == Left)
		{
			this->moveVec.x = +this->maxSpeed / 2.0f;
		}
		if (in.LStick.L.on&&this->angle_LR == Left)
		{
			this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
		}
		else if (in.LStick.R.on&&this->angle_LR == Right)
		{
			this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
		}
		//���˕����ɂ���Č�����ς���
		if (in.RStick.axis.x > 0)
		{
			this->angle_LR = Right;
		}
		else if (in.RStick.axis.x<0)
		{
			this->angle_LR = Left;
		}
	}
	//�X�g���v�X�^���_�[�h
	void Object::Stomp_Std()
	{
		//�����ɍU����`�𐶐�
		auto stompLandingRect = Shot00::Object::Create(true);
		stompLandingRect->state = StompLanding;
		//�U�����ɍU���͈͂𐶐����Ɏw��
		stompLandingRect->hitBase = this->range_Stomp;
		stompLandingRect->pos = ML::Vec2(this->pos.x, this->pos.y + this->hitBase.h / 4);
		stompLandingRect->Set_Limit(this->limit_Stomp);
		stompLandingRect->Set_Erase(0);
		stompLandingRect->Set_Power(this->power_Stomp);
		stompLandingRect->Set_Angle(0.0f);
		//�͈͍U���̂ӂ��Ƃї�x��+�̒l�Ŏw�肷��i�������]�͓��������ۂɍs���j
		stompLandingRect->moveBack = this->moveBack_Stomp;
		//�͈͍U���ł��邱�Ƃ�m�点��t���O��true
		stompLandingRect->wideRange = true;
		//��ʂ�h�炷���߂̐ݒ���s��
		auto map = ge->GetTask_One_G<Map2D::Object>("�t�B�[���h");
		map->Set_Dist_Quake(this->dist_Quake);
		map->Set_Limit_Quake(this->limit_Quake);
		//�G�t�F�N�g�̐���
		//�^�X�N�L���̓G�t�F�N�g���ōs��
		auto stompLandingEffect = Effect::Object::Create(true);
		stompLandingEffect->pos = this->pos;
		stompLandingEffect->Set_Limit(this->limit_StompEffect);
		stompLandingEffect->state = StompLanding;
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