//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"
using namespace ML;
namespace  Effect
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
		this->cntLimit = 0;
		this->animCnt = 0;
		this->moveCnt = 0;
		
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
		DG::Image_Draw(this->res->effectImage, di.draw, di.src);
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
			{Box2D(-96, -64, 192, 128),Box2D(  0,   0, 192, 128),dc},//�X�g���v���n�̏Ռ�1	[ 0]
			{Box2D(-96, -64, 192, 128),Box2D(  0, 128, 192, 128),dc},//�X�g���v���n�̏Ռ�2	[ 1]
			{Box2D(-96, -64, 192, 128),Box2D(  0, 256, 192, 128),dc},//�X�g���v���n�̏Ռ�3	[ 2]
			{Box2D(-96, -64, 192, 128),Box2D(192,   0, 192, 128),dc},//�p���`���؂�1			[ 3]
			{Box2D(-96, -64, 192, 128),Box2D(192, 128, 192, 128),dc},//�p���`���؂�2			[ 4]
			{Box2D(-96, -64, 192, 128),Box2D(192, 256, 192, 128),dc},//�p���`���؂�3			[ 5]
			{Box2D(-96, -64, 192, 128),Box2D(384,   0, 192, 128),dc},//�p���`�̏Ռ�1			[ 6]
			{Box2D(-96, -64, 192, 128),Box2D(384, 128, 192, 128),dc},//�p���`�̏Ռ�2			[ 7]
			{Box2D(-96, -64, 192, 128),Box2D(384, 256, 192, 128),dc},//�p���`�̏Ռ�3			[ 8]
			{Box2D(-96, -64, 192, 128),Box2D(576,   0, 192, 128),dc},//��̂����1			[ 9]
			{Box2D(-96, -64, 192, 128),Box2D(576, 128, 192, 128),dc},//��̂����2			[10]
			{Box2D(-96, -64, 192, 128),Box2D(576, 256, 192, 128),dc} //��̂����3			[11]

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
		case Punch1:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 3];
			break;
		case Punch2:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 3];
			break;
		case ImpactPunch:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case StompLanding:
			effectCnt	= this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+0];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
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
	//�Ăяo���ۂɏ��ł܂ł̎��Ԃ��w�肷��
	//����	�F	�i���ł܂ł̎��ԁj
	void Object::Set_Limit(const int& limit_)
	{
		this->cntLimit = limit_;
	}
	//��Ԃ��Ƃɍs�����w�肷��
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		switch (this->state)
		{
		case Punch1:
			//�p���`���̓v���C���̓����ɍ��킹�Ĕ����`���O�i����
			this->moveVec = pl->moveVec;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Punch2:
			//�p���`���̓v���C���̓����ɍ��킹�Ĕ����`���O�i����
			this->moveVec = pl->moveVec;
			//�v���C�����ǂɏՓ˂�����ړ��ʂ�0��
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
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