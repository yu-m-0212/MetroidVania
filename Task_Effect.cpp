//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Effect.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_Boss_Head.h"
#include  "Task_Display_Effect.h"

using namespace ML;
namespace  Task_Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->effectImage = "effectImage";
		DG::Image_Create(this->effectImage,"./data/image/effect.png");
		//�t�@�C���p�X
		this->base_file_path_sound = "./data/sound/wav/";
		this->name_sound_defeat = "sound_defeat";
		this->name_sound_hit = "hit_shot";
		this->name_sound_barrier = "sound_barrier";
		this->name_sound_barrier_recharge = "sound_barrier_recharge";
		//���̐���
		//wav�t�@�C���̒��ł��Đ��ł��Ȃ����̂���
		DM::Sound_CreateSE(this->name_sound_defeat, this->base_file_path_sound + "explosion_enemy01.wav");
		DM::Sound_CreateSE(this->name_sound_hit, this->base_file_path_sound + "hit_shot_player01.wav");
		DM::Sound_CreateSE(this->name_sound_barrier, this->base_file_path_sound + "activate_barrier_player01.wav");
		DM::Sound_CreateSE(this->name_sound_barrier_recharge, this->base_file_path_sound + "barrier_recharge.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
		DM::Sound_Erase(this->name_sound_defeat);
		DM::Sound_Erase(this->name_sound_hit);
		DM::Sound_Erase(this->name_sound_barrier);
		DM::Sound_Erase(this->name_sound_barrier_recharge);
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
		this->state_effect = Non;					//��ԊǗ�
		this->limit_erase = 0;						//���ԏ��ł܂�
		this->dist = 0.0f;							//��]����ۂ̒��S����̋���
		this->angle = 0.0f;							//�p�x
		this->add_angle_target_circle = 0.05f;		//�{�X�x���T�[�N���̉�]��
		this->add_angle_barrier = 0.025f;			//�o���A�̉�]��
		this->center = Vec2(0, 0);					//��]��
		this->num_bubble = 0;						//�A�̑傫��
		this->interval_bubble = 16;					//�A�̗h�����
		this->wide_bubble = 5.0f;					//�A�̗h�ꕝ
		this->limit_erase_hit_shot = 18;			//���ԏ��ł܂Ńq�b�g�V���b�g
		this->limit_erase_barrier_recharge = 15;	//���ԏ��ł܂Ńo���A���`���[�W
		this->limit_erase_defeat = 36;				//���ԏ��ł܂Ō��j
		this->limit_erase_heal = 24;				//���ԏ��ł܂ŉ�
		this->limit_erase_bubble = 600;				//���ԏ��ł܂ŖA
		this->limit_erase_appear = 180;				//���ԏ��ł܂œo��
		this->limit_erase_debris = 120;				//���ł܂ł̎��Ԕj��
		this->limit_erase_spark = 180;				//���ł܂ł̎��ԉΉ�
		this->limit_erase_highlight_head = 30;		//���ł܂ł̎��ԃn�C���C�g
		this->limit_effect_target_boss = 240;		//�{�X�x���G�t�F�N�g�̏��Ŏ���
		this->limit_effect_barrier = 30;			//���ł܂ł̎��ԃo���A
		this->limit_effect_boss_chance_mark = 360;	//���ł܂ł̎��ԃ{�X��_
		this->limit_quake_defeat = 30;				//���j�G�t�F�N�g�̉�ʗh�ꎞ��
		this->limit_quake_barrier = 15;				//�o���A�̉�ʗh�ꎞ��
		this->dist_quake_defeat = ML::Vec2(2, 2);	//���j�G�t�F�N�g�̉�ʗh�ꕝ
		this->dist_quake_barrier = ML::Vec2(0, 5);	//�o���A�̉�ʗh�ꕝ
		this->speed_surfacing = 3.0f;				//���x����
		this->speed_Debris = 6.0f;					//���x�j��
		this->speed_spark = 6.0f;					//���x�Ή�
		this->gravity = ML::Gravity(SIZE_CHIP);		//�d�͉����x
		this->render2D_Priority[1] = 0.3f;			//�`�揇
		this->choice_state = -1;					//�O�������Ԃ��w�肷��ہA�g�p
		Effect* eff = new Effect();					//���\�b�h�Ăяo��
		this->rate_size_circle = 10;				//�^�[�Q�b�g�T�[�N���̃T�C�Y�{��
		this->cnt_anim_highlight=0.0f;				//�{�X�X�^�����̃n�C���C�g�O���f�[�V�����J�E���^

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
		//��Ԃ����w��Ȃ��x�����w�肷��
		if (this->state_effect == Non)
		{
			switch (this->choice_state)
			{
			default:
			case -1:	//-1�͏������l
			case 0:		//���w��
				break;
			case 1:		//�q�b�g�V���b�g
				this->state_effect = Hit_Shot;
				this->limit_erase = this->limit_erase_hit_shot;
				//���E�ɂ���Ċp�x���C������
				if (this->angle_LR == Left)
				{
					this->angle = this->angle - 135.0f;
				}
				//SE�̍Đ�
				DM::Sound_Play_Volume(this->res->name_sound_hit, false, VOLUME_SE_HIT_SHOT_PLAYER);
				break;
			case 2:		//�o���A���`���[�W
				this->state_effect = Barrier_Recharge;
				this->limit_erase = this->limit_erase_barrier_recharge;
				//SE�Đ�
				DM::Sound_Play_Volume(this->res->name_sound_barrier_recharge, false, VOLUME_ALL_GAME);
				break;
			case 3:		//�G�l�~�[���j
			{
				this->state_effect = Defeat_Enemy;
				this->limit_erase = this->limit_erase_defeat;
				//��ʗh��
				auto map =
					ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_defeat, this->limit_quake_defeat);
				//SE�̍Đ�
				DM::Sound_Play_Volume(this->res->name_sound_defeat, false, VOLUME_ALL_GAME);
				break;
			}
			case 4:		//��
				this->state_effect = Heal;
				this->limit_erase = this->limit_erase_heal;
				break;
			case 5:		//�A
				this->state_effect = Bubble;
				//�O��ނ̒�����w�肷��
				this->num_bubble = rand() % 3;
				//�`��p�x
				this->angle = float(rand() % 360);
				//���ł܂ł̎���
				this->limit_erase = this->limit_erase_bubble;
				break;
			case 6:		//�o��
				this->state_effect = Appear;
				this->limit_erase = this->limit_erase_appear;
				break;
			case 7:		//�j��
				this->state_effect = Debris;
				this->limit_erase = this->limit_erase_debris;
				//����
				this->moveVec.y = sin(this->angle)*this->speed_Debris;
				break;
			case 8:		//�Ή�
				this->state_effect = Spark;
				this->limit_erase = this->limit_erase_spark;
				//����
				this->moveVec.y = sin(this->angle)*this->speed_spark;
				break;
			case 9:		//�{�X���o�T�[�N��00
				this->state_effect = Target_Circle_00;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 10:	//�{�X���o�T�[�N��01
				this->state_effect = Target_Circle_01;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 11:	//�{�X���o�o�[
				this->state_effect = Target_Bar;
				this->limit_erase = this->limit_effect_target_boss;
				break;
			case 12:	//�o���A
			{
				this->state_effect = Barrier;
				this->limit_erase = this->limit_effect_barrier;
				//SE�̍Đ�
				DM::Sound_Play_Volume(this->res->name_sound_barrier, false, VOLUME_SE_BARRIER);
				//��ʂ�h�炷���߂̐ݒ���s��
				auto map = ge->GetTask_One_G<Map2D::Object>(Map2D::defGroupName);
				map->Set_Quake(this->dist_quake_barrier, this->limit_quake_barrier);
				break;
			}
			case 13:	//�n�C���C�g
				this->state_effect = Highlight_Head;
				this->limit_erase = this->limit_erase_highlight_head;
				break;
			case 14:	//�{�X��_�\������
				this->state_effect = Boss_Chance_Mark_In_Line;
				this->limit_erase = this->limit_effect_boss_chance_mark;
				break;
			case 15:	//�{�X��_�\���O��
				this->state_effect = Boss_Chance_Mark_Out_Line;
				this->limit_erase = this->limit_effect_boss_chance_mark;
				break;
			}
		}
		this->animCnt++;
		//�|�[�Y
		if (ge->pause) { return; }
		this->moveCnt++;
		
		//��Ԗ��̍s��
		this->Move();
		//�ړ�
		this->pos += this->moveVec;
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= this->limit_erase)
		{
			//���Ő\��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->effectImage, this->angle, ML::Vec2(float(di.draw.w / 2), float(di.draw.h / 2)));
		DG::Image_Draw(this->res->effectImage, di.draw, di.src,di.color);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		//�f�t�H���g�J���[��錾
		Color dc(1, 1, 1, 1);
		//�e�G�t�F�N�g���e�[�u���ŗp�ӂ���
		BChara::DrawInfo imageTable[]
		{
			{ Box2D( -96, -64, 192, 128),Box2D(   0,   0, 192, 128),dc },//�X�g���v���n�̏Ռ�1								[ 0]
			{ Box2D( -96, -64, 192, 128),Box2D(   0, 128, 192, 128),dc },//�X�g���v���n�̏Ռ�2								[ 1]
			{ Box2D( -96, -64, 192, 128),Box2D(   0, 256, 192, 128),dc },//�X�g���v���n�̏Ռ�3								[ 2]
			{ Box2D( -96, -64, 192, 128),Box2D( 192,   0, 192, 128),dc },//�p���`���؂�1										[ 3]
			{ Box2D( -96, -64, 192, 128),Box2D( 192, 128, 192, 128),dc },//�p���`���؂�2										[ 4]
			{ Box2D( -96, -64, 192, 128),Box2D( 192, 256, 192, 128),dc },//�p���`���؂�3										[ 5]
			{ Box2D( -96, -64, 192, 128),Box2D( 384,   0, 192, 128),dc },//�p���`�̏Ռ�1										[ 6]
			{ Box2D( -96, -64, 192, 128),Box2D( 384, 128, 192, 128),dc },//�p���`�̏Ռ�2										[ 7]
			{ Box2D( -96, -64, 192, 128),Box2D( 384, 256, 192, 128),dc },//�p���`�̏Ռ�3										[ 8]
			{ Box2D( -96, -64, 192, 128),Box2D( 576,   0, 192, 128),dc },//��̂����1										[ 9]
			{ Box2D( -96, -64, 192, 128),Box2D( 576, 128, 192, 128),dc },//��̂����2										[10]
			{ Box2D( -96, -64, 192, 128),Box2D( 576, 256, 192, 128),dc },//��̂����3										[11]
			{ Box2D( -96, -96, 192, 192),Box2D( 768,   0, 192, 192),dc },//�G�l�~�[���U1										[12]
			{ Box2D( -96, -96, 192, 192),Box2D( 768, 192, 192, 192),dc },//�G�l�~�[���U2										[13]
			{ Box2D( -96, -96, 192, 192),Box2D( 768, 384, 192, 192),dc },//�G�l�~�[���U3										[14]
			{ Box2D( -96, -96, 192, 192),Box2D(1344,   0, 192, 192),dc },//�o���A���`���[�W1									[15]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 192, 192, 192),dc },//�o���A���`���[�W2									[16]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 384, 192, 192),dc },//�o���A���`���[�W3									[17]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 576, 192, 192),dc },//�o���A���`���[�W4									[18]
			{ Box2D( -96, -96, 192, 192),Box2D(1344, 768, 192, 192),dc },//�o���A���`���[�W5									[19]
			{ Box2D( -48, -48,  96,  96),Box2D(1536,   0,  96,  96),ML::Color(0.3f,1,1,1)},//�A1								[20]
			{ Box2D( -48, -48,  96,  96),Box2D(1536,  96,  96,  96),ML::Color(0.3f,1,1,1)},//�A2								[21]
			{ Box2D( -48, -48,  96,  96),Box2D(1536, 192,  96,  96),ML::Color(0.3f,1,1,1)},//�A3								[22]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,   0,  32,  32),dc },//�G�l�~�[���j1										[23]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  32,  32,  32),dc },//�G�l�~�[���j2										[24]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  64,  32,  32),dc },//�G�l�~�[���j3										[25]
			{ Box2D( -16, -16,  32,  32),Box2D(1632,  96,  32,  32),dc },//�G�l�~�[���j4										[26]
			{ Box2D( -16, -16,  32,  32),Box2D(1632, 128,  32,  32),dc },//�G�l�~�[���j5										[27]
			{ Box2D( -16, -16,  32,  32),Box2D(1632, 160,  32,  32),dc },//�G�l�~�[���j6										[28]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,   0,  32,  32),dc },//�����j��1											[29]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  32,  32,  32),dc },//�����j��2											[30]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  64,  32,  32),dc },//�����j��3											[31]
			{ Box2D( -16, -16,  32,  32),Box2D(1664,  96,  32,  32),dc },//�����j��4											[32]
			{ Box2D( -16, -16,  32,  32),Box2D(1664, 128,  32,  32),dc },//�����j��5											[33]
			{ Box2D( -16, -16,  32,  32),Box2D(1664, 160,  32,  32),dc },//�����j��6											[34]
			{ Box2D(  -6,  -6,  12,  12),Box2D(1728,   0,  12,  12),dc },//�Ή�												[35]
			{ Box2D(-128,-128, 256, 256),Box2D(1792,   0, 256, 256),dc },//�^�[�Q�b�g�T�[�N��1								[36]
			{ Box2D(-128,-128, 256, 256),Box2D(2048,   0, 256, 256),dc },//�^�[�Q�b�g�T�[�N��2								[37]
			{ Box2D(-192,-192, 384, 384),Box2D(2304,   0, 384, 384),dc },//�^�[�Q�b�g�o�[00									[38]
			{ Box2D(-192,-192, 384, 384),Box2D(2304, 384, 384, 384),dc },//�^�[�Q�b�g�o�[01									[39]
			{ Box2D( -96, -96, 192, 192),Box2D(2688,   0, 192, 192),ML::Color(0.5f,1.0f,1.0f,1.0f) },//�o���A				[40]
			{ Box2D( -96,-148, 192, 296),Box2D(2880,   0, 192, 296),dc },//�{�X�X�^���n�C���C�g								[41]
			{ Box2D(-128,-128, 256, 256),Box2D(3086,   0, 256, 256),dc },//�E�B�[�N�|�C���g�T�[�N��1							[42]
			{ Box2D(-128,-128, 256, 256),Box2D(3342,   0, 256, 256),dc },//�E�B�[�N�|�C���g�T�[�N��2							[43]
			{ Box2D(-960,-540,1920,1080),Box2D(3600,   0,1920,1080),dc } //HP�o�[�n�C���C�g									[44]
		};
		//�Ԃ��ϐ���p��
		BChara::DrawInfo  rtv;
		//�e�A�j���[�V�����̍ہA���̕ϐ���animCnt�����v�Z���s��
		int effectCnt = 0;
		//��Ԃ��Ƃ̕`�揈��
		switch (this->state_effect)
		{
		//�A�j���[�V������t����ꍇ
		//�؂�ւ��t���[����
		//�p�^�[����
		//effectCnt�ɑΏۂ̗v�f�ԍ��𑫂��ĕԂ�
		case Unnon:
			break;
		case Hit_Shot:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case Barrier_Recharge:
			effectCnt	= this->animCnt / 3;
			effectCnt %= 5;
			rtv = imageTable[effectCnt+15];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Defeat_Enemy:
			effectCnt = this->animCnt / 6;
			effectCnt %= 6;
			rtv = imageTable[effectCnt + 23];
			break;
		case Bubble:
			rtv = imageTable[this->num_bubble + 20];
			break;
		case Appear:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 9];
			rtv.color = ML::Color(1, 0, 0, 1);
			break;
		case Debris:
			effectCnt = this->animCnt / (this->limit_erase_debris / 6);
			effectCnt %= 6;
			rtv = imageTable[effectCnt + 29];
			break;
		case Spark:
			rtv = imageTable[35];
			break;
		case Target_Circle_00:
			rtv = imageTable[36];
			//�傫����Ԃœo�ꂵ���X�Ƀt�H�[�J�X���銴���ŏk��
			rtv.draw.x *= this->rate_size_circle;
			rtv.draw.y *= this->rate_size_circle;
			rtv.draw.w *= this->rate_size_circle;
			rtv.draw.h *= this->rate_size_circle;
			if (this->rate_size_circle > 1)
			{
				//���Ŏ��ɂ��傤��1�{�ɂȂ�
				this->rate_size_circle -= this->animCnt / (this->limit_effect_target_boss/this->rate_size_circle);
			}
			break;
		case Target_Circle_01:
			rtv = imageTable[37];
			//�傫����Ԃœo�ꂵ���X�Ƀt�H�[�J�X���銴���ŏk��
			rtv.draw.x *= this->rate_size_circle;
			rtv.draw.y *= this->rate_size_circle;
			rtv.draw.w *= this->rate_size_circle;
			rtv.draw.h *= this->rate_size_circle;
			if (this->rate_size_circle > 1)
			{
				//���Ŏ��ɂ��傤��1�{�ɂȂ�
				this->rate_size_circle -= this->animCnt / (this->limit_effect_target_boss / this->rate_size_circle);
			}
			break;
		case Target_Bar:
			effectCnt = this->animCnt / 2;
			effectCnt %= 2;
			rtv = imageTable[38+effectCnt];
			break;
		case Barrier:
			rtv = imageTable[40];
			break;
		case Highlight_Head:
		{
			rtv = imageTable[41];
			this->cnt_anim_highlight += 0.1f;
			float highlight_red_green =
				sinf(this->cnt_anim_highlight);
			if (highlight_red_green < 0.0f)
			{
				highlight_red_green = 0.0f;
			}
			rtv.color.Set(1.0f, highlight_red_green, highlight_red_green, 1.0f);
		}
			break;
		case Boss_Chance_Mark_In_Line:
			rtv = imageTable[42];
			break;
		case Boss_Chance_Mark_Out_Line:
			rtv = imageTable[43];
			break;
		}
		//	�����ɉ����ĉ摜�����E���]����
		if (this->angle_LR == Left)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//�G�t�F�N�g�𐶐�����i�p�x���w�肵�Ȃ��j
	//����	�F	�i���,���W�j
	void Object::Create_Effect(const int& choice_, const ML::Vec2&pos_)
	{
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
	}
	//�G�t�F�N�g�𐶐�����i�p�x���w�肷��j
	//����	�F	�i���,���W,�p�x,�����j
	void Object::Create_Effect(const int& choice_, const ML::Vec2& pos_, const float& angle_, const BChara::Angle_LR& angle_lr_)
	{
		//�|�C���^��錾���Ċm���ɑ�����邱��
		auto eff = Task_Effect::Object::Create(true);
		eff->choice_state = choice_;
		eff->pos = pos_;
		eff->angle = angle_;
		eff->angle_LR = angle_lr_;
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		switch (this->state_effect)
		{
		default:
			break;
		case Non:
			break;
		case Heal:
			if (nullptr == pl) { return; }
			this->moveVec = pl->moveVec;
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Bubble:
			this->moveVec = this->eff->Move_Bubble(this->moveCnt,this->interval_bubble,this->wide_bubble,this->speed_surfacing);
			break;
		case Debris:
			this->moveVec = this->eff->Move_Parabola(this->speed_Debris,this->moveVec.y,this->gravity,this->angle);
			break;
		case Spark:
			this->moveVec = this->eff->Move_Parabola(this->speed_spark, this->moveVec.y, this->gravity, this->angle);
			break;
		case Target_Circle_00:
		case Target_Circle_01:
		case Target_Bar:
		{
			//�{�X�w�b�h�̓���ɏ�Ɉړ�����
			auto boss_head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			if (nullptr == boss_head) { break; }
			ML::Vec2 pos_boss_head_top =
				boss_head->pos - ML::Vec2(0.0f, float(boss_head->hitBase.h / 2));
			this->pos = pos_boss_head_top;
			//��ʌ��ʂ��������Ƃ���Ŏ��g���L��
			auto display_effect =
				ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
			if (nullptr == display_effect)
			{
				this->Kill();
			}
			//�O���̃T�[�N���͔����v����
			if (this->state_effect == Target_Circle_00)
			{
				this->angle -= this->add_angle_target_circle;
			}
			//�����̃T�[�N���͎��v���
			else if (this->state_effect == Target_Circle_01)
			{
				this->angle += this->add_angle_target_circle;
			}
			break;
		}
		case Barrier:
			//�v���C����Ǐ]����
			this->pos = pl->pos;
			//��]
			if (pl->angle_LR == Left)
			{
				this->angle += this->add_angle_barrier;
			}
			else
			{
				this->angle -= this->add_angle_barrier;
			}
			break;
		case Highlight_Head:
		{
			auto head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			//�{�X�w�b�h�̎�_��Ǐ]����
			this->pos = head->pos;
			break;
		}
		case Boss_Chance_Mark_In_Line:
		case Boss_Chance_Mark_Out_Line:
			auto head =
				ge->GetTask_One_GN<Boss_Head::Object>(Boss_Head::defGroupName, Boss_Head::defName);
			if (nullptr == head) { break; }
			//�{�X�w�b�h�̎�_��Ǐ]����
			int correction_pos = head->Get_Correction_Pos_Weak_Point();
			ML::Vec2 chase_head =
				head->pos + ML::Vec2(0.0f, float(correction_pos));
			this->pos = chase_head;
			//�O���̃T�[�N���͔����v����
			if (this->state_effect == Boss_Chance_Mark_In_Line)
			{
				this->angle -= this->add_angle_target_circle;
			}
			//�����̃T�[�N���͎��v���
			else if (this->state_effect == Boss_Chance_Mark_Out_Line)
			{
				this->angle += this->add_angle_target_circle;
			}
			//�{�X�̏�Ԃɉ����ď���
			if (head->state != Stan)
			{
				this->Kill();
			}
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