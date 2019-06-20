#include "fmfmap.h"
#include "DxLib.h"
#include <stdio.h>
#include "main.h"

using namespace std;

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
CFmfMap::CFmfMap(void) : m_pLayerAddr(NULL)
{}
//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
CFmfMap::~CFmfMap()
{
	Close();
}
void CFmfMap::Initialize() {
	HANDLE hFind;
	vector<string> mapGr;
	WIN32_FIND_DATA fd;

	//最初のファイルの検索
	hFind = FindFirstFile("data/map/*.png", &fd);

	//検索失敗
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("マップ画像が読み込めませんでした\n");
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
//	マップを開いてデータを読み込む
// 引数：	szFilePath	= マップファイルのパス
// 戻り値:	正常終了	= TRUE
//			エラー		= FALSE
//-----------------------------------------------------------------------------
BOOL CFmfMap::Open(const char *szFilePath, const char *grFilePath)
{
	Close();

	char buf[256] = "data/map/";
	
	strcat_s(buf, sizeof(buf), szFilePath);

	// ファイルを開く
	HANDLE hFile = CreateFile(	buf, GENERIC_READ, FILE_SHARE_READ, NULL,
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;



	// ヘッダ情報を読む
	DWORD dwReadBytes;
	if (!ReadFile(hFile, &m_fmfHeader, sizeof(FMFHEADER), &dwReadBytes, NULL) ||
		dwReadBytes != sizeof(FMFHEADER))
		goto error_return;

	// 識別子のチェック
	if (memcmp(&m_fmfHeader.dwIdentifier, "FMF_", 4) != 0)
		goto error_return;

	// メモリ確保
	m_pLayerAddr = new BYTE[m_fmfHeader.dwSize];
	if (m_pLayerAddr == NULL)
		goto error_return;

	// レイヤーデータを読む
	if (!ReadFile(hFile, m_pLayerAddr, m_fmfHeader.dwSize, &dwReadBytes, NULL) ||
		dwReadBytes != m_fmfHeader.dwSize)
		goto error_return;
	
	// 正常終了
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
	// エラー終了
	CloseHandle(hFile);
	Close();
	return FALSE;
}
//-----------------------------------------------------------------------------
// マップが開かれているか
//-----------------------------------------------------------------------------
BOOL CFmfMap::IsOpen() const
{
	return m_pLayerAddr != NULL;
}
//-----------------------------------------------------------------------------
//	マップメモリを開放
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
//	指定レイヤの先頭アドレスを得る
//	引数：	レイヤ番号
//	戻り値:	正常終了	= レイヤデータのアドレス
//			エラー		= NULL
//	各レイヤデータは連続したメモリ領域に配置されてるので
//	指定レイヤデータのアドレスを計算で求める。
//-----------------------------------------------------------------------------
void* CFmfMap::GetLayerAddr(BYTE byLayerIndex) const
{
	// メモリチェック、範囲チェック
	if ((m_pLayerAddr == NULL) || (byLayerIndex >= m_fmfHeader.byLayerCount))
		return NULL;

	BYTE bySize = m_fmfHeader.byBitCount / 8;
	return m_pLayerAddr + m_fmfHeader.dwWidth * m_fmfHeader.dwHeight * bySize * byLayerIndex;
}
//-----------------------------------------------------------------------------
// レイヤ番号と座標を指定して直接データを貰う
// 引数：
// 	byLayerIndex	= レイヤ番号
// 	dwX				= X座標（0～m_fmfHeader.dwWidth - 1）
// 	dwY				= Y座標（0～m_fmfHeader.dwHeight - 1）
// 戻り値：
// 	正常終了	= 座標の値
//	エラー		= -1
//-----------------------------------------------------------------------------
int CFmfMap::GetValue(BYTE byLayerIndex, DWORD dwX, DWORD dwY) const
{
	int nIndex = -1;

	// 範囲チェック
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
// レイヤ番号と座標を指定してデータをセット
//-----------------------------------------------------------------------------
void CFmfMap::SetValue(BYTE byLayerIndex, DWORD dwX, DWORD dwY, int nValue)
{
	// 範囲チェック
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
// マップの横幅を得る
//-----------------------------------------------------------------------------
DWORD CFmfMap::GetMapWidth(void) const
{
	return m_fmfHeader.dwWidth;
}
//-----------------------------------------------------------------------------
// マップの高さを得る
//-----------------------------------------------------------------------------
DWORD CFmfMap::GetMapHeight(void) const
{
	return m_fmfHeader.dwHeight;
}
//-----------------------------------------------------------------------------
// チップの横幅を得る
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetChipWidth(void) const
{
	return m_fmfHeader.byChipWidth;
}
//-----------------------------------------------------------------------------
// チップの高さを得る
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetChipHeight(void) const
{
	return m_fmfHeader.byChipHeight;
}
//-----------------------------------------------------------------------------
// レイヤー数を得る
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetLayerCount(void) const
{
	return m_fmfHeader.byLayerCount;
}
//-----------------------------------------------------------------------------
// レイヤーデータのビットカウントを得る
//-----------------------------------------------------------------------------
BYTE CFmfMap::GetLayerBitCount(void) const
{
	return m_fmfHeader.byBitCount;
}
//-----------------------------------------------------------------------------
// マップを描写する
//-----------------------------------------------------------------------------
void CFmfMap::Draw(int px, int py) {

	//static int map = LoadGraph("data/map/maptip.png");

	BYTE* pLayer;
	// 0番（一番下のレイヤー）のアドレスを貰う
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

	// マップの描画
	for (BYTE ly = 0; ly < GetLayerCount() - 1; ly++) {
		for (DWORD y = 0; y < height; y++)
		{
			for (DWORD x = 0; x < width; x++)
			{
				//index = *(pLayer + y * width + x);
				// または
				index = GetValue(ly, x, y);

				if (index != 255) {
					// indexにはマップ座標(x, y)のマップデータが入ってるので
					// パーツ画像(srcHDC)からvalueに見合う矩形を算出して描画処理を行う。
					// マップが8bitの場合パーツのアラインメントは16、16bitなら256。
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
	// 0番（一番下のレイヤー）のアドレスを貰う
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
	
	// マップの描画
	for (BYTE ly = 0; ly < GetLayerCount() - 1; ly++) {
		for (DWORD y = 0; y < height; y++)
		{
			for (DWORD x = 0; x < width; x++)
			{
				//index = *(pLayer + y * width + x);
				// または
				index = GetValue(ly, x, y);


				if (index != 255) {
					// indexにはマップ座標(x, y)のマップデータが入ってるので
					// パーツ画像(srcHDC)からvalueに見合う矩形を算出して描画処理を行う。
					// マップが8bitの場合パーツのアラインメントは16、16bitなら256。
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