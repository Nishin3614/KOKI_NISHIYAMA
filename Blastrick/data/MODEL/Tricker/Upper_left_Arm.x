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
 126;
 7.11132;2.37044;5.92610;,
 7.38636;4.15377;4.04527;,
 1.10148;4.55009;3.30125;,
 0.93905;3.02645;4.73178;,
 0.94284;-0.68733;-5.85090;,
 0.85801;1.84916;-6.06335;,
 2.31909;2.12373;-5.72124;,
 2.41214;-0.43771;-5.78963;,
 9.30846;1.92338;6.17713;,
 9.32360;0.93414;6.49052;,
 10.70750;0.92119;6.65464;,
 10.86882;2.48478;5.79678;,
 6.98269;-1.60080;6.18785;,
 7.11759;-2.98804;5.68077;,
 9.12123;-3.51525;5.93519;,
 9.14155;-1.64608;6.53728;,
 9.31513;5.01071;-1.99166;,
 9.39705;6.10396;-0.50901;,
 10.53325;5.85437;-0.81733;,
 10.57778;5.25957;-1.93799;,
 9.30118;2.62218;-5.68214;,
 9.50782;4.54301;-3.55813;,
 10.66690;4.55696;-3.51041;,
 10.45841;2.49390;-5.81737;,
 9.23806;0.28779;-5.95055;,
 10.48366;0.50692;-6.03021;,
 9.32754;-0.98369;-5.54217;,
 10.64931;-0.70714;-5.71389;,
 9.52481;-5.59026;1.40066;,
 10.85288;-5.80128;1.96930;,
 10.78329;-5.38137;4.50402;,
 9.37487;-5.41919;4.09959;,
 9.19280;5.13900;2.85083;,
 9.31438;4.20312;4.43365;,
 10.74725;4.50918;4.47932;,
 10.75067;5.22798;3.34295;,
 9.24059;-4.29792;5.09660;,
 7.32798;-3.67465;5.16908;,
 7.62432;-5.13523;3.72437;,
 12.10770;1.86032;-3.18751;,
 12.07234;0.86300;-3.66890;,
 12.05143;-0.18943;-3.68358;,
 12.04089;-1.33838;-3.27678;,
 10.64287;-2.23573;-5.38619;,
 12.16477;-3.77162;1.04388;,
 12.06994;-3.64510;2.66466;,
 10.78594;-3.63147;6.13469;,
 10.88366;-1.63235;6.65895;,
 7.18604;0.84079;5.93583;,
 10.77621;-3.02433;-4.17733;,
 9.42045;-3.21696;-4.01895;,
 9.33798;-2.42915;-5.34305;,
 12.16551;-2.64814;-1.92108;,
 12.12052;-3.52635;-0.39961;,
 10.72263;-5.60990;-0.75387;,
 10.65396;-4.35108;-3.00520;,
 9.68619;-5.48610;-1.07961;,
 9.45918;-4.17619;-3.11029;,
 12.13698;-2.00554;-2.64886;,
 12.18936;-2.55124;3.83749;,
 12.16027;-1.19062;4.46465;,
 12.20308;0.59613;4.36042;,
 12.18783;1.77993;3.93043;,
 12.21330;2.93271;2.87596;,
 12.16742;3.50376;1.58282;,
 12.16105;3.57055;-0.16207;,
 12.21567;3.23865;-1.17571;,
 12.16105;3.57055;-0.16207;,
 10.53325;5.85437;-0.81733;,
 9.39705;6.10396;-0.50901;,
 10.76219;-4.38282;5.41497;,
 12.19620;-3.06013;3.35721;,
 0.39059;-3.00697;4.00095;,
 1.07438;-4.19193;2.23625;,
 -1.21632;-3.15468;3.99205;,
 -0.72166;-4.24300;2.18499;,
 12.17151;2.66516;-2.35325;,
 7.80023;-5.14777;1.04429;,
 8.23265;-5.05634;-1.44200;,
 7.99412;-3.96772;-3.02706;,
 7.92716;-3.03476;-3.96915;,
 7.57141;-2.22278;-5.34779;,
 7.59639;-0.89261;-5.41332;,
 7.65473;0.37397;-5.84722;,
 7.68925;2.83369;-5.37316;,
 7.68313;4.37363;-3.52273;,
 7.53298;5.09290;-2.24602;,
 7.61606;5.96565;0.08288;,
 7.61606;5.96565;0.08288;,
 7.39925;4.95335;2.45613;,
 0.19233;0.87658;5.42362;,
 0.18932;-0.82825;5.42428;,
 0.19077;-1.75224;5.35168;,
 1.51482;-4.43619;1.55133;,
 2.14776;-4.28192;-0.41576;,
 2.53966;-3.72047;-2.36912;,
 2.54539;-3.24901;-3.09850;,
 2.55136;-2.46772;-4.27711;,
 2.51098;-1.48295;-5.24938;,
 1.99598;4.33009;-4.28241;,
 1.82814;5.24281;-3.32015;,
 1.61258;5.68802;-0.70862;,
 1.61258;5.68802;-0.70862;,
 1.61577;5.63855;1.22143;,
 12.12052;-3.52635;-0.39961;,
 10.72263;-5.60990;-0.75387;,
 9.68619;-5.48610;-1.07961;,
 8.23265;-5.05634;-1.44200;,
 2.14776;-4.28192;-0.41576;,
 0.93772;-1.60508;-5.25109;,
 0.96303;-2.72089;-4.40471;,
 0.97512;-3.33734;-3.40041;,
 0.52542;-4.27381;-0.32345;,
 0.96457;-3.80448;-2.41636;,
 0.03306;-4.48123;1.06766;,
 0.52542;-4.27381;-0.32345;,
 -1.40949;-1.65244;5.48312;,
 -1.48160;-0.58407;5.15995;,
 -0.94003;1.12125;5.35918;,
 -0.35342;3.10717;4.55233;,
 -0.12146;4.68300;3.17401;,
 0.15380;5.64957;1.10924;,
 0.27274;5.84391;-1.21491;,
 0.33901;5.24916;-3.52366;,
 0.27274;5.84391;-1.21491;,
 0.59904;4.08111;-4.85863;;
 
 95;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,20,23,25;,
 4;26,24,25,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,31;,
 4;39,40,25,23;,
 4;41,42,43,27;,
 4;44,45,30,29;,
 4;15,14,46,47;,
 4;48,12,15,9;,
 4;0,48,9,8;,
 4;49,50,51,43;,
 4;52,53,54,55;,
 4;56,57,55,54;,
 4;58,49,43,42;,
 4;33,8,11,34;,
 4;59,60,47,46;,
 4;61,62,11,10;,
 4;63,64,35,34;,
 4;65,66,19,18;,
 4;51,26,27,43;,
 4;40,41,27,25;,
 4;64,67,68,35;,
 4;69,32,35,68;,
 4;70,71,59,46;,
 4;36,70,46,14;,
 4;37,72,73,38;,
 4;72,74,75,73;,
 4;60,61,10,47;,
 4;9,15,47,10;,
 4;21,16,19,22;,
 4;66,76,22,19;,
 4;62,63,34,11;,
 4;37,36,14,13;,
 4;77,28,31,38;,
 4;78,79,57,56;,
 4;50,80,81,51;,
 4;81,82,26,51;,
 4;82,83,24,26;,
 4;83,84,20,24;,
 4;84,85,21,20;,
 4;85,86,16,21;,
 4;86,87,17,16;,
 4;88,89,32,69;,
 4;89,1,33,32;,
 4;1,0,8,33;,
 4;48,0,3,90;,
 4;12,48,90,91;,
 4;13,12,91,92;,
 4;72,37,13,92;,
 4;93,77,38,73;,
 4;79,78,94,95;,
 4;80,96,97,81;,
 4;82,81,97,98;,
 4;83,82,98,7;,
 4;84,83,7,6;,
 4;85,84,6,99;,
 4;86,85,99,100;,
 4;87,86,100,101;,
 4;89,88,102,103;,
 4;1,89,103,2;,
 4;104,44,29,105;,
 4;29,28,106,105;,
 4;28,77,107,106;,
 4;77,93,108,107;,
 4;76,39,23,22;,
 4;109,4,7,98;,
 4;110,109,98,97;,
 4;96,111,110,97;,
 4;112,113,95,94;,
 4;114,115,108,93;,
 4;75,114,93,73;,
 4;74,72,92,116;,
 4;117,116,92,91;,
 4;118,117,91,90;,
 4;119,118,90,3;,
 4;120,119,3,2;,
 4;121,120,2,103;,
 4;122,121,103,102;,
 4;123,124,101,100;,
 4;125,123,100,99;,
 4;5,125,99,6;,
 4;111,96,95,113;,
 4;96,80,79,95;,
 4;80,50,57,79;,
 4;50,49,55,57;,
 4;49,58,52,55;,
 4;70,36,31,30;,
 4;71,70,30,45;;
 
 MeshMaterialList {
  8;
  95;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.765600;0.659200;0.241600;1.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.060000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.690400;0.251200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.768800;0.000000;1.000000;;
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
   0.743200;0.263200;0.094400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  115;
  -0.111601;0.443692;0.889204;,
  -0.066338;0.795277;0.602607;,
  -0.016259;0.803515;0.595062;,
  -0.065407;0.491719;0.868294;,
  -0.010054;0.798473;0.601947;,
  0.028872;0.966389;0.255458;,
  0.130093;-0.851385;-0.508153;,
  0.136565;-0.748536;-0.648879;,
  0.093101;-0.576596;-0.811708;,
  0.112320;-0.269458;-0.956440;,
  0.162444;0.258707;-0.952199;,
  0.093266;0.333318;-0.938190;,
  0.067810;-0.208945;-0.975573;,
  0.112055;0.888987;-0.444011;,
  0.143689;0.639394;-0.755333;,
  0.075075;0.994519;-0.072769;,
  -0.037229;0.523165;0.851418;,
  -0.080418;0.157980;0.984162;,
  -0.093175;0.457693;0.884215;,
  -0.131156;0.158129;0.978669;,
  0.456985;0.186830;0.869632;,
  -0.058818;0.484319;0.872912;,
  -0.148032;-0.104631;0.983432;,
  -0.181444;-0.490837;0.852149;,
  -0.172365;-0.512468;0.841229;,
  -0.146320;-0.131158;0.980504;,
  -0.044405;0.896320;-0.441179;,
  -0.034108;0.990794;-0.131010;,
  0.469715;0.870885;-0.144665;,
  0.421554;0.822538;-0.381737;,
  -0.066699;0.470305;-0.879980;,
  -0.031037;0.850212;-0.525525;,
  0.448349;0.748489;-0.488618;,
  0.290024;0.488873;-0.822733;,
  -0.083141;-0.085531;-0.992860;,
  -0.067223;-0.087480;-0.993895;,
  -0.053641;-0.216380;-0.974835;,
  0.456352;-0.181441;-0.871104;,
  -0.169801;-0.985478;0.001043;,
  -0.108701;-0.993729;0.026202;,
  -0.154222;-0.882817;0.443677;,
  -0.195693;-0.889141;0.413681;,
  -0.096995;0.917241;0.386343;,
  -0.127819;0.749938;0.649041;,
  -0.145278;0.717636;0.681096;,
  -0.074745;0.926613;0.368514;,
  -0.203541;-0.684804;0.699725;,
  -0.214156;-0.708469;0.672465;,
  -0.211837;-0.903924;0.371547;,
  0.836126;0.264174;-0.480735;,
  0.819310;0.464091;-0.336675;,
  0.836624;0.058443;-0.544651;,
  0.836436;-0.087628;-0.541014;,
  0.835549;-0.271121;-0.477862;,
  0.844279;-0.535904;0.000570;,
  0.869234;-0.452340;0.199554;,
  -0.132490;-0.509347;0.850301;,
  -0.097817;-0.133929;0.986152;,
  -0.152086;0.110654;0.982153;,
  0.003608;-0.996777;0.080147;,
  0.009109;-0.913169;0.407479;,
  0.470147;-0.654934;-0.591628;,
  -0.042218;-0.779227;-0.625318;,
  -0.033496;-0.547914;-0.835864;,
  0.803530;-0.477449;-0.355503;,
  0.821408;-0.535568;-0.196104;,
  0.440121;-0.852999;-0.280509;,
  0.445301;-0.703292;-0.554155;,
  -0.132727;-0.943495;-0.303646;,
  -0.071970;-0.773711;-0.629437;,
  0.806942;-0.412197;-0.423011;,
  0.059350;-0.928207;-0.367301;,
  0.025671;-0.984519;-0.173387;,
  0.870005;-0.267312;0.414290;,
  0.865095;-0.070756;0.496593;,
  0.843982;0.116987;0.523458;,
  0.835407;0.275391;0.475663;,
  0.840495;0.420976;0.341098;,
  0.827595;0.543376;0.140814;,
  0.812360;0.577457;-0.081326;,
  0.809662;0.544563;-0.218857;,
  0.867630;-0.370523;0.331558;,
  -0.077443;-0.762482;0.642358;,
  -0.088554;-0.918080;0.386377;,
  0.040176;-0.780171;0.624274;,
  -0.066921;0.029135;0.997333;,
  0.012181;-0.339904;0.940381;,
  -0.185648;-0.982594;-0.006676;,
  -0.166725;-0.942793;-0.288693;,
  -0.123343;-0.805946;-0.578997;,
  -0.097751;-0.801483;-0.589974;,
  -0.079411;-0.600299;-0.795823;,
  -0.043426;-0.303092;-0.951971;,
  -0.038406;-0.115513;-0.992563;,
  -0.032836;0.444872;-0.894992;,
  0.020565;0.798050;-0.602241;,
  0.026442;0.901808;-0.431327;,
  -0.025159;0.998350;-0.051623;,
  -0.061712;0.926236;0.371859;,
  -0.094698;0.133890;0.986461;,
  -0.084161;-0.025921;0.996115;,
  -0.072404;-0.407335;0.910404;,
  -0.069190;-0.996570;0.045406;,
  -0.054754;-0.974054;-0.219591;,
  -0.029319;-0.891082;-0.452894;,
  0.026685;-0.838381;-0.544431;,
  0.034760;-0.708297;-0.705059;,
  0.035665;-0.490376;-0.870781;,
  0.105009;0.691931;-0.714286;,
  0.088823;0.895620;-0.435861;,
  0.042354;0.997942;-0.048140;,
  0.012265;0.954648;0.297485;,
  0.839297;0.137711;-0.525943;,
  0.019346;-0.556167;-0.830845;,
  -0.172713;-0.686778;0.706050;;
  95;
  4;0,1,2,3;,
  4;9,10,11,12;,
  4;18,19,20,21;,
  4;22,23,24,25;,
  4;26,27,28,29;,
  4;30,31,32,33;,
  4;34,30,33,35;,
  4;36,34,35,37;,
  4;38,39,40,41;,
  4;42,43,44,45;,
  4;46,47,48,41;,
  4;49,51,51,112;,
  4;52,53,53,37;,
  4;54,55,55,54;,
  4;25,24,56,57;,
  4;58,22,25,19;,
  4;0,58,19,18;,
  4;61,62,63,113;,
  4;64,65,66,67;,
  4;68,69,67,66;,
  4;70,61,53,53;,
  4;43,18,21,44;,
  4;73,74,74,73;,
  4;75,76,76,20;,
  4;77,78,78,77;,
  4;79,80,29,28;,
  4;63,36,37,113;,
  4;51,52,37,51;,
  4;78,79,28,78;,
  4;27,42,45,28;,
  4;81,81,73,73;,
  4;46,114,56,24;,
  4;47,82,83,48;,
  4;82,84,60,83;,
  4;74,75,20,74;,
  4;19,25,57,20;,
  4;31,26,29,32;,
  4;80,50,32,29;,
  4;76,77,77,76;,
  4;47,46,24,23;,
  4;87,38,41,48;,
  4;88,89,69,68;,
  4;62,90,91,63;,
  4;91,92,36,63;,
  4;92,93,34,36;,
  4;93,94,30,34;,
  4;94,95,31,30;,
  4;95,96,26,31;,
  4;96,97,27,26;,
  4;97,98,42,27;,
  4;98,1,43,42;,
  4;1,0,18,43;,
  4;58,0,3,99;,
  4;22,58,99,100;,
  4;23,22,100,101;,
  4;82,47,23,101;,
  4;102,87,48,83;,
  4;89,88,103,104;,
  4;90,105,106,91;,
  4;92,91,106,107;,
  4;93,92,107,12;,
  4;94,93,12,11;,
  4;95,94,11,108;,
  4;96,95,108,109;,
  4;97,96,109,110;,
  4;98,97,110,111;,
  4;1,98,111,2;,
  4;65,54,54,66;,
  4;39,38,68,66;,
  4;38,87,88,68;,
  4;87,102,103,88;,
  4;50,49,33,32;,
  4;8,9,12,107;,
  4;7,8,107,106;,
  4;105,6,7,106;,
  4;72,71,104,103;,
  4;59,72,103,102;,
  4;60,59,102,83;,
  4;84,82,101,86;,
  4;85,86,101,100;,
  4;17,85,100,99;,
  4;16,17,99,3;,
  4;4,16,3,2;,
  4;5,4,2,111;,
  4;15,5,111,110;,
  4;13,15,110,109;,
  4;14,13,109,108;,
  4;10,14,108,11;,
  4;6,105,104,71;,
  4;105,90,89,104;,
  4;90,62,69,89;,
  4;62,61,67,69;,
  4;61,70,64,67;,
  4;114,46,41,40;,
  4;81,81,55,55;;
 }
 MeshTextureCoords {
  126;
  0.663990;0.602150;,
  0.633530;0.595810;,
  0.623580;0.675040;,
  0.651640;0.678980;,
  0.756690;0.323930;,
  0.744510;0.357260;,
  0.760510;0.366460;,
  0.772160;0.333160;,
  0.669470;0.572740;,
  0.680820;0.573030;,
  0.681350;0.553480;,
  0.662630;0.550360;,
  0.711330;0.605060;,
  0.729060;0.602630;,
  0.730530;0.574500;,
  0.709030;0.575880;,
  0.837230;0.449890;,
  0.836800;0.471560;,
  0.851450;0.468300;,
  0.854070;0.453490;,
  0.842360;0.399220;,
  0.840510;0.431560;,
  0.856070;0.434330;,
  0.857690;0.401540;,
  0.849580;0.373940;,
  0.865180;0.382590;,
  0.856640;0.361040;,
  0.872400;0.372110;,
  0.788220;0.557260;,
  0.778350;0.539280;,
  0.752490;0.546810;,
  0.758890;0.564700;,
  0.616430;0.569720;,
  0.637720;0.570270;,
  0.637260;0.550310;,
  0.623250;0.548980;,
  0.742970;0.570980;,
  0.738880;0.598860;,
  0.762250;0.588140;,
  0.893480;0.417820;,
  0.897080;0.401530;,
  0.902000;0.386900;,
  0.908920;0.370460;,
  0.879010;0.357090;,
  0.779960;0.507180;,
  0.755100;0.512910;,
  0.728210;0.551120;,
  0.707410;0.551340;,
  0.681890;0.602950;,
  0.889970;0.346080;,
  0.875790;0.333300;,
  0.864620;0.346990;,
  0.924120;0.348620;,
  0.936380;0.328820;,
  0.915950;0.302330;,
  0.899190;0.327730;,
  0.902190;0.298720;,
  0.884660;0.321290;,
  0.916070;0.359370;,
  0.729160;0.515220;,
  0.706230;0.517980;,
  0.679110;0.519230;,
  0.660260;0.519410;,
  0.636400;0.517170;,
  0.614910;0.515730;,
  0.887750;0.466100;,
  0.889320;0.452140;,
  0.590690;0.514870;,
  0.574770;0.548000;,
  0.576000;0.562560;,
  0.738520;0.549400;,
  0.740170;0.513860;,
  0.746310;0.684330;,
  0.775420;0.672390;,
  0.748000;0.703360;,
  0.777620;0.693610;,
  0.891190;0.434560;,
  0.793140;0.580650;,
  0.882780;0.295080;,
  0.866690;0.312530;,
  0.856810;0.324790;,
  0.842310;0.338500;,
  0.834750;0.352220;,
  0.829330;0.367220;,
  0.821430;0.396250;,
  0.817240;0.425260;,
  0.814470;0.443150;,
  0.814130;0.474400;,
  0.581510;0.585200;,
  0.612690;0.593460;,
  0.682590;0.689610;,
  0.706960;0.689640;,
  0.720330;0.688900;,
  0.784990;0.664940;,
  0.819070;0.253690;,
  0.806830;0.277290;,
  0.799750;0.286630;,
  0.789430;0.302630;,
  0.779870;0.319000;,
  0.750220;0.401090;,
  0.746440;0.418530;,
  0.743500;0.453920;,
  0.568770;0.658420;,
  0.593310;0.663840;,
  0.800790;0.505560;,
  0.809670;0.540010;,
  0.815890;0.553140;,
  0.822490;0.572650;,
  0.809830;0.651560;,
  0.763320;0.310190;,
  0.772430;0.293070;,
  0.781350;0.279640;,
  0.805150;0.241850;,
  0.789960;0.267770;,
  0.792420;0.681720;,
  0.810430;0.671160;,
  0.718310;0.708190;,
  0.702690;0.709320;,
  0.678820;0.702980;,
  0.649320;0.694460;,
  0.620850;0.689770;,
  0.590870;0.681670;,
  0.560450;0.673200;,
  0.728810;0.414510;,
  0.727790;0.447420;,
  0.734450;0.390880;;
 }
}
