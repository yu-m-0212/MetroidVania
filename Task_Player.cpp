//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Sprite.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Goal.h"
#include  "Task_Retry.h"
#include  "Task_Corpse.h"

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
		this->hitBase = ML::Box2D(-32, -64, 64, 128);
		this->recieveBase = this->hitBase;
		this->angle_LR = Right;
		this->controllerName = "P1";
		this->state = Stand;				//�L�����������
		this->max_Hp = 10;					//�w���X�ő�l
		this->hp = this->max_Hp;			//�w���X�����l
		this->maxSpeed = 10.0f;				//�ő�ړ����x�i���j
		this->addSpeed = 1.0f;				//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;				//�ڒn��Ԃ̎��̑��x�����ʁi���C
		this->maxFallSpeed = 15.0f;			//�ő嗎�����x
		this->max_StompFallSpeed = 30.0f;	//�X�g���v�̍ő�~�����x
		this->jumpPow = -15.0f;				//�W�����v�́i�����j
		this->gravity = ML::Gravity(64);	//�d�͉����x�����ԑ��x�ɂ����Z��
		this->reach = 64.0;					//�p���`�̎˒�
		this->slide = 10.0f;				//�p���`�őO�i���鋗��
		this->meleeCnt = 15;				//�i���U������p������
		this->shotSpeed = 20;				//�V���b�g���x
		this->stompHoldTime = 30;			//�X�g���v���n���̍d������
		this->addUnHitTime = 120;			//��e���ɓ����閳�G����

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
		//��̂Ƃ̐ڐG����
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			auto targets = ge->GetTask_Group_G<BChara>("���");
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
					healEffect->Set_Limit(24);
					healEffect->state = Heal;
					break;
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
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D you = goal->hitBase.OffsetCopy(goal->pos);
		if (you.Hit(me))
		{
			ge->clear = true;
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
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+4, -9); }
		else							{ this->moveVec = ML::Vec2(-4, -9); }
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
			if (in.R1.down) { nm = Shoot; }
			if (in.B1.down) { nm = Punch1; }
			/*if (in.B1.down) { nm = Bunker1; }*/
			if (!this->CheckFoot()) { nm = Fall; }//���� ��Q�@����
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.down) { nm = Shoot; }
			if (in.B1.down) { nm = Punch1; }
			/*if (in.B1.down) { nm = Bunker1; }*/
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			break;
		case SlowDown:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.B1.down) { nm = Punch1; }
			if (in.R1.down) { nm = Shoot; }
			/*if (in.B1.down) { nm = Bunker1; }*/
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt >= 12) { nm = Stand; }
			break;
		case  TakeOff:	//��ї���
			if (this->moveCnt >= 3) { nm = Jump; }
			break;
		case  Jump:		//�㏸��
			if (this->moveVec.y >= 0) { nm = Fall; }
			if (in.B1.down) { nm = Air; }//�󒆍U��
			if (in.R1.down) { nm = Airshoot; }
			if (in.B4.down) { nm = Stomp; }
			break;
		case  Fall:		//������1
			if (this->CheckFoot() == true) { nm = Landing; }//�����@��Q�@�L��
			if (in.B1.down) { nm = Air; }//�󒆍U��
			if (in.R1.down) { nm = Airshoot; }
			if (in.B4.down) { nm = Stomp; }
			break;
		case  Landing:	//���n
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = Jump; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			if (this->moveCnt >= 3) { nm = Stand; }
			break;
		case Punch1:
			if (this->moveCnt >= 20) { nm = Stand; }
			if (in.B2.down) { nm = Jump; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (in.B1.down) { nm = Punch2; }
			break;
		case Punch2:
			if (this->moveCnt >= 20) { nm = Stand; }
			if (in.B2.down) { nm = Jump; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case  Shoot:
			if (this->moveCnt >= 8) {nm = Stand;}
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Air:
			if (this->moveCnt >= 8 && this->moveVec.y >= 0) { nm = Fall; }
			break;
		case Airshoot:
			if (this->moveCnt >= 8 && this->moveVec.y >= 0) { nm = Fall; }
			break;
		case Stomp:
			if (this->CheckFoot()) { nm = StompLanding; }
			if (!this->CheckFoot() && this->moveCnt >= 60) { nm = Fall; }
			break;
		case StompLanding:
			if (this->moveCnt >= this->stompHoldTime) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		/*case Bunker1:
			if (this->moveCnt >= 8 && in.B1.off) { nm = Bunker2; }
			if (in.B1.down) { nm = Jump; }
			if (in.B4.down) { nm = Shoot; }
			if (in.B3.down) { nm = Punch1; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt < 30 && in.B1.off) { nm = Stand; }
			break;
		case Bunker2:
			if (this->moveCnt >= 20) { nm = Bunker3; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Bunker3:
			if (this->moveCnt >= 20) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;*/
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
	//	���[�V�����ɑΉ���������
	//(���[�V�����̕ύX��Think�ōs�����Ɓj
	void  Object::Move()
	{
		auto cam = ge->GetTask_One_G<Sprite::Object>("�I�v�V����");
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//�d�͉���
		switch (this->state) {
		default:
			//�W�����v�{�^�������������ŃW�����v�̍������ς��
			if (in.B2.on)
			{
				this->gravity = ML::Gravity(32) * 4;
			}
			else
			{
				this->gravity = ML::Gravity(32) * 10;
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
			//������
			break;
			//�ړ����x�����𖳌�������K�v�����郂�[�V�����͉���case������
		case Damage: break;
		case Unnon:	break;
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
			break;
		case  Jump:		//�㏸��
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->jumpPow;//�����ݒ�
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
		case Punch1:
		case Punch2:
			//�ڂ̑O�Ƀp���`��`�𐶐�
			if (this->moveCnt == 0) 
			{
				auto punch1 = Shot00::Object::Create(true);
				//�Ăяo���������`�Ɏv�l�����邽�ߏ�Ԃ��w��
				punch1->state = Punch1;
				//�U�����ɍU���͈͂𐶐����Ɏw��
				punch1->hitBase = ML::Box2D(-64, -32, 128, 64);
				punch1->Set_Limit(this->meleeCnt);
				punch1->Set_Erase(0);
				punch1->Set_Power(2);
				punch1->angle_LR = this->angle_LR;
				//�G�t�F�N�g�̌Ăяo��
				auto punch1Effect = Effect::Object::Create(true);
				punch1Effect->state = Punch1;
				punch1Effect->Set_Limit(18);
				punch1Effect->pos = this->pos;
				punch1Effect->angle_LR = this->angle_LR;
				//�ȉ��v���C���̍��E�ɂ���ĕω�����
				if (this->angle_LR == Left)
				{
					//�������W���v���C���̖ڂ̑O�Ɏw��
					punch1->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					//�ӂ��Ƃїʂ��w�肷��
					punch1->moveBack = ML::Vec2(-6, -4);
					//�U�����ɑO�i����
					this->moveVec.x = -this->slide;
				}
				else 
				{
					//�������W���v���C���̖ڂ̑O�Ɏw��
					punch1->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					//�ӂ��Ƃїʂ��w�肷��
					punch1->moveBack = ML::Vec2(+6, -4);
					//�U�����ɑO�i����
					this->moveVec.x = +this->slide;
				}
			}
			//�ڂ̑O�ɑ��ꂪ�Ȃ���ΑO�i����߂�(���ꂩ�痎�����Ȃ�)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Stomp:
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
				//�����ɍU����`�𐶐�
				auto stompLandingRect = Shot00::Object::Create(true);
				stompLandingRect->state = StompLanding;
				//�U�����ɍU���͈͂𐶐����Ɏw��
				stompLandingRect->hitBase = ML::Box2D(-128, -32, 256, 64);
				stompLandingRect->pos = ML::Vec2(this->pos.x, this->pos.y + this->hitBase.h / 4);
				stompLandingRect->Set_Limit(this->meleeCnt);
				stompLandingRect->Set_Erase(0);
				stompLandingRect->Set_Power(5);
				//�͈͍U���̂ӂ��Ƃї�x��+�̒l�Ŏw�肷��i�������]�͓��������ۂɍs���j
				stompLandingRect->moveBack = ML::Vec2(10, -9);
				//�͈͍U���ł��邱�Ƃ�m�点��t���O��true
				stompLandingRect->wideRange = true;
				//�G�t�F�N�g�̐���
				//�^�X�N�L���̓G�t�F�N�g���ōs��
				auto stompLandingEffect = Effect::Object::Create(true);
				stompLandingEffect->pos = this->pos;
				stompLandingEffect->Set_Limit(18);
				stompLandingEffect->state = StompLanding;
			}
			break;
		case Bunker1:
			//���ߒ��Ɍ�����ς�����
			if (in.LStick.L.down) { this->angle_LR = Left; }
			if (in.LStick.R.down) { this->angle_LR = Right; }
			break;
		case Bunker2:
			//�����Ĕ���
			if (this->moveCnt == 4) {
				if (this->angle_LR == Right) {
					//�e�𐳖ʂ�3���o��
					auto bunker = Shot00::Object::Create(true);
					//�U�����ɍU���͈͂𐶐����Ɏw��
					bunker->hitBase = ML::Box2D(-96, -32, 192, 64);
					bunker->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					bunker->moveVec = ML::Vec2(0, 0);
					bunker->Set_Limit(this->meleeCnt);
					bunker->Set_Erase(0);
					bunker->angle_LR = this->angle_LR;
				}
				else {
					auto bunker = Shot00::Object::Create(true);
					//�U�����ɍU���͈͂𐶐����Ɏw��
					bunker->hitBase = ML::Box2D(-96, -32, 192, 64);
					bunker->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					bunker->moveVec = ML::Vec2(0, 0);
					bunker->Set_Limit(this->meleeCnt);
					bunker->Set_Erase(0);
					bunker->angle_LR = this->angle_LR;
				}
			}
			break;
		case Bunker3:
			break;
		case  Shoot:	//�ˌ�
			//�ˌ������ړ��ł���
			if (in.LStick.L.on) 
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//4�t���[���ڂŒe�𔭎�
			if (this->moveCnt == 4)
			{
				auto shot = Shot00::Object::Create(true);
				shot->state = Shoot;
				//�U�����ɍU���͈͂𐶐����Ɏw��
				shot->hitBase = ML::Box2D(-32, -32, 64, 64);
				shot->Set_Limit(30);
				shot->Set_Erase(1);
				shot->Set_Power(1);
				shot->angle_LR = this->angle_LR;
				shot->tip = true;
				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveBack = ML::Vec2(+2, -1);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveBack = ML::Vec2(-2, -1);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
				}
			}
			break;
		case Air:
			//�󒆐���
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = +this->maxSpeed;
			}
			//�󒆍U��(�ڂ̑O�ɒe�𐶐�)
			if (this->moveCnt == 4) {//4�t���[���ڂŒe�𔭎�
				auto air = Shot00::Object::Create(true);
				air->state = Air;
				//�U�����ɍU���͈͂𐶐����Ɏw��
				air->hitBase = ML::Box2D(-64, -32, 128, 64);
				air->Set_Limit(this->meleeCnt);
				air->Set_Erase(0);
				air->Set_Power(3);
				air->angle_LR = this->angle_LR;
				//�ȉ��v���C���̍��E�ɂ���ĕω�����
				if (this->angle_LR == Right) 
				{
					air->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					air->moveBack = ML::Vec2(+6, -4);
				}
				else
				{
					air->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					air->moveBack = ML::Vec2(-6, -4);
				}
				//�G�t�F�N�g�̌Ăяo��
				auto airEffect = Effect::Object::Create(true);
				airEffect->state = Punch1;
				airEffect->Set_Limit(18);
				airEffect->pos = this->pos;
				airEffect->angle_LR = this->angle_LR;
			}
			break;
		case Airshoot:
			//�󒆐���
			if (in.LStick.L.on)
			{
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on)
			{
				this->moveVec.x = this->maxSpeed;
			}
			//�󒆎ˌ�
			//4�t���[���ڂŒe�𔭎�
			if (this->moveCnt == 4)
			{
				auto shot = Shot00::Object::Create(true);
				//�U�����ɍU���͈͂𐶐����Ɏw��
				shot->hitBase = ML::Box2D(-32, -32, 64, 64);
				shot->state = Airshoot;
				shot->Set_Limit(30);
				shot->Set_Erase(1);
				shot->Set_Power(1);
				shot->angle_LR = this->angle_LR;
				shot->tip = true;
				if (this->angle_LR == Left)
				{
					shot->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					shot->moveBack = ML::Vec2(-2, -1);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					shot->moveBack = ML::Vec2(+2, -1);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
				}
			}
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
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		//�f�t�H���g�J���[��錾
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(   0,   0, 64, 128), dc },//��~1			[0]
			{ this->hitBase, ML::Box2D(   0, 128, 64, 128), dc },//��~2			[1]
			{ this->hitBase, ML::Box2D(   0, 256, 64, 128), dc },//��~3			[2]
			{ this->hitBase, ML::Box2D(   0, 384, 64, 128), dc },//��~4			[3]
			{ this->hitBase, ML::Box2D(   0, 512, 64, 128), dc },//��~5			[4]
			{ this->hitBase, ML::Box2D(  64,   0, 64, 128), dc },//���s1			[5]
			{ this->hitBase, ML::Box2D(  64, 128, 64, 128), dc },//���s2			[6]
			{ this->hitBase, ML::Box2D(  64, 256, 64, 128), dc },//���s3			[7]
			{ this->hitBase, ML::Box2D( 128,   0, 64, 128), dc },//����			[8]
			{ this->hitBase, ML::Box2D( 192,   0, 64, 128), dc },//�p���`1		[9]
			{ this->hitBase, ML::Box2D( 256,   0, 64, 128), dc },//�p���`2		[10]
			{ this->hitBase, ML::Box2D( 320,   0, 64, 128), dc },//�󒆍U��		[11]
			{ this->hitBase, ML::Box2D( 384,   0, 64, 128), dc },//�X�g���v		[12]
			{ this->hitBase, ML::Box2D( 448,   0, 64, 128), dc },//�X�g���v���n	[13]
			{ this->hitBase, ML::Box2D( 512,   0, 64, 128), dc },//�ˌ�			[14]
			{ this->hitBase, ML::Box2D( 576,   0, 64, 128), dc },//�󒆎ˌ�		[15]
			{ this->hitBase, ML::Box2D( 640,   0, 64, 128), dc },//�o���J�[1		[16]
			{ this->hitBase, ML::Box2D( 704,   0, 64, 128), dc },//�o���J�[2		[17]
			{ this->hitBase, ML::Box2D( 768,   0, 64, 128), dc },//�o���J�[3		[18]
			{ this->hitBase, ML::Box2D( 832,   0, 64, 128), dc },//�W�����v		[19]
			{ this->hitBase, ML::Box2D( 896,   0, 64, 128), dc },//����			[20]
			{ this->hitBase, ML::Box2D( 960,   0, 64, 128), dc },//���n			[21]
			{ this->hitBase, ML::Box2D(1024,   0, 64, 128), dc },//�_���[�W		[22]
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
			//�؂�ւ��t���[����
			walkAnim = this->animCnt / 8;
			//�p�^�[����
			walkAnim %= 3;
			//�o���l�Ɉꖇ�ڂ̗v�f�ԍ��𑫂�
			rtv = imageTable[walkAnim + 5];
			break;
		//	����----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[8];	break;
		//	�p���`1-------------------------------------------------------------------------
		case Punch1:		rtv = imageTable[9];	break;
		//	�p���`2-------------------------------------------------------------------------
		case Punch2:		rtv = imageTable[10];	break;
		//	�󒆍U��-------------------------------------------------------------------------
		case Air:			rtv = imageTable[11];	break;
		//	�X�g���v-------------------------------------------------------------------------
		case Stomp:			rtv = imageTable[12];	break;
		//	�X�g���v���n---------------------------------------------------------------------
		case StompLanding:	rtv = imageTable[13];	break;
		//	�n��ˌ�-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[14];	break;
		//	�󒆎ˌ�-------------------------------------------------------------------------
		case Airshoot:		rtv = imageTable[15];	break;
		//	�o���J�[1-------------------------------------------------------------------------
		case Bunker1:		rtv = imageTable[16];	break;
		//	�o���J�[2-------------------------------------------------------------------------
		case Bunker2:		rtv = imageTable[17];	break;
		//	�o���J�[3-------------------------------------------------------------------------
		case Bunker3:		rtv = imageTable[18];	break;
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