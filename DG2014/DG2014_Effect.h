#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )

#include "../myLib.h"
#include <string>

namespace DG_
{
	//-----------------------------------------------------------------------
	struct  Light{
		BOOL		enable;
		enum  Kind{Directional,  Point,  Spot, Unnon = 0xFFFFFFFF};
		Kind		kind;		//0�f�B���N�V���i���@1�|�C���g	2�X�|�b�g
		ML::Vec3	direction;	//���C�g�Ǝ˕����i�f�B���N�V���i���E�X�|�b�g�j
		ML::Color	color;		//���C�g�̐F������
		ML::Vec3	pos;		//���C�g�̈ʒu�i�ǉ��I�j
		float		attenuation;//�������i�ǉ��I�j
		float		range;		//�L�������i�ǉ��I�j
		bool  operator==(const  Light&  obj){
			return	   (enable	  == obj.enable &&
						kind	  == obj.kind &&
						direction == obj.direction &&
						pos       == obj.pos &&				//�i�ǉ��I�j
						attenuation  == obj.attenuation &&	//�i�ǉ��I�j
						range  == obj.range &&				//�i�ǉ��I�j
						color	  == obj.color);
		}
		bool  operator!=(const  Light&  obj){
			return !((*this) == obj);
		}

	};
	class EffectParam
	{
	public:
		EffectParam( );
		//�s��
		ML::Mat4x4	matWorld;
		ML::Mat4x4	matView;
		ML::Mat4x4	matProjection;
		ML::Vec3	eyePos;		//���_���W(���C�e�B���O�ȂǂŎg�p�j
		//�}�e���A��
		ML::Color	mateDiffuse;	//�}�e���A�����:�f�t���[�Y
		ML::Color	mateSpeculer;	//�}�e���A�����:�X�y�L����
		ML::Color	mateAmbient;	//�}�e���A�����:�A���r�G���g
		ML::Color	mateEmissive;	//�}�e���A�����:�G�~�b�V�u
		ML::Color	objectColor;	//���f���̐F�������I�ɕω�������
		//�t�H�O
		bool		fogEnable;	//�t�H�O�̗L���E����
		bool		fogMode;	//�ʏ�t�H�Oor����
		float		fogNear;	//�t�H�O�J�n�n�_
		float		fogFore;	//�t�H�O�ő剻�n�_
		ML::Color	fogColor;	//�t�H�O�̐F
		//���C�g
		bool		lightsEnable;	//���C�g�̗L�������i�����j
		ML::Color	lightAmbient;	//����
		Light		light[4];
		//�����ȏ��
		ML::Color	bgColor;		//�w�i�F
		//�R���X�^���g�o�b�t�@�ȊO		
		ID3D10BlendState*			bs_Active;		//����s�v
		ID3D10RasterizerState*		rs_Active;		//����s�v
		ID3D10DepthStencilState*	dss_Active;		//����s�v
	};
	class EffectState
	{
		ID3D10BlendState*	bs_Std;		//�A���t�@�Ȃ��ʏ�`��
		ID3D10BlendState*	bs_Alpha;	//�A���t�@����
		ID3D10BlendState*	bs_Add;		//���Z����
		ID3D10BlendState*	bs_Dec;		//���Z����

		ID3D10RasterizerState*  rs_Def3D;		//���X�^���C�U�@�R�c�f�t�H���g
		ID3D10RasterizerState*  rs_Def2D;		//���X�^���C�U�@�Q�c�f�t�H���g

		ID3D10DepthStencilState*  dss_Def3D;			//�[�x�E�X�e���V���@�R�c�f�t�H���g
		ID3D10DepthStencilState*  dss_Def3D_Dis;		//�[�x�E�X�e���V���@�R�c�f�t�H���g�i�[�x�����j
		ID3D10DepthStencilState*  dss_Def3D_WriteDis;	//�[�x�E�X�e���V���@�R�c�f�t�H���g�i�[�x�������ݖ����j
		ID3D10DepthStencilState*  dss_Def2D;			//�[�x�E�X�e���V���@�Q�c�f�t�H���g

		EffectParam		prevParam;
	public:
		EffectParam		param;
//		void  Set(EffectParam&  p_);	//�p�����[�^��S�čĐݒ肷��
		EffectState( );
		~EffectState( );
		//Light


		//�r���[�|�[�g
		void  ViewPort(const  D3D10_VIEWPORT&  vp_);//�r���[�|�[�g�X�V
		//�u�����h�X�e�[�g
		void  BS_Set(ID3D10BlendState*  bs_);
		ID3D10BlendState*  BS_Get( );
		void  BS_Std( );		//�A���t�@�u�����h����
		void  BS_Alpha( );		//�A���t�@�u�����h�L��
		void  BS_AlphaAdd( );	//�A���t�@�u�����h���Z����
		void  BS_AlphaDec( );	//�A���t�@�u�����h���Z����
		//���X�^���C�U
		void  RS_Set(ID3D10RasterizerState*  rs_);
		ID3D10RasterizerState*  RS_Get( );
		void  RS_Def3D( );	//���X�^���C�U�R�c�f�t�H���g
		void  RS_Def2D( );	//���X�^���C�U�Q�c�f�t�H���g
		//�[�x�E�X�e���V��
		void  DSS_Set(ID3D10DepthStencilState*  dss_);
		ID3D10DepthStencilState*  DSS_Get( );
		void  DSS_Def3D( );	//�[�x�E�X�e���V���@�R�c�f�t�H���g
		void  DSS_Def3D_Dis( );	//�[�x�E�X�e���V���@�R�c�f�t�H���g(�[�x�����j
		void  DSS_Def3D_WriteDis( );	//�[�x�E�X�e���V���@�R�c�f�t�H���g(�[�x�������ݖ����j
		void  DSS_Def2D( );	//�[�x�E�X�e���V���@�Q�c�f�t�H���g

		//�����_�����O����O�ɌĂяo��param�Ɋ܂܂��ݒ�������_�����O�ɔ��f������
		void  Update_Params( );
	private:
		void  Update_Fog( );
		void  Update_Lights( );
		void  Update_Matrixs( );
		void  Update_Material( );
		void  Update_Etc( );
	};
}