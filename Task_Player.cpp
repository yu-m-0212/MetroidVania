//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_Player.h"
#include	"Task_Sprite.h"
#include	"Task_Shot00.h"
#include	"Task_Barrier.h"
#include	"Task_Map2D.h"
#include	"Task_Effect.h"
#include	"Task_Goal.h"
#include	"Task_Retry.h"
#include	"Task_Corpse.h"
#include	"Task_Gun.h"
#include	"Task_Player_Head.h"
#include	"Task_Enemy01.h"
#include	"Task_Tutorials.h"
#include	"Task_Display_Effect.h"
#include	"Task_Item00.h"
#include	"Task_Item01.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/player.png");

		this->base_file_path_sound = "./data/sound/wav/";

		this->name_sound_landing = "sound_landing";
		DM::Sound_CreateSE(this->name_sound_landing, this->base_file_path_sound + "landing_player01.wav");

		this->name_sound_jump = "jump_sound";
		DM::Sound_CreateSE(this->name_sound_jump, this->base_file_path_sound + "jump_player01.wav");

		this->name_sound_shot = "shot_sound";
		DM::Sound_CreateSE(this->name_sound_shot, this->base_file_path_sound + "shot_player01.wav");
		
		this->name_not_recharge = "not_recharge";
		DM::Sound_CreateSE(this->name_not_recharge, this->base_file_path_sound + "not_recharge_barrier_player01.wav");

		this->name_damage_player = "damage_player";
		DM::Sound_CreateSE(this->name_damage_player, this->base_file_path_sound + "damage_player.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DM::Sound_Erase(this->name_sound_landing);
		DM::Sound_Erase(this->name_sound_jump);
		DM::Sound_Erase(this->name_sound_shot);
		DM::Sound_Erase(this->name_not_recharge);
		DM::Sound_Erase(this->name_damage_player);
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
		this->barrier = true;								//�o���A�̎g�p����
		this->state = Stand;								//�L�����������
		this->max_hp = 5;									//HP�ő�l
		this->hp = this->max_hp;							//HP�����l
		this->maxSpeed = 7.5f;								//�ő�ړ����x�i���j
		this->addSpeed = 0.75f;								//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.5f;								//�ڒn��Ԃ̎��̑��x�����ʁi���C
		this->max_speed_fall = 15.0f;						//�ő嗎�����x
		this->max_StompFallSpeed = 17.5f;					//�X�g���v�̍ő�~�����x
		this->height_jump = -10.0f;							//�W�����v�́i�����j
		this->gravity = ML::Gravity(SIZE_CHIP);				//�d�͉����x�����ԑ��x�ɂ����Z��
		this->init_shot = 48.0f;							//�����ʒu�V���b�g
		this->speed_shot = 10;								//�V���b�g���x
		this->limit_stompHoldTime = 30;						//�X�g���v���n���̍d������
		this->limit_stomp = 15;								//�X�g���v�p������
		this->limit_quake = 15;								//��ʗh�ꎞ��
		this->limit_shot = 60;								//�p�����ԃV���b�g
		this->limit_JumpAngleChange = 16;					//�W�����v�����莞�ԓ��Ȃ������ς�����
		this->dist_quake = 5;								//��ʗh�ꕝ
		this->lv_stomp = 1;									//�X�g���v�A�b�v�O���[�h���x��
		this->interval_shot = 12;							//�ˌ��̔��ˊԊu�i�t���[���j
		this->range_stomp = ML::Box2D(-112, -112, 224, 224);//�͈̓X�g���v
		this->range_shot = ML::Box2D(-8, -8, 16, 16);		//�͈̓V���b�g
		this->power_stomp = 1;								//�U���̓X�g���v
		this->power_shot = 1;								//�U���̓V���b�g
		this->gauge_melee_max = 100;						//�ߐڍU�����`���[�W���
		this->gauge_melee = this->gauge_melee_max;			//�ߐڍU���̃��`���[�W
		this->moveBack_stomp = ML::Vec2(16, -6);			//�X�g���v�ӂ��Ƃї�
		this->eff = new Task_Effect::Object();				//���\�b�h�Ăяo��

		//���^�X�N�̐���
		auto gun = Gun::Object::Create(true);
		gun->pos = this->pos;
		auto plHead = Player_Head::Object::Create(true);
		plHead->pos = this->pos;

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		delete this->eff;
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
		//�t�F�[�h�A�E�g���Ă���A������n�߂�
		auto display_effect =
			ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
		if (nullptr != display_effect) { return; }
		//�N���A�A�s�k��
		if (ge->clear || ge->failure) { return; }
		//�|�[�Y
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//���G���Ԃ̌���
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//�ߐڍU���̃��`���[�W
		if (this->barrier)
		{
			if (this->gauge_melee < 100)
			{
				this->gauge_melee++;
			}
		}
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
						(*it)->Recieved(this, at,0);
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
						BChara::AttackInfo at = { 0,0,0 };
						(*it)->Recieved(this, at,0);
						//��x�����G�t�F�N�g�𐶐�����
						if (!(*it)->Get_Flag_Erase())
						{
							//�񕜃G�t�F�N�g�𐶐�
							eff->Create_Effect(4, this->pos);
							//�t���O���]
							(*it)->Set_Flag_Erase(1);
						}
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
		auto   map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
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
		auto goal = ge->GetTask_One_G<Goal::Object>(Goal::defGroupName);
		if (nullptr != goal)
		{
			ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D you = goal->hitBase.OffsetCopy(goal->pos);
			if (you.Hit(me))
			{
				ge->clear = true;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//���G����8�t���[����4�t���[���摜��\�����Ȃ��i�_�ł���j
		if (this->time_un_hit > 0) {
			if ((this->time_un_hit / 4) % 2 == 0) {
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
	//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
	void Object::Recieved(BChara* from_, AttackInfo at_,const int& un_hit_)
	{
		//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		if (this->time_un_hit > 0) { return; }
		//SE�̍Đ�
		DM::Sound_Play_Volume(this->res->name_damage_player, false, VOLUME_SE_DAMAGE_PLAYER);
		//���G���Ԃ̔���
		this->time_un_hit = un_hit_;
		//�_���[�W����
		if (!ge->debugMode)
		{
			this->hp -= at_.power;
		}
		//��ʌ���
		auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName,Map2D::defName);
		map->Set_Quake(this->dist_quake, this->limit_quake);
		//������΂����
		if (this->pos.x > from_->pos.x) { this->moveVec = ML::Vec2(+6.5f, -2.0f); }
		else							{ this->moveVec = ML::Vec2(-6.5f, -2.0f); }
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
			if (!this->CheckFoot()) { nm = Fall; }//���� ��Q�@����
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.off&&in.LStick.R.off) { nm = SlowDown; }
			if (in.B2.down) { nm = TakeOff; }
			if (in.R1.on) { nm = Shoot; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			break;
		case SlowDown:	//������
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
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
			break;
		case  Fall:		//������
			if (this->CheckFoot() == true) { nm = Landing; }//�����@��Q�@�L��
			if (in.R1.on) { nm = Fallshoot; }
			if (in.B1.down) { nm = AirStomp; }
			break;
		case  Landing:	//���n
			if (in.LStick.L.on) { nm = Walk; }
			if (in.LStick.R.on) { nm = Walk; }
			if (in.B2.down) { nm = TakeOff; }
			if (this->CheckFoot() == false) { nm = Fall; }//���� ��Q�@����
			if (this->moveCnt >= 6) { nm = Stand; }
			break;
		case  Shoot:	//�ˌ�
			if (in.R1.off) {nm = Stand;}
			if (in.B2.down) { nm = TakeOff; }
			if (!this->CheckFoot()) { nm = Fallshoot; }
			break;
		case Jumpshoot:	//�󒆎ˌ�
			if (in.R1.off) { nm = Fall; }
			if (this->moveVec.y >= 0) { nm = Fallshoot; }
			break;
		case Fallshoot:	//�����ˌ�
			if (in.R1.off) { nm = Fall; }
			if (this->CheckFoot()) { nm = Shoot; }
			break;
		case LandStomp:
			if (this->moveCnt > 0) { nm = Stand; }
			if (!this->CheckFoot()) { nm = Fall; }
			break;
		case AirStomp:
			if (this->moveCnt > 0) { nm = Fall; }
			break;
		case Damage:	//�_���[�W���󂯂Đ������ł���
			if (this->moveCnt >= 12 &&
				this->CheckFoot()) {nm = Stand;}
			else if (this->moveCnt >= 12 &&
				!this->CheckFoot()){nm = Fall;}
			break;
		}
		//�o���A�̔��������荞�܂���
		if (in.B1.down)
		{
			//�ڒn
			if (this->CheckFoot())
			{
				nm = LandStomp;
			}
			else
			{
				nm = AirStomp;
			}
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
			if ((this->state == Jump ||
				this->state == Jumpshoot ||
				this->state == AirStomp)
				&& in.B2.on)
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
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
			}
			//�n�ʂɐڐG���Ă���
			else 
			{
				this->moveVec.y = 0.0f;
			}
			break;
			//�d�͉����𖳌�������K�v�����郂�[�V�����͉���case�������i���ݑΏۖ����j
		case Unnon:	
		case Damage:
			break;
		}
		//���ꂩ�痎�����Ăق����Ȃ���Ԃ͈ȉ��ɋL�q
		switch (this->state)
		{
		default:
			break;
		case SlowDown:
			if (!this->CheckFrontFoot_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
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
		//�E�X�e�B�b�N��X���̒l�ɂ���ĕ�����ς���
		switch (this->state)
		{
		default:
			if (in.RStick.axis.x < 0.0f)
			{
				this->angle_LR = Left;
			}
			else if (in.RStick.axis.x > 0.0f)
			{
				this->angle_LR = Right;
			}
			break;
		//��O
		case SlowDown:
		case Landing:
		case LandStomp:
		case AirStomp:
		case Damage:
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
			break;
		case  Walk:		//�����Ă���
			if (in.LStick.L.on)
			{
				//�E�X�e�B�b�N���|����Ă���Ƃ��͕����n�߂��u�Ԃ̂݁A������ς���
				if (in.RStick.axis != ML::Vec2(0.0f, 0.0f))
				{
					if (this->moveCnt == 0)
					{
						this->angle_LR = Left;
					}
				}
				else
				{
					this->angle_LR = Left;
				}
				//��ނ��ł��邪�ړ����x��������
				if(this->angle_LR == Right)
				{
					this->moveVec.x = -this->maxSpeed / 2.0f;
				}
				else
				{
					this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
				}
			}
			if (in.LStick.R.on)
			{
				if (in.RStick.axis != ML::Vec2(0.0f, 0.0f))
				{
					if (this->moveCnt == 0)
					{
						this->angle_LR = Right;
					}
				}
				else
				{
					this->angle_LR = Right;
				}
				if(this->angle_LR == Left)
				{
					this->moveVec.x = +this->maxSpeed / 2.0f;
				}
				else
				{
					this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
				}
			}
			break;
		case SlowDown:
			break;
		case  Jump:		//�㏸��
			if (this->moveCnt == 0)
			{
				//�����ݒ�
				this->moveVec.y = this->height_jump;
				//SE�Đ�
				DM::Sound_Play_Volume(this->res->name_sound_jump,false,VOLUME_ALL_GAME);
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
			//SE�Đ�
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_landing, false,VOLUME_SE_LADING_PLAYER);
			}
			break;
		case LandStomp:
		case AirStomp:
			if (this->moveCnt == 0)
			{
				//���`���[�W�ς�
				if (this->gauge_melee == this->gauge_melee_max)
				{
					this->Stomp_Std();
					this->gauge_melee = 0;
				}
				//�����`���[�W
				else
				{
					DM::Sound_Play_Volume(this->res->name_not_recharge, false,VOLUME_ALL_GAME);
				}
			}
			//���s�֘A
			//�������Ɉړ�
			if (in.LStick.L.on)
			{
				//��ގ��͈ړ����x��������
				if (this->angle_LR == Right)
				{
					this->moveVec.x = -this->maxSpeed / 2.0f;
				}
				//�O�i�͒ʏ푬�x
				else
				{
					this->moveVec.x = max(-this->maxSpeed, this->moveVec.x - this->addSpeed);
				}
			}
			//�E�����Ɉړ�
			if (in.LStick.R.on)
			{
				//���
				if (this->angle_LR == Left)
				{
					this->moveVec.x = +this->maxSpeed / 2.0f;
				}
				//�O�i
				else
				{
					this->moveVec.x = min(this->maxSpeed, this->moveVec.x + this->addSpeed);
				}
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
	//-----------------------------------------------------------------------------
	//�A�j���[�V��������
	BChara::DrawInfo  Object::Anim()
	{
		//�f�t�H���g�̒l��p��
		int wide = 138, height = 92;
		ML::Box2D dd(-wide / 2, -height / 2, wide, height);
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw			src
			{ dd, ML::Box2D(wide *  0,height * 0,wide,height),dc },	//��~1				[0]
			{ dd, ML::Box2D(wide *  0,height * 1,wide,height),dc },	//��~2				[1]
			{ dd, ML::Box2D(wide *  0,height * 2,wide,height),dc },	//��~3				[2]
			{ dd, ML::Box2D(wide *  0,height * 3,wide,height),dc },	//��~4				[3]
			{ dd, ML::Box2D(wide *  1,height * 0,wide,height),dc },	//���s1				[4]
			{ dd, ML::Box2D(wide *  1,height * 1,wide,height),dc },	//���s2				[5]
			{ dd, ML::Box2D(wide *  1,height * 2,wide,height),dc },	//���s3				[6]
			{ dd, ML::Box2D(wide *  1,height * 3,wide,height),dc },	//���s4				[7]
			{ dd, ML::Box2D(wide *  2,height * 0,wide,height),dc },	//����				[8]
			{ dd, ML::Box2D(wide *  3,height * 0,wide,height),dc },	//����				[9]
			{ dd, ML::Box2D(wide *  4,height * 0,wide,height),dc },	//�W�����v			[10]
			{ dd, ML::Box2D(wide *  5,height * 0,wide,height),dc },	//�~��				[11]
			{ dd, ML::Box2D(wide *  6,height * 0,wide,height),dc },	//���n				[12]
			{ dd, ML::Box2D(wide *  7,height * 0,wide,height),dc },	//�V���b�g1			[13]
			{ dd, ML::Box2D(wide *  7,height * 1,wide,height),dc },	//�V���b�g2			[14]
			{ dd, ML::Box2D(wide *  7,height * 2,wide,height),dc },	//�V���b�g3			[15]
			{ dd, ML::Box2D(wide *  7,height * 3,wide,height),dc },	//�V���b�g4			[16]
			{ dd, ML::Box2D(wide *  8,height * 0,wide,height),dc },	//�W�����v�V���b�g	[17]
			{ dd, ML::Box2D(wide *  9,height * 0,wide,height),dc },	//�~���V���b�g		[18]
			{ dd, ML::Box2D(wide * 10,height * 0,wide,height),dc },	//�X�g���v�\������	[19]
			{ dd, ML::Box2D(wide * 11,height * 0,wide,height),dc },	//�n��X�g���v		[20]
			{ dd, ML::Box2D(wide * 12,height * 0,wide,height),dc },	//�󒆃X�g���v		[21]
			{ dd, ML::Box2D(wide * 13,height * 0,wide,height),dc },	//�X�g���v���n		[22]
			{ dd, ML::Box2D(wide * 14,height * 0,wide,height),dc },	//�_���[�W			[23]
		};
		BChara::DrawInfo  rtv;
		int  walkAnim,shootAnim;
		switch (this->state) {
		default:			rtv = imageTable[0];	break;
		//	��~----------------------------------------------------------------------------
		case  Stand:		rtv = imageTable[0];	break;
		//	���s----------------------------------------------------------------------------
		case  Walk:
			walkAnim = this->animCnt / 8;
			walkAnim %= 4;
			rtv = imageTable[walkAnim + 4];
			break;
		//	����----------------------------------------------------------------------------
		case SlowDown:		rtv = imageTable[8];	break;
		//	����----------------------------------------------------------------------------
		case TakeOff:		rtv = imageTable[9];	break;
		//	�W�����v------------------------------------------------------------------------
		case Jump:			rtv = imageTable[10];	break;
			//	�~��----------------------------------------------------------------------------
		case Fall:			rtv = imageTable[11];	break;
			//	���n----------------------------------------------------------------------------
		case Landing:		rtv = imageTable[12];	break;
			//	�V���b�g------------------------------------------------------------------------
		case Shoot:
			//�V���b�g�͕����Ă��鎞�̂݁A�A�j���[�V��������
			if (this->moveVec.x != 0.0f)
			{
				shootAnim = this->animCnt / 8;
				shootAnim %= 4;
				rtv = imageTable[shootAnim + 13];
			}
			else
			{
				rtv = imageTable[13];
			}
			break;
		//	�W�����v�V���b�g-----------------------------------------------------------------
		case Jumpshoot:		rtv = imageTable[17];	break;
		//	�~���V���b�g--------------------------------------------------------------------
		case Fallshoot:		rtv = imageTable[18];	break;
		//	�n��X�g���v--------------------------------------------------------------------
		case LandStomp:		rtv = imageTable[20];	break;
		//	�󒆃X�g���v--------------------------------------------------------------------
		case AirStomp:		rtv = imageTable[21];	break;
		//	�_���[�W------------------------------------------------------------------------
		case Damage:		rtv = imageTable[23];	break;
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
		if (this->moveCnt % this->interval_shot == 0)
		{
			auto shot = Shot00::Object::Create(true);
			shot->state = Shoot;
			//�U�����ɍU���͈͂𐶐����Ɏw��
			shot->hitBase = this->range_shot;
			shot->Set_Limit(this->limit_shot);
			shot->Set_Power(this->power_shot);
			shot->angle_LR = this->angle_LR;
			shot->Set_Tip(1);
			//�E�X�e�B�b�N�����͂���Ă��Ȃ���Έ��̋O����`��
			if (in.RStick.axis == ML::Vec2(0.0f, 0.0f))
			{
				shot->Set_Angle(0.0f);
				if (this->angle_LR == Right)
				{
					shot->pos = ML::Vec2(this->pos.x + this->init_shot, this->pos.y);
					shot->Set_Angle(0.0f);
					shot->moveVec = ML::Vec2(+this->speed_shot, 0);
				}
				else
				{
					shot->pos = ML::Vec2(this->pos.x - this->init_shot, this->pos.y);
					shot->Set_Angle(135.0f);
					shot->moveVec = ML::Vec2(-this->speed_shot, 0);
				}
			}
			//�X�e�B�b�N�̓��͕����֔���
			else
			{
				shot->pos = this->pos + ML::Vec2(cos(angle),sin(angle)) * this->init_shot;
				shot->moveVec = ML::Vec2(cos(angle), sin(angle)) * this->speed_shot;
				shot->Set_Angle(angle);
			}
			//SE�Đ�
			DM::Sound_Play_Volume(this->res->name_sound_shot,false,VOLUME_SE_SHOT);
		}
	}
	//�s���V���b�g��
	void Object::Move_Shot()
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//���˒��͈ړ����x��������
		if (in.LStick.L.on)
		{
			this->moveVec.x = -this->maxSpeed / 2.0f;
		}
		else if (in.LStick.R.on)
		{
			this->moveVec.x = +this->maxSpeed / 2.0f;
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
		auto barrier = Barrier::Object::Create(true);
		barrier->state = this->state;
		//�U�����ɍU���͈͂𐶐����Ɏw��
		barrier->hitBase = this->range_stomp;
		barrier->recieveBase = this->range_stomp;
		barrier->pos = this->pos;
		barrier->Set_Limit(this->limit_stomp);
		barrier->Set_Power(this->power_stomp);
		barrier->Set_Angle(0.0f);
		//�͈͍U���̂ӂ��Ƃї�x��+�̒l�Ŏw�肷��i�������]�͓��������ۂɍs���j
		barrier->Set_Move_Back(this->moveBack_stomp);
		//�͈͍U���ł��邱�Ƃ�m�点��t���O��true
		barrier->Set_Range_Wide(1);
		//��ʂ�h�炷���߂̐ݒ���s��
		auto map = ge->GetTask_One_G<Map2D::Object>(Map2D::defGroupName);
		map->Set_Quake(this->dist_quake, this->limit_quake);
		//�G�t�F�N�g�̐���
		this->eff->Create_Effect(2, this->pos);
	}
	//�o���A�̎擾�󋵂��擾����
	bool Object::Get_Barrier()
	{
		return this->barrier;
	}
	//�V���b�g�̔��ˊԊu���擾����
	int Object::Get_Interval_Shot()
	{
		return this->interval_shot;
	}
	//HP�̒l���擾����
	int Object::Get_HP()
	{
		return this->hp;
	}
	//HP�̍ő�l���擾����
	int Object::Get_Max_HP()
	{
		return this->max_hp;
	}
	//��Ԃ��擾����
	int Object::Get_State()
	{
		return this->state;
	}
	//�ߐڍU�����`���[�W�̒l���擾����
	int Object::Get_Gauge_Mlee()
	{
		return this->gauge_melee;
	}
	//�ߐڍU�����`���[�W����̒l���擾����
	int Object::Get_Gauge_Melle_Max()
	{
		return this->gauge_melee_max;
	}
	//HP�̍ő�l���w�肷��
	//����	�F	�iint�j
	void Object::Set_Max_HP(const int& max_)
	{
		this->max_hp = max_;
	}
	//�o���A�̎擾�󋵂��w�肷��
	//����	�F	�ibool�j
	void Object::Set_Barrier(const bool& flag_)
	{
		this->barrier = flag_;
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