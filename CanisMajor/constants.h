#ifndef Constants_H
#define Constants_H

//Defines to make life easier

#define Vector3 D3DXVECTOR3
#define Matrix D3DXMATRIX
#define Identity D3DXMatrixIdentity
#define Translate D3DXMatrixTranslation
#define RotateX D3DXMatrixRotationX
#define RotateY D3DXMatrixRotationY
#define RotateZ D3DXMatrixRotationZ
#define ToRadian D3DXToRadian
#define ToDegree D3DXToDegree
#define Normalize D3DXVec3Normalize
#define Length D3DXVec3Length
#define FAILED(hr) (((HRESULT)(hr)) < 0)

// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\sound Bank.xsb";
// XGS_FILE must be location of .xgs file.
const char XGS_FILE[]   = "audio\\Win\\audioProject.xgs";

const int CREAK_SIZE = 3;

//Audio cues
const char CLICK[] = "click";//Caelan
const char GROWL[] = "growl1";//Caelan
const char BG[] = "bg";//Caelan
const char CREAK[] = "creak";//Caelan
const char CREAK1[] = "creak1";//Caelan
const char CREAK2[] = "creak2";//Caelan
const char CREAK3[] = "creak3";//Caelan
const char HOWL[] = "howl";//Caelan
const char WALK1[] = "walk1";
const char RUN1[] = "run1";


#endif