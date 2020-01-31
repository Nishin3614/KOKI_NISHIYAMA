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
 23;
 0.00000;0.00000;0.00000;,
 1.24031;-0.51376;-0.00000;,
 0.00000;-0.51376;-1.24031;,
 0.00000;0.00000;0.00000;,
 0.00000;-0.51376;1.24030;,
 0.00000;0.00000;0.00000;,
 -1.24030;-0.51376;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;-0.51376;-1.24031;,
 1.75406;-1.75406;-0.00000;,
 0.00000;-1.75406;-1.75406;,
 0.00000;-1.75406;1.75405;,
 -1.75406;-1.75406;0.00000;,
 0.00000;-1.75406;-1.75406;,
 1.24031;-2.99437;-0.00000;,
 0.00000;-2.99437;-1.24031;,
 0.00000;-2.99437;1.24030;,
 -1.24030;-2.99437;0.00000;,
 0.00000;-2.99437;-1.24031;,
 0.00000;-3.50813;-0.00000;,
 0.00000;-3.50813;-0.00000;,
 0.00000;-3.50813;-0.00000;,
 0.00000;-3.50813;-0.00000;;
 
 16;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 4;2,1,9,10;,
 4;1,4,11,9;,
 4;4,6,12,11;,
 4;6,8,13,12;,
 4;10,9,14,15;,
 4;9,11,16,14;,
 4;11,12,17,16;,
 4;12,13,18,17;,
 3;15,14,19;,
 3;14,16,20;,
 3;16,17,21;,
 3;17,18,22;;
 
 MeshMaterialList {
  1;
  16;
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
    "data/TEXTURE/Model_Texture/00_giraffe_Texture.png";
   }
  }
 }
 MeshNormals {
  20;
  -0.000001;1.000000;0.000001;,
  0.267626;0.791442;-0.549541;,
  0.549541;0.791442;-0.267626;,
  0.267625;0.791441;0.549543;,
  -0.549542;0.791441;0.267628;,
  0.707107;0.000002;-0.707107;,
  0.707105;0.000002;0.707109;,
  -0.707106;0.000002;0.707108;,
  0.267627;-0.791443;-0.549539;,
  0.549539;-0.791443;-0.267627;,
  0.267626;-0.791442;0.549542;,
  -0.549540;-0.791442;0.267628;,
  -0.000001;-1.000000;0.000001;,
  0.678596;0.281085;0.678600;,
  -0.678597;0.281088;0.678599;,
  -0.678599;0.281089;-0.678596;,
  -0.707108;0.000002;-0.707105;,
  0.678597;-0.281082;0.678601;,
  -0.678598;-0.281085;0.678599;,
  -0.678600;-0.281085;-0.678597;;
  16;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,1,4;,
  4;1,2,5,5;,
  4;13,3,6,6;,
  4;14,4,7,7;,
  4;15,15,16,16;,
  4;5,5,9,8;,
  4;6,6,10,17;,
  4;7,7,11,18;,
  4;16,16,19,19;,
  3;8,9,12;,
  3;9,10,12;,
  3;10,11,12;,
  3;11,8,12;;
 }
 MeshTextureCoords {
  23;
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