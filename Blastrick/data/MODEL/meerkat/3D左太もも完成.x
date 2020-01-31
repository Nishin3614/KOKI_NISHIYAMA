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
 190;
 -0.88030;25.94457;6.36701;,
 2.42405;25.94457;7.73167;,
 2.42405;22.84819;7.73167;,
 -0.88030;22.84819;6.36701;,
 4.34485;25.94457;9.55387;,
 4.34485;22.84819;9.55387;,
 4.13398;27.81723;10.75299;,
 4.13398;22.84819;11.62348;,
 2.02113;27.37403;11.95177;,
 1.90338;22.84819;13.16339;,
 -1.03731;27.26819;12.76922;,
 -1.40720;22.84819;13.06812;,
 -4.55472;26.58587;11.74181;,
 -5.00017;22.84819;11.80094;,
 -7.01557;25.94457;9.72764;,
 -7.32482;22.84819;10.28854;,
 -7.37264;25.94457;7.60073;,
 -8.39683;22.84819;7.92467;,
 -7.37332;25.94457;6.78288;,
 -7.37135;22.84819;6.76539;,
 -4.54083;25.94457;6.16007;,
 -4.54427;22.84819;6.20227;,
 -0.88030;25.94457;6.36701;,
 -0.88030;22.84819;6.36701;,
 2.42405;22.12707;7.73167;,
 -0.88030;22.12707;6.36701;,
 4.34485;22.12707;9.55387;,
 4.13398;22.12707;11.62348;,
 1.90338;22.12707;13.16339;,
 -1.40720;22.12707;13.06812;,
 -5.01304;22.12707;11.95882;,
 -7.75645;22.12707;10.18621;,
 -8.79200;22.12707;8.15409;,
 -7.38287;22.12707;6.86773;,
 -4.53141;22.12707;6.04440;,
 -0.88030;22.12707;6.36701;,
 2.42405;21.40593;7.73167;,
 -0.88030;21.40593;6.36701;,
 4.34485;21.40593;9.55387;,
 4.13398;21.40593;11.62348;,
 1.90338;21.40593;13.16339;,
 -1.40720;21.40593;13.06812;,
 -5.00710;21.40593;11.88598;,
 -7.76022;21.40593;10.21965;,
 -8.88206;21.40593;8.09442;,
 -7.37911;21.40593;6.83430;,
 -4.53734;21.40593;6.11723;,
 -0.88030;21.40593;6.36701;,
 2.42405;20.68482;7.73167;,
 -0.88030;20.68482;6.36701;,
 4.34485;18.36231;9.55387;,
 4.13398;18.36231;11.62348;,
 1.90338;18.36231;13.16339;,
 -1.40720;20.68482;13.06812;,
 -5.01423;20.68482;11.97360;,
 -7.75460;19.85443;10.16974;,
 -8.70656;19.85443;8.09442;,
 -7.38473;19.85443;6.88420;,
 -4.53020;20.68482;6.02961;,
 -0.88030;20.68482;6.36701;,
 2.42405;19.96371;7.73167;,
 -0.88030;19.96371;6.36701;,
 4.34485;16.80313;9.55387;,
 4.13398;16.80313;11.62348;,
 1.90338;16.80313;13.16339;,
 -1.40720;18.46170;13.06812;,
 -5.01304;19.96371;11.95882;,
 -7.74736;18.71276;10.10549;,
 -8.70186;18.61591;8.19232;,
 -7.39196;18.71276;6.94846;,
 -4.53141;19.96371;6.04440;,
 -0.88030;19.96371;6.36701;,
 2.42405;17.07932;7.73167;,
 -0.88030;17.74058;6.36701;,
 4.34485;15.06935;9.55387;,
 4.13398;15.06935;11.62348;,
 1.90338;15.06935;13.16339;,
 -1.40720;17.07932;13.06812;,
 -5.00827;18.82204;11.90040;,
 -7.75185;17.57304;10.14534;,
 -8.57777;17.57304;8.25746;,
 -7.38747;17.57304;6.90860;,
 -4.53616;19.22791;6.10281;,
 -0.88030;17.74058;6.36701;,
 2.42405;15.27009;7.73167;,
 -1.18604;16.37879;6.79060;,
 3.42704;13.02869;9.31084;,
 3.48219;13.02869;11.45089;,
 0.98555;13.02869;12.92036;,
 -1.40720;15.27009;13.06812;,
 -4.71962;16.37879;11.68645;,
 -7.75004;16.61360;10.12928;,
 -8.57777;16.69231;8.25746;,
 -7.38927;16.61360;6.92466;,
 -4.51910;18.52146;5.89316;,
 -1.18604;16.37879;6.79060;,
 -2.13975;4.85976;7.12584;,
 -3.68215;6.44530;6.26359;,
 -1.30267;3.77762;8.39823;,
 -1.52203;3.54242;9.94747;,
 -2.78864;4.22891;11.47332;,
 -4.41528;5.61905;11.58732;,
 -6.13518;7.27154;11.04503;,
 -8.37730;14.69182;9.85132;,
 -8.54500;15.22886;8.04225;,
 -7.81450;15.01332;7.18553;,
 -5.44019;8.03084;6.08522;,
 -3.68215;6.44530;6.26359;,
 -2.49645;4.45158;7.01337;,
 -4.03887;6.03713;6.15112;,
 -1.65938;3.36946;8.28576;,
 -1.87874;3.13427;9.83501;,
 -3.14534;3.82073;11.36086;,
 -4.75165;5.21089;11.41035;,
 -6.49189;6.86336;10.93257;,
 -7.79380;8.80345;9.81402;,
 -7.92131;8.93997;7.92979;,
 -7.29449;9.14306;6.99790;,
 -5.79690;7.62267;5.97275;,
 -4.03887;6.03713;6.15112;,
 -2.85314;4.04344;6.90090;,
 -4.39556;5.62896;6.03866;,
 -2.01607;2.96128;8.17329;,
 -2.23543;2.72610;9.72254;,
 -3.50203;3.41258;11.24839;,
 -5.10835;4.80273;11.29788;,
 -6.84858;6.45520;10.82009;,
 -8.06962;7.84535;9.64769;,
 -8.27801;8.53181;7.81732;,
 -7.73207;8.29664;6.93930;,
 -6.15360;7.21451;5.86028;,
 -4.39556;5.62896;6.03866;,
 -3.41511;3.63526;7.43943;,
 -4.75227;5.22079;5.92619;,
 -2.37278;2.55311;8.06082;,
 -2.59214;2.31793;9.61007;,
 -3.85875;3.00441;11.13592;,
 -5.36847;4.39454;10.87907;,
 -7.20530;6.04704;10.70762;,
 -8.37373;7.43717;9.36836;,
 -8.63472;7.89293;7.70485;,
 -8.14140;7.88846;6.99368;,
 -6.51032;6.80632;5.74781;,
 -4.75227;5.22079;5.92619;,
 -3.77182;3.22708;7.32696;,
 -5.10897;4.81263;5.81372;,
 -2.72949;2.14494;7.94835;,
 -2.94885;1.90977;9.49760;,
 -4.17262;2.59624;10.88757;,
 -5.72517;3.98639;10.76660;,
 -7.54167;5.63887;10.53067;,
 -8.77904;6.70382;9.41004;,
 -8.78511;7.31686;7.65743;,
 -8.44950;7.15511;6.72706;,
 -6.86701;6.39815;5.63534;,
 -5.10897;4.81263;5.81372;,
 -4.12853;2.81892;7.21449;,
 -5.46568;4.40446;5.70125;,
 -3.08618;1.73679;7.83589;,
 -3.30555;1.50159;9.38514;,
 -4.40366;2.18807;10.37660;,
 -6.08187;3.57823;10.65413;,
 -7.89837;5.23070;10.41819;,
 -9.00934;6.31684;9.29069;,
 -8.76664;6.60140;7.66326;,
 -8.62940;6.76813;6.64617;,
 -7.22372;5.99000;5.52287;,
 -5.46568;4.40446;5.70125;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -1.93377;28.80015;9.17301;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;,
 -6.14108;4.40446;7.84334;;
 
 165;
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
 4;20,22,23,21;,
 4;3,2,24,25;,
 4;2,5,26,24;,
 4;5,7,27,26;,
 4;7,9,28,27;,
 4;9,11,29,28;,
 4;11,13,30,29;,
 4;13,15,31,30;,
 4;15,17,32,31;,
 4;17,19,33,32;,
 4;19,21,34,33;,
 4;21,23,35,34;,
 4;25,24,36,37;,
 4;24,26,38,36;,
 4;26,27,39,38;,
 4;27,28,40,39;,
 4;28,29,41,40;,
 4;29,30,42,41;,
 4;30,31,43,42;,
 4;31,32,44,43;,
 4;32,33,45,44;,
 4;33,34,46,45;,
 4;34,35,47,46;,
 4;37,36,48,49;,
 4;36,38,50,48;,
 4;38,39,51,50;,
 4;39,40,52,51;,
 4;40,41,53,52;,
 4;41,42,54,53;,
 4;42,43,55,54;,
 4;43,44,56,55;,
 4;44,45,57,56;,
 4;45,46,58,57;,
 4;46,47,59,58;,
 4;49,48,60,61;,
 4;48,50,62,60;,
 4;50,51,63,62;,
 4;51,52,64,63;,
 4;52,53,65,64;,
 4;53,54,66,65;,
 4;54,55,67,66;,
 4;55,56,68,67;,
 4;56,57,69,68;,
 4;57,58,70,69;,
 4;58,59,71,70;,
 4;61,60,72,73;,
 4;60,62,74,72;,
 4;62,63,75,74;,
 4;63,64,76,75;,
 4;64,65,77,76;,
 4;65,66,78,77;,
 4;66,67,79,78;,
 4;67,68,80,79;,
 4;68,69,81,80;,
 4;69,70,82,81;,
 4;70,71,83,82;,
 4;73,72,84,85;,
 4;72,74,86,84;,
 4;74,75,87,86;,
 4;75,76,88,87;,
 4;76,77,89,88;,
 4;77,78,90,89;,
 4;78,79,91,90;,
 4;79,80,92,91;,
 4;80,81,93,92;,
 4;81,82,94,93;,
 4;82,83,95,94;,
 4;85,84,96,97;,
 4;84,86,98,96;,
 4;86,87,99,98;,
 4;87,88,100,99;,
 4;88,89,101,100;,
 4;89,90,102,101;,
 4;90,91,103,102;,
 4;91,92,104,103;,
 4;92,93,105,104;,
 4;93,94,106,105;,
 4;94,95,107,106;,
 4;97,96,108,109;,
 4;96,98,110,108;,
 4;98,99,111,110;,
 4;99,100,112,111;,
 4;100,101,113,112;,
 4;101,102,114,113;,
 4;102,103,115,114;,
 4;103,104,116,115;,
 4;104,105,117,116;,
 4;105,106,118,117;,
 4;106,107,119,118;,
 4;109,108,120,121;,
 4;108,110,122,120;,
 4;110,111,123,122;,
 4;111,112,124,123;,
 4;112,113,125,124;,
 4;113,114,126,125;,
 4;114,115,127,126;,
 4;115,116,128,127;,
 4;116,117,129,128;,
 4;117,118,130,129;,
 4;118,119,131,130;,
 4;121,120,132,133;,
 4;120,122,134,132;,
 4;122,123,135,134;,
 4;123,124,136,135;,
 4;124,125,137,136;,
 4;125,126,138,137;,
 4;126,127,139,138;,
 4;127,128,140,139;,
 4;128,129,141,140;,
 4;129,130,142,141;,
 4;130,131,143,142;,
 4;133,132,144,145;,
 4;132,134,146,144;,
 4;134,135,147,146;,
 4;135,136,148,147;,
 4;136,137,149,148;,
 4;137,138,150,149;,
 4;138,139,151,150;,
 4;139,140,152,151;,
 4;140,141,153,152;,
 4;141,142,154,153;,
 4;142,143,155,154;,
 4;145,144,156,157;,
 4;144,146,158,156;,
 4;146,147,159,158;,
 4;147,148,160,159;,
 4;148,149,161,160;,
 4;149,150,162,161;,
 4;150,151,163,162;,
 4;151,152,164,163;,
 4;152,153,165,164;,
 4;153,154,166,165;,
 4;154,155,167,166;,
 3;168,1,0;,
 3;169,4,1;,
 3;170,6,4;,
 3;171,8,6;,
 3;172,10,8;,
 3;173,12,10;,
 3;174,14,12;,
 3;175,16,14;,
 3;176,18,16;,
 3;177,20,18;,
 3;178,22,20;,
 3;179,157,156;,
 3;180,156,158;,
 3;181,158,159;,
 3;182,159,160;,
 3;183,160,161;,
 3;184,161,162;,
 3;185,162,163;,
 3;186,163,164;,
 3;187,164,165;,
 3;188,165,166;,
 3;189,166,167;;
 
 MeshMaterialList {
  1;
  165;
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
  171;
  0.177007;0.735408;-0.654097;,
  0.202189;0.391592;-0.897650;,
  0.485722;0.432524;-0.759604;,
  0.721955;0.416463;-0.552576;,
  0.838095;0.105593;0.535208;,
  0.209945;0.625731;0.751255;,
  -0.188051;0.409036;0.892931;,
  -0.467204;0.545069;0.696147;,
  -0.691381;0.597922;0.405563;,
  -0.800626;0.599145;0.004838;,
  -0.513159;0.348754;-0.784244;,
  -0.069079;0.406447;-0.911059;,
  0.223458;0.025639;-0.974376;,
  0.544135;0.000000;-0.838998;,
  0.939141;0.003344;-0.343515;,
  0.850805;0.052561;0.522847;,
  0.301747;0.087786;0.949338;,
  -0.139021;0.085568;0.986586;,
  -0.442679;0.130972;0.887063;,
  -0.747766;0.283338;0.600471;,
  -0.963167;0.244885;-0.111083;,
  -0.570327;0.040933;-0.820397;,
  -0.081965;0.033085;-0.996086;,
  0.230255;0.014555;-0.973022;,
  0.937612;0.000000;-0.347683;,
  0.860750;0.000000;0.509028;,
  0.283774;0.000000;0.958891;,
  -0.169888;0.014010;0.985364;,
  -0.424938;0.057651;0.903385;,
  -0.724238;0.187260;0.663636;,
  -0.859695;0.275074;0.430417;,
  -0.482257;0.031229;-0.875473;,
  -0.089546;0.016666;-0.995843;,
  0.233381;0.002571;-0.972382;,
  0.937612;0.000000;-0.347682;,
  0.860750;0.000000;0.509028;,
  -0.166935;0.002492;0.985965;,
  -0.419705;0.006644;0.907636;,
  -0.738484;0.001809;0.674269;,
  -0.892394;-0.004762;0.451232;,
  -0.472046;0.014592;-0.881453;,
  -0.092598;0.006919;-0.995679;,
  0.236226;0.012737;-0.971615;,
  -0.164171;0.012347;0.986355;,
  -0.421169;0.013738;0.906878;,
  -0.747014;-0.013364;0.664674;,
  -0.899319;-0.028470;0.436366;,
  -0.483256;-0.022792;-0.875183;,
  -0.094725;0.014530;-0.995397;,
  0.236159;-0.012654;-0.971632;,
  0.283774;0.000000;0.958891;,
  -0.166699;-0.008829;0.985968;,
  -0.427974;-0.019219;0.903587;,
  -0.753380;-0.018217;0.657333;,
  -0.903585;-0.014470;0.428165;,
  -0.504644;-0.040843;-0.862361;,
  -0.099520;-0.027165;-0.994665;,
  0.219258;-0.057429;-0.973975;,
  0.519934;-0.072063;-0.851161;,
  0.920838;-0.130413;-0.367491;,
  0.848078;-0.179980;0.498369;,
  0.270583;-0.087307;0.958730;,
  -0.188571;-0.035020;0.981435;,
  -0.429024;-0.042320;0.902301;,
  -0.745264;-0.030411;0.666076;,
  -0.910175;-0.017234;0.413865;,
  -0.545811;0.010547;-0.837842;,
  -0.113020;0.011845;-0.993522;,
  0.227805;-0.065484;-0.971502;,
  0.496845;-0.154808;-0.853920;,
  0.865885;-0.325425;-0.379923;,
  0.797065;-0.408865;0.444429;,
  0.258941;-0.224287;0.939492;,
  -0.208728;-0.069060;0.975532;,
  -0.416493;-0.036413;0.908409;,
  -0.757664;0.009568;0.652575;,
  -0.943926;0.048474;0.326580;,
  -0.566236;0.043892;-0.823074;,
  -0.102825;0.017580;-0.994544;,
  0.275727;-0.018356;-0.961061;,
  0.511725;-0.197441;-0.836154;,
  0.796052;-0.471278;-0.379735;,
  0.724210;-0.569833;0.388344;,
  0.221382;-0.358733;0.906808;,
  -0.224816;-0.109802;0.968195;,
  -0.428895;-0.001786;0.903353;,
  -0.826098;0.019718;0.563181;,
  -0.964562;0.007673;-0.263746;,
  -0.575463;0.052836;-0.816119;,
  -0.076813;0.070270;-0.994566;,
  0.300704;0.000001;-0.953717;,
  0.521452;-0.229225;-0.821915;,
  0.746022;-0.547503;-0.379067;,
  0.653598;-0.668804;0.354275;,
  0.185359;-0.427331;0.884890;,
  -0.218691;-0.116527;0.968812;,
  -0.442350;0.030395;0.896327;,
  -0.837237;0.145858;0.527028;,
  -0.944928;0.229832;-0.233000;,
  -0.568737;0.281149;-0.772977;,
  -0.068047;0.200433;-0.977341;,
  0.244861;-0.092824;-0.965104;,
  0.359394;-0.362122;-0.860060;,
  0.660639;-0.611472;-0.435496;,
  0.653598;-0.668803;0.354277;,
  0.135232;-0.455723;0.879789;,
  -0.285069;-0.185549;0.940376;,
  -0.481714;0.004947;0.876315;,
  -0.782355;0.308949;0.540806;,
  -0.837656;0.525810;-0.147841;,
  -0.524051;0.471964;-0.708958;,
  -0.073674;0.250703;-0.965257;,
  0.289825;-0.139421;-0.946870;,
  0.345342;-0.344565;-0.872934;,
  0.319553;-0.311202;-0.895008;,
  0.641130;-0.680559;0.354671;,
  0.075790;-0.474208;0.877145;,
  -0.312952;-0.218044;0.924401;,
  -0.470341;-0.000386;0.882485;,
  -0.783764;0.316254;0.534507;,
  -0.877379;0.457720;-0.143870;,
  -0.560783;0.456533;-0.690724;,
  -0.077916;0.282466;-0.956108;,
  0.391274;-0.089565;-0.915905;,
  0.492180;-0.196454;-0.848036;,
  0.416981;-0.116011;-0.901481;,
  0.597715;-0.713732;0.365134;,
  0.030123;-0.512459;0.858183;,
  -0.259144;-0.200671;0.944762;,
  -0.409178;0.051950;0.910975;,
  -0.827797;0.287307;0.481879;,
  -0.942496;0.308703;-0.128077;,
  -0.668999;0.403676;-0.624088;,
  -0.085190;0.296368;-0.951267;,
  0.391272;-0.089568;-0.915906;,
  0.492178;-0.196457;-0.848036;,
  0.416979;-0.116013;-0.901482;,
  0.565414;-0.734657;0.374948;,
  -0.013779;-0.567315;0.823386;,
  -0.259326;-0.240225;0.935437;,
  -0.408222;0.053410;0.911319;,
  -0.867428;0.251114;0.429547;,
  -0.965751;0.234185;-0.111721;,
  -0.741270;0.361907;-0.565280;,
  -0.097767;0.288802;-0.952384;,
  -0.632590;-0.747002;-0.204495;,
  -0.685078;0.056335;-0.726288;,
  -0.658684;0.019893;-0.752156;,
  -0.669229;-0.047760;-0.741520;,
  -0.698547;-0.050855;-0.713755;,
  -0.732829;-0.028985;-0.679795;,
  -0.746859;0.040659;-0.663738;,
  0.752079;-0.659041;0.006445;,
  0.752084;-0.659036;0.006446;,
  0.752091;-0.659028;0.006448;,
  0.292952;0.538770;-0.789877;,
  -0.274427;0.921517;0.274765;,
  0.041059;0.911457;0.409341;,
  0.068646;0.923887;0.376458;,
  -0.464943;0.885340;0.000386;,
  -0.644323;-0.737276;-0.203155;,
  -0.644323;-0.737279;-0.203143;,
  -0.644320;-0.737280;-0.203148;,
  -0.644321;-0.737277;-0.203157;,
  -0.644325;-0.737276;-0.203149;,
  -0.644321;-0.737279;-0.203150;,
  -0.623712;-0.760238;-0.181721;,
  -0.609673;-0.769023;-0.192100;,
  -0.617859;-0.756092;-0.215814;,
  -0.613558;-0.758004;-0.221306;,
  -0.626212;-0.748236;-0.219092;;
  165;
  4;1,2,13,12;,
  4;2,3,14,13;,
  4;3,4,15,14;,
  4;4,5,16,15;,
  4;5,6,17,16;,
  4;6,7,18,17;,
  4;7,8,19,18;,
  4;8,9,20,19;,
  4;9,10,21,20;,
  4;10,11,22,21;,
  4;11,1,12,22;,
  4;12,13,13,23;,
  4;13,14,24,13;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  4;17,18,28,27;,
  4;18,19,29,28;,
  4;19,20,30,29;,
  4;20,21,31,146;,
  4;21,22,32,31;,
  4;22,12,23,32;,
  4;23,13,13,33;,
  4;13,24,34,13;,
  4;24,25,35,34;,
  4;25,26,26,35;,
  4;26,27,36,26;,
  4;27,28,37,36;,
  4;28,29,38,37;,
  4;29,30,39,38;,
  4;146,31,40,147;,
  4;31,32,41,40;,
  4;32,23,33,41;,
  4;33,13,13,42;,
  4;13,34,34,13;,
  4;34,35,25,34;,
  4;35,26,26,25;,
  4;26,36,43,26;,
  4;36,37,44,43;,
  4;37,38,45,44;,
  4;38,39,46,45;,
  4;147,40,47,148;,
  4;40,41,48,47;,
  4;41,33,42,48;,
  4;42,13,13,49;,
  4;13,34,34,13;,
  4;34,25,35,34;,
  4;25,26,50,35;,
  4;26,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;148,47,55,149;,
  4;47,48,56,55;,
  4;48,42,49,56;,
  4;49,13,58,57;,
  4;13,34,59,58;,
  4;34,35,60,59;,
  4;35,50,61,60;,
  4;50,51,62,61;,
  4;51,52,63,62;,
  4;52,53,64,63;,
  4;53,54,65,64;,
  4;149,55,66,150;,
  4;55,56,67,66;,
  4;56,49,57,67;,
  4;57,58,69,68;,
  4;58,59,70,69;,
  4;59,60,71,70;,
  4;60,61,72,71;,
  4;61,62,73,72;,
  4;62,63,74,73;,
  4;63,64,75,74;,
  4;64,65,76,75;,
  4;150,66,77,151;,
  4;66,67,78,77;,
  4;67,57,68,78;,
  4;68,69,80,79;,
  4;69,70,81,80;,
  4;70,71,82,81;,
  4;71,72,83,82;,
  4;72,73,84,83;,
  4;73,74,85,84;,
  4;74,75,86,85;,
  4;75,76,87,86;,
  4;151,77,88,87;,
  4;77,78,89,88;,
  4;78,68,79,89;,
  4;79,80,91,90;,
  4;80,81,92,91;,
  4;81,82,93,92;,
  4;82,83,94,93;,
  4;83,84,95,94;,
  4;84,85,96,95;,
  4;85,86,97,96;,
  4;86,87,98,97;,
  4;87,88,99,98;,
  4;88,89,100,99;,
  4;89,79,90,100;,
  4;90,91,102,101;,
  4;91,92,103,102;,
  4;92,93,104,103;,
  4;93,94,105,104;,
  4;94,95,106,105;,
  4;95,96,107,106;,
  4;96,97,108,107;,
  4;97,98,109,108;,
  4;98,99,110,109;,
  4;99,100,111,110;,
  4;100,90,101,111;,
  4;101,102,113,112;,
  4;102,103,114,113;,
  4;103,104,115,152;,
  4;104,105,116,115;,
  4;105,106,117,116;,
  4;106,107,118,117;,
  4;107,108,119,118;,
  4;108,109,120,119;,
  4;109,110,121,120;,
  4;110,111,122,121;,
  4;111,101,112,122;,
  4;112,113,124,123;,
  4;113,114,125,124;,
  4;152,115,126,153;,
  4;115,116,127,126;,
  4;116,117,128,127;,
  4;117,118,129,128;,
  4;118,119,130,129;,
  4;119,120,131,130;,
  4;120,121,132,131;,
  4;121,122,133,132;,
  4;122,112,123,133;,
  4;123,124,135,134;,
  4;124,125,136,135;,
  4;153,126,137,154;,
  4;126,127,138,137;,
  4;127,128,139,138;,
  4;128,129,140,139;,
  4;129,130,141,140;,
  4;130,131,142,141;,
  4;131,132,143,142;,
  4;132,133,144,143;,
  4;133,123,134,144;,
  3;0,2,1;,
  3;0,3,2;,
  3;0,155,3;,
  3;156,5,157;,
  3;156,158,5;,
  3;156,7,6;,
  3;156,8,7;,
  3;156,9,8;,
  3;156,159,9;,
  3;0,11,10;,
  3;0,1,11;,
  3;145,160,161;,
  3;145,161,162;,
  3;145,162,163;,
  3;145,163,164;,
  3;145,164,165;,
  3;145,165,166;,
  3;145,166,167;,
  3;145,167,168;,
  3;145,168,169;,
  3;145,169,170;,
  3;145,170,160;;
 }
 MeshTextureCoords {
  190;
  0.000000;0.000000;,
  0.090910;0.000000;,
  0.090910;0.076920;,
  0.000000;0.076920;,
  0.181820;0.000000;,
  0.181820;0.076920;,
  0.272730;0.000000;,
  0.272730;0.076920;,
  0.363640;0.000000;,
  0.363640;0.076920;,
  0.454550;0.000000;,
  0.454550;0.076920;,
  0.545450;0.000000;,
  0.545450;0.076920;,
  0.636360;0.000000;,
  0.636360;0.076920;,
  0.727270;0.000000;,
  0.727270;0.076920;,
  0.818180;0.000000;,
  0.818180;0.076920;,
  0.909090;0.000000;,
  0.909090;0.076920;,
  1.000000;0.000000;,
  1.000000;0.076920;,
  0.090910;0.153850;,
  0.000000;0.153850;,
  0.181820;0.153850;,
  0.272730;0.153850;,
  0.363640;0.153850;,
  0.454550;0.153850;,
  0.545450;0.153850;,
  0.636360;0.153850;,
  0.727270;0.153850;,
  0.818180;0.153850;,
  0.909090;0.153850;,
  1.000000;0.153850;,
  0.090910;0.230770;,
  0.000000;0.230770;,
  0.181820;0.230770;,
  0.272730;0.230770;,
  0.363640;0.230770;,
  0.454550;0.230770;,
  0.545450;0.230770;,
  0.636360;0.230770;,
  0.727270;0.230770;,
  0.818180;0.230770;,
  0.909090;0.230770;,
  1.000000;0.230770;,
  0.090910;0.307690;,
  0.000000;0.307690;,
  0.181820;0.307690;,
  0.272730;0.307690;,
  0.363640;0.307690;,
  0.454550;0.307690;,
  0.545450;0.307690;,
  0.636360;0.307690;,
  0.727270;0.307690;,
  0.818180;0.307690;,
  0.909090;0.307690;,
  1.000000;0.307690;,
  0.090910;0.384620;,
  0.000000;0.384620;,
  0.181820;0.384620;,
  0.272730;0.384620;,
  0.363640;0.384620;,
  0.454550;0.384620;,
  0.545450;0.384620;,
  0.636360;0.384620;,
  0.727270;0.384620;,
  0.818180;0.384620;,
  0.909090;0.384620;,
  1.000000;0.384620;,
  0.090910;0.461540;,
  0.000000;0.461540;,
  0.181820;0.461540;,
  0.272730;0.461540;,
  0.363640;0.461540;,
  0.454550;0.461540;,
  0.545450;0.461540;,
  0.636360;0.461540;,
  0.727270;0.461540;,
  0.818180;0.461540;,
  0.909090;0.461540;,
  1.000000;0.461540;,
  0.090910;0.538460;,
  0.000000;0.538460;,
  0.181820;0.538460;,
  0.272730;0.538460;,
  0.363640;0.538460;,
  0.454550;0.538460;,
  0.545450;0.538460;,
  0.636360;0.538460;,
  0.727270;0.538460;,
  0.818180;0.538460;,
  0.909090;0.538460;,
  1.000000;0.538460;,
  0.090910;0.615380;,
  0.000000;0.615380;,
  0.181820;0.615380;,
  0.272730;0.615380;,
  0.363640;0.615380;,
  0.454550;0.615380;,
  0.545450;0.615380;,
  0.636360;0.615380;,
  0.727270;0.615380;,
  0.818180;0.615380;,
  0.909090;0.615380;,
  1.000000;0.615380;,
  0.090910;0.692310;,
  0.000000;0.692310;,
  0.181820;0.692310;,
  0.272730;0.692310;,
  0.363640;0.692310;,
  0.454550;0.692310;,
  0.545450;0.692310;,
  0.636360;0.692310;,
  0.727270;0.692310;,
  0.818180;0.692310;,
  0.909090;0.692310;,
  1.000000;0.692310;,
  0.090910;0.769230;,
  0.000000;0.769230;,
  0.181820;0.769230;,
  0.272730;0.769230;,
  0.363640;0.769230;,
  0.454550;0.769230;,
  0.545450;0.769230;,
  0.636360;0.769230;,
  0.727270;0.769230;,
  0.818180;0.769230;,
  0.909090;0.769230;,
  1.000000;0.769230;,
  0.090910;0.846150;,
  0.000000;0.846150;,
  0.181820;0.846150;,
  0.272730;0.846150;,
  0.363640;0.846150;,
  0.454550;0.846150;,
  0.545450;0.846150;,
  0.636360;0.846150;,
  0.727270;0.846150;,
  0.818180;0.846150;,
  0.909090;0.846150;,
  1.000000;0.846150;,
  0.090910;0.923080;,
  0.000000;0.923080;,
  0.181820;0.923080;,
  0.272730;0.923080;,
  0.363640;0.923080;,
  0.454550;0.923080;,
  0.545450;0.923080;,
  0.636360;0.923080;,
  0.727270;0.923080;,
  0.818180;0.923080;,
  0.909090;0.923080;,
  1.000000;0.923080;,
  0.090910;1.000000;,
  0.000000;1.000000;,
  0.181820;1.000000;,
  0.272730;1.000000;,
  0.363640;1.000000;,
  0.454550;1.000000;,
  0.545450;1.000000;,
  0.636360;1.000000;,
  0.727270;1.000000;,
  0.818180;1.000000;,
  0.909090;1.000000;,
  1.000000;1.000000;,
  0.045450;0.000000;,
  0.136360;0.000000;,
  0.227270;0.000000;,
  0.318180;0.000000;,
  0.409090;0.000000;,
  0.500000;0.000000;,
  0.590910;0.000000;,
  0.681820;0.000000;,
  0.772730;0.000000;,
  0.863640;0.000000;,
  0.954550;0.000000;,
  0.045450;1.000000;,
  0.136360;1.000000;,
  0.227270;1.000000;,
  0.318180;1.000000;,
  0.409090;1.000000;,
  0.500000;1.000000;,
  0.590910;1.000000;,
  0.681820;1.000000;,
  0.772730;1.000000;,
  0.863640;1.000000;,
  0.954550;1.000000;;
 }
}
