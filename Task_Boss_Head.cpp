//-------------------------------------------------------------------
//�{�X�i�w�b�h�j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Head.h"
#include  "Task_Player.h"
#include  "Task_Boss_Lower.h"

namespace  Boss_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "head";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		this->name_rect = "rect";
		DG::Image_Create(this->name_rect, "./data/image/UI.png");
		//���\�[�X��
		this->base_file_path_sound = "./data/sound/wav/";
		this->name_sound_hit_reflect = "sound_hit_reflect";
		this->name_sound_wait_under_00 = "sound_wait_under00";
		this->name_sound_wait_under_01 = "sound_wait_under01";
		this->name_sound_hiding_under = "sound_hiding_under";
		this->name_sound_appear_under = "sound_appear_under";
		//�T�E���h�̐���
		DM::Sound_CreateSE(this->name_sound_hit_reflect, this->base_file_path_sound + "hit_reflect.wav");
		DM::Sound_CreateSE(this->name_sound_wait_under_00, this->base_file_path_sound + "wait_under_head00.wav");
		DM::Sound_CreateSE(this->name_sound_wait_under_01, this->base_file_path_sound + "wait_under_head01.wav");
		DM::Sound_CreateSE(this->name_sound_hiding_under, this->base_file_path_sound + "hiding_under.wav");
		DM::Sound_CreateSE(this->name_sound_appear_under, this->base_file_path_sound + "appear_under.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
		DG::Image_Erase(this->name_rect);
		DM::Sound_Erase(this->name_sound_hit_reflect);
		DM::Sound_Erase(this->name_sound_wait_under_00);
		DM::Sound_Erase(this->name_sound_wait_under_01);
		DM::Sound_Erase(this->name_sound_hiding_under);
		DM::Sound_Erase(this->name_sound_appear_under);
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
		this->state = Hiding_Under;									//��ԊǗ�
		this->render2D_Priority[1] = 0.85f;							//�`�揇
		this->hitBase = ML::Box2D(-92, -145, 184, 290);				//�\����`
		this->recieveBase = ML::Box2D(-92, -46, 184, 92);			//�L�����N�^�Ƃ̔����`
		this->correction_pos_weak_point = -75;						//��_��`�̖{�̂���̍��W�␳
		this->std_pos_x = 8382.0f;									//���h���l
		this->speed_shake_def = 64.0f;								//�ʏ펞�̉��h�ꑬ�x
		this->speed_shake_ref = 128.0f;								//���˔�e���̉��h�ꑬ�x
		this->speed_shake_stn = 32.0f;								//�C�⎞�̉��h�ꑬ�x
		this->speed_shake = this->speed_shake_def;					//���������s���Ǐ]���x
		this->cnt_shake = 0.0f;										//�J�E���^���h��
		this->interval_shake = 12.0f;								//�Ԋu���h��
		this->interval_shake_stan = 24.0f;							//�C�⎞�̉��h�ꑬ�x
		this->shot = new Shot01::Object();							//�V���b�g�I�u�W�F�N�g�C���X�^���X
		this->boss = new Boss();									//�{�X�N���X�C���X�^���X
		this->eff = new Task_Effect::Object();						//�G�t�F�N�g�I�u�W�F�N�g�C���X�^���X
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);				//�ړ��ʃV���b�g
		this->hit_shot = ML::Box2D(-16, -16, 32, 32);				//��`�V���b�g
		this->interval_shot = 360;									//�������ԃV���b�g
		this->hp = 40;												//HP�{�X
		this->add_un_hit = 60;										//�v���C���ɗ^���閳�G����
		this->num_shot = 5;											//�e�̐�����
		this->angle_create_shot = 18.0f;							//�e�𐶐�����Ԋu
		this->flag_alive_base = true;								//�y��̐���
		this->interval_to_appear = 240;								//�o�ꂷ��܂ł̎���
		this->interval_create_effect = 30;							//�G�t�F�N�g�̐����Ԋu
		this->interval_return = 180;								//�V���b�g����߂�܂ł̎���
		this->limit_move_vertically = 57;							//�c�������̓o��ړ�����
		this->speed_move_under = 10.0f;								//�c�������̓o��A�ޏꑬ�x
		this->speed_chase_hiding = 0.025f;							//���s���v���C���ɐڋ߂��銄��
		this->hit_vertically_long = ML::Box2D(-92, -46, 184, 92);	//�c���̎��̋�`�ihitBase�ɑ�����Ďg�p�j
		this->hit_horizontally_long = ML::Box2D(-46, -92, 92, 184);	//�����̎��̋�`�ihitBase�ɑ�����Ďg�p�j
		this->dist_quake_boss_return_appear = ML::Vec2(0,5);		//�{�X�p�o��ޏ��ʗh�ꕝ
		this->limit_quake_boss_return_appear = 90;					//�{�X�p�o��ޏ��ʗh�ꎞ��
		this->cnt_defeat_parts = 0;									//���̃p�[�c��j�󂷂�Ɖ��Z���A����ɒB����ƋC�₷��
		this->limit_stan = 360;										//�C�⎞��

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
		delete this->eff;
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
		//�J�E���^
		this->moveCnt++;
		this->animCnt++;
		this->cnt_shake += 1.0f;
		//���G���Ԃ̌���
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//���˂��󂯂��ꍇ�A���~�b�g������������
		if (this->limit_hit_reflect > 0)
		{
			this->limit_hit_reflect--;
		}
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
		//�ړ�����
		this->pos += this->moveVec;
		//���̃^�X�N��|���ƃN���A�t���O����
		if (this->hp <= 0)
		{
			ge->clear = true;
		}
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
		//���������`�̕\��
		if (ge->debugMode)
		{
			ML::Box2D draw = this->recieveBase;
			draw.Offset(this->pos);
			ML::Box2D  src(64, 0, 64, 64);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//�{�̍��W�����_�̈ʒu�␳
			draw.y += this->correction_pos_weak_point;
			DG::Image_Draw(this->res->name_rect, draw, src, ML::Color(0.5f, 1.0f, 0.0f, 0.0f));
		}
	}
	//�v�l&�󋵔��f(�X�e�[�^�X����)
	void Object::Think()
	{
		//�Ƃ肠�������̏�Ԃ��w��
		BChara::State nm = this->state;
		//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���邱�Ƃ�ړI�Ƃ��Ă���B
		//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
		switch (nm)
		{
		case Appear_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Wait_Under; }
			break;
		case Wait_Under:
			if (this->moveCnt > this->interval_shot) { nm = Shot_Under; }
			if (this->cnt_defeat_parts == DEFEAT_PARTS) { nm = Stan; }//�e�p�[�c���j�󂳂��ƋC���ԂɑJ�ڂ���
			break;
		case Shot_Under:
			if (this->moveCnt > this->interval_return) { nm = Return_Under; }
			if (this->cnt_defeat_parts == DEFEAT_PARTS) { nm = Stan; }//�e�p�[�c���j�󂳂��ƋC���ԂɑJ�ڂ���
			break;
		case Return_Under:
			if (this->moveCnt > this->limit_move_vertically) { nm = Choice_Vertically_Or_Horizontally; }
			break;
		case Hiding_Under:
			if (this->moveCnt > this->interval_to_appear) { nm = Appear_Under; }
			break;
		case Choice_Vertically_Or_Horizontally:
			//���ɏc�����݂̂�I������
			nm = Hiding_Under;
			break;
		case End_Pattern_Boss:
			if (this->moveCnt >= LIMIT_END_PATTERN_BOSS_HEAD) { nm = Lose; }
			break;
		case Stan:		//�C��
			if (this->moveCnt > this->limit_stan) { nm = Return_Under; }
			break;
		case Lose:		//���S
			break;
		}
		//���S����
		if (this->hp <= 0) { nm = End_Pattern_Boss; }
		//���[�V�����X�V
		this->UpdateMotion(nm);
	}
	//���[�V�����ɑΉ���������
	void Object::Move()
	{
		//�����̏�Ԃŋ��ʂ��čs�������͂����ɋL�q����
		//��O������ꍇ��switch���̉����ɋL�q����

		//�����蔻�� 
		ML::Box2D me = this->recieveBase.OffsetCopy(this->pos);
		auto targets = ge->GetTask_Group_G<BChara>("�v���C��");
		if (nullptr == targets) { return; }
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			//����ɐڐG�̗L�����m�F������
			if ((*it)->CheckHit(me)) 
			{
				//����Ƀ_���[�W�̏������s�킹��
				BChara::AttackInfo at = { 1,0,0 };
				(*it)->Recieved(this, at, this->add_un_hit);
				break;
			}
		}
		//�v���C���̈ʒu�����E�Ŕ��f����
		auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
		if (nullptr != pl)
		{
			this->angle_LR = this->Check_LR(this->angle_LR, pl->pos.x);
		}
		//���˒e��e��SE�Đ�
		//�����A�����Ƀf�N�������g����Ă��邽��-1�őΉ�
		//�čl�̗]�n����
		if (this->limit_hit_reflect == LIMIT_HIT_REFLECT - 1)
		{
			//�Đ����̖������~
			DM::Sound_Stop(this->res->name_sound_wait_under_00);
			DM::Sound_Stop(this->res->name_sound_wait_under_01);
			DM::Sound_Play_Volume(this->res->name_sound_hit_reflect, false, VOLUME_SE_HIT_REFLECTION);
		}
		//-------------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->state)
		{
		default:
			break;
		case Hiding_Under:
		{
			//SE�̍Đ�
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_hiding_under, false, VOLUME_ALL_GAME);
			}
			auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			//���s���̓v���C���̍��W�ɂ���荇�킹��
			float to_vec_x = pl->pos.x - this->pos.x;
			this->pos.x += to_vec_x * this->speed_chase_hiding;
			//��ԑJ�ڂ̒��O�ɗh��̊�_�����݂̍��W�ɕ␳����
			if (this->moveCnt == this->interval_to_appear)
			{
				this->std_pos_x = this->pos.x;
			}
			//���s���̃G�t�F�N�g�𐶐�����
			//�������Ԃƕϐ��錾�čl
			if (this->moveCnt % 10 == 0)
			{
				for (int i = 0; i < 5; ++i)
				{
					//�����ʒu��錾����
					ML::Vec2 init = ML::Vec2(this->pos.x, this->pos.y - float(this->hitBase.h / 2));
					//�G�t�F�N�g�̐���
					this->eff->Create_Effect(7,init,-(float(rand() % 180)),this->angle_LR);
				}
			}
			//SE�̒�~
			if (this->moveCnt == this->interval_to_appear)
			{
				DM::Sound_Stop(this->res->name_sound_hiding_under);
			}
			break;
		}
		case Appear_Under:
		{
			//�o�ꎞ�ɉ�ʂ�h�炷
			if (this->moveCnt == 0)
			{
				auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_boss_return_appear, this->limit_quake_boss_return_appear);
				//�o�ꎞ�ɑS�Ẵp�[�c���j�󂳂�Ă����炱���ŉ񕜂���
				if (this->cnt_defeat_parts == DEFEAT_PARTS)
				{
					this->cnt_defeat_parts = 0;
				}
				//SE�Đ�
				DM::Sound_Play_Volume(this->res->name_sound_appear_under, false, VOLUME_SE_APPEAR_UNDER);
			}
			//��ʒu�܂ňړ�����
			if (this->moveCnt<this->limit_move_vertically)
			{
				this->moveVec.y = -this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		}
		case Wait_Under:
		{
			//SE�̍Đ�
			if (this->moveCnt == 0)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_00, false, VOLUME_ALL_GAME);
			}
			if (this->moveCnt == this->interval_shot / 2)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_01, false, VOLUME_ALL_GAME);
			}
			if (this->moveCnt == this->interval_shot)
			{
				DM::Sound_Play_Volume(this->res->name_sound_wait_under_00, false, VOLUME_ALL_GAME);
			}
			//���˒e���󂯂���͈�莞�ԁA�s�����x���㏸����
			if (this->limit_hit_reflect)
			{
				//��莞�ԓ����������Ȃ�
				this->speed_shake = this->speed_shake_ref;
			}
			else
			{
				//�ʏ푬�x
				this->speed_shake = this->speed_shake_def;
			}
			//���E�ɗh���
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake)*this->speed_shake;
			break;
		}
		case Shot_Under:
			if (this->moveCnt == 0)
			{
				//�W�I�i�v���C���j�|�C���^�̐錾
				auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
				if (nullptr == pl) { return; }
				//�ڕW���W���o���Ă�������݂̂𒊏o����
				ML::Vec2 to_vec = pl->pos - this->pos;
				to_vec = to_vec.Normalize();
				//�p�x�̑�����
				float increase = ML::ToRadian(this->angle_create_shot);
				//�e�̐���
				for (int i = -this->num_shot / 2; i <= this->num_shot / 2; i++)
				{
					//�p�x�̊�l�ɑ����ʂ𐶐����ɗ^����
					float ang = acos(to_vec.x) + increase * i;
					ML::Vec2 vec = ML::Vec2(cos(ang), sin(ang));
					this->shot->Create_Shot(this->pos, vec*SPEED_SHOT, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
				}
			}
			break;
		case Return_Under:
			//��ʗh��
			if (this->moveCnt == 0)
			{
				auto map = ge->GetTask_One_GN<Map2D::Object>(Map2D::defGroupName, Map2D::defName);
				map->Set_Quake(this->dist_quake_boss_return_appear, this->limit_quake_boss_return_appear);
			}
			//�����n�ʂɐ���܂ŉ��Ɉړ�
			if (this->moveCnt<this->limit_move_vertically)
			{
				this->moveVec.y = +this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		case Stan:
			//��x�����A��_�������n�C���C�g�G�t�F�N�g�𐶐�����
			if (this->moveCnt == 0)
			{
				this->eff->Create_Effect(13, this->pos);
			}
			//�C�⒆�͂�����艡�h�ꂷ��
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake_stan)*this->speed_shake_stn;
			break;
		case End_Pattern_Boss:
			if (this->moveCnt == 0)
			{
				//�ړ�����߂�
				this->moveVec=ML::Vec2(0.0f,0.0f);
			}
			//���Ԋu�Ŕ����G�t�F�N�g�𐶐�
			if (this->moveCnt % INTERVAL_CREATE_END_EFFECT_HEAD == 0)
			{
				//�����_���Ȑ����ʒu���p�x����w��
				float ang = float(rand() % 360);
				//���g�̋�`�͈͓̔��Ƀ����_���ɐ���
				float x = float(cos(ang)*float(rand() % this->hitBase.w / 2));
				float y = float(sin(ang)*float(rand() % this->hitBase.h / 2));
				this->eff->Create_Effect(3, this->pos + ML::Vec2(x, y));
			}
			break;
		}
	}
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
	void Object::Recieved(BChara* from_, AttackInfo at_, const int& un_hit_)
	{
		//�C�⎞�ȊO�̓_���[�W���󂯂Ȃ�
		if (this->state != Stan) { return; }
		//���G���Ԓ��̓_���[�W���󂯂Ȃ�
		if (this->time_un_hit > 0)
		{
			return;
		}
		//���j���̓q�b�g�\�������s��Ȃ�
		if (this->state == End_Pattern_Boss) { return; }
		if (this->state == Lose) { return; }
		this->hp -= at_.power;
		//���G����
		this->time_un_hit = un_hit_;
		//SE�̍Đ�
		DM::Sound_Play_Volume(this->res->name_sound_hit_reflect, false, VOLUME_SE_HIT_REFLECTION);
		//���̃^�X�N�ł�Damage�ɑJ�ڂ��Ȃ�
		//�q�b�g�\���͖��G���Ԃ̗L���ŊǗ�����
	}
	//�A�j���[�V��������
	BChara::DrawInfo Object::Anim()
	{
		//�f�t�H���g�̒l��錾
		ML::Color dc(1, 1, 1, 1);
		;
		BChara::DrawInfo imageTable[] =
		{
			//draw		   src			 color
			{ this->hitBase,ML::Box2D(0, 368, 184, 290),dc },//default	[0]
			{ this->hitBase,ML::Box2D(0, 658, 184, 290),dc } //Lose	[1]
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
		case Lose:
			rtv = imageTable[1];
			break;
		}
		//�q�b�g�\��
		if (this->time_un_hit > 0)
		{
			rtv.src.y += rtv.src.h;
		}
		//���E�̔��]����
		if (false == this->angle_LR)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
		/*return imageTable[int(this->state)];*/
	}
	//���s����o��܂ł̎��Ԃ�����������
	//����	�F	�i�����ʁj
	void Object::Decrease_Interval_To_Appear(const int& decrease_)
	{
		this->interval_to_appear -= decrease_;
	}
	//�o�ꂩ��V���b�g�Ɉڍs����܂ł̎��Ԃ�����������
	//����	�F	�i�����ʁj
	void Object::Decrease_Interval_Shot(const int& decrease_)
	{
		this->interval_shot -= decrease_;
	}
	//���̃p�[�c�̔j�󐔂����Z����
	void Object::Add_Defeat_Parts()
	{
		this->cnt_defeat_parts++;
	}
	//���̃p�[�c�̔j�󐔂��擾����
	int Object::Get_Defeat_Parts()
	{
		return this->cnt_defeat_parts;
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