#include "DG2014_Ver3_51.h"
#pragma warning(disable:4316)

//OBJ�t�@�C���͕ϊ����s���A.SOB�`���ɕϊ������t�@�C����ǂݍ��ނ��̂ƂȂ���
namespace MeshLoad_SOB
{
	struct FileHeader
	{
		char  kind[32];
		bool Check( )
		{
			string  k =  kind;
			string  v =  "3DMesh YS Ver0.2";
			return  k == v;
		}
	};
	//�}�e���A���R���e�i
	class MaterialContainerS{
	public:
		char					name[64];	//�}�e���A����
		ML::Color				diffuse;	//�}�e���A�����:�f�t���[�Y
		ML::Color				ambient;	//�}�e���A�����:�A���r�G���g
		ML::Color				speculer;	//�}�e���A�����:�X�y�L����
		ML::Color				emissive;	//�}�e���A�����:�G�~�b�V�u
		char					texName[64];//�}�e���A����
		char					bw[320];//���g�p�G���A�i���v512�o�C�g�j
	};
	class MeshGroupS{
	public:
		DWORD					mcNumber;	//�Ή�����}�e���A���ԍ�
		DWORD					indexNum;	//�C���f�b�N�X��
		vector<DWORD>			vIndex;		//�C���f�b�N�X
	};
}

namespace DG_
{
	vector<MeshLoad_SOB::MaterialContainerS>	mateConts;
	vector<MeshLoad_SOB::MeshGroupS>			meshGroups;

	//SOB�t�@�C�����烁�b�V���𐶐�����
	Mesh::SP  Mesh::Create_FromSOBFileSub(	const string&			fpath_)
	{
		if( Mesh::SP sp = Mesh::SP( new Mesh( ) ) )
		{
			if( sp->Initialize_FromSOBFile(fpath_) ){
				return sp;
			}
		}
		return nullptr;
	}
	//SOB�t�@�C�����烁�b�V���𐶐�����
	bool Mesh::Initialize_FromSOBFile(	const string&		fpath_)
	{
		//�t�@�C�����J���ē��e��ǂݍ���
		FILE* fp=NULL;
		fopen_s(&fp, fpath_.c_str( ),"rb");
		if(fp == nullptr){
			MessageBox(nullptr, "�t�@�C�����J���܂���B", fpath_.c_str( ), MB_OK);
			return false;
		}
		//�t�@�C���`�F�b�N
		MeshLoad_SOB::FileHeader  wfh;
		fread(&wfh, sizeof(MeshLoad_SOB::FileHeader), 1, fp);
		if(!wfh.Check( )){	
			MessageBox(nullptr, "�t�@�C���o�[�W��������v���܂���B�Ή��o�[�W������SOB Ver0.2�ł�", fpath_.c_str( ), MB_OK);
			return false;
		}


		//�}�e���A�����ǂݍ���
		DWORD  mateNum;
		fread(&mateNum, sizeof(DWORD), 1, fp);
		vector<MeshLoad_SOB::MaterialContainerS>  mateContS(mateNum);
		fread(&mateContS[0], sizeof(MeshLoad_SOB::MaterialContainerS), mateContS.size( ), fp);

		//���b�V���O���[�v�ǂݍ���
		DWORD  mesnGNum;
		fread(&mesnGNum, sizeof(DWORD), 1, fp);
		vector<MeshLoad_SOB::MeshGroupS>  meshG(mesnGNum);
		for(DWORD  c=0; c<mesnGNum; c++){
			DWORD  ww;
			fread(&ww, sizeof(DWORD), 1, fp);
			meshG[c].mcNumber = ww;
			fread(&ww, sizeof(DWORD), 1, fp);
			meshG[c].indexNum = ww;
			//fread(&meshG[c].mcNumber, sizeof(DWORD), 1, fp);
			//fread(&meshG[c].indexNum, sizeof(DWORD), 1, fp);
			meshG[c].vIndex.resize( meshG[c].indexNum );
			fread(&meshG[c].vIndex[0], sizeof(DWORD), meshG[c].indexNum, fp);
		}

		//���_���֘A�̓ǂݏo��
		DWORD  vCoordNum;
		vector<ML::Vec3>  vCoord;
		DWORD  vUVNum;
		vector<ML::Vec2>  vUV;
		DWORD  vNormalNum;
		vector<ML::Vec3>  vNormal;

		while(1)
		{
			char  dataTitle[16];
			if(16 !=  fread(dataTitle, sizeof(char), 16, fp))
			{
				break;
			}
			if(		0 == strcmp(dataTitle, "POSITION") ){
				fread(&vCoordNum, sizeof(DWORD), 1, fp);
				vCoord.resize(vCoordNum);
				fread(&vCoord[0], sizeof(ML::Vec3), vCoord.size( ), fp);
			}
			else if(0 == strcmp(dataTitle, "TEX_UV") ){
				fread(&vUVNum, sizeof(DWORD), 1, fp);
				vUV.resize(vUVNum);
				fread(&vUV[0], sizeof(ML::Vec2), vUV.size( ), fp);
			}
			else if(0 == strcmp(dataTitle, "NORMAL") ){
				fread(&vNormalNum, sizeof(DWORD), 1, fp);
				vNormal.resize(vNormalNum);
				fread(&vNormal[0], sizeof(ML::Vec3), vNormal.size( ), fp);
			}
		}
		//�t�@�C������K�v�ȃf�[�^�͂��ׂēǂݍ��񂾂�
		fclose(fp);	

	//�R���e�i�ɕ��荞��
		this->vbc = DG_::VB_Container::Create(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		bool rtv;
		rtv = vbc->PushBack((BYTE*)&vCoord[0], sizeof(D3DXVECTOR3), vCoord.size( ));
		if(!rtv){ return false; }
		rtv = vbc->PushBack((BYTE*)&vUV[0], sizeof(D3DXVECTOR2), vUV.size( ));
		if(!rtv){ return false; }
		rtv = vbc->PushBack((BYTE*)&vNormal[0], sizeof(D3DXVECTOR3), vNormal.size( ));
		if(!rtv){ return false; }

	//���b�V���O���[�v�i�C���f�b�N�X�ƃ}�e���A��ID�ō\������O���[�v�j
		for(auto  it = meshG.begin( );  it != meshG.end( );  it++){
			MeshGroup  wMeshGroup;
			wMeshGroup.mcNumber = it->mcNumber;
			wMeshGroup.index = DG_::IndexBuffer::Create((BYTE*)&it->vIndex[0], sizeof(DWORD), it->vIndex.size( ));
			//���b�V���O���[�v�ɐݒ�
			meshGroups.push_back(wMeshGroup);
		}
	//�}�e���A���̐���
		for(auto  it = mateContS.begin( );  it != mateContS.end( );  it++){
			Material  wMateCon;
			wMateCon.ambient  = it->ambient;
			wMateCon.diffuse  = it->diffuse;
			wMateCon.speculer = it->speculer;
			wMateCon.emissive = it->emissive;
			wMateCon.name     = it->name;
			if(it->texName[0] != '\0'){
				wMateCon.tex  = DG_::Texture::Create(ML::CreateFilePath(fpath_, it->texName));
			}
			mateConts.push_back(wMateCon);
		}

	//===========================================
		//�o�[�e�b�N�X�V�F�[�_���������_�C���v�b�g���C�A�E�g����
		//���_�C���v�b�g���C�A�E�g���`	
		D3D10_INPUT_ELEMENT_DESC layout[] =	{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		1, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	2, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
		};
		//���_�C���v�b�g���C�A�E�g�̐���
		this->inputLayout  = DG_::InputLayout::Create(0, layout, _countof(layout));
		if(!this->inputLayout ){return false;}

		return true;
	}
}
