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
//   row_major float4x4 fPositionMatrix;// Offset:    0 Size:    64
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
// cbuffer Flyweight
// {
//
//   float4 f4StartColor;               // Offset:    0 Size:    16
//   float4 f4EndColor;                 // Offset:   16 Size:    16
//   float4 f4CameraPos;                // Offset:   32 Size:    16
//   float fStartSize;                  // Offset:   48 Size:     4 [unused]
//   float fEndSize;                    // Offset:   52 Size:     4 [unused]
//   float2 padding;                    // Offset:   56 Size:     8 [unused]
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
// Flyweight                         cbuffer      NA          NA   10        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITIONS                0   xyzw        0     NONE   float   xyzw
// STARTLIFE                0   x           1     NONE   float   x   
// CURRENTLIFE              0    y          1     NONE   float    y  
// SIZE                     0     z         1     NONE   float     z 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
gs_4_0
dcl_constantbuffer cb0[4], immediateIndexed
dcl_constantbuffer cb1[8], immediateIndexed
dcl_constantbuffer cb10[3], immediateIndexed
dcl_input v[1][0].xyzw
dcl_input v[1][1].x
dcl_input v[1][1].y
dcl_input v[1][1].z
dcl_temps 5
dcl_inputprimitive point 
dcl_outputtopology trianglestrip 
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_maxout 4
add r0.xy, -cb10[2].xzxx, v[0][0].xzxx
dp2 r0.z, r0.xyxx, r0.xyxx
rsq r1.x, r0.z
mul r1.yz, r0.xxyx, r1.xxxx
mul r0.xy, r1.zxzz, l(1.000000, 0.000000, 0.000000, 0.000000)
mad r0.xy, r1.xyxx, l(0.000000, 1.000000, 0.000000, 0.000000), -r0.xyxx
dp2 r0.z, r0.xyxx, r0.xyxx
rsq r0.z, r0.z
mul r0.xy, r0.zzzz, r0.xyxx
mul r0.xy, r0.xyxx, v[0][1].zzzz
mul r0.xz, r0.xxyx, l(0.500000, 0.000000, 0.500000, 0.000000)
mov r0.y, l(0)
add r1.xyz, -r0.xyzx, v[0][0].xyzx
add r0.xyz, r0.xyzx, v[0][0].xyzx
mad r2.xyz, -v[0][1].zzzz, l(0.000000, 0.500000, 0.000000, 0.000000), r1.xyzx
mad r1.xyz, v[0][1].zzzz, l(0.000000, 0.500000, 0.000000, 0.000000), r1.xyzx
mul r3.xyzw, r2.yyyy, cb0[1].xyzw
mad r3.xyzw, r2.xxxx, cb0[0].xyzw, r3.xyzw
mad r2.xyzw, r2.zzzz, cb0[2].xyzw, r3.xyzw
mad r2.xyzw, v[0][0].wwww, cb0[3].xyzw, r2.xyzw
mul r3.xyzw, r2.yyyy, cb1[1].xyzw
mad r3.xyzw, r2.xxxx, cb1[0].xyzw, r3.xyzw
mad r3.xyzw, r2.zzzz, cb1[2].xyzw, r3.xyzw
mad r2.xyzw, r2.wwww, cb1[3].xyzw, r3.xyzw
mul r3.xyzw, r2.yyyy, cb1[5].xyzw
mad r3.xyzw, r2.xxxx, cb1[4].xyzw, r3.xyzw
mad r3.xyzw, r2.zzzz, cb1[6].xyzw, r3.xyzw
mad r2.xyzw, r2.wwww, cb1[7].xyzw, r3.xyzw
mov o0.xyzw, r2.xyzw
div r0.w, v[0][1].y, v[0][1].x
add r0.w, -r0.w, l(1.000000)
add r2.xyzw, -cb10[0].xyzw, cb10[1].xyzw
mad r2.xyzw, r0.wwww, r2.xyzw, cb10[0].xyzw
mov o1.xyzw, r2.xyzw
mov o2.xy, l(0,1.000000,0,0)
emit 
mad r3.xyz, -v[0][1].zzzz, l(0.000000, 0.500000, 0.000000, 0.000000), r0.xyzx
mad r0.xyz, v[0][1].zzzz, l(0.000000, 0.500000, 0.000000, 0.000000), r0.xyzx
mul r4.xyzw, r3.yyyy, cb0[1].xyzw
mad r4.xyzw, r3.xxxx, cb0[0].xyzw, r4.xyzw
mad r3.xyzw, r3.zzzz, cb0[2].xyzw, r4.xyzw
mad r3.xyzw, v[0][0].wwww, cb0[3].xyzw, r3.xyzw
mul r4.xyzw, r3.yyyy, cb1[1].xyzw
mad r4.xyzw, r3.xxxx, cb1[0].xyzw, r4.xyzw
mad r4.xyzw, r3.zzzz, cb1[2].xyzw, r4.xyzw
mad r3.xyzw, r3.wwww, cb1[3].xyzw, r4.xyzw
mul r4.xyzw, r3.yyyy, cb1[5].xyzw
mad r4.xyzw, r3.xxxx, cb1[4].xyzw, r4.xyzw
mad r4.xyzw, r3.zzzz, cb1[6].xyzw, r4.xyzw
mad r3.xyzw, r3.wwww, cb1[7].xyzw, r4.xyzw
mov o0.xyzw, r3.xyzw
mov o1.xyzw, r2.xyzw
mov o2.xy, l(1.000000,1.000000,0,0)
emit 
mul r3.xyzw, r1.yyyy, cb0[1].xyzw
mad r3.xyzw, r1.xxxx, cb0[0].xyzw, r3.xyzw
mad r1.xyzw, r1.zzzz, cb0[2].xyzw, r3.xyzw
mad r1.xyzw, v[0][0].wwww, cb0[3].xyzw, r1.xyzw
mul r3.xyzw, r1.yyyy, cb1[1].xyzw
mad r3.xyzw, r1.xxxx, cb1[0].xyzw, r3.xyzw
mad r3.xyzw, r1.zzzz, cb1[2].xyzw, r3.xyzw
mad r1.xyzw, r1.wwww, cb1[3].xyzw, r3.xyzw
mul r3.xyzw, r1.yyyy, cb1[5].xyzw
mad r3.xyzw, r1.xxxx, cb1[4].xyzw, r3.xyzw
mad r3.xyzw, r1.zzzz, cb1[6].xyzw, r3.xyzw
mad r1.xyzw, r1.wwww, cb1[7].xyzw, r3.xyzw
mov o0.xyzw, r1.xyzw
mov o1.xyzw, r2.xyzw
mov o2.xy, l(0,0,0,0)
emit 
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, r0.xxxx, cb0[0].xyzw, r1.xyzw
mad r0.xyzw, r0.zzzz, cb0[2].xyzw, r1.xyzw
mad r0.xyzw, v[0][0].wwww, cb0[3].xyzw, r0.xyzw
mul r1.xyzw, r0.yyyy, cb1[1].xyzw
mad r1.xyzw, r0.xxxx, cb1[0].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb1[2].xyzw, r1.xyzw
mad r0.xyzw, r0.wwww, cb1[3].xyzw, r1.xyzw
mul r1.xyzw, r0.yyyy, cb1[5].xyzw
mad r1.xyzw, r0.xxxx, cb1[4].xyzw, r1.xyzw
mad r1.xyzw, r0.zzzz, cb1[6].xyzw, r1.xyzw
mad r0.xyzw, r0.wwww, cb1[7].xyzw, r1.xyzw
mov o0.xyzw, r0.xyzw
mov o1.xyzw, r2.xyzw
mov o2.xy, l(1.000000,0,0,0)
emit 
ret 
// Approximately 87 instruction slots used
#endif

const BYTE ParticleGS[] =
{
     68,  88,  66,  67, 185, 100, 
    150,  52, 113,  30, 160, 169, 
    150, 171, 239,  57,  90, 249, 
    134, 187,   1,   0,   0,   0, 
    180,  16,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    224,   2,   0,   0, 120,   3, 
      0,   0, 236,   3,   0,   0, 
     56,  16,   0,   0,  82,  68, 
     69,  70, 164,   2,   0,   0, 
      3,   0,   0,   0, 152,   0, 
      0,   0,   3,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
     83,  71,   0,   1,   0,   0, 
    112,   2,   0,   0, 124,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    132,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 139,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  98,  79, 
     98, 106, 101,  99, 116,   0, 
     98,  83,  99, 101, 110, 101, 
      0,  70, 108, 121, 119, 101, 
    105, 103, 104, 116,   0, 171, 
    171, 171, 124,   0,   0,   0, 
      1,   0,   0,   0, 224,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 132,   0,   0,   0, 
      2,   0,   0,   0,  24,   1, 
      0,   0, 128,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 139,   0,   0,   0, 
      6,   0,   0,   0, 104,   1, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 248,   0,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
      8,   1,   0,   0,   0,   0, 
      0,   0, 102,  80, 111, 115, 
    105, 116, 105, 111, 110,  77, 
     97, 116, 114, 105, 120,   0, 
      2,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  72,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   8,   1,   0,   0, 
      0,   0,   0,   0,  84,   1, 
      0,   0,  64,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   8,   1,   0,   0, 
      0,   0,   0,   0, 102,  86, 
    105, 101, 119,  77,  97, 116, 
    114, 105, 120,   0, 102,  80, 
    114, 111, 106, 101,  99, 116, 
    105, 111, 110,  77,  97, 116, 
    114, 105, 120,   0, 171, 171, 
    248,   1,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0,   8,   2, 
      0,   0,   0,   0,   0,   0, 
     24,   2,   0,   0,  16,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0,   8,   2, 
      0,   0,   0,   0,   0,   0, 
     35,   2,   0,   0,  32,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0,   8,   2, 
      0,   0,   0,   0,   0,   0, 
     47,   2,   0,   0,  48,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  60,   2, 
      0,   0,   0,   0,   0,   0, 
     76,   2,   0,   0,  52,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  60,   2, 
      0,   0,   0,   0,   0,   0, 
     85,   2,   0,   0,  56,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,  96,   2, 
      0,   0,   0,   0,   0,   0, 
    102,  52,  83, 116,  97, 114, 
    116,  67, 111, 108, 111, 114, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 102,  52,  69, 110, 
    100,  67, 111, 108, 111, 114, 
      0, 102,  52,  67,  97, 109, 
    101, 114,  97,  80, 111, 115, 
      0, 102,  83, 116,  97, 114, 
    116,  83, 105, 122, 101,   0, 
    171, 171,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    102,  69, 110, 100,  83, 105, 
    122, 101,   0, 112,  97, 100, 
    100, 105, 110, 103,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   0,   0, 
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
      1,   0,   0,   0,   1,   1, 
      0,   0, 124,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   2,   2, 
      0,   0, 136,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   4,   4, 
      0,   0,  80,  79,  83,  73, 
     84,  73,  79,  78,  83,   0, 
     83,  84,  65,  82,  84,  76, 
     73,  70,  69,   0,  67,  85, 
     82,  82,  69,  78,  84,  76, 
     73,  70,  69,   0,  83,  73, 
     90,  69,   0, 171, 171, 171, 
     79,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0,  98,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,  12,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171,  83,  72,  68,  82, 
     68,  12,   0,   0,  64,   0, 
      2,   0,  17,   3,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  89,   0,   0,   4, 
     70, 142,  32,   0,  10,   0, 
      0,   0,   3,   0,   0,   0, 
     95,   0,   0,   4, 242,  16, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   4,  18,  16,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  95,   0,   0,   4, 
     34,  16,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     95,   0,   0,   4,  66,  16, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 104,   0, 
      0,   2,   5,   0,   0,   0, 
     93,   8,   0,   1,  92,  40, 
      0,   1, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   2,   0,   0,   0, 
     94,   0,   0,   2,   4,   0, 
      0,   0,   0,   0,   0,  10, 
     50,   0,  16,   0,   0,   0, 
      0,   0, 134, 128,  32, 128, 
     65,   0,   0,   0,  10,   0, 
      0,   0,   2,   0,   0,   0, 
    134,  16,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5,  18,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  98,   0, 
     16,   0,   1,   0,   0,   0, 
      6,   1,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10,  50,   0,  16,   0, 
      0,   0,   0,   0,  38,  10, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  13, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  50,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
    166,  26,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     56,   0,   0,  10,  82,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   1,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   9, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  18,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,  18,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  14, 
    114,   0,  16,   0,   2,   0, 
      0,   0, 166,  26,  32, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  13, 114,   0,  16,   0, 
      1,   0,   0,   0, 166,  26, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 166,  10,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  31,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  86,   5,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0,   6,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 166,  10,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    246,  15,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      3,   0,   0,   0,  86,   5, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  14,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,  16,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  10,  16,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32, 128,  65,   0,   0,   0, 
     10,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
     10,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,  10,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  19,   0, 
      0,   1,  50,   0,   0,  14, 
    114,   0,  16,   0,   3,   0, 
      0,   0, 166,  26,  32, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  13, 114,   0,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  86,   5,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 166,  10,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   3,   0,   0,   0, 
    246,  31,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  86,   5,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   4,   0, 
      0,   0, 166,  10,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      4,   0,   0,   0,  86,   5, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   4,   0,   0,   0, 
      6,   0,  16,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      4,   0,   0,   0, 166,  10, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 246,  15,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  54,   0,   0,   5, 
    242,  32,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   8,  50,  32,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     19,   0,   0,   1,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      3,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246,  31,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      3,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   3,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  54,   0,   0,   5, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   8,  50,  32, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  19,   0,   0,   1, 
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
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0, 246,  31, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  19,   0, 
      0,   1,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,  87,   0,   0,   0, 
      5,   0,   0,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     69,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
