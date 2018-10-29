#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include "BChara.h"
#include "Effect.h"

using namespace ML;

namespace  Task_Effect
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�t�F�N�g");	//�O���[�v��
	const  string  defName("NoName");			//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		string effectImage;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		int cntLimit;			//���ł܂ł̎���
		int num_bubble;			//�A�̑傫�����w�肷��
		int interval_bubble;	//�A�̗h�����
		float speed_surfacing;	//�A�̕��㑬�x
		float wide_bubble;		//�A�̗h�ꕝ
		float dist;				//���j�G�t�F�N�g�̒��S����̋���
		float angle;			//�\������p�x
		Vec2 center;			//��]��
	public:
		Object();
		//�A�G�t�F�N�g�𐶐�����
		//����	�F	�i�ԍ�,�������W,�h�����,�h�ꕝ,���㑬�x,�p�x,���ł܂ł̎��ԁj
		void Create_Bubble(const int&, const ML::Vec2&, const int&, const float&, const float&, const float&, const int&);
		//��Ԃ��Ƃɍs�����w�肷��
		void Move();
		//�A�j���[�V����
		BChara::DrawInfo Anim();

		//�A�N�Z�T���\�b�h

		//�Ăяo���ۂɏ��ł܂ł̎��Ԃ��w�肷��
		//����	�F	�i���ł܂ł̎��ԁj
		inline void Set_Limit(const int&);
		//�A�̗h��������w�肷��
		//����	�F	�i�h������j
		void Set_Interval_Bubble(const int&);
		//�A�̕��㑬�x���w�肷��
		//����	�F	�i���㑬�x�j
		void Set_Speed_Surfacing(const float&);
		//�A�̑傫�����w�肷��
		//����	�F	�i0~3)
		void Set_Num_Bubble(const int&);
		//�A�̗h�ꕝ���w�肷��
		//����	�F	�i�h�ꕝ�j
		void Set_Wide_Bubble(const float&);
		//���S�_����L����G�t�F�N�g���Ăяo���ہA���S����̏����ʒu���w�肷��
		//����	�F	�i���S�_����̏����ʒu)
		void Set_Dist(const float&);
		//�\������p�x���w�肷��
		//����	�F	 (ML::ToRadian(�p�x))
		void Set_Angle(const float&);
		//��]�̒��S���O������w�肷��
		//����	�F	�i���S���W�j
		void Set_Center(const Vec2&);
		//���S�_����L����G�t�F�N�g�̒��S����̋������擾����
		float Get_Dist();

		//�G�t�F�N�g�N���X
		Effect eff;
	};
}