#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITIONS                0   xyzw        0     NONE   float   xyzw
// NORMALS                  0   xyzw        1     NONE   float       
// TEXCOORDS                0   xyzw        2     NONE   float   xy  
// TANGENTS                 0   xyzw        3     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
// HORIZONTAL               1     zw        1     NONE   float     zw
// HORIZONTAL               0   xyzw        2     NONE   float   xyzw
// HORIZONTAL               3   xyzw        3     NONE   float   xyzw
// HORIZONTAL               5   xyzw        4     NONE   float   xyzw
// HORIZONTAL               7   xyzw        5     NONE   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_input v0.xyzw
dcl_input v2.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output o1.zw
dcl_output o2.xyzw
dcl_output o3.xyzw
dcl_output o4.xyzw
dcl_output o5.xyzw
mov o0.xyzw, v0.xyzw
add o1.xyzw, v2.xyxy, l(0.000000, 0.000000, -0.031250, 0.000000)
add o2.xyzw, v2.xyxy, l(-0.041667, 0.000000, -0.020833, 0.000000)
add o3.xyzw, v2.xyxy, l(-0.010417, 0.000000, 0.000000, 0.000000)
add o4.xyzw, v2.xyxy, l(0.010417, 0.000000, 0.020833, 0.000000)
add o5.xyzw, v2.xyxy, l(0.031250, 0.000000, 0.041667, 0.000000)
ret 
// Approximately 7 instruction slots used
#endif

const BYTE HBlurVS[] =
{
     68,  88,  66,  67, 115, 247, 
    197, 169,  39, 212, 248, 153, 
    247, 157, 245,   9, 146, 158, 
     98,  76,   1,   0,   0,   0, 
     28,   4,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    172,   0,   0,   0,  68,   1, 
      0,   0,  28,   2,   0,   0, 
    128,   3,   0,   0,  82,  68, 
     69,  70, 112,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
     60,   0,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  54, 
     46,  51,  46,  57,  54,  48, 
     48,  46,  49,  54,  51,  56, 
     52,   0, 171, 171,  73,  83, 
     71,  78, 144,   0,   0,   0, 
      4,   0,   0,   0,   8,   0, 
      0,   0, 104,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,  15, 
      0,   0, 114,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  15,   0, 
      0,   0, 122,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,   3, 
      0,   0, 132,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      3,   0,   0,   0,  15,   0, 
      0,   0,  80,  79,  83,  73, 
     84,  73,  79,  78,  83,   0, 
     78,  79,  82,  77,  65,  76, 
     83,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,  83,   0, 
     84,  65,  78,  71,  69,  78, 
     84,  83,   0, 171, 171, 171, 
     79,  83,  71,  78, 208,   0, 
      0,   0,   7,   0,   0,   0, 
      8,   0,   0,   0, 176,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 188,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      3,  12,   0,   0, 197,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     12,   3,   0,   0, 197,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     15,   0,   0,   0, 197,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,   0,   0,   0, 197,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
     15,   0,   0,   0, 197,   0, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0,  72,  79,  82,  73,  90, 
     79,  78,  84,  65,  76,   0, 
     83,  72,  69,  88,  92,   1, 
      0,   0,  80,   0,   1,   0, 
     87,   0,   0,   0, 106,   8, 
      0,   1,  95,   0,   0,   3, 
    242,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
     50,  16,  16,   0,   2,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 194,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   4,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   5,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,  10, 
    242,  32,  16,   0,   1,   0, 
      0,   0,  70,  20,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0, 189,   0,   0,   0,   0, 
      0,   0,   0,  10, 242,  32, 
     16,   0,   2,   0,   0,   0, 
     70,  20,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
    171, 170,  42, 189,   0,   0, 
      0,   0, 171, 170, 170, 188, 
      0,   0,   0,   0,   0,   0, 
      0,  10, 242,  32,  16,   0, 
      3,   0,   0,   0,  70,  20, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 171, 170, 
     42, 188,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  10, 
    242,  32,  16,   0,   4,   0, 
      0,   0,  70,  20,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0, 171, 170,  42,  60, 
      0,   0,   0,   0, 171, 170, 
    170,  60,   0,   0,   0,   0, 
      0,   0,   0,  10, 242,  32, 
     16,   0,   5,   0,   0,   0, 
     70,  20,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      1,   0,   0,  61,   0,   0, 
      0,   0, 171, 170,  42,  61, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,   7,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   9,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
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
      0,   0,   0,   0,   0,   0, 
      0,   0
};
