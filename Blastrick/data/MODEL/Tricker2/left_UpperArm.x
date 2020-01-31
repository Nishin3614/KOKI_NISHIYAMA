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
 5.21868;1.70471;4.23761;,
 5.41121;2.95304;2.92103;,
 1.01180;3.23046;2.40021;,
 0.89809;2.16392;3.40159;,
 0.90075;-0.43573;-4.00629;,
 0.84137;1.33981;-4.15500;,
 1.86412;1.53201;-3.91553;,
 1.92926;-0.26099;-3.96340;,
 6.75668;1.39177;4.41333;,
 6.76728;0.69930;4.63270;,
 7.73601;0.69023;4.74759;,
 7.84893;1.78475;4.14709;,
 5.12864;-1.07516;4.42084;,
 5.22307;-2.04623;4.06588;,
 6.62562;-2.41527;4.24397;,
 6.63984;-1.10685;4.66544;,
 6.76135;3.55290;-1.30482;,
 6.81869;4.31817;-0.26697;,
 7.61403;4.14346;-0.48279;,
 7.64521;3.72710;-1.26725;,
 6.75159;1.88093;-3.88816;,
 6.89623;3.22551;-2.40135;,
 7.70759;3.23527;-2.36795;,
 7.56165;1.79113;-3.98282;,
 6.70740;0.24686;-4.07604;,
 7.57932;0.40025;-4.13181;,
 6.77004;-0.64318;-3.79018;,
 7.69528;-0.44960;-3.91038;,
 6.90813;-3.86778;1.06980;,
 7.83778;-4.01549;1.46785;,
 7.78906;-3.72156;3.24215;,
 6.80317;-3.74803;2.95905;,
 6.67572;3.64270;2.08492;,
 6.76083;2.98759;3.19289;,
 7.76383;3.20183;3.22486;,
 7.76623;3.70499;2.42941;,
 6.70917;-2.96314;3.65696;,
 5.37035;-2.52685;3.70770;,
 5.57778;-3.54926;2.69640;,
 8.71615;1.34763;-2.14192;,
 8.69140;0.64950;-2.47889;,
 8.67676;-0.08720;-2.48917;,
 8.66938;-0.89146;-2.20441;,
 7.69077;-1.51961;-3.68099;,
 8.75610;-2.59473;0.82006;,
 8.68972;-2.50617;1.95460;,
 7.79092;-2.49663;4.38362;,
 7.85932;-1.09724;4.75061;,
 5.27099;0.63396;4.24442;,
 7.78411;-2.07163;-2.83479;,
 6.83507;-2.20647;-2.72392;,
 6.77735;-1.65500;-3.65079;,
 8.75662;-1.80830;-1.25542;,
 8.72512;-2.42304;-0.19039;,
 7.74660;-3.88153;-0.43837;,
 7.69853;-3.00035;-2.01430;,
 7.02109;-3.79487;-0.66639;,
 6.86218;-2.87793;-2.08786;,
 8.73665;-1.35848;-1.76486;,
 8.77331;-1.74047;2.77558;,
 8.75295;-0.78803;3.21460;,
 8.78291;0.46269;3.14163;,
 8.77224;1.29135;2.84064;,
 8.79007;2.09830;2.10251;,
 8.75795;2.49803;1.19731;,
 8.75349;2.54479;-0.02411;,
 8.79173;2.31246;-0.73366;,
 8.75349;2.54479;-0.02411;,
 7.61403;4.14346;-0.48279;,
 6.81869;4.31817;-0.26697;,
 7.77429;-3.02257;3.87982;,
 8.77810;-2.09669;2.43939;,
 0.51417;-2.05948;2.89000;,
 0.99283;-2.88895;1.65471;,
 -0.61066;-2.16287;2.88378;,
 -0.26440;-2.92470;1.61883;,
 8.76082;1.91101;-1.55793;,
 5.70092;-3.55804;0.82034;,
 6.00361;-3.49404;-0.92006;,
 5.83664;-2.73200;-2.02960;,
 5.78977;-2.07893;-2.68906;,
 5.54075;-1.51054;-3.65411;,
 5.55823;-0.57942;-3.69998;,
 5.59907;0.30718;-4.00371;,
 5.62323;2.02899;-3.67187;,
 5.61895;3.10694;-2.37657;,
 5.51385;3.61043;-1.48287;,
 5.57200;4.22136;0.14736;,
 5.57200;4.22136;0.14736;,
 5.42023;3.51275;1.80863;,
 0.37539;0.65901;3.88587;,
 0.37328;-0.53437;3.88634;,
 0.37430;-1.18117;3.83552;,
 1.30113;-3.05993;1.17527;,
 1.74419;-2.95194;-0.20169;,
 2.01852;-2.55893;-1.56904;,
 2.02253;-2.22890;-2.07961;,
 2.02671;-1.68200;-2.90464;,
 1.99845;-0.99266;-3.58523;,
 1.63795;3.07647;-2.90835;,
 1.52046;3.71537;-2.23476;,
 1.36957;4.02702;-0.40669;,
 1.36957;4.02702;-0.40669;,
 1.37180;3.99239;0.94434;,
 8.72512;-2.42304;-0.19039;,
 7.74660;-3.88153;-0.43837;,
 7.02109;-3.79487;-0.66639;,
 6.00361;-3.49404;-0.92006;,
 1.74419;-2.95194;-0.20169;,
 0.89716;-1.07815;-3.58642;,
 0.91488;-1.85922;-2.99396;,
 0.92334;-2.29074;-2.29095;,
 0.60855;-2.94626;-0.13707;,
 0.91596;-2.61773;-1.60211;,
 0.26390;-3.09146;0.83670;,
 0.60855;-2.94626;-0.13707;,
 -0.74588;-1.11131;3.92752;,
 -0.79636;-0.36345;3.70131;,
 -0.41726;0.83028;3.84077;,
 -0.00663;2.22042;3.27597;,
 0.15574;3.32350;2.31115;,
 0.34842;4.00010;0.86581;,
 0.43168;4.13614;-0.76110;,
 0.47807;3.71981;-2.37722;,
 0.43168;4.13614;-0.76110;,
 0.66009;2.90218;-3.31170;;
 
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
  -0.111600;0.443691;0.889204;,
  -0.066338;0.795277;0.602606;,
  -0.016258;0.803516;0.595062;,
  -0.065406;0.491720;0.868294;,
  -0.010053;0.798473;0.601946;,
  0.028871;0.966388;0.255460;,
  0.130096;-0.851385;-0.508152;,
  0.136566;-0.748534;-0.648881;,
  0.093099;-0.576595;-0.811709;,
  0.112319;-0.269457;-0.956440;,
  0.162441;0.258705;-0.952200;,
  0.093265;0.333317;-0.938191;,
  0.067810;-0.208945;-0.975574;,
  0.112051;0.888988;-0.444010;,
  0.143685;0.639395;-0.755333;,
  0.075072;0.994519;-0.072770;,
  -0.037228;0.523167;0.851417;,
  -0.080415;0.157981;0.984162;,
  -0.093177;0.457692;0.884215;,
  -0.131157;0.158129;0.978669;,
  0.456984;0.186830;0.869632;,
  -0.058821;0.484318;0.872913;,
  -0.148032;-0.104630;0.983432;,
  -0.181442;-0.490839;0.852148;,
  -0.172364;-0.512470;0.841228;,
  -0.146321;-0.131159;0.980504;,
  -0.044405;0.896320;-0.441179;,
  -0.034107;0.990794;-0.131011;,
  0.469714;0.870885;-0.144663;,
  0.421555;0.822538;-0.381736;,
  -0.066700;0.470305;-0.879980;,
  -0.031036;0.850212;-0.525524;,
  0.448350;0.748489;-0.488618;,
  0.290024;0.488874;-0.822732;,
  -0.083143;-0.085531;-0.992860;,
  -0.067226;-0.087480;-0.993895;,
  -0.053642;-0.216380;-0.974834;,
  0.456351;-0.181442;-0.871104;,
  -0.169800;-0.985478;0.001044;,
  -0.108700;-0.993729;0.026202;,
  -0.154222;-0.882818;0.443676;,
  -0.195692;-0.889142;0.413680;,
  -0.096995;0.917241;0.386343;,
  -0.127819;0.749939;0.649041;,
  -0.145281;0.717636;0.681096;,
  -0.074748;0.926612;0.368513;,
  -0.203540;-0.684804;0.699725;,
  -0.214155;-0.708470;0.672464;,
  -0.211836;-0.903924;0.371547;,
  0.836126;0.264174;-0.480734;,
  0.819310;0.464091;-0.336676;,
  0.836624;0.058443;-0.544651;,
  0.836436;-0.087629;-0.541014;,
  0.835549;-0.271119;-0.477863;,
  0.844279;-0.535903;0.000569;,
  0.869233;-0.452340;0.199555;,
  -0.132492;-0.509348;0.850301;,
  -0.097819;-0.133930;0.986151;,
  -0.152085;0.110654;0.982154;,
  0.003608;-0.996777;0.080145;,
  0.009110;-0.913169;0.407479;,
  0.470145;-0.654934;-0.591629;,
  -0.042219;-0.779226;-0.625319;,
  -0.033497;-0.547915;-0.835863;,
  0.803528;-0.477452;-0.355503;,
  0.821407;-0.535568;-0.196104;,
  0.440122;-0.852999;-0.280509;,
  0.445300;-0.703293;-0.554155;,
  -0.132726;-0.943495;-0.303647;,
  -0.071970;-0.773710;-0.629439;,
  0.806941;-0.412196;-0.423014;,
  0.059348;-0.928207;-0.367299;,
  0.025670;-0.984519;-0.173388;,
  0.870004;-0.267313;0.414290;,
  0.865095;-0.070755;0.496592;,
  0.843983;0.116988;0.523457;,
  0.835407;0.275390;0.475662;,
  0.840495;0.420975;0.341098;,
  0.827596;0.543375;0.140814;,
  0.812360;0.577458;-0.081323;,
  0.809662;0.544564;-0.218855;,
  0.867629;-0.370524;0.331559;,
  -0.077444;-0.762483;0.642356;,
  -0.088554;-0.918080;0.386377;,
  0.040175;-0.780172;0.624273;,
  -0.066921;0.029134;0.997333;,
  0.012178;-0.339907;0.940380;,
  -0.185647;-0.982594;-0.006675;,
  -0.166725;-0.942793;-0.288694;,
  -0.123343;-0.805945;-0.578999;,
  -0.097751;-0.801482;-0.589976;,
  -0.079411;-0.600299;-0.795823;,
  -0.043426;-0.303092;-0.951971;,
  -0.038406;-0.115513;-0.992563;,
  -0.032836;0.444871;-0.894992;,
  0.020565;0.798051;-0.602239;,
  0.026442;0.901808;-0.431327;,
  -0.025157;0.998350;-0.051623;,
  -0.061711;0.926235;0.371860;,
  -0.094697;0.133891;0.986461;,
  -0.084161;-0.025920;0.996115;,
  -0.072406;-0.407338;0.910403;,
  -0.069190;-0.996570;0.045406;,
  -0.054755;-0.974054;-0.219592;,
  -0.029320;-0.891082;-0.452895;,
  0.026687;-0.838381;-0.544431;,
  0.034761;-0.708296;-0.705059;,
  0.035665;-0.490375;-0.870781;,
  0.105007;0.691932;-0.714285;,
  0.088822;0.895621;-0.435860;,
  0.042353;0.997942;-0.048141;,
  0.012264;0.954647;0.297487;,
  0.839297;0.137710;-0.525944;,
  0.019344;-0.556168;-0.830845;,
  -0.172715;-0.686778;0.706050;;
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
