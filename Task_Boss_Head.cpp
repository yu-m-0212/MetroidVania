//-------------------------------------------------------------------
//�{�X�i�w�b�h�j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Head.h"
#include  "Task_Player.h"

namespace  Boss_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "head";
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
		this->state = Appear_Under;									//��ԊǗ�
		this->render2D_Priority[1] = 0.6f;							//�`�揇
		this->hitBase = ML::Box2D(-92, -46, 184, 92);				//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;							//�L�����N�^�Ƃ̔����`
		this->std_pos_x = 700.0f;									//���h���l
		this->speed_shake = 64.0f;									//���x���h��
		this->cnt_shake = 0.0f;										//�J�E���^���h��
		this->interval_shake = 12.0f;								//�Ԋu���h��
		this->shot = new Shot01::Object();							//�V���b�g�I�u�W�F�N�g�C���X�^���X
		this->boss = new Boss();									//�{�X�N���X�C���X�^���X
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);				//�ړ��ʃV���b�g
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);					//��`�V���b�g
		this->interval_shot = 360;									//�������ԃV���b�g
		this->hp = HP_BOSS;											//HP�{�X
		this->add_un_hit = 60;										//�v���C���ɗ^���閳�G����
		this->num_shot = 5;											//�e�̐�����
		this->angle_create_shot = 18.0f;							//�e�𐶐�����Ԋu
		this->flag_alive_base = true;								//�y��̐���
		this->interval_to_appear = 180;								//�o�ꂷ��܂ł̎���
		this->interval_create_effect = 30;							//�G�t�F�N�g�̐����Ԋu
		this->interval_return = 180;								//�V���b�g����߂�܂ł̎���
		this->limit_move_vertically = 120;							//�c�������̓o��A�ޏ�ړ�����
		this->speed_move_under = 5.0f;								//�c�������̓o��A�ޏꑬ�x
		this->speed_chase_hiding = 0.05f;							//���s���v���C���ɐڋ߂��銄��
		this->hit_vertically_long = ML::Box2D(-92, -46, 184, 92);	//�c���̎��̋�`�ihitBase�ɑ�����Ďg�p�j
		this->hit_horizontally_long = ML::Box2D(-46, -92, 92, 184);	//�����̎��̋�`�ihitBase�ɑ�����Ďg�p�j

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
		//�J�E���^
		this->cnt_shake += 1.0f;
		this->moveCnt++;
		this->animCnt++;
		//���G���Ԃ̌���
		if (this->time_un_hit > 0) { this->time_un_hit--; }
		//�v�l�E�󋵔��f
		this->Think();
		//�����[�V�����ɑΉ���������
		this->Move();
		//�ړ�����
		this->pos += this->moveVec;
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
			break;
		case Shot_Under:
			if (this->moveCnt > this->interval_return) { nm = Return_Under; }
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
		case Lose:		//���S
			break;
		}
		//���S����
		if (this->hp <= 0) { nm = Lose; }
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
			if ((*it)->CheckHit(me)) {
				//����Ƀ_���[�W�̏������s�킹��
				BChara::AttackInfo at = { 1,0,0 };
				(*it)->Received(this, at, this->add_un_hit);
				break;
			}
		}
		//���E�ɗh���
		//���̃^�X�N�̓�������Ƃ���
		switch (this->state)
		{
		default:
			break;
		case Wait_Under:
			this->pos.x = this->std_pos_x + sinf(this->cnt_shake / this->interval_shake)*this->speed_shake;
			break;
		case Hiding_Under:
			auto pl = ge->GetTask_One_G<Player::Object>(Player::defGroupName);
			//���s���̓v���C���̍��W�ɂ���荇�킹��
			float to_vec_x= pl->pos.x - this->pos.x;
			this->pos.x += to_vec_x * this->speed_chase_hiding;
			//��ԑJ�ڂ̒��O�ɗh��̊�_�����݂̍��W�ɕ␳����
			if (this->moveCnt == this->interval_to_appear)
			{
				this->std_pos_x = this->pos.x;
			}
		}
		//-------------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->state)
		{
		default:
			break;
		case Hiding_Under:
			break;
		case Appear_Under:
			//��ʒu�܂ňړ�����
			if (this->moveCnt < this->limit_move_vertically)
			{
				this->moveVec.y = -this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		case Shot_Under:
			if (this->moveCnt == 0)
			{
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
			if (this->moveCnt < this->limit_move_vertically)
			{
				this->moveVec.y = +this->speed_move_under;
			}
			else
			{
				this->moveVec.y = 0.0f;
			}
			break;
		}
	}
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
	void Object::Received(BChara* from_, AttackInfo at_, const int& un_hit_)
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
		case Lose:
			rtv = imageTable[2];
			break;
		}
		//�q�b�g�\��
		if (this->time_un_hit > 0)
		{
			rtv.src.y += rtv.src.h;
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