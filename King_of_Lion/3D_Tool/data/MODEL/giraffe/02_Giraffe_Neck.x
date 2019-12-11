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
 75;
 0.00000;-1.87178;-3.24201;,
 2.54283;-1.51432;-2.62283;,
 3.55177;-9.00076;0.31191;,
 0.00000;-9.50008;-0.55294;,
 4.11440;-0.57840;-1.00185;,
 5.74689;-7.69355;2.57609;,
 4.11440;0.57841;1.00184;,
 5.74689;-6.07772;5.37478;,
 2.43686;1.24103;2.25965;,
 3.55177;-4.77049;7.63897;,
 0.00000;1.87177;3.24203;,
 0.00000;-4.27118;8.50378;,
 -2.64881;1.24103;2.25965;,
 -3.55178;-4.77049;7.63897;,
 -4.11439;0.57841;1.00184;,
 -5.74689;-6.07772;5.37478;,
 -4.11439;-0.57840;-1.00185;,
 -5.74689;-7.69355;2.57609;,
 -2.54283;-1.51432;-2.62283;,
 -3.55177;-9.00077;0.31191;,
 0.00000;-1.87178;-3.24201;,
 0.00000;-9.50008;-0.55294;,
 4.12502;-17.89480;4.65849;,
 0.00000;-18.47468;3.65405;,
 6.67445;-16.37658;7.28809;,
 6.67445;-14.49999;10.53851;,
 4.12502;-12.98176;13.16812;,
 0.00000;-12.40185;14.17254;,
 -4.12504;-12.98176;13.16812;,
 -6.67445;-14.49999;10.53851;,
 -6.67445;-16.37658;7.28809;,
 -4.12503;-17.89480;4.65849;,
 0.00000;-18.47468;3.65405;,
 4.81475;-28.97681;10.10811;,
 0.00000;-29.65370;8.93574;,
 7.79046;-27.20475;13.17741;,
 7.79046;-25.01436;16.97131;,
 4.81475;-23.24227;20.04062;,
 -0.03560;-22.49175;21.35354;,
 -4.78685;-23.26203;19.99456;,
 -7.79046;-25.01436;16.97131;,
 -7.79046;-27.20475;13.17741;,
 -4.81477;-28.97681;10.10811;,
 0.00000;-29.65370;8.93574;,
 4.81475;-38.31094;15.49717;,
 0.00000;-38.98783;14.32480;,
 7.79046;-36.53890;18.56648;,
 7.79046;-34.34848;22.36037;,
 4.81475;-32.57641;25.42969;,
 -0.02792;-31.87977;26.64813;,
 -4.77917;-32.65009;25.28914;,
 -7.79046;-34.34848;22.36037;,
 -7.79046;-36.53890;18.56648;,
 -4.81477;-38.31094;15.49717;,
 0.00000;-38.98783;14.32480;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;,
 0.00000;-35.44368;20.46343;;
 
 60;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;3,2,22,23;,
 4;2,5,24,22;,
 4;5,7,25,24;,
 4;7,9,26,25;,
 4;9,11,27,26;,
 4;11,13,28,27;,
 4;13,15,29,28;,
 4;15,17,30,29;,
 4;17,19,31,30;,
 4;19,21,32,31;,
 4;23,22,33,34;,
 4;22,24,35,33;,
 4;24,25,36,35;,
 4;25,26,37,36;,
 4;26,27,38,37;,
 4;27,28,39,38;,
 4;28,29,40,39;,
 4;29,30,41,40;,
 4;30,31,42,41;,
 4;31,32,43,42;,
 4;34,33,44,45;,
 4;33,35,46,44;,
 4;35,36,47,46;,
 4;36,37,48,47;,
 4;37,38,49,48;,
 4;38,39,50,49;,
 4;39,40,51,50;,
 4;40,41,52,51;,
 4;41,42,53,52;,
 4;42,43,54,53;,
 3;55,1,0;,
 3;56,4,1;,
 3;57,6,4;,
 3;58,8,6;,
 3;59,10,8;,
 3;60,12,10;,
 3;61,14,12;,
 3;62,16,14;,
 3;63,18,16;,
 3;64,20,18;,
 3;65,45,44;,
 3;66,44,46;,
 3;67,46,47;,
 3;68,47,48;,
 3;69,48,49;,
 3;70,49,50;,
 3;71,50,51;,
 3;72,51,52;,
 3;73,52,53;,
 3;74,53,54;;
 
 MeshMaterialList {
  1;
  60;
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
   0.800000;0.768800;0.194400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/giraffe/00_giraffe_Texture.png";
   }
  }
 }
 MeshNormals {
  72;
  0.000207;0.868966;-0.494872;,
  0.000000;-0.332460;-0.943117;,
  0.531906;-0.245958;-0.810297;,
  0.918957;0.012639;-0.394156;,
  0.900158;0.381925;0.209402;,
  0.529870;0.595485;0.603851;,
  0.008125;0.674775;0.737979;,
  -0.538116;0.588145;0.603751;,
  -0.908388;0.365233;0.203558;,
  -0.918957;0.012640;-0.394156;,
  -0.531906;-0.245958;-0.810297;,
  0.000000;-0.378899;-0.925438;,
  0.536731;-0.293932;-0.790901;,
  0.928539;-0.036919;-0.369394;,
  0.919978;0.318361;0.228661;,
  0.535994;0.547260;0.642820;,
  0.004024;0.624103;0.781332;,
  -0.540076;0.543311;0.642753;,
  -0.923567;0.310068;0.225571;,
  -0.928540;-0.036918;-0.369393;,
  -0.536731;-0.293932;-0.790901;,
  0.000000;-0.425816;-0.904810;,
  0.540188;-0.342596;-0.768651;,
  0.935353;-0.087968;-0.342602;,
  0.935353;0.252718;0.247483;,
  0.543082;0.494678;0.678495;,
  -0.001911;0.573259;0.819372;,
  -0.543545;0.493705;0.678833;,
  -0.934910;0.252909;0.248961;,
  -0.935353;-0.087967;-0.342601;,
  -0.540188;-0.342596;-0.768651;,
  0.000000;-0.463980;-0.885846;,
  0.541807;-0.382399;-0.748476;,
  0.938599;-0.130412;-0.319412;,
  0.938600;0.211412;0.272645;,
  0.548558;0.455047;0.701439;,
  -0.008285;0.534687;0.845010;,
  -0.550184;0.451690;0.702335;,
  -0.936433;0.213188;0.278648;,
  -0.938600;-0.130411;-0.319411;,
  -0.541808;-0.382398;-0.748476;,
  0.000001;-0.500000;-0.866026;,
  0.542345;-0.420078;-0.727597;,
  0.939641;-0.171082;-0.296322;,
  0.939641;0.171081;0.296321;,
  0.549919;0.415478;0.724546;,
  -0.012739;0.493353;0.869736;,
  -0.552178;0.410846;0.725469;,
  -0.936161;0.174136;0.305415;,
  -0.939641;-0.171081;-0.296321;,
  -0.542345;-0.420078;-0.727596;,
  -0.000092;-0.866032;0.499988;,
  -0.000001;0.866021;-0.500007;,
  0.000000;0.866024;-0.500002;,
  -0.000005;0.866024;-0.500002;,
  -0.004089;0.873201;-0.487343;,
  0.007207;0.873185;-0.487336;,
  0.000903;0.866027;-0.499997;,
  -0.006171;0.873421;-0.486927;,
  0.004222;0.873431;-0.486930;,
  0.000005;0.866024;-0.500002;,
  0.000001;0.866021;-0.500007;,
  0.000000;-0.866025;0.500001;,
  0.000000;-0.866026;0.499998;,
  -0.000002;-0.866026;0.500000;,
  0.000000;-0.866025;0.500001;,
  0.000491;-0.866232;0.499643;,
  -0.000674;-0.866440;0.499280;,
  -0.000951;-0.865853;0.500298;,
  0.000215;-0.865642;0.500663;,
  0.000002;-0.866026;0.500000;,
  -0.000000;-0.866026;0.499998;;
  60;
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;6,7,17,16;,
  4;7,8,18,17;,
  4;8,9,19,18;,
  4;9,10,20,19;,
  4;10,1,11,20;,
  4;11,12,22,21;,
  4;12,13,23,22;,
  4;13,14,24,23;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  4;17,18,28,27;,
  4;18,19,29,28;,
  4;19,20,30,29;,
  4;20,11,21,30;,
  4;21,22,32,31;,
  4;22,23,33,32;,
  4;23,24,34,33;,
  4;24,25,35,34;,
  4;25,26,36,35;,
  4;26,27,37,36;,
  4;27,28,38,37;,
  4;28,29,39,38;,
  4;29,30,40,39;,
  4;30,21,31,40;,
  4;31,32,42,41;,
  4;32,33,43,42;,
  4;33,34,44,43;,
  4;34,35,45,44;,
  4;35,36,46,45;,
  4;36,37,47,46;,
  4;37,38,48,47;,
  4;38,39,49,48;,
  4;39,40,50,49;,
  4;40,31,41,50;,
  3;0,52,53;,
  3;0,54,52;,
  3;0,55,54;,
  3;0,56,55;,
  3;0,57,56;,
  3;0,58,57;,
  3;0,59,58;,
  3;0,60,59;,
  3;0,61,60;,
  3;0,53,61;,
  3;51,62,63;,
  3;51,63,64;,
  3;51,64,65;,
  3;51,65,66;,
  3;51,66,67;,
  3;51,67,68;,
  3;51,68,69;,
  3;51,69,70;,
  3;51,70,71;,
  3;51,71,62;;
 }
 MeshTextureCoords {
  75;
  0.000000;0.000000;,
  0.100000;0.000000;,
  0.100000;0.250000;,
  0.000000;0.250000;,
  0.200000;0.000000;,
  0.200000;0.250000;,
  0.300000;0.000000;,
  0.300000;0.250000;,
  0.400000;0.000000;,
  0.400000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.600000;0.000000;,
  0.600000;0.250000;,
  0.700000;0.000000;,
  0.700000;0.250000;,
  0.800000;0.000000;,
  0.800000;0.250000;,
  0.900000;0.000000;,
  0.900000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.100000;0.500000;,
  0.000000;0.500000;,
  0.200000;0.500000;,
  0.300000;0.500000;,
  0.400000;0.500000;,
  0.500000;0.500000;,
  0.600000;0.500000;,
  0.700000;0.500000;,
  0.800000;0.500000;,
  0.900000;0.500000;,
  1.000000;0.500000;,
  0.100000;0.750000;,
  0.000000;0.750000;,
  0.200000;0.750000;,
  0.300000;0.750000;,
  0.400000;0.750000;,
  0.500000;0.750000;,
  0.600000;0.750000;,
  0.700000;0.750000;,
  0.800000;0.750000;,
  0.900000;0.750000;,
  1.000000;0.750000;,
  0.100000;1.000000;,
  0.000000;1.000000;,
  0.200000;1.000000;,
  0.300000;1.000000;,
  0.400000;1.000000;,
  0.500000;1.000000;,
  0.600000;1.000000;,
  0.700000;1.000000;,
  0.800000;1.000000;,
  0.900000;1.000000;,
  1.000000;1.000000;,
  0.050000;0.000000;,
  0.150000;0.000000;,
  0.250000;0.000000;,
  0.350000;0.000000;,
  0.450000;0.000000;,
  0.550000;0.000000;,
  0.650000;0.000000;,
  0.750000;0.000000;,
  0.850000;0.000000;,
  0.950000;0.000000;,
  0.050000;1.000000;,
  0.150000;1.000000;,
  0.250000;1.000000;,
  0.350000;1.000000;,
  0.450000;1.000000;,
  0.550000;1.000000;,
  0.650000;1.000000;,
  0.750000;1.000000;,
  0.850000;1.000000;,
  0.950000;1.000000;;
 }
}
