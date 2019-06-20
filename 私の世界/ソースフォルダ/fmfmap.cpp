#include "fmfmap.h"
#include "DxLib.h"
#include <stdio.h>
#include "main.h"

using namespace std;

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CFmfMap::CFmfMap(void) : m_pLayerAddr(NULL)
{}
//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
CFmfMap::~CFmfMap()
{
	Close();
}
void CFmfMap::Initialize() {
	HANDLE hFind;
	vector<string> mapGr;
	WIN32_FIND_DATA fd;

	//�ŏ��̃t�@�C���̌���
	hFind = FindFirstFile("data/map/*.png", &fd);

	//�������s
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("�}�b�v�摜���ǂݍ��߂܂���ł���\n");
		return;
	}
	do {
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		}
		else {
		mapGr.push_back(fd.cFileName);
		DrawFormatString(100,200,Black,"%s",mapGr.back().c_str());
		WaitKey();
		}
	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);
}

//-----------------------------------------------------------------------------
//	�}�b�v���J���ăf�[�^��ǂݍ���
// �����F	szFilePath	= �}�b�v�t�@�C���̃p�X
// �߂�l:	����I��	= TRUE
//			�G���[		= FALSE
//-----------------------------------------------------------------------------
BOOL CFmfMap::Open(const char *szFilePath, const char *grFilePath)
{
	Close();

	char buf[256] = "data/map/";
	
	strcat_s(buf, sizeof(buf), szFilePath);

	// �t�@�C�����J��
	HANDLE hFile = CreateFile(	buf, GENERIC_READ, FILE_SHARE_READ, NULL,
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;



	// �w�b�_����ǂ�
	DWORD dwReadBytes;
	if (!ReadFile(hFile, &m_fmfHeader, sizeof(FMFHEADER), &dwReadBytes, NULL) ||
		dwReadBytes != sizeof(FMFHEADER))
		goto error_return;

	// ���ʎq�̃`�F�b�N
	if (memcmp(&m_fmfHeader.dwIdentifier, "FMF_", 4) != 0)
		goto error_return;

	// �������m��
	m_pLayerAddr = new BYTE[m_fmfHeader.dwSize];
	if (m_pLayerAddr == NULL)
		goto error_return;

	// ���C���[�f�[�^��ǂ�
	if (!ReadFile(hFile, m_pLayerAddr, m_fmfHeader.dwSize, &dwReadBytes, NULL) ||
		dwReadBytes != m_fmfHeader.dwSize)
		goto error_return;
	
	// ����I��
	CloseHandle(hFile);

	strcpy_s(buf, "data/map/");
	strcat_s(buf, sizeof(buf), grFilePath);
	GetGraphSize(LoadGraph(buf), &GrsizeX, &GrsizeY);
	//LoadDivGraph(buf, (GrsizeX / m_fmfHeader.byChipHeight)*(GrsizeY / (int)m_fmfHeader.byChipHeight), GrsizeX / m_fmfHeader.byChipWidth, GrsizeY / m_fmfHeader.byChipHeight, m_fmfHeader.byChipWidth, m_fmfHeader.byChipHeight, mapGr);
	
	LoadDivGraph(buf, (GrsizeX / m_fmfHeader.byChipHeight)*(GrsizeY / (int)m_fmfHeader.byChipHeight), GrsizeX / m_fmfHeader.byChipWidth, GrsizeY / m_fmfHeader.byChipHeight, m_fmfHeader.byChipWidth, m_fmfHeader.byChipHeight, mapGr);
	int l = 0;
	
	for (int j = 0; j < GrsizeY / m_fmfHeader.byChipHeight; j++)
		for (int i = 0; i < GrsizeX / m_fmfHeader.byChipWidth; i++) {
			mapGrbuf[i][j] = mapGr[l];
			l++;
		}
	
	return TRUE;

error_return:
	// �G���[�I��
	CloseHandle(hFile);
	Close();
	return FALSE;
}
//-----------------------------------------------------------------------------
// �}�b�v���J����Ă��邩
//-----------------------------------------------------------------------------
BOOL CFmfMap::IsOpen() const
{
	return m_pLayerAddr != NULL;
}
//-----------------------------------------------------------------------------
//	�}�b�v���������J��
//-----------------------------------------------------------------------------
void CFmfMap::Close(void)
{
	if (m_pLayerAddr != NULL)
	{
		delete [] m_pLayerAddr;
		m_pLayerAddr = NULL;
	}
}
//-----------------------------------------------------------------------------
//	�w�背�C���̐擪�A�h���X�𓾂�
//	�����F	���C���ԍ�
//	�߂�l:	����I��	= ���C���f�[�^�̃A�h���X
//			�G���[		= NULL
//	�e���C���f�[�^�͘A�������������̈�ɔz�u����Ă�̂�
//	�w�背�C���f�[�^�̃A�h���X���v�Z�ŋ��߂�B
//-----------------------------------------------------------------------------
void* CFmfMap::GetLayerAddr(BYTE byLayerIndex) const
{
	// �������`�F�b�N�A�͈̓`�F�b�N
	if ((m_pLayerAddr == NULL) || (byLayerIndex >= m_fmfHeader.byLayerCount))
		return NULL;

	BYTE bySize = m_fmfHeader.byBitCount / 8;
	return m_pLayerAddr + m_fmfHeader.dwWidth * m_fmfHeader.dwHeight * bySize * byLayerIndex;
}
//-----------------------------------------------------------------------------
// ���C���ԍ��ƍ��W���w�肵�Ē��ڃf�[�^��Ⴄ
// �����F
// 	byLayerIndex	= ���C���ԍ�
// 	dwX				= X���W�i0�`m_fmfHeader.dwWidth - 1�j
// 	dwY				= Y���W�i0�`m_fmfHeader.dwHeight - 1�j
// �߂�l�F
// 	����I��	= ���W�̒l
//	�G���[		= -1
//-----------------------------------------------------------------------------
int CFmfMap::GetValue(BYTE byLayerIndex, DWORD dwX, DWORD dwY) const
{
	int nIndex = -1;

	// �͈̓`�F�b�N
	if (byLayerIndex >= m_fmfHeader.byLayerCount ||
		dwX >= m_fmfHeader.dwWidth ||
		dwY >= m_fmfHeader.dwHeight)
		return nIndex;

	if (m_fmfHeader.byBitCount == 8)
	{
		// 8bit layer
		BYTE* pLayer = (BYTE*)GetLayerAddr(byLayerIndex);
		nIndex = *(pLayer + dwY * m_fmfHeader.dwWidth + dwX);
	}
	else
	{
		// 16bit layer	
		WORD* pLayer = (WORD*)GetLayerAddr(byLayerIndex);
		nIndex = *(pLayer + dwY * m_fmfHeader.dwWidth + dwX);
	}

	return nIndex;
}

//-----------------------------------------------------------------------------
// ���C���ԍ��ƍ��W���w�肵�ăf�[�^���Z�b�g
//-----------------------------------------------------------------------------
void CFmfMap::SetValue(BYTE byLayerIndex, DWORD dwX, DWORD dwY, int nValue)
{
	// �͈̓`�F�b�N
	if (byLayerIndex >= m_fmfHeader.byLayerCount ||
		dwX >= m_fmfHeader.dwWidth ||
		dwY >= m_fmfHeader.dwHeight)
		return;

	if (m_fmfHeader.byBitCount == 8)
	{
		// 8bit layer
		BYTE* pLayer = (BYTE*)GetLayerAddr(byLayerIndex);
		*(pLayer + dwY * m_fmfHeader.dwWidth + dwX) = (BYTE)nValue;
	}
	else
	{
		// 16bit layer	
		WORD* pLayer = (WORD*)GetLayerAddr(byLayerIndex);
		*(pLayer + dwY * m_fmfHeader.dwWidth + dwX) = (WORD)nValue;
	}
}

//-----------------------------------------------------------------------------
// �}�b�v�̉����𓾂�
//-----------------------------------------------------------------------------
DWORD CFmfMap::GetMapWidth(void) const
{
	return m_fmfHeader.dwWidth;
}
//-----------------------------------------------------------------------------
// �}�b�v�̍����𓾂�
//-----------------------------------------------------------------------------
DWORD CFmfMap::GetMapHeight(void) const
{
	return m_fmfHeader.dwHeight;
}
//-----------------------------------------------------------------------------
// �`�b�v�̉����𓾂�
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetChipWidth(void) const
{
	return m_fmfHeader.byChipWidth;
}
//-----------------------------------------------------------------------------
// �`�b�v�̍����𓾂�
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetChipHeight(void) const
{
	return m_fmfHeader.byChipHeight;
}
//-----------------------------------------------------------------------------
// ���C���[���𓾂�
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetLayerCount(void) const
{
	return m_fmfHeader.byLayerCount;
}
//-----------------------------------------------------------------------------
// ���C���[�f�[�^�̃r�b�g�J�E���g�𓾂�
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetLayerBitCount(void) const
{
	return m_fmfHeader.byBitCount;
}
//-----------------------------------------------------------------------------
// �}�b�v��`�ʂ���
//-----------------------------------------------------------------------------
void CFmfMap::Draw(int px, int py) {

	//static int map = LoadGraph("data/map/maptip.png");

	BYTE* pLayer;
	// 0�ԁi��ԉ��̃��C���[�j�̃A�h���X��Ⴄ
	pLayer = (BYTE*)GetLayerAddr(0);
	if (pLayer == NULL)
		Close();

	DWORD width = GetMapWidth();
	DWORD height = GetMapHeight();
	//DWORD cWidth = GetChipWidth();
	//DWORD cHeight = GetChipHeight();
	//int srcX, srcY;
	BYTE index;
	int xx = -px;
	int yy = -py;

	// �}�b�v�̕`��
	for (BYTE ly = 0; ly < GetLayerCount() - 1; ly++) {
		for (DWORD y = 0; y < height; y++)
		{
			for (DWORD x = 0; x < width; x++)
			{
				//index = *(pLayer + y * width + x);
				// �܂���
				index = GetValue(ly, x, y);

				if (index != 255) {
					// index�ɂ̓}�b�v���W(x, y)�̃}�b�v�f�[�^�������Ă�̂�
					// �p�[�c�摜(srcHDC)����value�Ɍ�������`���Z�o���ĕ`�揈�����s���B
					// �}�b�v��8bit�̏ꍇ�p�[�c�̃A���C�������g��16�A16bit�Ȃ�256�B
					//srcX = (index % 256) *cWidth;
					//srcY = (index / 256) *cHeight;
					//DrawRectGraph(x *cWidth, y *cHeight, srcX, srcY, cWidth, cHeight, mapGr, TRUE, FALSE);
					//DrawRectGraph(x *cWidth, y *cHeight, GetValue(ly, x, y) % 256 * cWidth, GetValue(ly, x, y) / 256 * cHeight, cWidth, cHeight, map, TRUE, FALSE);

					//DrawExtendGraph(xx + x * 32, yy + y * 32, xx + x * 32 + 32, yy + y * 32 + 32, mapGr[GetValue(ly, x, y) % 256 + GetValue(ly, x, y) / 256 * GrsizeY / 16], TRUE);
					DrawExtendGraph(xx + x * 32, yy + y * 32, xx + x * 32 + 32, yy + y * 32 + 32, mapGrbuf[GetValue(ly, x, y) % 256][GetValue(ly, x, y) / 256], TRUE);
				}
			}
		}
	}
}

void CFmfMap::Preview() {

	//static int map = LoadGraph("data/map/maptip.png");

	BYTE* pLayer;
	// 0�ԁi��ԉ��̃��C���[�j�̃A�h���X��Ⴄ
	pLayer = (BYTE*)GetLayerAddr(0);
	if (pLayer == NULL)
		Close();
	
	DWORD width = GetMapWidth();
	DWORD height = GetMapHeight();
	//DWORD cWidth = GetChipWidth();
	//DWORD cHeight = GetChipHeight();
	//int srcX, srcY;
	BYTE index;
	int xx = 200;
	int yy = 30;

	DrawBox(xx - 5, yy - 5, 620 + 5, 400 + 5,Black,TRUE);
	
	SetDrawArea(xx, yy, 620, 400);
	
	// �}�b�v�̕`��
	for (BYTE ly = 0; ly < GetLayerCount() - 1; ly++) {
		for (DWORD y = 0; y < height; y++)
		{
			for (DWORD x = 0; x < width; x++)
			{
				//index = *(pLayer + y * width + x);
				// �܂���
				index = GetValue(ly, x, y);


				if (index != 255) {
					// index�ɂ̓}�b�v���W(x, y)�̃}�b�v�f�[�^�������Ă�̂�
					// �p�[�c�摜(srcHDC)����value�Ɍ�������`���Z�o���ĕ`�揈�����s���B
					// �}�b�v��8bit�̏ꍇ�p�[�c�̃A���C�������g��16�A16bit�Ȃ�256�B
					//srcX = (index % 256) *cWidth;
					//srcY = (index / 256) *cHeight;
					//DrawRectGraph(x *cWidth, y *cHeight, srcX, srcY, cWidth, cHeight, mapGr, TRUE, FALSE);
					//DrawRectGraph(x *cWidth, y *cHeight, GetValue(ly, x, y) % 256 * cWidth, GetValue(ly, x, y) / 256 * cHeight, cWidth, cHeight, map, TRUE, FALSE);
					
					//DrawExtendGraph(xx + x * 32, yy + y * 32, xx + x * 32 + 32, yy + y * 32 + 32, mapGr[GetValue(ly, x, y) % 256 + GetValue(ly, x, y) / 256 * GrsizeY / 16], TRUE);
					DrawExtendGraph(xx + x * 32, yy + y * 32, xx + x * 32 + 32, yy + y * 32 + 32, mapGrbuf[GetValue(ly, x, y) % 256][GetValue(ly, x, y) / 256], TRUE);
				}
			}
		}
	}
	SetDrawAreaFull();
}