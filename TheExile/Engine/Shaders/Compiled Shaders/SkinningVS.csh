#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer bObject
// {
//
//   row_major float4x4 fTransformMatrix;// Offset:    0 Size:    64
//
// }
//
// cbuffer bScene
// {
//
//   row_major float4x4 fViewMatrix;    // Offset:    0 Size:    64
//   row_major float4x4 fProjectionMatrix;// Offset:   64 Size:    64
//
// }
//
// cbuffer BONES
// {
//
//   row_major float4x4 CurBones[100];  // Offset:    0 Size:  6400
//   row_major float4x4 InvBones[100];  // Offset: 6400 Size:  6400
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// bObject                           cbuffer      NA          NA    0        1
// bScene                            cbuffer      NA          NA    1        1
// BONES                             cbuffer      NA          NA    2        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITIONS                0   xyzw        0     NONE   float   xyzw
// NORMALS                  0   xyzw        1     NONE   float   xyz 
// TEXCOORDS                0   xyzw        2     NONE   float   xyzw
// TANGENTS                 0   xyzw        3     NONE   float       
// BONEINDICES              0   xyzw        4     NONE    uint   xyzw
// BONEWEIGHTS              0   xyzw        5     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// OLD_POSITION             0   xyzw        0     NONE   float   xyzw
// SV_POSITION              0   xyzw        1      POS   float   xyzw
// NORMALS                  0   xyzw        2     NONE   float   xyzw
// TEXCOORD                 0   xyzw        3     NONE   float   xyzw
// TANGENTS                 0   xyzw        4     NONE   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[4], immediateIndexed
dcl_constantbuffer cb1[8], immediateIndexed
dcl_constantbuffer cb2[800], dynamicIndexed
dcl_input v0.xyzw
dcl_input v1.xyz
dcl_input v2.xyzw
dcl_input v4.xyzw
dcl_input v5.xyzw
dcl_output o0.xyzw
dcl_output_siv o1.xyzw, position
dcl_output o2.xyzw
dcl_output o3.xyzw
dcl_output o4.xyzw
dcl_temps 4
ishl r0.xyzw, v4.xyzw, l(2, 2, 2, 2)
mul r1.xyzw, v0.yyyy, cb2[r0.y + 401].xyzw
mad r1.xyzw, v0.xxxx, cb2[r0.y + 400].xyzw, r1.xyzw
mad r1.xyzw, v0.zzzz, cb2[r0.y + 402].xyzw, r1.xyzw
mad r1.xyzw, v0.wwww, cb2[r0.y + 403].xyzw, r1.xyzw
mul r2.xyz, r1.yyyy, cb2[r0.y + 1].xyzx
mad r2.xyz, r1.xxxx, cb2[r0.y + 0].xyzx, r2.xyzx
mad r1.xyz, r1.zzzz, cb2[r0.y + 2].xyzx, r2.xyzx
mad r1.xyz, r1.wwww, cb2[r0.y + 3].xyzx, r1.xyzx
mul r1.xyz, r1.xyzx, v5.yyyy
mul r2.xyzw, v0.yyyy, cb2[r0.x + 401].xyzw
mad r2.xyzw, v0.xxxx, cb2[r0.x + 400].xyzw, r2.xyzw
mad r2.xyzw, v0.zzzz, cb2[r0.x + 402].xyzw, r2.xyzw
mad r2.xyzw, v0.wwww, cb2[r0.x + 403].xyzw, r2.xyzw
mul r3.xyz, r2.yyyy, cb2[r0.x + 1].xyzx
mad r3.xyz, r2.xxxx, cb2[r0.x + 0].xyzx, r3.xyzx
mad r2.xyz, r2.zzzz, cb2[r0.x + 2].xyzx, r3.xyzx
mad r2.xyz, r2.wwww, cb2[r0.x + 3].xyzx, r2.xyzx
mad r1.xyz, r2.xyzx, v5.xxxx, r1.xyzx
mul r2.xyzw, v0.yyyy, cb2[r0.z + 401].xyzw
mad r2.xyzw, v0.xxxx, cb2[r0.z + 400].xyzw, r2.xyzw
mad r2.xyzw, v0.zzzz, cb2[r0.z + 402].xyzw, r2.xyzw
mad r2.xyzw, v0.wwww, cb2[r0.z + 403].xyzw, r2.xyzw
mul r3.xyz, r2.yyyy, cb2[r0.z + 1].xyzx
mad r3.xyz, r2.xxxx, cb2[r0.z + 0].xyzx, r3.xyzx
mad r2.xyz, r2.zzzz, cb2[r0.z + 2].xyzx, r3.xyzx
mad r0.xyz, r2.wwww, cb2[r0.z + 3].xyzx, r2.xyzx
mad r0.xyz, r0.xyzx, v5.zzzz, r1.xyzx
mul r1.xyzw, v0.yyyy, cb2[r0.w + 401].xyzw
mad r1.xyzw, v0.xxxx, cb2[r0.w + 400].xyzw, r1.xyzw
mad r1.xyzw, v0.zzzz, cb2[r0.w + 402].xyzw, r1.xyzw
mad r1.xyzw, v0.wwww, cb2[r0.w + 403].xyzw, r1.xyzw
mul r2.xyz, r1.yyyy, cb2[r0.w + 1].xyzx
mad r2.xyz, r1.xxxx, cb2[r0.w + 0].xyzx, r2.xyzx
mad r1.xyz, r1.zzzz, cb2[r0.w + 2].xyzx, r2.xyzx
mad r1.xyz, r1.wwww, cb2[r0.w + 3].xyzx, r1.xyzx
mad r0.xyz, r1.xyzx, v5.wwww, r0.xyzx
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[2].xyzw, r1.xyzw
add r0.xyzw, r0.xyzw, cb0[3].xyzw
mov o0.xyzw, r0.xyzw
mul r1.xyzw, r0.yyyy, cb1[1].xyzw
mad r1.xyzw, r0.xxxx, cb1[0].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb1[2].xyzw, r1.xyzw
mad r0.xyzw, r0.wwww, cb1[3].xyzw, r1.xyzw
mul r1.xyzw, r0.yyyy, cb1[5].xyzw
mad r1.xyzw, r0.xxxx, cb1[4].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb1[6].xyzw, r1.xyzw
mad o1.xyzw, r0.wwww, cb1[7].xyzw, r1.xyzw
mul r0.xyzw, v1.yyyy, cb0[1].xyzw
mad r0.xyzw, v1.xxxx, cb0[0].xyzw, r0.xyzw
mad o2.xyzw, v1.zzzz, cb0[2].xyzw, r0.xyzw
mov o3.xyzw, v2.xyzw
mov o4.xyzw, l(0,0,0,0)
ret 
// Approximately 56 instruction slots used
#endif

const BYTE SkinningVS[] =
{
     68,  88,  66,  67, 124, 115, 
      1,  27, 107, 118, 120, 203, 
    133, 130, 243, 186,  97, 103, 
    254,  95,   1,   0,   0,   0, 
    208,  14,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    244,   2,   0,   0, 212,   3, 
      0,   0, 144,   4,   0,   0, 
     52,  14,   0,   0,  82,  68, 
     69,  70, 184,   2,   0,   0, 
      3,   0,   0,   0, 180,   0, 
      0,   0,   3,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
    132,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 164,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 171,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     98,  79,  98, 106, 101,  99, 
    116,   0,  98,  83,  99, 101, 
    110, 101,   0,  66,  79,  78, 
     69,  83,   0, 171, 171, 171, 
    156,   0,   0,   0,   1,   0, 
      0,   0, 252,   0,   0,   0, 
     64,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    164,   0,   0,   0,   2,   0, 
      0,   0, 100,   1,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    171,   0,   0,   0,   2,   0, 
      0,   0, 212,   1,   0,   0, 
      0,  50,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     36,   1,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  64,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 102,  84, 
    114,  97, 110, 115, 102, 111, 
    114, 109,  77,  97, 116, 114, 
    105, 120,   0, 102, 108, 111, 
     97, 116,  52, 120,  52,   0, 
    171, 171,   2,   0,   3,   0, 
      4,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  53,   1, 
      0,   0, 180,   1,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     64,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    192,   1,   0,   0,  64,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  64,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 102,  86, 
    105, 101, 119,  77,  97, 116, 
    114, 105, 120,   0, 102,  80, 
    114, 111, 106, 101,  99, 116, 
    105, 111, 110,  77,  97, 116, 
    114, 105, 120,   0, 171, 171, 
     36,   2,   0,   0,   0,   0, 
      0,   0,   0,  25,   0,   0, 
      2,   0,   0,   0,  48,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  84,   2, 
      0,   0,   0,  25,   0,   0, 
      0,  25,   0,   0,   2,   0, 
      0,   0,  96,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  67, 117, 114,  66, 
    111, 110, 101, 115,   0, 171, 
    171, 171,   2,   0,   3,   0, 
      4,   0,   4,   0, 100,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  53,   1, 
      0,   0,  73, 110, 118,  66, 
    111, 110, 101, 115,   0, 171, 
    171, 171,   2,   0,   3,   0, 
      4,   0,   4,   0, 100,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  53,   1, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0, 171, 171, 
     73,  83,  71,  78, 216,   0, 
      0,   0,   6,   0,   0,   0, 
      8,   0,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0, 162,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   7,   0,   0, 170,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     15,  15,   0,   0, 180,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,   0,   0,   0, 189,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
     15,  15,   0,   0, 201,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
     15,  15,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
     83,   0,  78,  79,  82,  77, 
     65,  76,  83,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
     83,   0,  84,  65,  78,  71, 
     69,  78,  84,  83,   0,  66, 
     79,  78,  69,  73,  78,  68, 
     73,  67,  69,  83,   0,  66, 
     79,  78,  69,  87,  69,  73, 
     71,  72,  84,  83,   0, 171, 
    171, 171,  79,  83,  71,  78, 
    180,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
    141,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,   0,   0,   0, 
    153,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  15,   0,   0,   0, 
    161,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   0,   0,   0, 
    170,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,  15,   0,   0,   0, 
     79,  76,  68,  95,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  83,  86,  95,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  78,  79,  82,  77,  65, 
     76,  83,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     84,  65,  78,  71,  69,  78, 
     84,  83,   0, 171,  83,  72, 
     69,  88, 156,   9,   0,   0, 
     80,   0,   1,   0, 103,   2, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  89,   8,   0,   4, 
     70, 142,  32,   0,   2,   0, 
      0,   0,  32,   3,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   0,   0,   0,   0, 
     95,   0,   0,   3, 114,  16, 
     16,   0,   1,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   2,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   4,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   5,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    103,   0,   0,   4, 242,  32, 
     16,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      3,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      4,   0,   0,   0, 104,   0, 
      0,   2,   4,   0,   0,   0, 
     41,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  30,  16,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      2,   0,   0,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
      2,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,  21, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   2,   0, 
      0,   0, 145,   1,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    144,   1,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    146,   1,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246,  31,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    147,   1,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,  10, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   2,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   4, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  86,  21, 
     16,   0,   5,   0,   0,   0, 
     56,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86,  21,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      2,   0,   0,   0, 145,   1, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  12, 242,   0,  16,   0, 
      2,   0,   0,   0,   6,  16, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   2,   0, 
      0,   0, 144,   1,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  12, 242,   0,  16,   0, 
      2,   0,   0,   0, 166,  26, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   2,   0, 
      0,   0, 146,   1,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  12, 242,   0,  16,   0, 
      2,   0,   0,   0, 246,  31, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   2,   0, 
      0,   0, 147,   1,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      3,   0,   0,   0,  86,   5, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   6,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    114,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   4,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  12, 114,   0,  16,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   6,   2,   0, 
      0,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  12, 114,   0,  16,   0, 
      2,   0,   0,   0, 246,  15, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   6,   2,   0, 
      0,   0,   3,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      6,  16,  16,   0,   5,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  86,  21, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   6,   2,   0, 
      0,   0, 145,   1,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   2,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    144,   1,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    146,   1,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 246,  31,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    147,   1,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 114,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   4, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  12, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 166,  10,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      2,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  12, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   6,   2,   0,   0,   0, 
      3,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,   9, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     16,   0,   5,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   2,   0,   0,   0, 
    145,   1,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  16,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      2,   0,   0,   0, 144,   1, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  26,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      2,   0,   0,   0, 146,   1, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  31,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   6, 
      2,   0,   0,   0, 147,   1, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,  10, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      2,   0,   0,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  11, 114,   0,  16,   0, 
      2,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   4,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      2,   0,   0,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   6, 
      2,   0,   0,   0,   3,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0, 246,  31,  16,   0, 
      5,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     86,  21,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0,   6,  16,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 242,  32, 
     16,   0,   2,   0,   0,   0, 
    166,  26,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      3,   0,   0,   0,  70,  30, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   8, 242,  32, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  56,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,   0,   0, 
     51,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};