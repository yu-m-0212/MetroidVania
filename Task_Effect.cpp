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
		this->cntLimit = 0;					//���ł܂ł̎���
		this->dist = 0.0f;					//��]����ۂ̒��S����̋���
		this->angle = 0.0f;					//�p�x
		this->center = Vec2(0, 0);			//��]��
		this->num_bubble = 0;				//�A�̑傫��
		this->interval_bubble = 0;			//�A�̗h�����
		this->wide_bubble = 0.0f;			//�A�̗h�ꕝ
		this->render2D_Priority[1] = 0.5f;	//�`�揇\

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
		//��Ԗ��̍s��
		this->Move();
		//���f�[�^���^�X�N���
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= this->cntLimit) {
			//���Ő\��
			this->Kill();
			return;
		}
		//�ړ�
		this->pos += this->moveVec;
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
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
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
		switch (this->state)
		{
		//�A�j���[�V������t����ꍇ
		//�؂�ւ��t���[����
		//�p�^�[����
		//effectCnt�ɑΏۂ̗v�f�ԍ��𑫂��ĕԂ�
		case Unnon:
			break;
		case ImpactPunch:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case StompLanding:
			effectCnt	= this->animCnt / 3;
			effectCnt %= 5;
			rtv = imageTable[effectCnt+15];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Lose:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 12];
			break;
		case Bubble:
			rtv = imageTable[this->num_bubble + 20];
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
	//�A�G�t�F�N�g�𐶐�����
	//����	�F	�i�ԍ�,�������W,�h�����,�h�ꕝ,���㑬�x,�p�x,���ł܂ł̎��ԁj
	void Object::Create_Bubble(const int& num_, const ML::Vec2& pos_, const int& interval_, const float& wide_, const float& speedY_, const float& angle_, const int& limit_)
	{
		auto bubble = Task_Effect::Object::Create(true);
		bubble->Set_State(Bubble);
		bubble->Set_Num_Bubble(num_);
		bubble->pos = pos_;
		bubble->Set_Interval_Bubble(interval_);
		bubble->Set_Wide_Bubble(wide_);
		bubble->Set_Speed_Surfacing(speedY_);
		bubble->Set_Angle(angle_);
		bubble->Set_Limit(limit_);
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		switch (this->state)
		{
		default:
			break;
		case StompLanding:
			break;
		case Shoot:
			break;
		case Jumpshoot:
		case Fallshoot:
			break;
		case Heal:
			if (nullptr == pl) { return; }
			this->moveVec = pl->moveVec;
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Lose:
			break;
		case Bubble:
			this->moveVec = eff.Move_Bubble(this->moveCnt, this->interval_bubble, this->wide_bubble, this->speed_surfacing);
			/*this->moveVec.x = float(sin(this->moveCnt / this->interval_bubble)*this->wide_bubble);
			this->moveVec.y = -this->speed_surfacing;*/
			break;
		}
	}
	//�A�N�Z�T

	//�Ăяo���ۂɏ��ł܂ł̎��Ԃ��w�肷��
	//����	�F	�i���ł܂ł̎��ԁj
	void Object::Set_Limit(const int& limit_)
	{
		this->cntLimit = limit_;
	}
	//�A�̑傫�����w�肷��
	//����	�F	�i0~3)
	void Object::Set_Num_Bubble(const int& num_)
	{
		this->num_bubble = num_;
	}
	//�A�̗h��������w�肷��
	//����	�F	�i�h������j
	void Object::Set_Interval_Bubble(const int& interval_)
	{
		this->interval_bubble = interval_;
	}
	//�A�̕��㑬�x���w�肷��
	//����	�F	�i���㑬�x�j
	void Object::Set_Speed_Surfacing(const float& speedY_)
	{
		this->speed_surfacing = speedY_;
	}
	//�A�̗h�ꕝ���w�肷��
	//����	�F	�i�h�ꕝ�j
	void Object::Set_Wide_Bubble(const float& wide_)
	{
		this->wide_bubble = wide_;
	}
	//���S�_����L����G�t�F�N�g���Ăяo���ہA���S����̏����ʒu���w�肷��
	//����	�F	�i���S�_����̏����ʒu)
	void Object::Set_Dist(const float& dist_)
	{
		this->dist = dist_;
	}
	//���S�_����L����G�t�F�N�g�̒��S����̋������擾����
	float Object::Get_Dist()
	{
		return this->dist;
	}
	//�\������p�x���w�肷��
	//����	�F	 (ML::ToRadian(�p�x))
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
	}
	//��]�̒��S���O������w�肷��
	//����	�F	�i���S���W�j
	void Object::Set_Center(const Vec2& center_)
	{
		this->center = center_;
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