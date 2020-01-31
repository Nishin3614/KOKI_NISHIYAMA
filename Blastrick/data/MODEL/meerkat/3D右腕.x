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
 104;
 -0.76552;57.72899;-5.00657;,
 -0.85073;55.61746;-5.00657;,
 0.60218;55.57354;-5.15042;,
 1.00817;57.68511;-5.15042;,
 1.72224;55.53970;-5.54900;,
 0.64992;60.40497;-7.87934;,
 2.25286;53.52869;-9.40422;,
 0.21489;60.16179;-9.79143;,
 2.07250;53.53414;-10.69044;,
 0.16099;59.68270;-10.55379;,
 1.22245;53.55982;-11.58529;,
 0.36417;58.29647;-11.11263;,
 -0.10253;53.59987;-11.64246;,
 -0.66100;58.33655;-11.33246;,
 -1.59892;53.64508;-11.11926;,
 -2.48778;58.38173;-11.11926;,
 -5.11013;55.68009;-11.21827;,
 -5.95233;58.49563;-10.29789;,
 -6.24156;55.70579;-10.19835;,
 -7.39898;60.00042;-8.65107;,
 -7.29027;53.71624;-7.74896;,
 -7.74885;59.62952;-5.94339;,
 -7.32621;53.70023;-6.61447;,
 -7.38293;55.81173;-5.33479;,
 -2.30363;55.66137;-5.15042;,
 -2.53922;57.77290;-5.15042;,
 -0.85073;55.61746;-5.00657;,
 -0.76552;57.72899;-5.00657;,
 -1.31257;47.93380;-7.33690;,
 -0.38248;48.86869;-7.48076;,
 1.45971;49.58941;-7.87934;,
 1.79939;49.93085;-9.40422;,
 1.68392;49.81480;-10.69044;,
 0.01458;49.26781;-11.58529;,
 -0.83362;48.41525;-11.64246;,
 -1.79152;47.45238;-11.11926;,
 -2.63970;46.59979;-11.06208;,
 -3.33754;46.52782;-10.68215;,
 -3.64210;46.41141;-9.40422;,
 -3.01206;46.76202;-8.15799;,
 -2.24264;46.99893;-7.48076;,
 -1.31257;47.93380;-7.33690;,
 0.42228;46.52004;-7.33690;,
 1.35235;47.45493;-7.48076;,
 2.06936;48.17565;-7.87934;,
 2.40905;48.51708;-9.40422;,
 2.29358;48.40103;-10.69044;,
 1.74941;47.85407;-11.58529;,
 0.90123;47.00148;-11.64246;,
 -0.05668;46.03861;-11.11926;,
 -0.90485;45.18602;-11.06208;,
 -2.65577;45.73292;-10.78019;,
 -1.44901;44.63908;-10.63884;,
 -1.56448;44.52299;-9.40422;,
 -2.77124;45.61687;-9.40422;,
 -1.22481;44.86444;-8.20129;,
 -2.43154;45.95830;-8.05995;,
 -0.50780;45.58516;-7.48076;,
 0.42228;46.52004;-7.33690;,
 2.15705;45.10633;-7.33690;,
 3.08713;46.04122;-7.48076;,
 3.80414;46.76193;-7.87934;,
 4.14382;47.10337;-9.40422;,
 4.02836;46.98732;-10.69044;,
 3.48421;46.44033;-11.58529;,
 2.63602;45.58775;-11.64246;,
 1.67811;44.62490;-11.11926;,
 0.82993;43.77232;-11.06208;,
 0.28577;43.22535;-10.57955;,
 0.17031;43.10929;-9.40422;,
 0.50998;43.45073;-8.26059;,
 1.22700;44.17145;-7.48076;,
 2.15705;45.10633;-7.33690;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -3.17270;64.28746;-7.97191;,
 -2.21871;46.27821;-7.87934;,
 -2.81597;46.05281;-10.96081;,
 -5.11013;55.68009;-11.21827;,
 -2.63970;46.59979;-11.06208;,
 -6.24156;55.70579;-10.19835;,
 -2.81597;46.05281;-10.96081;,
 -3.33754;46.52782;-10.68215;,
 -7.32621;53.70023;-6.61447;,
 -3.01206;46.76202;-8.15799;,
 -2.30363;55.66137;-5.15042;,
 -2.21871;46.27821;-7.87934;,
 -2.24264;46.99893;-7.48076;,
 -0.90485;45.18602;-11.06208;,
 -2.65577;45.73292;-10.78019;,
 -1.44901;44.63908;-10.63884;,
 -2.43154;45.95830;-8.05995;,
 -1.22481;44.86444;-8.20129;,
 -0.50780;45.58516;-7.48076;;
 
 89;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;1,28,29,2;,
 4;2,29,30,4;,
 4;4,30,31,6;,
 4;6,31,32,8;,
 4;8,32,33,10;,
 4;10,33,34,12;,
 4;12,34,35,14;,
 4;14,35,36,16;,
 4;18,37,38,20;,
 4;20,38,39,22;,
 4;24,40,41,26;,
 4;28,42,43,29;,
 4;29,43,44,30;,
 4;30,44,45,31;,
 4;31,45,46,32;,
 4;32,46,47,33;,
 4;33,47,48,34;,
 4;34,48,49,35;,
 4;35,49,50,36;,
 4;51,52,53,54;,
 4;54,53,55,56;,
 4;40,57,58,41;,
 4;42,59,60,43;,
 4;43,60,61,44;,
 4;44,61,62,45;,
 4;45,62,63,46;,
 4;46,63,64,47;,
 4;47,64,65,48;,
 4;48,65,66,49;,
 4;49,66,67,50;,
 4;50,67,68,52;,
 4;52,68,69,53;,
 4;53,69,70,55;,
 4;55,70,71,57;,
 4;57,71,72,58;,
 3;73,0,3;,
 3;74,3,5;,
 3;75,5,7;,
 3;76,7,9;,
 3;77,9,11;,
 3;78,11,13;,
 3;79,13,15;,
 3;80,15,17;,
 3;81,17,19;,
 3;82,19,21;,
 3;83,21,23;,
 3;84,23,25;,
 3;85,25,27;,
 3;86,39,56;,
 4;51,54,38,37;,
 3;87,51,37;,
 4;54,56,39,38;,
 3;88,89,90;,
 3;89,91,90;,
 3;90,91,92;,
 3;93,94,95;,
 3;94,96,95;,
 3;95,96,97;,
 3;89,98,91;,
 3;91,98,99;,
 3;99,98,100;,
 3;96,101,97;,
 3;101,102,97;,
 3;97,102,103;,
 4;70,67,62,61;,
 4;61,71,68,63;,
 4;65,63,69,68;,
 4;63,60,70,69;,
 4;64,61,72,70;,
 4;65,62,60,71;,
 4;68,66,60,72;,
 4;63,62,71,70;,
 4;61,60,68,67;,
 4;72,71,67,66;,
 4;70,69,64,63;,
 3;61,65,64;;
 
 MeshMaterialList {
  1;
  89;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  93;
  0.144359;0.422345;0.894866;,
  0.004725;0.209347;0.977830;,
  0.422186;0.321138;0.847720;,
  0.851953;0.470540;0.229713;,
  0.828674;0.493655;-0.263827;,
  0.808942;0.419135;-0.412237;,
  0.247812;0.339672;-0.907310;,
  0.002576;0.288438;-0.957495;,
  -0.154196;0.264030;-0.952109;,
  -0.358923;0.293518;-0.886014;,
  -0.793917;0.335028;-0.507398;,
  -0.973817;0.207438;-0.093007;,
  -0.998819;-0.048440;0.003710;,
  -0.100108;0.128471;0.986648;,
  0.066062;-0.153255;0.985976;,
  0.375956;-0.147213;0.914869;,
  0.961354;0.092404;0.259344;,
  0.995457;0.077257;-0.055651;,
  0.899061;0.081666;-0.430139;,
  0.439877;0.059880;-0.896059;,
  -0.114333;0.030370;-0.992978;,
  -0.186383;0.036554;-0.981797;,
  -0.391142;-0.008676;-0.920289;,
  -0.723305;-0.128830;-0.678405;,
  -0.960957;-0.254282;-0.109096;,
  -0.946818;-0.295231;0.127960;,
  -0.150659;-0.225316;0.962567;,
  0.066235;-0.150345;0.986412;,
  0.219407;-0.105132;0.969953;,
  0.716300;0.025604;0.697323;,
  0.987774;0.141584;0.065245;,
  0.856060;0.149928;-0.494655;,
  0.331431;0.074741;-0.940514;,
  -0.141238;-0.057510;-0.988304;,
  -0.172636;-0.076752;-0.981991;,
  -0.244669;-0.092120;-0.965221;,
  -0.446709;-0.321498;-0.834919;,
  -0.444187;-0.229601;0.866014;,
  -0.183930;-0.315948;0.930778;,
  -0.000000;-0.000000;1.000000;,
  0.145220;0.159917;0.976390;,
  0.547735;0.451701;0.704239;,
  0.794936;0.600137;0.088952;,
  0.689138;0.570760;-0.446454;,
  0.289295;0.276181;-0.916533;,
  -0.101148;-0.124120;-0.987098;,
  -0.130791;-0.160495;-0.978333;,
  -0.182895;-0.277094;-0.943275;,
  -0.521405;-0.647125;-0.556207;,
  -0.641835;-0.757033;0.122262;,
  -0.463823;-0.578773;0.670739;,
  -0.225675;-0.281343;0.932693;,
  -0.000000;-0.000000;1.000000;,
  0.151545;0.185962;0.970800;,
  0.462225;0.567200;0.681639;,
  0.629209;0.772106;0.089150;,
  0.577450;0.708593;-0.405521;,
  0.277454;0.340466;-0.898389;,
  -0.101148;-0.124120;-0.987098;,
  -0.130791;-0.160495;-0.978333;,
  -0.176375;-0.225881;-0.958055;,
  -0.520620;-0.650576;-0.552908;,
  -0.623908;-0.771219;0.126333;,
  -0.492660;-0.617614;0.613057;,
  -0.226418;-0.286681;0.930886;,
  -0.826065;-0.563114;0.022789;,
  -0.759230;-0.452502;-0.467774;,
  -0.556609;-0.408257;0.723541;,
  -0.578248;-0.646037;-0.498262;,
  -0.697940;-0.705712;0.121861;,
  -0.541032;-0.513524;0.666016;,
  0.005219;0.098460;0.995127;,
  -0.098997;-0.288745;0.952274;,
  0.315477;-0.355936;0.879650;,
  0.151450;0.622370;-0.767931;,
  -0.248453;0.330628;-0.910471;,
  -0.712451;0.701710;0.004062;,
  0.244193;0.175440;0.953725;,
  -0.995629;0.017662;-0.091712;,
  0.014419;0.484274;-0.874798;,
  0.708929;-0.705280;0.000003;,
  0.708931;-0.705277;0.000004;,
  0.708929;-0.705280;-0.000000;,
  0.708929;-0.705280;0.000004;,
  0.708932;-0.705277;-0.000001;,
  0.708930;-0.705279;0.000004;,
  0.708930;-0.705279;0.000002;,
  0.708930;-0.705279;0.000004;,
  0.708931;-0.705278;0.000006;,
  0.708930;-0.705279;0.000004;,
  0.708930;-0.705279;0.000006;,
  0.708932;-0.705277;0.000003;,
  0.708934;-0.705275;0.000001;;
  89;
  4;1,14,15,2;,
  4;2,15,16,3;,
  4;3,16,17,4;,
  4;4,17,18,5;,
  4;5,18,19,6;,
  4;6,19,20,7;,
  4;7,20,21,8;,
  4;8,21,22,9;,
  4;9,22,23,10;,
  4;10,23,24,11;,
  4;11,24,25,12;,
  4;71,72,26,13;,
  4;13,26,14,1;,
  4;14,27,28,15;,
  4;15,28,29,73;,
  4;16,29,30,17;,
  4;17,30,31,18;,
  4;18,31,32,19;,
  4;19,32,33,20;,
  4;20,33,34,21;,
  4;21,34,35,22;,
  4;23,66,65,24;,
  4;24,65,67,25;,
  4;26,38,27,14;,
  4;27,39,40,28;,
  4;28,40,41,29;,
  4;29,41,42,30;,
  4;30,42,43,31;,
  4;31,43,44,32;,
  4;32,44,45,33;,
  4;33,45,46,34;,
  4;34,46,47,35;,
  4;68,48,49,69;,
  4;69,49,50,70;,
  4;38,51,39,27;,
  4;39,52,53,40;,
  4;40,53,54,41;,
  4;41,54,55,42;,
  4;42,55,56,43;,
  4;43,56,57,44;,
  4;44,57,58,45;,
  4;45,58,59,46;,
  4;46,59,60,47;,
  4;47,60,61,48;,
  4;48,61,62,49;,
  4;49,62,63,50;,
  4;50,63,64,51;,
  4;51,64,52,39;,
  3;0,1,2;,
  3;0,2,3;,
  3;74,3,4;,
  3;74,4,5;,
  3;75,75,6;,
  3;74,6,7;,
  3;74,7,8;,
  3;74,8,9;,
  3;74,9,10;,
  3;76,10,11;,
  3;0,77,71;,
  3;0,71,13;,
  3;0,13,1;,
  3;37,67,70;,
  4;68,69,65,66;,
  3;36,68,66;,
  4;69,70,67,65;,
  3;22,35,23;,
  3;35,36,23;,
  3;23,36,66;,
  3;72,67,26;,
  3;67,37,26;,
  3;78,78,78;,
  3;35,47,36;,
  3;36,47,68;,
  3;68,47,48;,
  3;79,79,79;,
  3;70,50,38;,
  3;38,50,51;,
  4;80,81,82,83;,
  4;83,84,85,86;,
  4;87,86,88,85;,
  4;86,89,80,88;,
  4;90,83,91,80;,
  4;87,82,89,84;,
  4;85,92,89,91;,
  4;86,82,84,80;,
  4;83,89,85,81;,
  4;91,84,81,92;,
  4;80,88,90,86;,
  3;83,87,90;;
 }
 MeshTextureCoords {
  104;
  0.000000;0.000000;,
  0.000000;0.100000;,
  0.076920;0.100000;,
  0.076920;0.000000;,
  0.153850;0.100000;,
  0.153850;0.000000;,
  0.230770;0.100000;,
  0.230770;0.000000;,
  0.307690;0.100000;,
  0.307690;0.000000;,
  0.384620;0.100000;,
  0.384620;0.000000;,
  0.461540;0.100000;,
  0.461540;0.000000;,
  0.538460;0.100000;,
  0.538460;0.000000;,
  0.615380;0.100000;,
  0.615380;0.000000;,
  0.692310;0.100000;,
  0.692310;0.000000;,
  0.769230;0.100000;,
  0.769230;0.000000;,
  0.846150;0.100000;,
  0.846150;0.000000;,
  0.923080;0.100000;,
  0.923080;0.000000;,
  1.000000;0.100000;,
  1.000000;0.000000;,
  0.000000;0.200000;,
  0.076920;0.200000;,
  0.153850;0.200000;,
  0.230770;0.200000;,
  0.307690;0.200000;,
  0.384620;0.200000;,
  0.461540;0.200000;,
  0.538460;0.200000;,
  0.615380;0.200000;,
  0.692310;0.200000;,
  0.769230;0.200000;,
  0.846150;0.200000;,
  0.923080;0.200000;,
  1.000000;0.200000;,
  0.000000;0.300000;,
  0.076920;0.300000;,
  0.153850;0.300000;,
  0.230770;0.300000;,
  0.307690;0.300000;,
  0.384620;0.300000;,
  0.461540;0.300000;,
  0.538460;0.300000;,
  0.615380;0.300000;,
  0.692310;0.200000;,
  0.692310;0.300000;,
  0.769230;0.300000;,
  0.769230;0.200000;,
  0.846150;0.300000;,
  0.846150;0.200000;,
  0.923080;0.300000;,
  1.000000;0.300000;,
  0.000000;0.400000;,
  0.076920;0.400000;,
  0.153850;0.400000;,
  0.230770;0.400000;,
  0.307690;0.400000;,
  0.384620;0.400000;,
  0.461540;0.400000;,
  0.538460;0.400000;,
  0.615380;0.400000;,
  0.692310;0.400000;,
  0.769230;0.400000;,
  0.846150;0.400000;,
  0.923080;0.400000;,
  1.000000;0.400000;,
  0.038460;0.000000;,
  0.115380;0.000000;,
  0.192310;0.000000;,
  0.269230;0.000000;,
  0.346150;0.000000;,
  0.423080;0.000000;,
  0.500000;0.000000;,
  0.576920;0.000000;,
  0.653850;0.000000;,
  0.730770;0.000000;,
  0.807690;0.000000;,
  0.884620;0.000000;,
  0.961540;0.000000;,
  0.846150;0.200000;,
  0.692310;0.200000;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
