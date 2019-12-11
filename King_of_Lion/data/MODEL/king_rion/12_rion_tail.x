xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 73;
 -2.17120;0.00000;0.00000;,
 -1.53527;1.72425;-0.64246;,
 -1.53527;-14.85081;-29.04183;,
 -2.17120;-16.57505;-28.39946;,
 0.00000;2.43844;-0.90854;,
 -0.00000;-14.13661;-29.30802;,
 1.53526;1.72425;-0.64246;,
 1.53526;-14.85081;-29.04183;,
 2.17120;0.00000;0.00000;,
 2.17120;-16.57505;-28.39946;,
 1.53526;-1.72423;0.64231;,
 1.53526;-18.29928;-27.75712;,
 0.00000;-2.43846;0.90839;,
 -0.00000;-19.01351;-27.49101;,
 -1.53527;-1.72423;0.64231;,
 -1.53527;-18.29928;-27.75712;,
 -2.17120;0.00000;0.00000;,
 -2.17120;-16.57505;-28.39946;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.23128;3.56462;0.08619;,
 -0.72806;3.05052;-1.57542;,
 -1.68739;3.05052;0.08619;,
 0.23128;3.56462;0.08619;,
 1.19062;3.05052;-1.57542;,
 0.23128;3.56462;0.08619;,
 2.14995;3.05052;0.08619;,
 0.23128;3.56462;0.08619;,
 1.19062;3.05052;1.74774;,
 0.23128;3.56462;0.08619;,
 -0.72806;3.05052;1.74774;,
 0.23128;3.56462;0.08619;,
 -1.68739;3.05052;0.08619;,
 -1.43034;1.64596;-2.79184;,
 -3.09196;1.64596;0.08619;,
 1.89290;1.64596;-2.79184;,
 3.55452;1.64596;0.08619;,
 1.89290;1.64596;2.96422;,
 -1.43034;1.64596;2.96422;,
 -3.09196;1.64596;0.08619;,
 -1.68739;-0.27273;-3.23708;,
 -3.60606;-0.27273;0.08619;,
 4.06862;-0.27273;0.08619;,
 2.14995;-0.27273;-3.23708;,
 2.14995;-0.27273;3.40938;,
 -1.68739;-0.27273;3.40938;,
 -3.60606;-0.27273;0.08619;,
 -1.43034;-2.19139;-2.79184;,
 -3.09196;-2.19139;0.08619;,
 3.55452;-2.19139;0.08619;,
 1.89290;-2.19139;-2.79184;,
 1.89290;-2.19139;2.96422;,
 -1.43034;-2.19139;2.96422;,
 -3.09196;-2.19139;0.08619;,
 -0.72806;-3.59597;-1.57542;,
 -1.68739;-3.59597;0.08619;,
 1.19062;-3.59597;-1.57542;,
 2.14995;-3.59597;0.08619;,
 1.19062;-3.59597;1.74774;,
 -0.72806;-3.59597;1.74774;,
 -1.68739;-3.59597;0.08619;,
 0.23128;-4.11006;0.08619;,
 0.23128;-4.11006;0.08619;,
 0.23128;-4.11006;0.08619;,
 0.23128;-4.11006;0.08619;,
 0.23128;-4.11006;0.08619;,
 0.23128;-4.11006;0.08619;;
 
 50;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,27,28;,
 3;29,30,27;,
 3;31,32,30;,
 3;33,34,32;,
 3;35,36,34;,
 3;37,38,36;,
 4;28,27,39,40;,
 4;27,30,41,39;,
 4;30,32,42,41;,
 4;32,34,43,42;,
 4;34,36,44,43;,
 4;36,38,45,44;,
 4;40,39,46,47;,
 4;41,42,48,49;,
 4;42,43,50,48;,
 4;43,44,51,50;,
 4;44,45,52,51;,
 4;47,46,53,54;,
 4;49,48,55,56;,
 4;48,50,57,55;,
 4;50,51,58,57;,
 4;51,52,59,58;,
 4;54,53,60,61;,
 4;53,56,62,60;,
 4;56,55,63,62;,
 4;55,57,64,63;,
 4;57,58,65,64;,
 4;58,59,66,65;,
 3;61,60,67;,
 3;60,62,68;,
 3;62,63,69;,
 3;63,64,70;,
 3;64,65,71;,
 3;65,66,72;;
 
 MeshMaterialList {
  6;
  50;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.768800;0.191200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.483200;0.009600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.072000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.003200;0.000000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  49;
  0.000000;0.349118;0.937079;,
  -1.000000;-0.000004;0.000002;,
  -0.747430;0.573766;-0.334873;,
  0.000002;0.863663;-0.504069;,
  0.747430;0.573767;-0.334874;,
  1.000000;-0.000004;0.000002;,
  0.747428;-0.573769;0.334875;,
  0.000002;-0.863663;0.504069;,
  -0.747428;-0.573768;0.334874;,
  -0.000000;1.000000;0.000004;,
  -0.492317;0.870416;0.000003;,
  -0.246159;0.870417;-0.426358;,
  0.246159;0.870417;-0.426358;,
  0.492316;0.870416;0.000002;,
  0.246156;0.870417;0.426358;,
  -0.246157;0.870417;0.426358;,
  -0.862094;0.506749;0.000003;,
  -0.557227;0.571025;-0.602850;,
  0.557227;0.571026;-0.602850;,
  0.862093;0.506750;0.000003;,
  0.431046;0.506749;0.746595;,
  -0.431047;0.506747;0.746596;,
  -1.000000;-0.000002;0.000004;,
  -0.866028;-0.000002;-0.499996;,
  0.866028;-0.000002;-0.499996;,
  1.000000;-0.000002;0.000004;,
  0.499997;-0.000002;0.866027;,
  -0.499998;-0.000002;0.866027;,
  -0.862094;-0.506748;0.000003;,
  -0.557228;-0.571022;-0.602852;,
  0.557227;-0.571023;-0.602852;,
  0.862094;-0.506749;0.000003;,
  0.431047;-0.506748;0.746596;,
  -0.431047;-0.506747;0.746596;,
  -0.492317;-0.870416;0.000002;,
  -0.246160;-0.870416;-0.426358;,
  0.246159;-0.870416;-0.426358;,
  0.492317;-0.870416;0.000002;,
  0.246156;-0.870417;0.426358;,
  -0.246156;-0.870417;0.426358;,
  -0.000000;-1.000000;0.000004;,
  -0.000004;0.349148;0.937068;,
  0.000000;0.349118;0.937079;,
  0.000000;0.349145;0.937069;,
  0.000004;0.349148;0.937068;,
  0.000000;0.349118;0.937079;,
  0.000004;0.349088;0.937090;,
  0.000000;0.349091;0.937089;,
  -0.000004;0.349088;0.937090;;
  50;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,41,42;,
  3;0,43,41;,
  3;0,44,43;,
  3;0,45,44;,
  3;0,46,45;,
  3;0,47,46;,
  3;0,48,47;,
  3;0,42,48;,
  3;9,11,10;,
  3;9,12,11;,
  3;9,13,12;,
  3;9,14,13;,
  3;9,15,14;,
  3;9,10,15;,
  4;10,11,17,16;,
  4;11,12,18,17;,
  4;12,13,19,18;,
  4;13,14,20,19;,
  4;14,15,21,20;,
  4;15,10,16,21;,
  4;16,17,23,22;,
  4;18,19,25,24;,
  4;19,20,26,25;,
  4;20,21,27,26;,
  4;21,16,22,27;,
  4;22,23,29,28;,
  4;24,25,31,30;,
  4;25,26,32,31;,
  4;26,27,33,32;,
  4;27,22,28,33;,
  4;28,29,35,34;,
  4;29,30,36,35;,
  4;30,31,37,36;,
  4;31,32,38,37;,
  4;32,33,39,38;,
  4;33,28,34,39;,
  3;34,35,40;,
  3;35,36,40;,
  3;36,37,40;,
  3;37,38,40;,
  3;38,39,40;,
  3;39,34,40;;
 }
 MeshTextureCoords {
  73;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.083330;0.000000;,
  0.166670;0.166670;,
  0.000000;0.166670;,
  0.250000;0.000000;,
  0.333330;0.166670;,
  0.416670;0.000000;,
  0.500000;0.166670;,
  0.583330;0.000000;,
  0.666670;0.166670;,
  0.750000;0.000000;,
  0.833330;0.166670;,
  0.916670;0.000000;,
  1.000000;0.166670;,
  0.166670;0.333330;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.500000;0.333330;,
  0.666670;0.333330;,
  0.833330;0.333330;,
  1.000000;0.333330;,
  0.166670;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.333330;0.500000;,
  0.666670;0.500000;,
  0.833330;0.500000;,
  1.000000;0.500000;,
  0.166670;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.333330;0.666670;,
  0.666670;0.666670;,
  0.833330;0.666670;,
  1.000000;0.666670;,
  0.166670;0.833330;,
  0.000000;0.833330;,
  0.333330;0.833330;,
  0.500000;0.833330;,
  0.666670;0.833330;,
  0.833330;0.833330;,
  1.000000;0.833330;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;;
 }
}