//--------------------------------------------------------------------------------------
//Light
struct   sLightParam
{
	bool		enable;
	int			kind;			//0�f�B���N�V���i���@1�|�C���g	2�X�|�b�g
	float		attenuation;	//�����i�ǉ��I�j
	float		range;			//�L�������i�ǉ��I�j
	float4		direction;		//���C�g�Ǝ˕���
	float4		directionInv;	//���C�g�Ǝ˕���(�t�x�N�g���j
	float4		pos;			//�����̈ʒu�i�ǉ��I�j
	float4		color;			//���C�g�̐F������
};
struct   sLightsParam
{
	bool		enable;
	bool		n0;			//�T�C�Y�����p
	bool		n1;			//�T�C�Y�����p
	bool		n2;			//�T�C�Y�����p
	float4		ambient;
	sLightParam	light[4];
};
cbuffer  LightsCB{
	sLightsParam	 lights;
};
//--------------------------------------------------------------------------------------
//Fog
struct  sFogParam
{
	bool	enable;
	bool	mode;
	bool	n0;
	bool	n1;
	float	near;
	float	fore;
	float	p0;
	float	p1;
	float4	color;
};
cbuffer  FogCB
{
	sFogParam	fog;
};
//--------------------------------------------------------------------------------------
//Matrix
struct  sMatrixParam
{
	matrix		world;
	matrix		view;
	matrix		projection;

	matrix		w_v;
	matrix		w_v_p;
	matrix		v_p;
};
cbuffer  MatrixCB
{
	sMatrixParam		mat;
};
//--------------------------------------------------------------------------------------
//Material
struct  sMaterialParam
{
	float4		diffuse;
	float4		speculer;
	float4		ambient;
	float4		emissive;
	float4		objectColor;	//�����F�ύX
};
cbuffer  MaterialCB
{
	sMaterialParam	mate;
};
//--------------------------------------------------------------------------------------
//������
struct  sEtcParam
{
	float4		eyePos;				//���_���W
};
cbuffer  EtcCB
{
	sEtcParam	etc;
};
//--------------------------------------------------------------------------------------
//�e�N�X�`��
Texture2D	tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7;

//--------------------------------------------------------------------------------------
//�T���v��
//���`��ԁ@UV�~���[�����O
SamplerState  samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;	//Wrap;
    AddressV = MIRROR;	//Wrap;
};
//������������������������������������������������������������������
//�V�F�[�_�\0
//------------------------------------------------------------------
//���ʔ��˂��������̋t�x�N�g�������߂�
float4  SpecularReflectInv(float4  toLight_,  float4  n_)//���ʔ���
{
	float  d =  dot(n_, toLight_);
	return  toLight_ + (n_ * (-d * 2));
}
//------------------------------------------------------------------
//���C�e�B���O
float4  Lighting(float4  pos_, float4  normal_,  sLightParam  lp_)
{
	float4  color  =  (float4)0;
	//�f�B���N�V���i���i���s�����j
	if(lp_.kind  ==  0){
		//�g�U���˂ɑ΂�����ʌv�Z
		{	//���ˊp�ɂ����ʂ����߂�
			float  angleL  =  max(0, dot(normal_,  lp_.directionInv));
			//���ˌ������߂�
			color  +=  lp_.color  *  mate.diffuse  *  angleL;
		}
		//���ʔ��˂ɑ΂�����ʌv�Z(����m�F�ׂ̈̉��ϔŁj
		{	//�����x�N�g�������߂�
			float4  eyeVec  =  normalize(pos_  -  etc.eyePos);
			//���ˌ��̋t�x�N�g�������߂�
			float4	refInv  =  SpecularReflectInv(lp_.directionInv,  normal_);
			//���ˌ��Ǝ����ɂ����ʂ����߂�
			float  angleL  =  max(0,  dot(eyeVec,  refInv));
			//���ˌ������߂�
			color  +=  lp_.color  *  mate.speculer  *  angleL;
		}
	//�|�C���g
	}else  if(lp_.kind  ==  1){
		//���ˊp�Ƌt�����̃x�N�g���i�����Ɍ������x�N�g���j�����߂�
		float4  toLight  =  lp_.pos  -  pos_;
		//�����܂ł̋��������߂�
		float  len  =  length(toLight);
		//�L���������������C�e�B���O�������s��
		if(lp_.range  >=  len){
			//�����ɂ����̗ʂ����߂�i�����̂Q��Ō����キ�Ȃ�j
			float  distL  =  1 / max(1, len  *  len  *  lp_.attenuation);
			//�����Ɍ������P�ʃx�N�g�������߂�
			float4  toLightNormal  =  toLight  /  len;
			//�g�U���˂ɑ΂�����ʌv�Z
			{	//�p�x�ɂ�锽�ˌ��̗ʂ����߂�
				//float  angleL  =  1.0f;		//�i�������j
				float  angleL  =  max(0, dot(normal_,  toLightNormal));
				//�g�U���˂ɑ΂�����ʌv�Z
				color += lp_.color  *  mate.diffuse  *  angleL  *  distL;
			}
			//���ʔ��˂ɑ΂�����ʌv�Z
			{	//�����x�N�g�������߂�
				float4  eyeVec  =  normalize(pos_  -  etc.eyePos);
				//���ˌ��̋t�x�N�g�������߂�
				float4	refInv  =  SpecularReflectInv(toLightNormal,  normal_);
				//���ˌ��Ǝ����ɂ����ʂ����߂�
				float  angleL  =  max(0,  dot(eyeVec,  refInv));
				//���ˌ������߂�
				color  +=  lp_.color  *  mate.speculer  *  angleL  *  distL;
			}
		}
	}else{
	}
	return  color;
}
//------------------------------------------------------------------
//�o�[�e�b�N�X�V�F�[�_�o�͍\���́@�@���V�F�[�_�ł͂Ȃ��o�b�t�@�ɂȂ��Ă���̂͋L�q�~�X
struct sVS_PNT_Out
{	
    float4  pos			: SV_POSITION;
	float2  uv			: TEXCOORD0;
	float	viewZ		: POSITION0;
	float4  posW		: POSITION1;
	float4  norW		: NORMAL;
	//float4  color		: COLOR;
};
//------------------------------------------------------------------
//
//�o�[�e�b�N�X�V�F�[�_�[
//
//���_�@�@���@UV�̍\���̒��_�ɑ΂��鏈��
//
sVS_PNT_Out  VS_PNT(	float3  pos_		: POSITION,
						float3  normal_		: NORMAL,
						float2  uv_			: TEXCOORD)
{
    sVS_PNT_Out output = (sVS_PNT_Out)0;

//�@�����I�ɂ͖��ʂ������ꂪ�ǂ���`�ŋL�q���Ă���

	//���_�Ɩ@�������[���h�ϊ�
	float4  posW  =  mul( float4(pos_.xyz,1), mat.world );
	float4  norW  =  mul( float4(normal_.xyz,0), mat.world );
	norW = normalize(norW);	//�@���͐��K��

	////���C�e�B���O����(�������j
	//float4  color  =  float4(0,0,0,0);
	//if(lights.enable){
	//	for(int  i = 0;  i < 4;  i++){
	//		if(lights.light[i].enable){
	//			//�g�U���ˁE���ʔ���
	//			color += Lighting(posW,  norW,  lights.light[i]);
	//		}
	//	}
	//	//�������˗ʂ�K�p
	//	color += mate.ambient  *  lights.ambient;
	//	//�����x�̓}�e���A���̃f�t���[�Y���瓾��
	//	color.a  =  mate.diffuse.a;
	//}
	//else{
	//	//���C�g������
	//	color += mate.diffuse;
	//}
	////���Ȕ����ʂ�K�p
	//color  +=  mate.emissive;
	////�I�u�W�F�N�g�J���[�i�����F�ύX�j��K�p
	//color  *=  mate.objectColor;

	//���_���r���[�ϊ�
	float4  posV  =  mul( posW, mat.view );
	//���_���v���W�F�N�V�����ϊ�
	float4  posP  =  mul( posV, mat.projection );

	//�o�[�e�b�N�X�o�b�t�@�̏o�͏���ݒ�
    output.pos    = posP;
    output.uv     = uv_;
	output.viewZ  = posV.z;
	output.posW = posW;
	output.norW = norW;
	//output.color  = color;
    return output;
}
//------------------------------------------------------------------
////�t�H�O
float4  Fog(float  viewZ_,  float4  color_)
{	
	//near�ȉ���0�Ƃ���
	float  d  =  max(viewZ_ - fog.near,  0);
	//near�`fore�̊ԂŔZ�x�𒲐�����
	float  m  =  min(  d / (fog.fore - fog.near), 1);

	float4  rtv;
	if(fog.mode){
		rtv  =  fog.color  *  m  +  color_  *  (1 - m);
	}
	else{
		rtv  =  float4(color_.r, color_.g, color_.b, color_.a * (1 - m));
	}
	return  rtv;
}
//------------------------------------------------------------------
//
//�s�N�Z���V�F�[�_�[
//
//���_�@�@���@UV�̍\���̒��_�ɑ΂��鏈��
//
float4 PS_PNT( sVS_PNT_Out  ip_ ) : SV_Target
{
	//���C�e�B���O����
	float4  color = float4(0, 0, 0, 0);
	if (lights.enable){
		for (int i = 0; i < 4; i++){
			if (lights.light[i].enable){
				//�g�U���ˁE���ʔ���
				color += Lighting(ip_.posW, ip_.norW, lights.light[i]);
			}
		}
		//�������˗ʂ�K�p
		color += mate.ambient  *  lights.ambient;
		//�����x�̓}�e���A���̃f�t���[�Y���瓾��
		color.a = mate.diffuse.a;
	}
	else{
		//���C�g������
		color += mate.diffuse;
	}
	//���Ȕ����ʂ�K�p
	color.xyz += mate.emissive.xyz;
	//�I�u�W�F�N�g�J���[�i�����F�ύX�j��K�p
	color *= mate.objectColor;

	//�e�N�X�`������s�N�Z�����擾
	color *= tex0.Sample( samLinear, ip_.uv);
	//�t�H�O�𔽉f������
	if(fog.enable){
		color  =  Fog(ip_.viewZ,  color);
	}
	return  color;
}
//������������������������������������������������������������������
//������������������������������������������������������������������
//�V�F�[�_�\1
//------------------------------------------------------------------
//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct sVS_PCT_Out
{	
    float4  pos			: SV_POSITION;
	float4	color		: COLOR;
	float2  uv			: TEXCOORD0;
	float	viewZ		: POSITION;
};
//------------------------------------------------------------------
//
//�o�[�e�b�N�X�V�F�[�_�[
//
//���_�@�J���[�@UV�̍\���̒��_�ɑ΂��鏈��
sVS_PCT_Out VS_PCT(		float3  pos_		: POSITION,
						float4  col_		: COLOR,
						float2  uv_			: TEXCOORD)
{
	sVS_PCT_Out output = (sVS_PCT_Out)0;
	output.pos = mul( float4(pos_.xyz,1), mat.w_v_p );
	output.color = mate.objectColor * col_;
	output.uv  = uv_;
	output.viewZ  = 0;
	return output;
}


//------------------------------------------------------------------
//
//�s�N�Z���V�F�[�_�[
//
//���_�@�F�@UV�̍\���̒��_�ɑ΂��鏈��
//
float4 PS_PCT( sVS_PCT_Out  ip_ ) : SV_Target
{
	float4  col = ip_.color;
	col *= tex0.Sample( samLinear, ip_.uv);
	return  col;
}
//������������������������������������������������������������������

//������������������������������������������������������������������
//�V�F�[�_�\2
//���C�`��p
//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct sVS_Ray_Out
{
	float4  pos			: SV_POSITION;
};
//------------------------------------------------------------------
//
//�o�[�e�b�N�X�V�F�[�_�[
//
//���_���W�ō\�����ꂽ���_�ɑ΂��鏈��
//
sVS_Ray_Out VS_Ray(float3  pos_		: POSITION)
{
	sVS_Ray_Out output = (sVS_Ray_Out)0;
	float4  posW = mul(float4(pos_.xyz, 1), mat.world);
	float4  posV = mul(posW, mat.view);
	float4  posP = mul(posV, mat.projection);

	output.pos = posP;
	return output;
}
//------------------------------------------------------------------
//
//�s�N�Z���V�F�[�_�[
//
//���_���W�ō\�����ꂽ�|���S���̃s�N�Z���ɑ΂��鏈��
//
float4 PS_Ray(sVS_Ray_Out  ip_) : SV_Target
{
	return  mate.objectColor;
}
//������������������������������������������������������������������
//������������������������������������������������������������������
//�e�N�j�b�N(�V�F�[�_�͂�������ɏ����Ȃ��Ɠ����Ȃ���)
//
technique10 MyStdShaderTec
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS_PNT() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS_PNT() ) );
	}
	pass P1
	{
		SetVertexShader( CompileShader( vs_4_0, VS_PCT() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS_PCT() ) );
	}
	pass P2
	{
		SetVertexShader(CompileShader(vs_4_0, VS_Ray()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Ray()));
	}

}

