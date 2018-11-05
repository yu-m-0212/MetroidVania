//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Effect.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"

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
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
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
		this->state_effect = Non;			//��ԊǗ�
		this->limit_erase = 0;				//���ԏ��ł܂�
		this->dist = 0.0f;					//��]����ۂ̒��S����̋���
		this->angle = 0.0f;					//�p�x
		this->center = Vec2(0, 0);			//��]��
		this->num_bubble = 0;				//�A�̑傫��
		this->interval_bubble = 16;			//�A�̗h�����
		this->wide_bubble = 5.0f;			//�A�̗h�ꕝ
		this->limit_erase_hit_shot = 18;	//���ԏ��ł܂Ńq�b�g�V���b�g
		this->limit_erase_barrier = 15;		//���ԏ��ł܂Ńo���A
		this->limit_erase_defeat = 24;		//���ԏ��ł܂Ō��j
		this->limit_erase_heal = 24;		//���ԏ��ł܂ŉ�
		this->limit_erase_bubble = 600;		//���ԏ��ł܂ŖA
		this->limit_erase_appear = 180;		//���ԏ��ł܂œo��
		this->speed_surfacing = 3.0f;		//���x����
		this->render2D_Priority[1] = 0.3f;	//�`�揇
		this->choice_state = -1;			//�O�������Ԃ��w�肷��ہA�g�p
		Effect* eff = new Effect();			//���\�b�h�Ăяo��

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
				break;
			case 2:		//�o���A
				this->state_effect = Barrier;
				this->limit_erase = this->limit_erase_barrier;
				break;
			case 3:
				this->state_effect = Defeat;
				this->limit_erase = this->limit_erase_defeat;
				break;
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
			}
		}
		//�|�[�Y
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
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
			{Box2D(-96, -64, 192, 128),Box2D(   0,   0, 192, 128),dc},//�X�g���v���n�̏Ռ�1		[ 0]
			{Box2D(-96, -64, 192, 128),Box2D(   0, 128, 192, 128),dc},//�X�g���v���n�̏Ռ�2		[ 1]
			{Box2D(-96, -64, 192, 128),Box2D(   0, 256, 192, 128),dc},//�X�g���v���n�̏Ռ�3		[ 2]
			{Box2D(-96, -64, 192, 128),Box2D( 192,   0, 192, 128),dc},//�p���`���؂�1				[ 3]
			{Box2D(-96, -64, 192, 128),Box2D( 192, 128, 192, 128),dc},//�p���`���؂�2				[ 4]
			{Box2D(-96, -64, 192, 128),Box2D( 192, 256, 192, 128),dc},//�p���`���؂�3				[ 5]
			{Box2D(-96, -64, 192, 128),Box2D( 384,   0, 192, 128),dc},//�p���`�̏Ռ�1				[ 6]
			{Box2D(-96, -64, 192, 128),Box2D( 384, 128, 192, 128),dc},//�p���`�̏Ռ�2				[ 7]
			{Box2D(-96, -64, 192, 128),Box2D( 384, 256, 192, 128),dc},//�p���`�̏Ռ�3				[ 8]
			{Box2D(-96, -64, 192, 128),Box2D( 576,   0, 192, 128),dc},//��̂����1				[ 9]
			{Box2D(-96, -64, 192, 128),Box2D( 576, 128, 192, 128),dc},//��̂����2				[10]
			{Box2D(-96, -64, 192, 128),Box2D( 576, 256, 192, 128),dc},//��̂����3				[11]
			{Box2D(-96, -96, 192, 192),Box2D( 768,   0, 192, 192),dc},//�G�l�~�[���U1				[12]
			{Box2D(-96, -96, 192, 192),Box2D( 768, 192, 192, 192),dc},//�G�l�~�[���U2				[13]
			{Box2D(-96, -96, 192, 192),Box2D( 768, 384, 192, 192),dc},//�G�l�~�[���U3				[14]
			{Box2D(-96, -96, 192, 192),Box2D(1344,   0, 192, 192),dc},//�Ռ��g1					[15]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 192, 192, 192),dc},//�Ռ��g2					[16]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 384, 192, 192),dc},//�Ռ��g3					[17]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 576, 192, 192),dc},//�Ռ��g4					[18]
			{Box2D(-96, -96, 192, 192),Box2D(1344, 768, 192, 192),dc},//�Ռ��g5					[19]
			{Box2D(-48, -48,  96,  96),Box2D(1536,   0,  96,  96),ML::Color(0.3f,1,1,1)},//�A1	[20]
			{Box2D(-48, -48,  96,  96),Box2D(1536,  96,  96,  96),ML::Color(0.3f,1,1,1)},//�A2	[21]
			{Box2D(-48, -48,  96,  96),Box2D(1536, 192,  96,  96),ML::Color(0.3f,1,1,1)},//�A3	[22]
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
		case Barrier:
			effectCnt	= this->animCnt / 3;
			effectCnt %= 5;
			rtv = imageTable[effectCnt+15];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Defeat:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 12];
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