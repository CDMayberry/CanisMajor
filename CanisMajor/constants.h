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

//Audio cues
const char FIRING[] = "firing";//Caelan
const char EXP1[] = "exp1";//Caelan
const char EXP2[] = "exp2";//Stuart
const char EXP3[] = "exp3";//Caelan
const char PEXP[] = "expPlayer";//Caelan
const char DROWN[] = "drown";//Caelan
const char COIN[] = "coin";//Caelan
const char BUB[] = "bubble";//Zared
const char COCK[] = "power";//Caelan
const char SHIELD[] = "shield";//Caelan
const char BLOCK[] = "block";//Caelan
const char BACKGROUND[] = "background";//Justin

#endif