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
 94;
 0.10254;26.08030;5.69816;,
 1.93204;27.79213;4.60007;,
 2.63810;56.72349;26.66379;,
 0.80860;55.01169;27.76190;,
 1.92724;31.36034;3.92840;,
 2.63329;60.29180;25.99216;,
 0.09095;34.69493;4.07674;,
 0.79700;63.62634;26.14048;,
 -2.50116;35.84241;4.95812;,
 -1.79510;64.77380;27.02184;,
 -4.33066;34.13062;6.05622;,
 -3.62460;63.06201;28.11997;,
 -4.32586;30.56226;6.72781;,
 -3.61980;59.49374;28.79159;,
 -2.48957;27.22774;6.57951;,
 -1.78351;56.15919;28.64328;,
 0.10254;26.08030;5.69816;,
 0.80860;55.01169;27.76190;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -1.19931;30.96132;5.32811;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 -0.49325;59.89276;27.39188;,
 0.00000;0.00000;-12.29079;,
 0.00000;65.20000;-0.27079;,
 8.49942;0.00000;-8.77021;,
 0.00000;65.20000;-0.27079;,
 12.02000;0.00000;-0.27078;,
 0.00000;65.20000;-0.27079;,
 8.49942;0.00000;8.22864;,
 0.00000;65.20000;-0.27079;,
 0.00000;0.00000;11.74922;,
 0.00000;65.20000;-0.27079;,
 -8.49942;0.00000;8.22864;,
 0.00000;65.20000;-0.27079;,
 -12.02000;0.00000;-0.27079;,
 0.00000;65.20000;-0.27079;,
 -8.49942;0.00000;-8.77021;,
 0.00000;65.20000;-0.27079;,
 0.00000;0.00000;-12.29079;,
 0.00000;0.00000;-0.27079;,
 0.00000;0.00000;-12.29079;,
 8.49942;0.00000;-8.77021;,
 12.02000;0.00000;-0.27078;,
 8.49942;0.00000;8.22864;,
 0.00000;0.00000;11.74922;,
 -8.49942;0.00000;8.22864;,
 -12.02000;0.00000;-0.27079;,
 -8.49942;0.00000;-8.77021;,
 0.10254;55.99388;-35.07575;,
 1.93204;57.09197;-33.36392;,
 2.63810;35.02824;-4.43255;,
 0.80860;33.93014;-6.14435;,
 1.92724;57.76364;-29.79571;,
 2.63329;35.69988;-0.86425;,
 0.09095;57.61530;-26.46113;,
 0.79700;35.55156;2.47029;,
 -2.50116;56.73392;-25.31364;,
 -1.79510;34.67019;3.61776;,
 -4.33066;55.63582;-27.02544;,
 -3.62460;33.57206;1.90597;,
 -4.32586;54.96423;-30.59379;,
 -3.61980;32.90044;-1.66230;,
 -2.48957;55.11254;-33.92831;,
 -1.78351;33.04875;-4.99686;,
 0.10254;55.99388;-35.07575;,
 0.80860;33.93014;-6.14435;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -1.19931;56.36393;-30.19473;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;,
 -0.49325;34.30015;-1.26329;;
 
 64;
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
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 3;34,35,36;,
 3;36,37,38;,
 3;38,39,40;,
 3;40,41,42;,
 3;42,43,44;,
 3;44,45,46;,
 3;46,47,48;,
 3;48,49,50;,
 3;51,52,53;,
 3;51,53,54;,
 3;51,54,55;,
 3;51,55,56;,
 3;51,56,57;,
 3;51,57,58;,
 3;51,58,59;,
 3;51,59,52;,
 4;60,61,62,63;,
 4;61,64,65,62;,
 4;64,66,67,65;,
 4;66,68,69,67;,
 4;68,70,71,69;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;74,76,77,75;,
 3;78,61,60;,
 3;79,64,61;,
 3;80,66,64;,
 3;81,68,66;,
 3;82,70,68;,
 3;83,72,70;,
 3;84,74,72;,
 3;85,76,74;,
 3;86,63,62;,
 3;87,62,65;,
 3;88,65,67;,
 3;89,67,69;,
 3;90,69,71;,
 3;91,71,73;,
 3;92,73,75;,
 3;93,75,77;;
 
 MeshMaterialList {
  3;
  64;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\wood000.jpg";
   }
  }
  Material {
   0.800000;0.414400;0.677600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\spring_SakuraTexture_003.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  61;
  -0.384275;-0.171265;-0.907194;,
  0.395293;-0.563096;0.725718;,
  0.932059;-0.233812;0.276763;,
  0.920753;0.222131;-0.320737;,
  0.383558;0.554088;-0.738830;,
  -0.420760;0.194933;-0.885981;,
  -0.730856;0.026409;-0.682021;,
  -0.920753;-0.222131;0.320737;,
  -0.383559;-0.554087;0.738830;,
  0.420761;-0.194929;0.885981;,
  0.730857;-0.026403;0.682019;,
  -0.395294;0.563096;-0.725718;,
  -0.932059;0.233813;-0.276764;,
  0.384279;0.171267;0.907192;,
  -0.384269;-0.171276;-0.907195;,
  -0.384267;-0.171269;-0.907197;,
  -0.384278;-0.171271;-0.907192;,
  -0.384280;-0.171258;-0.907193;,
  -0.384275;-0.171263;-0.907194;,
  -0.384273;-0.171270;-0.907194;,
  0.384281;0.171269;0.907191;,
  0.384280;0.171268;0.907191;,
  0.384279;0.171269;0.907192;,
  0.384279;0.171268;0.907192;,
  0.384278;0.171268;0.907192;,
  0.384282;0.171267;0.907191;,
  0.000000;0.181301;-0.983428;,
  0.695389;0.181301;-0.695388;,
  0.983428;0.181301;0.000000;,
  0.695388;0.181301;0.695388;,
  0.000000;0.181301;0.983428;,
  -0.695389;0.181301;0.695388;,
  -0.983428;0.181301;-0.000000;,
  -0.695388;0.181301;-0.695388;,
  0.000000;-1.000000;-0.000000;,
  -0.384274;0.907194;-0.171265;,
  0.395293;-0.725718;-0.563096;,
  0.932059;-0.276763;-0.233812;,
  0.920752;0.320738;0.222131;,
  0.383558;0.738830;0.554088;,
  -0.420760;0.885981;0.194933;,
  -0.730856;0.682021;0.026408;,
  -0.920753;-0.320737;-0.222131;,
  -0.383559;-0.738830;-0.554088;,
  0.420761;-0.885981;-0.194930;,
  0.730858;-0.682019;-0.026403;,
  -0.395293;0.725718;0.563096;,
  -0.932059;0.276764;0.233813;,
  0.384281;-0.907191;0.171268;,
  -0.384269;0.907195;-0.171276;,
  -0.384264;0.907198;-0.171268;,
  -0.384279;0.907191;-0.171272;,
  -0.384282;0.907192;-0.171259;,
  -0.384276;0.907194;-0.171262;,
  -0.384272;0.907195;-0.171267;,
  0.384284;-0.907189;0.171271;,
  0.384283;-0.907190;0.171269;,
  0.384280;-0.907191;0.171269;,
  0.384279;-0.907192;0.171269;,
  0.384279;-0.907192;0.171268;,
  0.384283;-0.907191;0.171266;;
  64;
  4;1,2,10,9;,
  4;2,3,3,10;,
  4;3,4,4,3;,
  4;4,5,11,4;,
  4;5,6,12,11;,
  4;6,7,7,12;,
  4;7,8,8,7;,
  4;8,1,9,8;,
  3;0,14,15;,
  3;0,16,14;,
  3;0,17,16;,
  3;0,5,17;,
  3;0,6,5;,
  3;0,18,6;,
  3;0,19,18;,
  3;0,15,19;,
  3;13,9,10;,
  3;13,10,20;,
  3;13,20,21;,
  3;13,21,22;,
  3;13,22,23;,
  3;13,23,24;,
  3;13,24,25;,
  3;13,25,9;,
  3;26,27,27;,
  3;27,27,28;,
  3;28,29,29;,
  3;29,29,30;,
  3;30,31,31;,
  3;31,31,32;,
  3;32,33,33;,
  3;33,33,26;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  3;34,34,34;,
  4;36,37,45,44;,
  4;37,38,38,45;,
  4;38,39,39,38;,
  4;39,40,46,39;,
  4;40,41,47,46;,
  4;41,42,42,47;,
  4;42,43,43,42;,
  4;43,36,44,43;,
  3;35,49,50;,
  3;35,51,49;,
  3;35,52,51;,
  3;35,40,52;,
  3;35,41,40;,
  3;35,53,41;,
  3;35,54,53;,
  3;35,50,54;,
  3;48,44,45;,
  3;48,45,55;,
  3;48,55,56;,
  3;48,56,57;,
  3;48,57,58;,
  3;48,58,59;,
  3;48,59,60;,
  3;48,60,44;;
 }
 MeshTextureCoords {
  94;
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
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
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
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}