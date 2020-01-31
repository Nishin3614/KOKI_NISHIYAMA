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
 147;
 0.00000;0.00000;0.00000;,
 0.00000;-3.77410;-18.32843;,
 6.14943;-0.47936;-1.11661;,
 0.00000;-3.77410;-18.32843;,
 8.69661;5.04773;-3.81236;,
 0.00000;-3.77410;-18.32843;,
 6.14943;10.57479;-6.50809;,
 0.00000;-3.77410;-18.32843;,
 -0.00001;11.66700;-6.50787;,
 0.00000;-3.77410;-18.32843;,
 -6.14944;10.57479;-6.50809;,
 0.00000;-3.77410;-18.32843;,
 -8.69662;5.04773;-3.81236;,
 0.00000;-3.77410;-18.32843;,
 -6.14944;-0.47936;-1.11661;,
 0.00000;-3.77410;-18.32843;,
 0.00000;0.00000;0.00000;,
 0.00000;7.06859;-0.77680;,
 0.00000;0.00000;0.00000;,
 6.14943;-0.47936;-1.11661;,
 8.69661;5.04773;-3.81236;,
 6.14943;10.57479;-6.50809;,
 -0.00001;11.66700;-6.50787;,
 -6.14944;10.57479;-6.50809;,
 -8.69662;5.04773;-3.81236;,
 -6.14944;-0.47936;-1.11661;,
 0.25134;1.99103;-17.48687;,
 2.63521;1.00361;-17.48687;,
 0.25134;1.00361;-19.87072;,
 0.25134;1.99103;-17.48687;,
 0.25134;1.00361;-15.10299;,
 0.25134;1.99103;-17.48687;,
 -2.13253;1.00361;-17.48687;,
 0.25134;1.99103;-17.48687;,
 0.25134;1.00361;-19.87072;,
 3.62265;-1.38025;-17.48687;,
 0.25134;-1.38025;-20.85815;,
 0.25134;-1.38025;-14.11556;,
 -3.11996;-1.38025;-17.48687;,
 0.25134;-1.38025;-20.85815;,
 2.63521;-3.76413;-17.48687;,
 0.25134;-3.76413;-19.87072;,
 0.25134;-3.76413;-15.10299;,
 -2.13253;-3.76413;-17.48687;,
 0.25134;-3.76413;-19.87072;,
 0.25134;-4.75156;-17.48687;,
 0.25134;-4.75156;-17.48687;,
 0.25134;-4.75156;-17.48687;,
 0.25134;-4.75156;-17.48687;,
 0.70605;7.20443;-9.85236;,
 1.19607;7.43011;-9.63328;,
 1.44608;6.82324;-10.09888;,
 0.37254;6.91677;-10.10859;,
 2.11435;7.58958;-9.43352;,
 2.15702;6.90739;-9.97563;,
 3.05257;7.43011;-9.48717;,
 2.87847;6.82324;-9.98615;,
 3.57083;7.20443;-9.62690;,
 3.94031;6.91677;-9.82781;,
 1.31179;6.44210;-10.41425;,
 0.21510;6.44210;-10.50055;,
 2.18612;6.44210;-10.34543;,
 3.06044;6.44210;-10.27661;,
 4.15713;6.44210;-10.19031;,
 1.49376;6.06096;-10.70473;,
 0.43192;5.96744;-10.86307;,
 2.21522;5.97681;-10.71524;,
 2.92615;6.06096;-10.59199;,
 3.99969;5.96744;-10.58228;,
 1.31966;5.45410;-11.20370;,
 0.80141;5.67979;-11.06398;,
 2.25789;5.29464;-11.25735;,
 3.17617;5.45410;-11.05759;,
 3.66618;5.67979;-10.83851;,
 -3.71018;7.15109;-9.58751;,
 -3.18092;7.37678;-9.49810;,
 -3.05545;6.76991;-10.01146;,
 -4.09723;6.86343;-9.75207;,
 -2.24187;7.53624;-9.53462;,
 -2.33726;6.84715;-10.07557;,
 -1.34697;7.37678;-9.82147;,
 -1.64046;6.76991;-10.26096;,
 -0.88021;7.15109;-10.08651;,
 -0.57279;6.86343;-10.37352;,
 -3.25159;6.38877;-10.28541;,
 -4.34779;6.38877;-10.09213;,
 -2.40071;6.38877;-10.43545;,
 -1.54984;6.38877;-10.58549;,
 -0.45365;6.38877;-10.77877;,
 -3.16098;6.00762;-10.60995;,
 -4.22865;5.91411;-10.49738;,
 -2.46417;5.93038;-10.79535;,
 -1.74599;6.00762;-10.85945;,
 -0.70421;5.91411;-11.11884;,
 -3.45446;5.40076;-11.04944;,
 -3.92123;5.62645;-10.78440;,
 -2.55956;5.24130;-11.33629;,
 -1.62051;5.40076;-11.37281;,
 -1.09125;5.62645;-11.28340;,
 -6.38725;7.01650;-6.38168;,
 -13.27962;8.19352;-2.24197;,
 -5.44188;8.09438;-6.44829;,
 -13.27962;8.19352;-2.24197;,
 -6.10314;5.47560;-5.18779;,
 -13.27962;8.19352;-2.24197;,
 -6.38725;7.01650;-6.38168;,
 -4.81052;6.35141;-4.54948;,
 -6.38725;7.01650;-6.38168;,
 -5.44188;8.09438;-6.44829;,
 -6.10314;5.47560;-5.18779;,
 6.32863;5.99296;-6.38168;,
 15.23029;6.57916;-1.37155;,
 6.34715;4.71271;-5.57786;,
 15.23029;6.57916;-1.37155;,
 7.60188;7.41366;-5.18779;,
 15.23029;6.57916;-1.37155;,
 6.32863;5.99296;-6.38168;,
 6.56971;6.24209;-4.54948;,
 6.32863;5.99296;-6.38168;,
 6.34715;4.71271;-5.57786;,
 7.60188;7.41366;-5.18779;,
 -0.41185;8.45264;-8.36415;,
 -0.41185;16.28213;-14.28617;,
 0.84089;8.83523;-8.01357;,
 -0.41185;16.28213;-14.28617;,
 1.35979;9.75883;-7.16723;,
 -0.41185;16.28213;-14.28617;,
 0.84089;10.68246;-6.32091;,
 -0.41185;16.28213;-14.28617;,
 -0.41185;11.06505;-5.97032;,
 -0.41185;16.28213;-14.28617;,
 -1.66460;10.68246;-6.32091;,
 -0.41185;16.28213;-14.28617;,
 -2.18350;9.75883;-7.16723;,
 -0.41185;16.28213;-14.28617;,
 -1.66460;8.83523;-8.01357;,
 -0.41185;16.28213;-14.28617;,
 -0.41185;8.45264;-8.36415;,
 -0.41185;9.75883;-7.16723;,
 -0.41185;8.45264;-8.36415;,
 0.84089;8.83523;-8.01357;,
 1.35979;9.75883;-7.16723;,
 0.84089;10.68246;-6.32091;,
 -0.41185;11.06505;-5.97032;,
 -1.66460;10.68246;-6.32091;,
 -2.18350;9.75883;-7.16723;,
 -1.66460;8.83523;-8.01357;;
 
 92;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;,
 3;26,27,28;,
 3;29,30,27;,
 3;31,32,30;,
 3;33,34,32;,
 4;28,27,35,36;,
 4;27,30,37,35;,
 4;30,32,38,37;,
 4;32,34,39,38;,
 4;36,35,40,41;,
 4;35,37,42,40;,
 4;37,38,43,42;,
 4;38,39,44,43;,
 3;41,40,45;,
 3;40,42,46;,
 3;42,43,47;,
 3;43,44,48;,
 4;49,50,51,52;,
 4;50,53,54,51;,
 4;53,55,56,54;,
 4;55,57,58,56;,
 4;52,51,59,60;,
 4;51,54,61,59;,
 4;54,56,62,61;,
 4;56,58,63,62;,
 4;60,59,64,65;,
 4;59,61,66,64;,
 4;61,62,67,66;,
 4;62,63,68,67;,
 4;65,64,69,70;,
 4;64,66,71,69;,
 4;66,67,72,71;,
 4;67,68,73,72;,
 4;74,75,76,77;,
 4;75,78,79,76;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;77,76,84,85;,
 4;76,79,86,84;,
 4;79,81,87,86;,
 4;81,83,88,87;,
 4;85,84,89,90;,
 4;84,86,91,89;,
 4;86,87,92,91;,
 4;87,88,93,92;,
 4;90,89,94,95;,
 4;89,91,96,94;,
 4;91,92,97,96;,
 4;92,93,98,97;,
 3;99,100,101;,
 3;101,102,103;,
 3;103,104,105;,
 3;106,107,108;,
 3;106,108,109;,
 3;106,109,107;,
 3;110,111,112;,
 3;112,113,114;,
 3;114,115,116;,
 3;117,118,119;,
 3;117,119,120;,
 3;117,120,118;,
 3;121,122,123;,
 3;123,124,125;,
 3;125,126,127;,
 3;127,128,129;,
 3;129,130,131;,
 3;131,132,133;,
 3;133,134,135;,
 3;135,136,137;,
 3;138,139,140;,
 3;138,140,141;,
 3;138,141,142;,
 3;138,142,143;,
 3;138,143,144;,
 3;138,144,145;,
 3;138,145,146;,
 3;138,146,139;;
 
 MeshMaterialList {
  4;
  92;
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
  1,
  3,
  3,
  3,
  3,
  3,
  1,
  1,
  3,
  3,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  1,
  1,
  3,
  3,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
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
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.768800;0.194400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/Model_Texture/00_giraffe_Texture.png";
   }
  }
  Material {
   0.800000;0.404800;0.119200;1.000000;;
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
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  122;
  -0.000000;-0.979451;0.201684;,
  -0.039697;-0.978679;0.201525;,
  0.889512;-0.204827;-0.408430;,
  0.521042;0.398489;-0.754799;,
  -0.000000;0.607861;-0.794044;,
  -0.521042;0.398489;-0.754800;,
  -0.889512;-0.204827;-0.408430;,
  -0.845765;-0.491287;-0.208131;,
  0.000000;0.462130;0.886812;,
  0.000000;1.000000;-0.000002;,
  0.267625;0.791446;-0.549536;,
  0.549533;0.791447;-0.267629;,
  0.267627;0.791448;0.549533;,
  -0.549535;0.791447;0.267625;,
  0.707104;0.000001;-0.707110;,
  0.707107;0.000001;0.707106;,
  -0.707108;0.000001;0.707106;,
  0.267626;-0.791444;-0.549539;,
  0.549535;-0.791445;-0.267630;,
  0.267627;-0.791446;0.549535;,
  -0.549537;-0.791445;0.267625;,
  0.000000;-1.000000;-0.000002;,
  0.061386;0.623320;-0.779554;,
  0.061367;0.623326;-0.779551;,
  0.061351;0.623331;-0.779547;,
  0.061334;0.623328;-0.779551;,
  0.061314;0.623324;-0.779556;,
  0.061368;0.623343;-0.779537;,
  0.061359;0.623348;-0.779533;,
  0.061352;0.623351;-0.779532;,
  0.061340;0.623342;-0.779540;,
  0.061327;0.623336;-0.779546;,
  0.061344;0.623370;-0.779517;,
  0.061350;0.623375;-0.779513;,
  0.061353;0.623377;-0.779510;,
  0.061348;0.623370;-0.779517;,
  0.061344;0.623365;-0.779521;,
  0.061297;0.623314;-0.779566;,
  0.061327;0.623337;-0.779545;,
  0.061354;0.623362;-0.779522;,
  0.061356;0.623360;-0.779524;,
  0.061362;0.623356;-0.779527;,
  0.061255;0.623252;-0.779618;,
  0.061306;0.623296;-0.779580;,
  0.061353;0.623340;-0.779541;,
  0.061363;0.623335;-0.779544;,
  0.061377;0.623330;-0.779547;,
  -0.135761;0.623315;-0.770096;,
  -0.135775;0.623324;-0.770086;,
  -0.135784;0.623333;-0.770077;,
  -0.135798;0.623324;-0.770082;,
  -0.135816;0.623315;-0.770086;,
  -0.135768;0.623338;-0.770076;,
  -0.135777;0.623344;-0.770070;,
  -0.135783;0.623352;-0.770062;,
  -0.135789;0.623350;-0.770063;,
  -0.135798;0.623344;-0.770065;,
  -0.135778;0.623369;-0.770049;,
  -0.135780;0.623372;-0.770046;,
  -0.135781;0.623376;-0.770043;,
  -0.135779;0.623374;-0.770045;,
  -0.135778;0.623372;-0.770047;,
  -0.135797;0.623350;-0.770061;,
  -0.135790;0.623355;-0.770058;,
  -0.135782;0.623357;-0.770058;,
  -0.135776;0.623349;-0.770065;,
  -0.135771;0.623344;-0.770071;,
  -0.135813;0.623324;-0.770079;,
  -0.135798;0.623329;-0.770077;,
  -0.135784;0.623335;-0.770076;,
  -0.135775;0.623326;-0.770084;,
  -0.135765;0.623318;-0.770092;,
  -0.527451;-0.143451;-0.837387;,
  -0.442030;0.336300;-0.831572;,
  0.455384;0.290144;0.841690;,
  0.579983;-0.465912;-0.668241;,
  0.492991;-0.091374;-0.865223;,
  0.456586;-0.468332;-0.756435;,
  -0.439577;0.074940;0.895073;,
  -0.920641;0.384286;0.068885;,
  0.000001;-0.603248;-0.797553;,
  0.695502;-0.390848;-0.602922;,
  0.983581;0.121921;-0.133057;,
  0.695504;0.634692;0.336810;,
  0.000001;0.847096;0.531439;,
  -0.695503;0.634693;0.336810;,
  -0.983581;0.121922;-0.133057;,
  -0.695501;-0.390848;-0.602923;,
  0.000000;-0.675588;0.737279;,
  0.845765;-0.491287;-0.208131;,
  -0.311576;0.498200;-0.809146;,
  0.039697;-0.978679;0.201525;,
  0.000000;0.109237;0.994016;,
  0.281121;0.186654;0.941346;,
  0.383912;0.407527;0.828573;,
  0.266239;0.668371;0.694548;,
  -0.000000;0.779968;0.625819;,
  -0.266238;0.668371;0.694549;,
  -0.383912;0.407527;0.828573;,
  -0.281121;0.186654;0.941346;,
  0.678598;0.281087;0.678597;,
  -0.678599;0.281086;0.678597;,
  -0.678596;0.281085;-0.678601;,
  -0.707104;0.000001;-0.707109;,
  0.678599;-0.281085;0.678598;,
  -0.678600;-0.281083;0.678597;,
  -0.678596;-0.281083;-0.678601;,
  -0.487607;-0.589133;-0.644331;,
  0.545549;-0.519140;-0.657929;,
  -0.623989;0.461196;0.630822;,
  0.612025;-0.410762;-0.675796;,
  -0.812492;0.077909;0.577742;,
  0.452856;0.299771;-0.839678;,
  0.773919;-0.427059;0.467621;,
  -0.748088;0.663549;0.008208;,
  0.000000;-0.675595;0.737273;,
  0.000001;-0.675596;0.737272;,
  0.000000;-0.675587;0.737280;,
  0.000005;-0.675581;0.737286;,
  0.000000;-0.675584;0.737283;,
  -0.000005;-0.675581;0.737286;,
  -0.000001;-0.675596;0.737272;;
  92;
  3;0,0,1;,
  3;89,2,2;,
  3;2,2,3;,
  3;3,90,4;,
  3;4,90,5;,
  3;5,90,6;,
  3;6,7,7;,
  3;91,0,0;,
  3;8,92,93;,
  3;8,93,94;,
  3;8,94,95;,
  3;8,95,96;,
  3;8,96,97;,
  3;8,97,98;,
  3;8,98,99;,
  3;8,99,92;,
  3;9,11,10;,
  3;9,12,11;,
  3;9,13,12;,
  3;9,10,13;,
  4;10,11,14,14;,
  4;100,12,15,15;,
  4;101,13,16,16;,
  4;102,102,103,103;,
  4;14,14,18,17;,
  4;15,15,19,104;,
  4;16,16,20,105;,
  4;103,103,106,106;,
  3;17,18,21;,
  3;18,19,21;,
  3;19,20,21;,
  3;20,17,21;,
  4;22,23,28,27;,
  4;23,24,29,28;,
  4;24,25,30,29;,
  4;25,26,31,30;,
  4;27,28,33,32;,
  4;28,29,34,33;,
  4;29,30,35,34;,
  4;30,31,36,35;,
  4;32,33,38,37;,
  4;33,34,39,38;,
  4;34,35,40,39;,
  4;35,36,41,40;,
  4;37,38,43,42;,
  4;38,39,44,43;,
  4;39,40,45,44;,
  4;40,41,46,45;,
  4;47,48,53,52;,
  4;48,49,54,53;,
  4;49,50,55,54;,
  4;50,51,56,55;,
  4;52,53,58,57;,
  4;53,54,59,58;,
  4;54,55,60,59;,
  4;55,56,61,60;,
  4;57,58,63,62;,
  4;58,59,64,63;,
  4;59,60,65,64;,
  4;60,61,66,65;,
  4;62,63,68,67;,
  4;63,64,69,68;,
  4;64,65,70,69;,
  4;65,66,71,70;,
  3;72,72,73;,
  3;74,74,74;,
  3;107,72,72;,
  3;75,75,108;,
  3;109,109,109;,
  3;75,110,75;,
  3;76,76,77;,
  3;111,78,78;,
  3;112,76,76;,
  3;79,79,111;,
  3;113,113,113;,
  3;79,114,79;,
  3;80,81,81;,
  3;81,81,82;,
  3;82,83,83;,
  3;83,83,84;,
  3;84,85,85;,
  3;85,85,86;,
  3;86,87,87;,
  3;87,87,80;,
  3;88,115,116;,
  3;88,116,117;,
  3;88,117,118;,
  3;88,118,119;,
  3;88,119,120;,
  3;88,120,117;,
  3;88,117,121;,
  3;88,121,115;;
 }
 MeshTextureCoords {
  147;
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
  0.875000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
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
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
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
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
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
  0.000000;0.000000;;
 }
}
