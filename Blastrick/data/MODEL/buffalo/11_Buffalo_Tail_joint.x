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
 54;
 0.00000;0.00000;0.00000;,
 0.25591;-0.13509;0.43439;,
 0.50415;-0.13509;-0.00443;,
 0.00000;0.00000;0.00000;,
 -0.24828;-0.13509;0.43878;,
 0.00000;0.00000;0.00000;,
 -0.50414;-0.13509;0.00433;,
 0.00000;0.00000;0.00000;,
 -0.25588;-0.13509;-0.43445;,
 0.00000;0.00000;0.00000;,
 0.24827;-0.13509;-0.43883;,
 0.00000;0.00000;0.00000;,
 0.50415;-0.13509;-0.00443;,
 0.44321;-0.50417;0.75242;,
 0.87321;-0.50417;-0.00762;,
 -0.43001;-0.50417;0.76000;,
 -0.87323;-0.50417;0.00757;,
 -0.44320;-0.50417;-0.75244;,
 0.43002;-0.50417;-0.76010;,
 0.87321;-0.50417;-0.00762;,
 0.51178;-1.00835;0.86876;,
 1.00832;-1.00835;-0.00886;,
 -0.49655;-1.00835;0.87758;,
 -1.00831;-1.00835;0.00878;,
 -0.51179;-1.00835;-0.86887;,
 0.49652;-1.00835;-0.87762;,
 1.00832;-1.00835;-0.00886;,
 0.44321;-1.51252;0.75242;,
 0.87321;-1.51252;-0.00762;,
 -0.43001;-1.51252;0.76000;,
 -0.87323;-1.51252;0.00757;,
 -0.44320;-1.51252;-0.75244;,
 0.43002;-1.51252;-0.76010;,
 0.87321;-1.51252;-0.00762;,
 0.25591;-1.88160;0.43439;,
 0.50415;-1.88160;-0.00443;,
 -0.24828;-1.88160;0.43878;,
 -0.50414;-1.88160;0.00433;,
 -0.25588;-1.88160;-0.43445;,
 0.24827;-1.88160;-0.43883;,
 0.50415;-1.88160;-0.00443;,
 0.50415;-1.88160;-0.00443;,
 0.25591;-1.88160;0.43439;,
 0.00000;-2.01670;-0.00000;,
 -0.24828;-1.88160;0.43878;,
 0.00000;-2.01670;-0.00000;,
 -0.50414;-1.88160;0.00433;,
 0.00000;-2.01670;-0.00000;,
 -0.25588;-1.88160;-0.43445;,
 0.00000;-2.01670;-0.00000;,
 0.24827;-1.88160;-0.43883;,
 0.00000;-2.01670;-0.00000;,
 0.50415;-1.88160;-0.00443;,
 0.00000;-2.01670;-0.00000;;
 
 36;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 4;2,1,13,14;,
 4;1,4,15,13;,
 4;4,6,16,15;,
 4;6,8,17,16;,
 4;8,10,18,17;,
 4;10,12,19,18;,
 4;14,13,20,21;,
 4;13,15,22,20;,
 4;15,16,23,22;,
 4;16,17,24,23;,
 4;17,18,25,24;,
 4;18,19,26,25;,
 4;21,20,27,28;,
 4;20,22,29,27;,
 4;22,23,30,29;,
 4;23,24,31,30;,
 4;24,25,32,31;,
 4;25,26,33,32;,
 4;28,27,34,35;,
 4;27,29,36,34;,
 4;29,30,37,36;,
 4;30,31,38,37;,
 4;31,32,39,38;,
 4;32,33,40,39;,
 3;41,42,43;,
 3;42,44,45;,
 3;44,46,47;,
 3;46,48,49;,
 3;48,50,51;,
 3;50,52,53;;
 
 MeshMaterialList {
  5;
  36;
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
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.216471;0.216471;0.241569;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.721569;0.721569;0.721569;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.360784;0.360784;0.401569;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.480000;0.241569;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  32;
  -0.000001;1.000000;0.000002;,
  0.492306;0.870411;-0.004297;,
  0.249872;0.870414;0.424196;,
  -0.242451;0.870411;0.428489;,
  -0.492309;0.870410;0.004280;,
  -0.249863;0.870417;-0.424196;,
  0.242432;0.870420;-0.428480;,
  0.862067;0.506738;-0.007520;,
  0.437548;0.506730;0.742816;,
  -0.424532;0.506739;0.750325;,
  -0.862058;0.506754;0.007508;,
  -0.437539;0.506750;-0.742809;,
  0.424520;0.506753;-0.750323;,
  0.999962;0.000001;-0.008722;,
  0.507531;0.000001;0.861633;,
  -0.492426;0.000001;0.870354;,
  -0.999962;0.000001;0.008722;,
  -0.507531;0.000001;-0.861633;,
  0.492425;0.000001;-0.870355;,
  0.862067;-0.506739;-0.007520;,
  0.437548;-0.506728;0.742818;,
  -0.424528;-0.506743;0.750325;,
  -0.862055;-0.506760;0.007506;,
  -0.437539;-0.506750;-0.742808;,
  0.424519;-0.506753;-0.750323;,
  0.492307;-0.870411;-0.004297;,
  0.249873;-0.870413;0.424199;,
  -0.242447;-0.870412;0.428488;,
  -0.492305;-0.870412;0.004278;,
  -0.249863;-0.870417;-0.424196;,
  0.242432;-0.870420;-0.428481;,
  -0.000001;-1.000000;0.000002;;
  36;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,1,6;,
  4;1,2,8,7;,
  4;2,3,9,8;,
  4;3,4,10,9;,
  4;4,5,11,10;,
  4;5,6,12,11;,
  4;6,1,7,12;,
  4;7,8,14,13;,
  4;8,9,15,14;,
  4;9,10,16,15;,
  4;10,11,17,16;,
  4;11,12,18,17;,
  4;12,7,13,18;,
  4;13,14,20,19;,
  4;14,15,21,20;,
  4;15,16,22,21;,
  4;16,17,23,22;,
  4;17,18,24,23;,
  4;18,13,19,24;,
  4;19,20,26,25;,
  4;20,21,27,26;,
  4;21,22,28,27;,
  4;22,23,29,28;,
  4;23,24,30,29;,
  4;24,19,25,30;,
  3;25,26,31;,
  3;26,27,31;,
  3;27,28,31;,
  3;28,29,31;,
  3;29,30,31;,
  3;30,25,31;;
 }
 MeshTextureCoords {
  54;
  0.083333;0.000000;,
  0.166667;0.166667;,
  0.000000;0.166667;,
  0.250000;0.000000;,
  0.333333;0.166667;,
  0.416667;0.000000;,
  0.500000;0.166667;,
  0.583333;0.000000;,
  0.666667;0.166667;,
  0.750000;0.000000;,
  0.833333;0.166667;,
  0.916667;0.000000;,
  1.000000;0.166667;,
  0.166667;0.333333;,
  0.000000;0.333333;,
  0.333333;0.333333;,
  0.500000;0.333333;,
  0.666667;0.333333;,
  0.833333;0.333333;,
  1.000000;0.333333;,
  0.166667;0.500000;,
  0.000000;0.500000;,
  0.333333;0.500000;,
  0.500000;0.500000;,
  0.666667;0.500000;,
  0.833333;0.500000;,
  1.000000;0.500000;,
  0.166667;0.666667;,
  0.000000;0.666667;,
  0.333333;0.666667;,
  0.500000;0.666667;,
  0.666667;0.666667;,
  0.833333;0.666667;,
  1.000000;0.666667;,
  0.166667;0.833333;,
  0.000000;0.833333;,
  0.333333;0.833333;,
  0.500000;0.833333;,
  0.666667;0.833333;,
  0.833333;0.833333;,
  1.000000;0.833333;,
  0.000000;0.833333;,
  0.166667;0.833333;,
  0.083333;1.000000;,
  0.333333;0.833333;,
  0.250000;1.000000;,
  0.500000;0.833333;,
  0.416667;1.000000;,
  0.666667;0.833333;,
  0.583333;1.000000;,
  0.833333;0.833333;,
  0.750000;1.000000;,
  1.000000;0.833333;,
  0.916667;1.000000;;
 }
}