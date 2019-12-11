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
 65;
 0.00000;-2.65244;-0.00632;,
 1.55972;-2.14586;-0.00512;,
 1.55972;-1.53607;25.80011;,
 0.00000;-2.04264;25.79887;,
 2.52368;-0.81967;-0.00197;,
 2.52368;-0.20988;25.80329;,
 2.52368;0.81964;0.00194;,
 2.52368;1.42942;25.80720;,
 1.55972;2.14584;0.00513;,
 1.55972;2.75563;25.81032;,
 0.00000;2.65240;0.00633;,
 0.00000;3.26217;25.81158;,
 -1.55971;2.14584;0.00513;,
 -1.55971;2.75563;25.81032;,
 -2.52367;0.81964;0.00194;,
 -2.52367;1.42942;25.80720;,
 -2.52367;-0.81967;-0.00197;,
 -2.52367;-0.20988;25.80329;,
 -1.55971;-2.14586;-0.00512;,
 -1.55971;-1.53607;25.80011;,
 0.00000;-2.65244;-0.00632;,
 0.00000;-2.04264;25.79887;,
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
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 0.00000;0.60976;25.80520;,
 -0.41077;4.11306;26.10770;,
 1.97310;3.12565;26.10770;,
 -0.41077;3.12565;23.72385;,
 -0.41077;4.11306;26.10770;,
 -0.41077;3.12565;28.49158;,
 -0.41077;4.11306;26.10770;,
 -2.79464;3.12565;26.10770;,
 -0.41077;4.11306;26.10770;,
 -0.41077;3.12565;23.72385;,
 2.96054;0.74179;26.10770;,
 -0.41077;0.74179;22.73642;,
 -0.41077;0.74179;30.96427;,
 -3.78207;0.74179;26.10770;,
 -0.41077;0.74179;22.73642;,
 1.97310;-1.64210;26.10770;,
 -0.41077;-1.64210;23.72385;,
 -0.41077;-1.64210;28.49158;,
 -2.79464;-1.64210;26.10770;,
 -0.41077;-1.64210;23.72385;,
 -0.41077;-2.62953;26.10770;,
 -0.41077;-2.62953;26.10770;,
 -0.41077;-2.62953;26.10770;,
 -0.41077;-2.62953;26.10770;;
 
 46;
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
 3;22,1,0;,
 3;23,4,1;,
 3;24,6,4;,
 3;25,8,6;,
 3;26,10,8;,
 3;27,12,10;,
 3;28,14,12;,
 3;29,16,14;,
 3;30,18,16;,
 3;31,20,18;,
 3;32,3,2;,
 3;33,2,5;,
 3;34,5,7;,
 3;35,7,9;,
 3;36,9,11;,
 3;37,11,13;,
 3;38,13,15;,
 3;39,15,17;,
 3;40,17,19;,
 3;41,19,21;,
 3;42,43,44;,
 3;45,46,43;,
 3;47,48,46;,
 3;49,50,48;,
 4;44,43,51,52;,
 4;43,46,53,51;,
 4;46,48,54,53;,
 4;48,50,55,54;,
 4;52,51,56,57;,
 4;51,53,58,56;,
 4;53,54,59,58;,
 4;54,55,60,59;,
 3;57,56,61;,
 3;56,58,62;,
 3;58,59,63;,
 3;59,60,64;;
 
 MeshMaterialList {
  2;
  46;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.768800;0.194400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game313\\Desktop\\3dモデル作成\\3dゲームモデル\\写真\\giraffe\\giraffe_Texture.png";
   }
  }
  Material {
   0.800000;0.404706;0.119216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  51;
  0.000000;0.002387;-0.999997;,
  -0.000001;-0.999721;0.023624;,
  0.587547;-0.808964;0.019116;,
  0.950999;-0.309107;0.007304;,
  0.951000;0.309104;-0.007304;,
  0.587543;0.808967;-0.019116;,
  -0.000001;0.999721;-0.023624;,
  -0.587544;0.808967;-0.019116;,
  -0.951000;0.309104;-0.007304;,
  -0.950999;-0.309107;0.007304;,
  -0.587547;-0.808964;0.019116;,
  0.000000;-0.002386;0.999997;,
  0.000000;1.000000;-0.000002;,
  0.267625;0.791447;-0.549534;,
  0.656203;0.754087;-0.027375;,
  0.270390;0.820243;0.504074;,
  -0.540021;0.810858;0.225582;,
  0.707104;0.000002;-0.707110;,
  0.764218;0.000002;0.644958;,
  -0.764218;0.000002;0.644958;,
  0.267626;-0.791444;-0.549539;,
  0.549536;-0.791445;-0.267630;,
  0.270391;-0.820240;0.504079;,
  -0.540026;-0.810854;0.225583;,
  0.000000;-1.000000;-0.000002;,
  -0.000006;0.002382;-0.999997;,
  0.000000;0.002383;-0.999997;,
  -0.000007;0.002383;-0.999997;,
  -0.000008;0.002391;-0.999997;,
  -0.000002;0.002392;-0.999997;,
  -0.000000;0.002386;-0.999997;,
  0.000002;0.002392;-0.999997;,
  0.000008;0.002391;-0.999997;,
  0.000007;0.002383;-0.999997;,
  0.000006;0.002382;-0.999997;,
  0.000000;-0.002386;0.999997;,
  -0.000018;-0.002386;0.999997;,
  -0.000018;-0.002385;0.999997;,
  -0.000020;-0.002377;0.999997;,
  0.000001;-0.002387;0.999997;,
  -0.000000;-0.002405;0.999997;,
  -0.000001;-0.002387;0.999997;,
  0.000020;-0.002377;0.999997;,
  0.000018;-0.002385;0.999997;,
  0.000018;-0.002386;0.999997;,
  -0.700579;0.399517;0.591250;,
  -0.678596;0.281085;-0.678601;,
  -0.707104;0.000001;-0.707109;,
  0.700579;-0.399514;0.591251;,
  -0.700580;-0.399513;0.591251;,
  -0.678596;-0.281082;-0.678601;;
  46;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,1,1,10;,
  3;0,25,26;,
  3;0,27,25;,
  3;0,28,27;,
  3;0,29,28;,
  3;0,30,29;,
  3;0,31,30;,
  3;0,32,31;,
  3;0,33,32;,
  3;0,34,33;,
  3;0,26,34;,
  3;11,35,36;,
  3;11,36,37;,
  3;11,37,38;,
  3;11,38,39;,
  3;11,39,40;,
  3;11,40,41;,
  3;11,41,42;,
  3;11,42,43;,
  3;11,43,44;,
  3;11,44,35;,
  3;12,14,13;,
  3;12,15,14;,
  3;12,16,15;,
  3;12,13,16;,
  4;13,14,17,17;,
  4;14,15,18,18;,
  4;45,16,19,19;,
  4;46,46,47,47;,
  4;17,17,21,20;,
  4;18,18,22,48;,
  4;19,19,23,49;,
  4;47,47,50,50;,
  3;20,21,24;,
  3;21,22,24;,
  3;22,23,24;,
  3;23,20,24;;
 }
 MeshTextureCoords {
  65;
  0.000000;0.000000;,
  0.100000;0.000000;,
  0.100000;1.000000;,
  0.000000;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.300000;0.000000;,
  0.300000;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.700000;0.000000;,
  0.700000;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  0.900000;0.000000;,
  0.900000;1.000000;,
  1.000000;0.000000;,
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
  0.950000;1.000000;,
  0.125000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.375000;0.000000;,
  0.500000;0.250000;,
  0.625000;0.000000;,
  0.750000;0.250000;,
  0.875000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.125000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.875000;1.000000;;
 }
}
