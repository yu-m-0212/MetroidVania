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
public:
	//�ύX������������������������������������������������������
	//�L�����N�^�̍s����ԃt���O
	enum State
	{
		Unnon = -1,		//	����(�g���܂���j
		Stand,			//	��~
		Dash,			//	�_�b�V��
		SlowDown,		//	����
		Punch1,			//	�p���`1
		Punch2,			//	�p���`2
		Air,			//	�󒆍U��
		Stomp,			//	�X�g���v
		StompLanding,	//	�X�g���v���n
		Shoot,			//	�ˌ�(�n��)
		Airshoot,		//	�ˌ�(��)
		Bunker1,		//	�o���J�[�\��
		Bunker2,		//	�o���J�[�ˏo
		Bunker3,		//	�o���J�[�r�߂�
		Jump,Jump2,		//	�W�����v
		Fall,Fall2,		//	����
		Landing,		//	���n
		Damage,			//	��e
		Walk,			//	���s
		Turn,			//	����
		TakeOff,		//	��ї��u��
		Bound,			//	�_���[�W���󂯂Đ������ł���
		Lose,			//	������E���V
	};
	//���E�̌����i2D�����_�Q�[����p�j
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	WP			target;
	//�L�����N�^���ʃ����o�ϐ�
	State		state;			//���݂̍s���������t���O
	ML::Vec2    pos;			//�L�����N�^�ʒu
	ML::Vec2	moveVec;		//�ړ��x�N�g��
	ML::Box2D   hitBase;		//�����蔻��͈�
	int			moveCnt;		//�s���J�E���^
	int			unHitTime;		//���G���ԃJ�E���^
	int			animCnt;		//�A�j���[�V�����J�E���^
	int			hp;				//�w���X�|�C���g
	float		jumpPow;		//�W�����v����
	float		maxFallSpeed;	//�����ő呬�x
	float		gravity;		//�t���[���P�ʂ̉��Z��
	float		maxSpeed;		//���E�����ւ̈ړ��̉��Z��
	float		addSpeed;		//���E�����ւ̈ړ��̉��Z��
	float		decSpeed;		//���E�����ւ̈ړ��̌�����
	float		reach;			//�p���`�̎˒�
	float		slide;			//�p���`���ɑO�i����
	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BChara()
		:state(Stand)
		, angle_LR(Right)
		, pos(0, 0)
		, moveVec(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveCnt(0)
		, unHitTime(0)
		,animCnt(0)
		, hp(1)
		, jumpPow(0)
		, maxFallSpeed(0)
		, gravity(0)
		, maxSpeed(0)
		, addSpeed(0)
		, decSpeed(0)
		, reach(0)
		, slide(0)
	{
	}
	virtual  ~BChara(){}
	//�L�����N�^���ʃ��\�b�h
	//�߂荞�܂Ȃ��ړ�����
	virtual  void  CheckMove(ML::Vec2&  est_);
	//�����ڐG����
	virtual  bool  CheckFoot();
	//����ڐG����
	virtual  bool  CheckHead();
	//���ʐڐG����i�T�C�h�r���[�Q�[����p�j
	virtual bool CheckFront_LR();
	//���ʑ����`�F�b�N�i�T�C�h�r���[�Q�[����p�j
	virtual bool CheckFrontFoot_LR();
	//�ڐG����
	virtual bool CheckHit(const ML::Box2D& hit);
	//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
	bool  UpdateMotion(State  nm_);

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
	//�ڐG���̉��������i���ꎩ�̂̓_�~�[�̂悤�ȃ��m�j
	virtual	void Received(BChara* from_, AttackInfo at_)
	{
		ML::MsgBox("Recieved ��������Ă��܂���");
	}
};