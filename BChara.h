#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
	//�ύX������������������������������������������������������

	//�\����
	//�L�����N�^�̍s����ԃt���O
	enum State
	{
		Unnon = -1,		//	����(�g���܂���j
		Stand,			//	��~
		Dash,			//	�_�b�V��
		SlowDown,		//	����
		Air,			//	�󒆍U��
		PreStomp,		//	�X�g���v�̗\������
		LandStomp,		//	�X�g���v
		AirStomp,		//	�󒆂���̍~��
		StompLanding,	//	�X�g���v���n
		Shoot,			//	�ˌ�(�n��)
		Jumpshoot,		//	�ˌ�(�󒆏㏸)
		Fallshoot,		//	�ˌ�(�󒆍~��)
		Jump,Jump2,		//	�W�����v
		Fall,Fall2,		//	����
		Landing,		//	���n
		Damage,			//	��e
		Walk,			//	���s
		Turn,			//	����
		TakeOff,		//	��ї��u��
		Bound,			//	�_���[�W���󂯂Đ������ł���
		Lose,			//	������E���V
		Caution,		//	�G�l�~�[�x����
		TargetLost,		//	�Ώۂ����������Ƃ�
		//�ȉ��̓{�X�p�̃X�e�[�g�����g
		Boss_Floating,	//���ɂ��G��Ă��Ȃ����
		Boss_Base,		//�ڒn���y��̖�����S��
		Boss_Common		//���̃{�X��`�ɐڐG���Ǐ]�^������
	};
	//���E�̌����i2D�����_�Q�[����p�j
	enum Angle_LR { Left, Right };
	//�A�j���[�V�������\����
	//draw,src,color
	struct DrawInfo {
		ML::Box2D		draw, src;
		ML::Color		color;
	};
	//�U�����
	//(�U���̈З�,�������x,�U���̑���)
	struct AttackInfo {
		int power;	//�U���̈З�
		int hit;	//�������x
		int element;//�U���̑���
					//���̑��K�v�ɉ�����
	};
	//�L�����N�^���ʃ����o�ϐ�
	//���X�ɔ���J�ɂ��邱��
	//BChara���\�b�h�Ɏg�p���Ȃ����̂͂����Ɏ������邱��
	Angle_LR	angle_LR;
	State		state;			//���݂̍s���������t���O
	ML::Vec2    pos;			//�L�����N�^�ʒu
	ML::Vec2	moveVec;		//�ړ��x�N�g��
	ML::Box2D   hitBase;		//�}�b�v�Ƃ̔���͈�(�L�����N�^�Ƃ̐ڐG�͕ʂɍs��)
	ML::Box2D	recieveBase;	//�L�����N�^�Ƃ̔���͈�(�}�b�v�p����ύX���Ȃ��ꍇ��hitbase�ŏ���������)
	int			moveCnt;		//�s���J�E���^
	int			time_un_hit;	//���G���ԃJ�E���^
	int			animCnt;		//�A�j���[�V�����J�E���^
	int			hp;				//�w���X�|�C���g
	float		gravity;		//�t���[���P�ʂ̉��Z��
	float		maxSpeed;		//���E�����ւ̈ړ��̍ő呬
	float		addSpeed;		//���E�����ւ̈ړ��̉��Z��
private:
	ML::Vec2	move_back;		//�U���̂ӂ��Ƃї�
	bool		tip;			//�m�b�N�o�b�N�̔������Ȃ��U���̏ꍇ�A�e��������true
	bool		range_Wide;		//�U�����L�͈͂��ۂ�(�ӂ��Ƃѕ��������߂�ۂɎg�p)
	bool		flag_erase;		//��̂ɐG�ꂽ�Ƃ��Atrue
	float		angle;			//�p�x
public:
	//���\�b�h
	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BChara()
		: state(Stand)
		, angle_LR(Right)
		, pos(0, 0)
		, moveVec(0, 0)
		, move_back(0, 0)
		, hitBase(0, 0, 0, 0)
		, recieveBase(0, 0, 0, 0)
		, moveCnt(0)
		, time_un_hit(0)
		, animCnt(0)
		, hp(1)
		, gravity(0)
		, maxSpeed(0)
		, addSpeed(0)
		, tip(false)
		, range_Wide(false)
		, flag_erase(false)
		, angle(0)
	{
	}
	virtual  ~BChara(){}
	//�e�N���X���\�b�h�̃I�[�o�[���C�h
	void UpDate()
	{
		ML::MsgBox("��������Ă��܂���");
	}
	//�L�����N�^���ʃ��\�b�h
	//�߂荞�܂Ȃ��ړ�����
	virtual  void  CheckMove(ML::Vec2&  est_);
	//�����ڐG����
	virtual  bool  CheckFoot();
	//����ڐG����
	virtual  bool  CheckHead();
	//���ʐڐG����i�T�C�h�r���[�Q�[����p�j
	virtual bool CheckFront_LR();
	//�w�ʐڐG����i�T�C�h�r���[�Q�[����p�j
	virtual bool CheckBack_LR();
	//���ʑ����`�F�b�N�i�T�C�h�r���[�Q�[����p�j
	virtual bool CheckFrontFoot_LR();
	//�ڐG����
	virtual bool CheckHit(const ML::Box2D& hit);
	//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
	bool  UpdateMotion(State  nm_);
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	virtual	void Received(BChara* from_, AttackInfo at_,const int&)
	{
		ML::MsgBox("Recieved ��������Ă��܂���");
	}

	//�A�N�Z�T���\�b�h

	//��Ԃ��w�肷��
	//����	�F	�iState�^�j
	void Set_State(const State&);
	//�͈͍U�����ۂ����w�肷��
	//����	�F	�i0��1�j
	void Set_Range_Wide(const int&);
	//�m�b�N�o�b�N���邩�ۂ����w�肷��
	//����	�F	�i0��1�j
	void Set_Tip(const int&);
	//��̂̏��Ńt���O���w�肷��
	//����	�F	�i0��1�j
	void Set_Flag_Erase(const int&);
	//�ӂ��Ƃїʂ��w�肷��
	//����	�F	�iML::Vec2�j
	void Set_Move_Back(const ML::Vec2&);
	//�͈͍U�����ۂ����擾����
	bool Get_Range_Wide();
	//�m�b�N�o�b�N���邩�ۂ����擾����
	bool Get_Tip();
	//��̂̏��Ńt���O���擾����
	bool Get_Flag_Erase();
	//�p�x���擾����
	float Get_Angle();
	//�ӂ��Ƃїʂ��擾����
	ML::Vec2 Get_Move_Back();
};