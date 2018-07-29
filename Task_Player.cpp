//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/chara(��)01.png");
		DG::Font_Create("FontA", "�l�r �S�V�b�N", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontA");
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
		this->angle_LR = Right;
		this->controllerName = "P1";
		this->motion = Stand;				//�L�����������
		this->max_Hp = 10;					//�w���X�ő�l
		this->hp = this->max_Hp;			//�w���X�����l
		this->maxSpeed = 10.0f;				//�ő�ړ����x�i���j
		this->addSpeed = 1.0f;				//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;				//�ڒn��Ԃ̎��̑��x�����ʁi���C
		this->maxFallSpeed = 15.0f;			//�ő嗎�����x
		this->jumpPow = -15.0f;				//�W�����v�́i�����j
		this->gravity = ML::Gravity(64);	//�d�͉����x�����ԑ��x�ɂ����Z��
		this->reach = 100.0f;				//�p���`�̎˒�
		this->slide = 10.0f;				//�p���`�őO�i���鋗��
		this->meleeCnt = 15;				//�i���U������p������
		this->shotSpeed = 20;

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
		ML::Vec2  est = this->moveVec;
		this->CheckMove(est);

		//�����蔻��
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
		//hp0�Ń^�X�N�L��
		if (this->hp <= 0)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->debugMode)
		{
			ML::Box2D debugBox(150, 50, 1000, 1000);
			string debugText =
				"motion = " + to_string(this->motion) + "\n" +
				"moveVec.x = " + to_string(this->moveVec.x) + "\n" +
				"moveVec.y = " + to_string(this->moveVec.y) + "\n" +
				"angle = " + to_string(this->angle_LR) + "\n"
				"moveCnt = " + to_string(this->moveCnt) + "\n" +
				"unHitTime = " + to_string(this->unHitTime) + "\n" +
				"hp=" + to_string(this->hp)+"\n"+
				"Select�{�^���Ńf�o�b�O���[�h�ؑ�";
			DG::Font_Draw("FontA", debugBox, debugText, ML::Color(1, 0, 0, 0));
		}
		//�ȏ�f�o�b�O----------------------------------------------------
		//8�t���[����4�t���[���摜��\�����Ȃ�
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
		this->unHitTime = 180;
		this->hp -= at_.power;	//������
		if (this->hp <= 0) {
			this->Kill();
		}
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
		BChara::Motion  nm = this->motion;	//�Ƃ肠�������̏�Ԃ��w��
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);

		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���鎖��ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm) {
		case  Stand:	//�����Ă���
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (in.R2.down) { nm = Bunker1; }
			if (!this->CheckFoot()) { nm = Fall; }//���� ��Q�@����
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (in.R2.down) { nm = Bunker1; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			break;
		case SlowDown:
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.down) { nm = Punch1; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R2.down) { nm = Bunker1; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt >= 12) { nm = Stand; }
			break;
		case  TakeOff:	//��ї���
			if (this->moveCnt >= 3) { nm = Jump; }
			break;
		case  Jump:		//�㏸��
			if (this->moveVec.y >= 0) { nm = Fall; }
			if (in.R1.down) { nm = Air; }//�󒆍U��
			if (in.L1.down) { nm = Airshoot; }
			if (in.R2.down) { nm = Stomp; }
			break;
		case  Fall:		//������1
			if (this->CheckFoot() == true) { nm = Landing; }//�����@��Q�@�L��
			if (in.R1.down) { nm = Air; }//�󒆍U��
			if (in.L1.down) { nm = Airshoot; }
			if (in.R2.down) { nm = Stomp; }
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
			if (in.R1.down) { nm = Punch2; }
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
			if (!this->CheckFoot() && this->moveCnt >= 30) { nm = Fall; }
			break;
		case StompLanding:
			if (this->moveCnt >= 30) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Bunker1:
			if (this->moveCnt >= 8 && in.R2.off) { nm = Bunker2; }
			if (in.B2.down) { nm = Jump; }
			if (in.L1.down) { nm = Shoot; }
			if (in.R1.down) { nm = Punch1; }
			if (!this->CheckFoot()) { nm = Fall; }
			if (this->moveCnt < 30 && in.R2.off) { nm = Stand; }
			break;
		case Bunker2:
			if (this->moveCnt >= 20) { nm = Bunker3; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case Bunker3:
			if (this->moveCnt >= 20) { nm = Stand; }
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
	//	���[�V�����ɑΉ���������
	//(���[�V�����̕ύX��Think�ōs�����Ɓj
	void  Object::Move()
	{
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//�d�͉���
		switch (this->motion) {
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
		switch (this->motion) {
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
		switch (this->motion) {
		case  Stand:	//�����Ă���
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
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
			if (this->moveCnt == 0) {
				this->moveVec.y = this->jumpPow;//�����ݒ�
			}
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			//�V��ɏՓ˂ŏ㏸�͂�0��
			if (this->CheckHead() == true) { this->moveVec.y = 0; }
			break;
		case  Fall:		//������
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
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
			//�ڂ̑O�Ƀp���`��`�𐶐�
			if (this->moveCnt == 0) {
				auto punch1 = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					punch1->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					punch1->moveVec = ML::Vec2(0, 0);
					punch1->Set_Limit(this->meleeCnt);
					punch1->Set_Erase(0);
					//�U�����ɑO�i����
					this->moveVec.x = this->slide;
				}
				else {
					punch1->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					punch1->moveVec = ML::Vec2(0, 0);
					punch1->Set_Limit(this->meleeCnt);
					punch1->Set_Erase(0);
					//�U�����ɑO�i����
					this->moveVec.x = -this->slide;
				}
			}
			//�ڂ̑O�ɑ��ꂪ�Ȃ���ΑO�i����߂�(���ꂩ�痎�����Ȃ�)
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Punch2:
			//�ڂ̑O�Ƀp���`��`�𐶐�
			if (this->moveCnt == 0) {
				auto punch2 = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					punch2->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					punch2->moveVec = ML::Vec2(0, 0);
					punch2->Set_Limit(this->meleeCnt);
					punch2->Set_Erase(0);
					//�U�����ɑO�i����
					this->moveVec.x = this->slide;
				}
				else {
					punch2->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					punch2->moveVec = ML::Vec2(0, 0);
					punch2->Set_Limit(this->meleeCnt);
					punch2->Set_Erase(0);
					//�U�����ɑO�i����
					this->moveVec.x = -this->slide;
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
				stompFallSpeed = -3.5f;
				//���G���Ԃ���
				this->unHitTime = 60;
			}
			this->stompFallSpeed+=0.5f;
			this->moveVec.y += stompFallSpeed;
			break;
		case StompLanding:
			//���n�̍ہA���g�̍����E�ɍU��
			if (this->moveCnt == 0)
			{
				for (int i = -1; i < 2; ++i)
				{
					auto stompLanding = Shot00::Object::Create(true);
					//�����ɍU����`�𐶐�
					stompLanding->pos = ML::Vec2(this->pos.x + this->reach * i, this->pos.y + this->hitBase.h / 2);
					stompLanding->moveVec = ML::Vec2(0, 0);
					stompLanding->Set_Limit(this->meleeCnt);
					stompLanding->Set_Erase(0);
				}
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
					for (int i = 0; i < 3; ++i)
					{
						auto bunker = Shot00::Object::Create(true);
						bunker->pos = ML::Vec2(this->pos.x + this->reach * i, this->pos.y);
						bunker->moveVec = ML::Vec2(0, 0);
						bunker->Set_Limit(this->meleeCnt);
						bunker->Set_Erase(0);
					}
				}
				else {
					for (int i = 0; i < 3; ++i)
					{
						auto bunker = Shot00::Object::Create(true);
						bunker->pos = ML::Vec2(this->pos.x - this->reach * i, this->pos.y);
						bunker->moveVec = ML::Vec2(0, 0);
						bunker->Set_Limit(this->meleeCnt);
						bunker->Set_Erase(0);
					}
				}
			}
			break;
		case Bunker3:
				break;
		case  Shoot:	//�ˌ�
			if (in.LStick.L.on) {
				this->angle_LR = Left;
				this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
			}
			if (in.LStick.R.on) {
				this->angle_LR = Right;
				this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
			}
			if (this->moveCnt == 4) {//4�t���[���ڂŒe�𔭎�
				auto shot = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					shot->pos = ML::Vec2(this->pos.x + 20, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
				else {
					shot->pos = ML::Vec2(this->pos.x - 20, this->pos.y);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
			}
			break;
		case Air:
			//�󒆐���
			if (in.LStick.L.on) {
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) {
				this->moveVec.x = this->maxSpeed;
			}
			//�󒆍U��(�ڂ̑O�ɒe�𐶐�)
			if (this->moveCnt == 4) {//4�t���[���ڂŒe�𔭎�
				auto air = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					air->pos = ML::Vec2(this->pos.x + this->reach, this->pos.y);
					air->moveVec = ML::Vec2(0, 0);
					air->Set_Limit(this->meleeCnt);
					air->Set_Erase(0);
				}
				else {
					air->pos = ML::Vec2(this->pos.x - this->reach, this->pos.y);
					air->moveVec = ML::Vec2(0, 0);
					air->Set_Limit(this->meleeCnt);
					air->Set_Erase(0);
				}
			}
			break;
		case Airshoot:
			//�󒆐���
			if (in.LStick.L.on) {
				this->moveVec.x = -this->maxSpeed;
			}
			if (in.LStick.R.on) {
				this->moveVec.x = this->maxSpeed;
			}
			//�󒆎ˌ�
			if (this->moveCnt == 4) {//4�t���[���ڂŒe�𔭎�
				auto shot = Shot00::Object::Create(true);
				if (this->angle_LR == Right) {
					shot->pos = ML::Vec2(this->pos.x + 20, this->pos.y);
					shot->moveVec = ML::Vec2(+this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
				}
				else {
					shot->pos = ML::Vec2(this->pos.x - 20, this->pos.y);
					shot->moveVec = ML::Vec2(-this->shotSpeed, 0);
					shot->Set_Limit(30);
					shot->Set_Erase(1);
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
		BChara::DrawInfo imageTable[] = {
			//draw							src
			{ this->hitBase, ML::Box2D(   0,  0, 64, 128), ML::Color(1, 1, 1, 1) },//��~		[0]
			{ this->hitBase, ML::Box2D(  64,  0, 64, 128), ML::Color(1, 1, 1, 1) },//���s1		[1]
			{ this->hitBase, ML::Box2D(  64,128, 64, 128), ML::Color(1, 1, 1, 1) },//���s2		[2]
			{ this->hitBase, ML::Box2D(  64,256, 64, 128), ML::Color(1, 1, 1, 1) },//���s3		[3]
			{ this->hitBase, ML::Box2D( 128,  0, 64, 128), ML::Color(1, 1, 1, 1) },//����		[4]
			{ this->hitBase, ML::Box2D( 192,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�p���`1		[5]
			{ this->hitBase, ML::Box2D( 256,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�p���`2		[6]
			{ this->hitBase, ML::Box2D( 320,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�󒆍U��	[7]
			{ this->hitBase, ML::Box2D( 384,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�X�g���v	[8]
			{ this->hitBase, ML::Box2D( 448,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�X�g���v���n	[9]																				 //�X�g���v���n									 64	 128
			{ this->hitBase, ML::Box2D( 512,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�ˌ�		[10]
			{ this->hitBase, ML::Box2D( 576,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�󒆎ˌ�	[11]
			{ this->hitBase, ML::Box2D( 640,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�o���J�[1	[12]
			{ this->hitBase, ML::Box2D( 704,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�o���J�[2	[13]
			{ this->hitBase, ML::Box2D( 768,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�o���J�[3	[14]
			{ this->hitBase, ML::Box2D( 832,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�W�����v	[15]
			{ this->hitBase, ML::Box2D( 896,  0, 64, 128), ML::Color(1, 1, 1, 1) },//����		[16]
			{ this->hitBase, ML::Box2D( 960,  0, 64, 128), ML::Color(1, 1, 1, 1) },//���n		[17]
			{ this->hitBase, ML::Box2D(1024,  0, 64, 128), ML::Color(1, 1, 1, 1) },//�_���[�W	[18]
		};
		BChara::DrawInfo  rtv;
		int  walkAnim;
		switch (this->motion) {
		default:			rtv = imageTable[0];	break;
		//	��~----------------------------------------------------------------------------
		case  Stand:		rtv = imageTable[0];	break;
		//	���s----------------------------------------------------------------------------
		case  Walk:
			//���s�A�j���[�V����
			walkAnim = this->animCnt / 8;
			walkAnim %= 3;
			rtv = imageTable[walkAnim + 1];
			break;
		//	����----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[4];	break;
		//	�p���`1-------------------------------------------------------------------------
		case Punch1:		rtv = imageTable[5];	break;
		//	�p���`2-------------------------------------------------------------------------
		case Punch2:		rtv = imageTable[6];	break;
		//	�󒆍U��-------------------------------------------------------------------------
		case Air:			rtv = imageTable[7];	break;
		//	�X�g���v-------------------------------------------------------------------------
		case Stomp:			rtv = imageTable[8];	break;
		//	�X�g���v���n---------------------------------------------------------------------
		case StompLanding:	rtv = imageTable[9];	break;
		//	�n��ˌ�-------------------------------------------------------------------------
		case Shoot:			rtv = imageTable[10];	break;
		//	�󒆎ˌ�-------------------------------------------------------------------------
		case Airshoot:		rtv = imageTable[11];	break;
		//	�o���J�[1-------------------------------------------------------------------------
		case Bunker1:		rtv = imageTable[12];	break;
		//	�o���J�[2-------------------------------------------------------------------------
		case Bunker2:		rtv = imageTable[13];	break;
		//	�o���J�[3-------------------------------------------------------------------------
		case Bunker3:		rtv = imageTable[14];	break;
		//	�W�����v------------------------------------------------------------------------
		case  Jump:			rtv = imageTable[15];	break;
		//	����----------------------------------------------------------------------------
		case  Fall:			rtv = imageTable[16];	break;
		//���n�d��--------------------------------------------------------------------------
		case  Landing:		rtv = imageTable[17];	break;
		//�_���[�W--------------------------------------------------------------------------
		case  Damage:		rtv = imageTable[18];	break;
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