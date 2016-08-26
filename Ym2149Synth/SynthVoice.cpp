/*
 * Ym2149Synth
 * http://trash80.com
 * Copyright (c) 2016 Timothy Lamb
 *
 * This file is part of Ym2149Synth.
 *
 * Ym2149Synth is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ym2149Synth is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 #include "SynthVoice.h"

/*
 *
 * ~~~~~ BEHOLD ALLMIGHTY JAVASCRIPT FOR TABLE GENERATION. ~~~
 * Run in a browser console.
 *
 * For YM pitch "freqTable":
 * function nf(n) {return Math.round((2000000.0/((Math.pow(2,(((n) - 69)/12)) * 440.0)))/16);};var out = [];for(var i =0; i<128; i += 0.1) out.push(nf(i));copy(out.toString());console.log("Data in copybuffer. Array size is "+out.length);
 *
 * For SoftSynth pitch "softFreqTable":
 * function nf(n) {return Math.round((Math.pow(2,(((n) - 69)/12)) * 440.0) * (1.0/22050) * 1000000) / 1000000;};var out = [];for(var i =0; i<128; i += 0.1) out.push(nf(i));copy(out.toString());console.log("Data in copybuffer. Array size is "+out.length);
 *
*/
const static uint16_t tableSize = 1281;
const static uint16_t freqTable[tableSize] = {
    15289,15201,15113,15026,14940,14854,14768,14683,14599,14515,14431,14348,14265,14183,14101,14020,13939,13859,13779,13700,13621,13543,13465,13387,13310,13233,13157,13081,13006,12931,12856,12782,12709,12636,12563,12490,12419,12347,12276,12205,12135,12065,11996,11926,11858,11789,11722,11654,11587,11520,11454,11388,11322,11257,11192,11128,11064,11000,10937,10874,10811,10749,10687,10625,10564,10503,10443,10383,10323,10263,10204,10145,10087,10029,9971,9914,9857,9800,9743,9687,9631,9576,9521,9466,9411,9357,9303,9250,9197,9144,9091,9039,8986,8935,8883,8832,8781,8731,8680,8630,8581,8531,8482,8433,8385,8336,8288,8241,8193,8146,8099,8052,8006,7960,7914,7869,7823,7778,7733,7689,7645,7600,7557,7513,7470,7427,7384,7342,7299,7257,7215,7174,7133,7092,7051,7010,6970,6930,6890,6850,6810,6771,6732,6693,6655,6617,6578,6541,6503,6465,6428,6391,6354,6318,6281,6245,6209,6174,6138,6103,6067,6033,5998,5963,5929,5895,5861,5827,5793,5760,5727,5694,5661,5629,5596,5564,5532,5500,5468,5437,5405,5374,5343,5313,5282,5252,5221,5191,5161,5132,5102,5073,5043,5014,4986,4957,4928,4900,4872,4844,4816,4788,4760,4733,4706,4679,4652,4625,4598,4572,4545,4519,4493,4467,4442,4416,4391,4365,4340,4315,4290,4266,4241,4217,4192,4168,4144,4120,4097,4073,4050,4026,4003,3980,3957,3934,3912,3889,3867,3844,3822,3800,3778,3757,3735,3713,3692,3671,3650,3629,3608,3587,3566,3546,3525,3505,3485,3465,3445,3425,3405,3386,3366,3347,3327,3308,3289,3270,3251,3233,3214,3196,3177,3159,3141,3123,3105,3087,3069,3051,3034,3016,2999,2982,2964,2947,2930,2914,2897,2880,2863,2847,2831,2814,2798,2782,2766,2750,2734,2718,2703,2687,2672,2656,2641,2626,2611,2596,2581,2566,2551,2536,2522,2507,2493,2478,2464,2450,2436,2422,2408,2394,2380,2367,2353,2339,2326,2312,2299,2286,2273,2260,2247,2234,2221,2208,2195,2183,2170,2158,2145,2133,2121,2108,2096,2084,2072,2060,2048,2036,2025,2013,2002,1990,1979,1967,1956,1945,1933,1922,1911,1900,1889,1878,1867,1857,1846,1835,1825,1814,1804,1793,1783,1773,1763,1753,1742,1732,1722,1712,1703,1693,1683,1673,1664,1654,1645,1635,1626,1616,1607,1598,1589,1579,1570,1561,1552,1543,1534,1526,1517,1508,1499,1491,1482,1474,1465,1457,1448,1440,1432,1423,1415,1407,1399,1391,1383,1375,1367,1359,1351,1344,1336,1328,1321,1313,1305,1298,1290,1283,1276,1268,1261,1254,1246,1239,1232,1225,1218,1211,1204,1197,1190,1183,1176,1170,1163,1156,1150,1143,1136,1130,1123,1117,1110,1104,1098,1091,1085,1079,1073,1066,1060,1054,1048,1042,1036,1030,1024,1018,1012,1007,1001,995,989,984,978,972,967,961,956,950,945,939,934,928,923,918,912,907,902,897,892,886,881,876,871,866,861,856,851,846,842,837,832,827,822,818,813,808,804,799,794,790,785,781,776,772,767,763,758,754,750,745,741,737,733,728,724,720,716,712,708,704,700,695,691,687,684,680,676,672,668,664,660,656,653,649,645,641,638,634,630,627,623,620,616,612,609,605,602,599,595,592,588,585,581,578,575,571,568,565,562,558,555,552,549,546,543,539,536,533,530,527,524,521,518,515,512,509,506,503,500,497,495,492,489,486,483,481,478,475,472,470,467,464,462,459,456,454,451,448,446,443,441,438,436,433,431,428,426,423,421,418,416,414,411,409,406,404,402,399,397,395,393,390,388,386,384,381,379,377,375,373,371,368,366,364,362,360,358,356,354,352,350,348,346,344,342,340,338,336,334,332,330,328,326,324,323,321,319,317,315,313,312,310,308,306,304,303,301,299,298,296,294,292,291,289,287,286,284,282,281,279,278,276,274,273,271,270,268,267,265,264,262,261,259,258,256,255,253,252,250,249,247,246,244,243,242,240,239,238,236,235,233,232,231,229,228,227,225,224,223,222,220,219,218,217,215,214,213,212,210,209,208,207,206,204,203,202,201,200,199,197,196,195,194,193,192,191,190,189,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,159,158,157,156,155,154,153,152,151,150,150,149,148,147,146,145,145,144,143,142,141,140,140,139,138,137,136,136,135,134,133,133,132,131,130,130,129,128,127,127,126,125,124,124,123,122,122,121,120,119,119,118,117,117,116,115,115,114,113,113,112,111,111,110,110,109,108,108,107,106,106,105,105,104,103,103,102,102,101,100,100,99,99,98,98,97,96,96,95,95,94,94,93,93,92,92,91,91,90,89,89,88,88,87,87,86,86,85,85,84,84,83,83,83,82,82,81,81,80,80,79,79,78,78,77,77,77,76,76,75,75,74,74,74,73,73,72,72,71,71,71,70,70,69,69,69,68,68,67,67,67,66,66,66,65,65,64,64,64,63,63,63,62,62,61,61,61,60,60,60,59,59,59,58,58,58,57,57,57,56,56,56,55,55,55,54,54,54,54,53,53,53,52,52,52,51,51,51,51,50,50,50,49,49,49,49,48,48,48,47,47,47,47,46,46,46,46,45,45,45,44,44,44,44,43,43,43,43,42,42,42,42,42,41,41,41,41,40,40,40,40,39,39,39,39,39,38,38,38,38,37,37,37,37,37,36,36,36,36,36,35,35,35,35,35,34,34,34,34,34,33,33,33,33,33,32,32,32,32,32,31,31,31,31,31,31,30,30,30,30,30,30,29,29,29,29,29,29,28,28,28,28,28,28,27,27,27,27,27,27,26,26,26,26,26,26,26,25,25,25,25,25,25,25,24,24,24,24,24,24,24,23,23,23,23,23,23,23,23,22,22,22,22,22,22,22,21,21,21,21,21,21,21,21,21,20,20,20,20,20,20,20,20,19,19,19,19,19,19,19,19,19,18,18,18,18,18,18,18,18,18,18,17,17,17,17,17,17,17,17,17,17,16,16,16,16,16,16,16,16,16,16,16,15,15,15,15,15,15,15,15,15,15,15,15,14,14,14,14,14,14,14,14,14,14,14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9
};
const static float softFreqTable[tableSize] = {
    0.000371,0.000373,0.000375,0.000377,0.000379,0.000382,0.000384,0.000386,0.000388,0.000391,0.000393,0.000395,0.000397,0.0004,0.000402,0.000404,0.000407,0.000409,0.000411,0.000414,0.000416,0.000419,0.000421,0.000423,0.000426,0.000428,0.000431,0.000433,0.000436,0.000438,0.000441,0.000443,0.000446,0.000449,0.000451,0.000454,0.000456,0.000459,0.000462,0.000464,0.000467,0.00047,0.000473,0.000475,0.000478,0.000481,0.000484,0.000486,0.000489,0.000492,0.000495,0.000498,0.000501,0.000504,0.000507,0.000509,0.000512,0.000515,0.000518,0.000521,0.000524,0.000527,0.00053,0.000534,0.000537,0.00054,0.000543,0.000546,0.000549,0.000552,0.000556,0.000559,0.000562,0.000565,0.000569,0.000572,0.000575,0.000578,0.000582,0.000585,0.000589,0.000592,0.000595,0.000599,0.000602,0.000606,0.000609,0.000613,0.000616,0.00062,0.000624,0.000627,0.000631,0.000634,0.000638,0.000642,0.000646,0.000649,0.000653,0.000657,0.000661,0.000664,0.000668,0.000672,0.000676,0.00068,0.000684,0.000688,0.000692,0.000696,0.0007,0.000704,0.000708,0.000712,0.000716,0.00072,0.000725,0.000729,0.000733,0.000737,0.000742,0.000746,0.00075,0.000755,0.000759,0.000763,0.000768,0.000772,0.000777,0.000781,0.000786,0.00079,0.000795,0.000799,0.000804,0.000809,0.000813,0.000818,0.000823,0.000828,0.000832,0.000837,0.000842,0.000847,0.000852,0.000857,0.000862,0.000867,0.000872,0.000877,0.000882,0.000887,0.000892,0.000897,0.000902,0.000908,0.000913,0.000918,0.000924,0.000929,0.000934,0.00094,0.000945,0.000951,0.000956,0.000962,0.000967,0.000973,0.000979,0.000984,0.00099,0.000996,0.001001,0.001007,0.001013,0.001019,0.001025,0.001031,0.001037,0.001043,0.001049,0.001055,0.001061,0.001067,0.001073,0.001079,0.001086,0.001092,0.001098,0.001105,0.001111,0.001118,0.001124,0.001131,0.001137,0.001144,0.00115,0.001157,0.001164,0.00117,0.001177,0.001184,0.001191,0.001198,0.001205,0.001212,0.001219,0.001226,0.001233,0.00124,0.001247,0.001254,0.001262,0.001269,0.001276,0.001284,0.001291,0.001299,0.001306,0.001314,0.001321,0.001329,0.001337,0.001344,0.001352,0.00136,0.001368,0.001376,0.001384,0.001392,0.0014,0.001408,0.001416,0.001424,0.001433,0.001441,0.001449,0.001458,0.001466,0.001475,0.001483,0.001492,0.0015,0.001509,0.001518,0.001527,0.001535,0.001544,0.001553,0.001562,0.001571,0.00158,0.00159,0.001599,0.001608,0.001617,0.001627,0.001636,0.001646,0.001655,0.001665,0.001674,0.001684,0.001694,0.001704,0.001714,0.001723,0.001733,0.001743,0.001754,0.001764,0.001774,0.001784,0.001795,0.001805,0.001815,0.001826,0.001837,0.001847,0.001858,0.001869,0.001879,0.00189,0.001901,0.001912,0.001923,0.001935,0.001946,0.001957,0.001968,0.00198,0.001991,0.002003,0.002014,0.002026,0.002038,0.00205,0.002061,0.002073,0.002085,0.002097,0.00211,0.002122,0.002134,0.002147,0.002159,0.002171,0.002184,0.002197,0.002209,0.002222,0.002235,0.002248,0.002261,0.002274,0.002287,0.002301,0.002314,0.002327,0.002341,0.002354,0.002368,0.002382,0.002395,0.002409,0.002423,0.002437,0.002451,0.002466,0.00248,0.002494,0.002509,0.002523,0.002538,0.002553,0.002567,0.002582,0.002597,0.002612,0.002627,0.002643,0.002658,0.002673,0.002689,0.002704,0.00272,0.002736,0.002752,0.002768,0.002784,0.0028,0.002816,0.002832,0.002849,0.002865,0.002882,0.002899,0.002915,0.002932,0.002949,0.002966,0.002983,0.003001,0.003018,0.003036,0.003053,0.003071,0.003089,0.003107,0.003125,0.003143,0.003161,0.003179,0.003198,0.003216,0.003235,0.003253,0.003272,0.003291,0.00331,0.00333,0.003349,0.003368,0.003388,0.003407,0.003427,0.003447,0.003467,0.003487,0.003507,0.003528,0.003548,0.003569,0.003589,0.00361,0.003631,0.003652,0.003673,0.003694,0.003716,0.003737,0.003759,0.003781,0.003803,0.003825,0.003847,0.003869,0.003891,0.003914,0.003937,0.00396,0.003982,0.004006,0.004029,0.004052,0.004076,0.004099,0.004123,0.004147,0.004171,0.004195,0.004219,0.004244,0.004268,0.004293,0.004318,0.004343,0.004368,0.004393,0.004419,0.004444,0.00447,0.004496,0.004522,0.004548,0.004575,0.004601,0.004628,0.004655,0.004682,0.004709,0.004736,0.004763,0.004791,0.004819,0.004847,0.004875,0.004903,0.004931,0.00496,0.004989,0.005018,0.005047,0.005076,0.005105,0.005135,0.005165,0.005195,0.005225,0.005255,0.005285,0.005316,0.005347,0.005378,0.005409,0.00544,0.005472,0.005503,0.005535,0.005567,0.0056,0.005632,0.005665,0.005697,0.00573,0.005764,0.005797,0.005831,0.005864,0.005898,0.005933,0.005967,0.006001,0.006036,0.006071,0.006106,0.006142,0.006177,0.006213,0.006249,0.006285,0.006322,0.006358,0.006395,0.006432,0.006469,0.006507,0.006545,0.006583,0.006621,0.006659,0.006698,0.006736,0.006775,0.006815,0.006854,0.006894,0.006934,0.006974,0.007014,0.007055,0.007096,0.007137,0.007178,0.00722,0.007262,0.007304,0.007346,0.007389,0.007431,0.007475,0.007518,0.007561,0.007605,0.007649,0.007694,0.007738,0.007783,0.007828,0.007873,0.007919,0.007965,0.008011,0.008057,0.008104,0.008151,0.008198,0.008246,0.008294,0.008342,0.00839,0.008438,0.008487,0.008537,0.008586,0.008636,0.008686,0.008736,0.008787,0.008838,0.008889,0.00894,0.008992,0.009044,0.009097,0.009149,0.009202,0.009256,0.009309,0.009363,0.009417,0.009472,0.009527,0.009582,0.009637,0.009693,0.009749,0.009806,0.009863,0.00992,0.009977,0.010035,0.010093,0.010152,0.010211,0.01027,0.010329,0.010389,0.010449,0.01051,0.010571,0.010632,0.010693,0.010755,0.010818,0.01088,0.010943,0.011007,0.011071,0.011135,0.011199,0.011264,0.011329,0.011395,0.011461,0.011527,0.011594,0.011661,0.011729,0.011797,0.011865,0.011934,0.012003,0.012073,0.012142,0.012213,0.012284,0.012355,0.012426,0.012498,0.012571,0.012643,0.012717,0.01279,0.012864,0.012939,0.013014,0.013089,0.013165,0.013241,0.013318,0.013395,0.013473,0.013551,0.013629,0.013708,0.013788,0.013868,0.013948,0.014029,0.01411,0.014192,0.014274,0.014357,0.01444,0.014524,0.014608,0.014692,0.014777,0.014863,0.014949,0.015036,0.015123,0.01521,0.015299,0.015387,0.015476,0.015566,0.015656,0.015747,0.015838,0.01593,0.016022,0.016115,0.016208,0.016302,0.016397,0.016492,0.016587,0.016683,0.01678,0.016877,0.016975,0.017073,0.017172,0.017271,0.017372,0.017472,0.017573,0.017675,0.017778,0.017881,0.017984,0.018088,0.018193,0.018298,0.018404,0.018511,0.018618,0.018726,0.018835,0.018944,0.019054,0.019164,0.019275,0.019387,0.019499,0.019612,0.019725,0.01984,0.019955,0.02007,0.020187,0.020303,0.020421,0.020539,0.020658,0.020778,0.020898,0.021019,0.021141,0.021264,0.021387,0.021511,0.021635,0.021761,0.021887,0.022014,0.022141,0.022269,0.022398,0.022528,0.022659,0.02279,0.022922,0.023055,0.023188,0.023323,0.023458,0.023594,0.02373,0.023868,0.024006,0.024145,0.024285,0.024426,0.024567,0.024709,0.024853,0.024996,0.025141,0.025287,0.025433,0.025581,0.025729,0.025878,0.026028,0.026179,0.02633,0.026483,0.026636,0.026791,0.026946,0.027102,0.027259,0.027417,0.027576,0.027735,0.027896,0.028058,0.02822,0.028384,0.028548,0.028713,0.02888,0.029047,0.029215,0.029385,0.029555,0.029726,0.029898,0.030071,0.030246,0.030421,0.030597,0.030774,0.030953,0.031132,0.031312,0.031494,0.031676,0.03186,0.032044,0.03223,0.032416,0.032604,0.032793,0.032983,0.033174,0.033366,0.03356,0.033754,0.03395,0.034146,0.034344,0.034543,0.034743,0.034944,0.035147,0.03535,0.035555,0.035761,0.035968,0.036177,0.036386,0.036597,0.036809,0.037022,0.037237,0.037452,0.037669,0.037888,0.038107,0.038328,0.03855,0.038773,0.038998,0.039224,0.039451,0.039679,0.039909,0.04014,0.040373,0.040607,0.040842,0.041079,0.041317,0.041556,0.041797,0.042039,0.042282,0.042527,0.042774,0.043022,0.043271,0.043521,0.043774,0.044027,0.044282,0.044539,0.044797,0.045056,0.045317,0.04558,0.045844,0.046109,0.046376,0.046645,0.046915,0.047187,0.04746,0.047735,0.048012,0.04829,0.04857,0.048851,0.049134,0.049419,0.049705,0.049993,0.050283,0.050574,0.050867,0.051161,0.051458,0.051756,0.052056,0.052357,0.052661,0.052966,0.053273,0.053581,0.053892,0.054204,0.054518,0.054834,0.055151,0.055471,0.055792,0.056115,0.05644,0.056767,0.057096,0.057427,0.057759,0.058094,0.058431,0.058769,0.05911,0.059452,0.059796,0.060143,0.060491,0.060842,0.061194,0.061549,0.061905,0.062264,0.062624,0.062987,0.063352,0.063719,0.064088,0.064459,0.064833,0.065208,0.065586,0.065966,0.066348,0.066733,0.067119,0.067508,0.067899,0.068292,0.068688,0.069086,0.069486,0.069889,0.070294,0.070701,0.07111,0.071522,0.071937,0.072353,0.072772,0.073194,0.073618,0.074044,0.074473,0.074905,0.075339,0.075775,0.076214,0.076656,0.0771,0.077546,0.077996,0.078447,0.078902,0.079359,0.079819,0.080281,0.080746,0.081214,0.081684,0.082157,0.082633,0.083112,0.083594,0.084078,0.084565,0.085055,0.085547,0.086043,0.086541,0.087043,0.087547,0.088054,0.088564,0.089077,0.089593,0.090112,0.090634,0.091159,0.091687,0.092219,0.092753,0.09329,0.093831,0.094374,0.094921,0.095471,0.096024,0.09658,0.09714,0.097702,0.098268,0.098837,0.09941,0.099986,0.100565,0.101148,0.101734,0.102323,0.102916,0.103512,0.104112,0.104715,0.105321,0.105931,0.106545,0.107162,0.107783,0.108407,0.109035,0.109667,0.110302,0.110941,0.111584,0.11223,0.112881,0.113534,0.114192,0.114854,0.115519,0.116188,0.116861,0.117538,0.118219,0.118904,0.119593,0.120286,0.120982,0.121683,0.122388,0.123097,0.12381,0.124527,0.125249,0.125974,0.126704,0.127438,0.128176,0.128919,0.129666,0.130417,0.131172,0.131932,0.132696,0.133465,0.134238,0.135016,0.135798,0.136585,0.137376,0.138172,0.138972,0.139777,0.140587,0.141401,0.142221,0.143044,0.143873,0.144707,0.145545,0.146388,0.147236,0.148089,0.148947,0.14981,0.150677,0.15155,0.152428,0.153311,0.154199,0.155093,0.155991,0.156895,0.157804,0.158718,0.159637,0.160562,0.161492,0.162428,0.163369,0.164315,0.165267,0.166224,0.167187,0.168156,0.16913,0.170109,0.171095,0.172086,0.173083,0.174086,0.175094,0.176108,0.177129,0.178155,0.179187,0.180225,0.181269,0.182319,0.183375,0.184437,0.185506,0.18658,0.187661,0.188748,0.189842,0.190941,0.192048,0.19316,0.194279,0.195404,0.196536,0.197675,0.19882,0.199972,0.20113,0.202295,0.203467,0.204646,0.205831,0.207024,0.208223,0.209429,0.210643,0.211863,0.21309,0.214324,0.215566,0.216815,0.218071,0.219334,0.220605,0.221883,0.223168,0.224461,0.225761,0.227069,0.228384,0.229707,0.231038,0.232376,0.233723,0.235076,0.236438,0.237808,0.239186,0.240571,0.241965,0.243366,0.244776,0.246194,0.24762,0.249055,0.250498,0.251949,0.253408,0.254876,0.256353,0.257838,0.259331,0.260834,0.262345,0.263864,0.265393,0.26693,0.268477,0.270032,0.271596,0.27317,0.274752,0.276344,0.277944,0.279555,0.281174,0.282803,0.284441,0.286089,0.287746,0.289413,0.29109,0.292776,0.294472,0.296178,0.297894,0.299619,0.301355,0.303101,0.304856,0.306622,0.308399,0.310185,0.311982,0.313789,0.315607,0.317435,0.319274,0.321124,0.322984,0.324855,0.326737,0.32863,0.330534,0.332448,0.334374,0.336311,0.338259,0.340219,0.34219,0.344172,0.346166,0.348171,0.350188,0.352217,0.354257,0.356309,0.358373,0.360449,0.362537,0.364638,0.36675,0.368875,0.371011,0.373161,0.375322,0.377497,0.379683,0.381883,0.384095,0.38632,0.388558,0.390809,0.393073,0.39535,0.39764,0.399944,0.402261,0.404591,0.406935,0.409292,0.411663,0.414048,0.416446,0.418859,0.421285,0.423726,0.42618,0.428649,0.431132,0.43363,0.436142,0.438668,0.441209,0.443765,0.446336,0.448922,0.451522,0.454138,0.456769,0.459415,0.462076,0.464753,0.467445,0.470153,0.472876,0.475616,0.478371,0.481142,0.483929,0.486733,0.489552,0.492388,0.495241,0.49811,0.500995,0.503897,0.506816,0.509752,0.512705,0.515675,0.518663,0.521667,0.524689,0.527729,0.530786,0.533861,0.536953,0.540064,0.543192,0.546339,0.549504,0.552687,0.555889,0.559109,0.562348,0.565606,0.568882,0.572178,0.575492,0.578826,0.582179,0.585552,0.588944,0.592356,0.595787,0.599238,0.60271
};
void SynthVoice::begin(YM2149 * ym, uint8_t sy)
{
    Ym = ym;
    synth = sy;

    enableVoice = true;
    enableSoftsynth = true;

    Ym->setNoise(synth,0);
    Ym->setEnv(synth,0);
    volumeEnvelope.begin();
    pitchEnvelope.begin();

    volumeEnvelope.setShape(0x00);
    volumeEnvelope.setRange(0,15);

    pitchEnvelope.setShape(0x00);
    pitchEnvelope.setRange(0,255);

    synthType = 255;
    setSynthType(0);
}

void SynthVoice::updateSoftsynth()
{
    if(!enableSoftsynth || volume <= 0) {
        return;
    }

    softPhase += softIncrement;

    if(softPhase > 1) softPhase -= 1;
    if(softPhase < 0) softPhase += 1;

    if(softPhase >= softWidth) {
        if(softWavPos) {
            softWavPos = 0;
            Ym->setVolume(synth,volume);
        }
    } else {
        if(!softWavPos) {
            softWavPos = 1;
            Ym->setVolume(synth,0);
        }
    }
}

void SynthVoice::updateEvents()
{
    uint16_t f = currentFreq+transpose;

    if(playing) {
        if(glide && notePitch != currentFreq) {
            if(notePitch > currentFreq) {
                currentFreq += glideIncrement;
                if(currentFreq > notePitch) currentFreq = notePitch;
                f = currentFreq+transpose;
            } else if(notePitch < currentFreq) {
                currentFreq -= glideIncrement;
                if(currentFreq < notePitch) currentFreq = notePitch;
                f = currentFreq+transpose;
            }
        }

        if(vibratoAmount && currentFreq) {
            vibratoPhase += vibratoIncrement;
            if(vibratoPhase > 1) vibratoPhase -= 2;
            if(vibratoPhase < -1) vibratoPhase += 2;
            f += (((abs(vibratoPhase) * 2) - 1) * vibratoAmount);
        }

        if(pitchEnvAmount) {
            pitchEnvelope.update();
            float envAmt = pitchEnvelope.read();
            envAmt /=255.0f;
            if(pitchEnvelope.getShape() & 0x80) {
                envAmt *= pitchEnvAmount * 10;
            } else {
                envAmt = (1.0f - envAmt) * pitchEnvAmount * -10;
            }
            f += envAmt;
        }

        if(bendWheel) {
            f += (bendWheel * 10);
        }

        if(f != lastNoteFreq) {
            lastNoteFreq = f;

            if(enableSoftsynth) {
                uint16_t sf = f;
                if(enableSoftDetune) sf +=pwmFreq+softFreqDetune;
                if(sf>=tableSize) sf = tableSize-1;
                softIncrement = softFreqTable[sf];
            }

            if(enableVoice) {
                Ym->setTone(synth,freqTable[f]);
            }
            if(enableEnv) {
                Ym->setTone(4,freqTable[f+pwmFreq]);
            }
            if (enableNoise || noiseDelayTriggered) {
                Ym->setTone(3,31 - ((f/10)>>2));
            }
        }

        if(volumeEnvelope.update()) {
            volume = volumeEnvelope.read();
            if(volume == 0 && volumeEnvelope.getShape() & 0x80) {
                playing = false;
            }
            Ym->setVolume(synth,volume);
        }

        if(noiseDelay && !noiseDelayTriggered) {
            noiseDelayPhase += noiseDelayIncrement;
            if(noiseDelayPhase >= 1) {
                noiseDelayPhase = 0;
                noiseDelayTriggered = true;
                if(!enableNoise) {
                    Ym->setNoise(synth,1);
                } else {
                    Ym->setNoise(synth,0);
                }
            }
        }
    }
}

void SynthVoice::playNote(uint8_t n, uint8_t v)
{
    if(v) {
        velocity = v;
        note     = n;
        notePitch = ((uint16_t)n)*10;
        if(playing == false) {
            volumeEnvelope.reset();
            volumeEnvelope.setRange(0,v>>3);
            currentFreq = notePitch;
            volume = -1;
        } else {
            if(glide) {
                glideIncrement = abs(((int)notePitch) - currentFreq);
                glideIncrement *= glide;
            } else {
                glideIncrement = 0;
                currentFreq = notePitch;
            }
        }

        pitchEnvelope.reset();

        if(softIncrement > 1) softIncrement = 1;
        if(softIncrement < 0) softIncrement = 0;

        if(!enableVoice) {
            Ym->setNote(synth,255);
        }

        if(enableNoise) {
            Ym->setNoise(synth,1);
        } else {
            noiseDelayTriggered = false;
            enableNoise = false;
            Ym->setNoise(synth,0);
        }
        noiseDelayPhase = 0;

        if(enableEnv) {
            Ym->setEnv(synth,1);
            Ym->setEnvShape(0,0,0,0);
            if(envType == 1) {
                Ym->setEnvShape(1,0,0,0);
            } else {
                Ym->setEnvShape(1,0,1,0);
            }
        }

        playing = true;
        lastNoteFreq = 0;
    } else if(n == note) {
        playing = false;
        volume = 0;
        if(enableEnv) {
            Ym->setEnv(synth,0);
        }
        Ym->setVolume(synth,0);
    }
}

void SynthVoice::setPitchbend(int v)
{
    bendWheel = v;
    bendWheel /= 0x2000;
    bendWheel *= 2;
}

void SynthVoice::setGlide(uint8_t v)
{
    unsigned int ms = map(v, 0, 127, 0, 1000);
    glide = 1.0f/ms;
}

void SynthVoice::setPitchEnvelopeAmount(uint8_t v)
{
    pitchEnvAmount = v;
}

void SynthVoice::setVibratoAttack(uint8_t v)
{
    unsigned int ms = map(v, 0, 127, 0, 1000);
    vibratoDelayIncrement = 1.0f/ms;
}

void SynthVoice::setVibratoAmount(uint8_t v)
{
    vibratoAmount = v;
}

void SynthVoice::setVibratoFreq(uint8_t v)
{
    unsigned int ms = (pow(8000.0f,1.0f - ((float)v)/127.0f)) + 5;
    vibratoIncrement = 1.0f/ms;
}

void SynthVoice::setPwmFreq(uint8_t v)
{
    pwmFreq = v;

    if(synthType == 6) {
        softWidth = ((float)(pwmFreq)/255) + 0.5;
    }
    if(playing) {
        lastNoteFreq = 0;
    }
}

void SynthVoice::setSoftDetune(uint8_t v)
{
    softFreqDetune = v;
    softFreqDetune *= 10;
    if(playing) {
        lastNoteFreq = 0;
    }
}

void SynthVoice::setNoiseDelay(uint8_t v)
{
    noiseDelay = v;
    unsigned int ms = (pow(4000.0f,((float)v)/127.0f)) + 5;
    noiseDelayIncrement = 1.0f/ms;
}

void SynthVoice::setTranspose(uint8_t v)
{
    transpose = (((int)v) - 64) * 10;
}

void SynthVoice::setSynthType(uint8_t v)
{
    if(v == synthType) return;

    synthType = v;

    if(enableNoise) Ym->setNoise(synth,0);
    if(enableEnv) Ym->setEnv(synth,0);

    enableVoice = false;
    enableSoftsynth = false;
    enableEnv   = false;
    envType     = 0;
    enableNoise = false;
    enableSoftDetune = false;
    softShape   = 1;

    switch(synthType)
    {
        case 0:
            enableVoice = true;
        break;
        case 1:
            enableVoice = true;
            enableEnv   = true;
            envType     = 1;
        break;
        case 2:
            enableVoice = true;
            enableEnv   = true;
            envType     = 2;
        break;
        case 3:
            enableVoice = false;
            enableEnv   = true;
            envType     = 2;
        break;
        case 4:
            enableVoice = false;
            enableEnv   = true;
            envType     = 1;
        break;
        case 5:
            enableVoice = true;
            enableSoftsynth = true;
            enableSoftDetune  = true;
            softWidth = 0.5;
            softShape = 1;
        break;
        case 6:
            enableSoftsynth = true;
            softWidth = ((float)(pwmFreq)/255) + 0.5;
        break;
        case 7:
            enableNoise = true;
        break;
        default:
            enableVoice = true;
        break;
    }
    if(playing) {
        lastNoteFreq = 0;
        playing = false;
        playNote(note, velocity);
    }
}