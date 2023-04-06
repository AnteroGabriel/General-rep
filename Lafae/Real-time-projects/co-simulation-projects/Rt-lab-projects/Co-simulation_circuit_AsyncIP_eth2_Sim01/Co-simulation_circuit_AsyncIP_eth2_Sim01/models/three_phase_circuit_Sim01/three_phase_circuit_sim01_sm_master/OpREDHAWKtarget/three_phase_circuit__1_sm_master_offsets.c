#include "three_phase_circuit__1_sm_master.h"
#include <stdlib.h>
#include <stdint.h>

// Declared in OpalSimCore
extern uint32_t opalNumParameters;
extern uint32_t opalNumSignals;
extern void** opalParametersAdressMapping;
extern uint32_t* opalParametersSizeMapping;
extern uint32_t* opalParametersNumElementsMapping;
extern void** opalSignalsAdressMapping;
extern uint32_t* opalSignalsSizeMapping;
extern uint32_t* opalSignalsNumElementsMapping;
void OpalUnInitAdress(void)
{
  opalNumParameters = 0;
  opalNumSignals = 0;
  if (opalParametersAdressMapping)
    free(opalParametersAdressMapping);
  if (opalParametersSizeMapping)
    free(opalParametersSizeMapping);
  if (opalParametersNumElementsMapping)
    free(opalParametersNumElementsMapping);
  if (opalSignalsAdressMapping)
    free(opalSignalsAdressMapping);
  if (opalSignalsSizeMapping)
    free(opalSignalsSizeMapping);
  if (opalSignalsNumElementsMapping)
    free(opalSignalsNumElementsMapping);
}

void OpalInitAdress(void)
{
  OpalUnInitAdress();
  opalParametersAdressMapping = (void**)malloc(sizeof(void*) * 167);
  opalParametersSizeMapping = (uint32_t*)malloc(sizeof(uint32_t) * 167);
  opalParametersNumElementsMapping = (uint32_t*)malloc(sizeof(uint32_t) * 167);
  opalSignalsAdressMapping = (void**)malloc(sizeof(void*) * 32);
  opalSignalsSizeMapping = (uint32_t*)malloc(sizeof(uint32_t) * 32);
  opalSignalsNumElementsMapping = (uint32_t*)malloc(sizeof(uint32_t) * 32);
  opalParametersAdressMapping[0] = (void*)&rtP.ThreePhaseSource_Frequency;
  opalParametersSizeMapping[0] = sizeof(rtP.ThreePhaseSource_Frequency);
  opalParametersNumElementsMapping[0] = 1;
  opalParametersAdressMapping[1] = (void*)&rtP.ThreePhaseSource1_Frequency;
  opalParametersSizeMapping[1] = sizeof(rtP.ThreePhaseSource1_Frequency);
  opalParametersNumElementsMapping[1] = 1;
  opalParametersAdressMapping[2] = (void*)&rtP.SFunction1_Value;
  opalParametersSizeMapping[2] = sizeof(rtP.SFunction1_Value);
  opalParametersNumElementsMapping[2] = 1;
  opalParametersAdressMapping[3] = (void*)&rtP.SFunction_InitialCondition;
  opalParametersSizeMapping[3] = sizeof(rtP.SFunction_InitialCondition);
  opalParametersNumElementsMapping[3] = 1;
  opalParametersAdressMapping[4] = (void*)&rtP.timeout_Value;
  opalParametersSizeMapping[4] = sizeof(rtP.timeout_Value);
  opalParametersNumElementsMapping[4] = 1;
  opalParametersAdressMapping[5] = (void*)&rtP.SFunction1_P1_Size;
  opalParametersSizeMapping[5] = sizeof(rtP.SFunction1_P1_Size[0]);
  opalParametersNumElementsMapping[5] = 2;
  opalParametersAdressMapping[6] = (void*)&rtP.SFunction1_P1;
  opalParametersSizeMapping[6] = sizeof(rtP.SFunction1_P1);
  opalParametersNumElementsMapping[6] = 1;
  opalParametersAdressMapping[7] = (void*)&rtP.SFunction1_P2_Size;
  opalParametersSizeMapping[7] = sizeof(rtP.SFunction1_P2_Size[0]);
  opalParametersNumElementsMapping[7] = 2;
  opalParametersAdressMapping[8] = (void*)&rtP.SFunction1_P2;
  opalParametersSizeMapping[8] = sizeof(rtP.SFunction1_P2);
  opalParametersNumElementsMapping[8] = 1;
  opalParametersAdressMapping[9] = (void*)&rtP.SFunction1_P3_Size;
  opalParametersSizeMapping[9] = sizeof(rtP.SFunction1_P3_Size[0]);
  opalParametersNumElementsMapping[9] = 2;
  opalParametersAdressMapping[10] = (void*)&rtP.SFunction1_P3;
  opalParametersSizeMapping[10] = sizeof(rtP.SFunction1_P3);
  opalParametersNumElementsMapping[10] = 1;
  opalParametersAdressMapping[11] = (void*)&rtP.SFunction1_P4_Size;
  opalParametersSizeMapping[11] = sizeof(rtP.SFunction1_P4_Size[0]);
  opalParametersNumElementsMapping[11] = 2;
  opalParametersAdressMapping[12] = (void*)&rtP.SFunction1_P4;
  opalParametersSizeMapping[12] = sizeof(rtP.SFunction1_P4);
  opalParametersNumElementsMapping[12] = 1;
  opalParametersAdressMapping[13] = (void*)&rtP.SFunction1_P5_Size;
  opalParametersSizeMapping[13] = sizeof(rtP.SFunction1_P5_Size[0]);
  opalParametersNumElementsMapping[13] = 2;
  opalParametersAdressMapping[14] = (void*)&rtP.SFunction1_P5;
  opalParametersSizeMapping[14] = sizeof(rtP.SFunction1_P5);
  opalParametersNumElementsMapping[14] = 1;
  opalParametersAdressMapping[15] = (void*)&rtP.SFunction1_P6_Size;
  opalParametersSizeMapping[15] = sizeof(rtP.SFunction1_P6_Size[0]);
  opalParametersNumElementsMapping[15] = 2;
  opalParametersAdressMapping[16] = (void*)&rtP.SFunction1_P6;
  opalParametersSizeMapping[16] = sizeof(rtP.SFunction1_P6);
  opalParametersNumElementsMapping[16] = 1;
  opalParametersAdressMapping[17] = (void*)&rtP.SFunction1_P7_Size;
  opalParametersSizeMapping[17] = sizeof(rtP.SFunction1_P7_Size[0]);
  opalParametersNumElementsMapping[17] = 2;
  opalParametersAdressMapping[18] = (void*)&rtP.SFunction1_P7;
  opalParametersSizeMapping[18] = sizeof(rtP.SFunction1_P7);
  opalParametersNumElementsMapping[18] = 1;
  opalParametersAdressMapping[19] = (void*)&rtP.SFunction1_P8_Size;
  opalParametersSizeMapping[19] = sizeof(rtP.SFunction1_P8_Size[0]);
  opalParametersNumElementsMapping[19] = 2;
  opalParametersAdressMapping[20] = (void*)&rtP.SFunction1_P8;
  opalParametersSizeMapping[20] = sizeof(rtP.SFunction1_P8[0]);
  opalParametersNumElementsMapping[20] = 7;
  opalParametersAdressMapping[21] = (void*)&rtP.SFunction1_P9_Size;
  opalParametersSizeMapping[21] = sizeof(rtP.SFunction1_P9_Size[0]);
  opalParametersNumElementsMapping[21] = 2;
  opalParametersAdressMapping[22] = (void*)&rtP.SFunction1_P9;
  opalParametersSizeMapping[22] = sizeof(rtP.SFunction1_P9[0]);
  opalParametersNumElementsMapping[22] = 7;
  opalParametersAdressMapping[23] = (void*)&rtP.SFunction1_P10_Size;
  opalParametersSizeMapping[23] = sizeof(rtP.SFunction1_P10_Size[0]);
  opalParametersNumElementsMapping[23] = 2;
  opalParametersAdressMapping[24] = (void*)&rtP.SFunction1_P10;
  opalParametersSizeMapping[24] = sizeof(rtP.SFunction1_P10[0]);
  opalParametersNumElementsMapping[24] = 7;
  opalParametersAdressMapping[25] = (void*)&rtP.SFunction1_P11_Size;
  opalParametersSizeMapping[25] = sizeof(rtP.SFunction1_P11_Size[0]);
  opalParametersNumElementsMapping[25] = 2;
  opalParametersAdressMapping[26] = (void*)&rtP.SFunction1_P11;
  opalParametersSizeMapping[26] = sizeof(rtP.SFunction1_P11[0]);
  opalParametersNumElementsMapping[26] = 7;
  opalParametersAdressMapping[27] = (void*)&rtP.SFunction1_P12_Size;
  opalParametersSizeMapping[27] = sizeof(rtP.SFunction1_P12_Size[0]);
  opalParametersNumElementsMapping[27] = 2;
  opalParametersAdressMapping[28] = (void*)&rtP.SFunction1_P12;
  opalParametersSizeMapping[28] = sizeof(rtP.SFunction1_P12[0]);
  opalParametersNumElementsMapping[28] = 7;
  opalParametersAdressMapping[29] = (void*)&rtP.SFunction1_P13_Size;
  opalParametersSizeMapping[29] = sizeof(rtP.SFunction1_P13_Size[0]);
  opalParametersNumElementsMapping[29] = 2;
  opalParametersAdressMapping[30] = (void*)&rtP.SFunction1_P13;
  opalParametersSizeMapping[30] = sizeof(rtP.SFunction1_P13);
  opalParametersNumElementsMapping[30] = 1;
  opalParametersAdressMapping[31] = (void*)&rtP.SFunction1_P14_Size;
  opalParametersSizeMapping[31] = sizeof(rtP.SFunction1_P14_Size[0]);
  opalParametersNumElementsMapping[31] = 2;
  opalParametersAdressMapping[32] = (void*)&rtP.SFunction1_P14;
  opalParametersSizeMapping[32] = sizeof(rtP.SFunction1_P14);
  opalParametersNumElementsMapping[32] = 1;
  opalParametersAdressMapping[33] = (void*)&rtP.SineWaveA_Amp;
  opalParametersSizeMapping[33] = sizeof(rtP.SineWaveA_Amp);
  opalParametersNumElementsMapping[33] = 1;
  opalParametersAdressMapping[34] = (void*)&rtP.SineWaveA_Bias;
  opalParametersSizeMapping[34] = sizeof(rtP.SineWaveA_Bias);
  opalParametersNumElementsMapping[34] = 1;
  opalParametersAdressMapping[35] = (void*)&rtP.SineWaveA_Hsin;
  opalParametersSizeMapping[35] = sizeof(rtP.SineWaveA_Hsin);
  opalParametersNumElementsMapping[35] = 1;
  opalParametersAdressMapping[36] = (void*)&rtP.SineWaveA_HCos;
  opalParametersSizeMapping[36] = sizeof(rtP.SineWaveA_HCos);
  opalParametersNumElementsMapping[36] = 1;
  opalParametersAdressMapping[37] = (void*)&rtP.SineWaveA_PSin;
  opalParametersSizeMapping[37] = sizeof(rtP.SineWaveA_PSin);
  opalParametersNumElementsMapping[37] = 1;
  opalParametersAdressMapping[38] = (void*)&rtP.SineWaveA_PCos;
  opalParametersSizeMapping[38] = sizeof(rtP.SineWaveA_PCos);
  opalParametersNumElementsMapping[38] = 1;
  opalParametersAdressMapping[39] = (void*)&rtP.SineWaveB_Amp;
  opalParametersSizeMapping[39] = sizeof(rtP.SineWaveB_Amp);
  opalParametersNumElementsMapping[39] = 1;
  opalParametersAdressMapping[40] = (void*)&rtP.SineWaveB_Bias;
  opalParametersSizeMapping[40] = sizeof(rtP.SineWaveB_Bias);
  opalParametersNumElementsMapping[40] = 1;
  opalParametersAdressMapping[41] = (void*)&rtP.SineWaveB_Hsin;
  opalParametersSizeMapping[41] = sizeof(rtP.SineWaveB_Hsin);
  opalParametersNumElementsMapping[41] = 1;
  opalParametersAdressMapping[42] = (void*)&rtP.SineWaveB_HCos;
  opalParametersSizeMapping[42] = sizeof(rtP.SineWaveB_HCos);
  opalParametersNumElementsMapping[42] = 1;
  opalParametersAdressMapping[43] = (void*)&rtP.SineWaveB_PSin;
  opalParametersSizeMapping[43] = sizeof(rtP.SineWaveB_PSin);
  opalParametersNumElementsMapping[43] = 1;
  opalParametersAdressMapping[44] = (void*)&rtP.SineWaveB_PCos;
  opalParametersSizeMapping[44] = sizeof(rtP.SineWaveB_PCos);
  opalParametersNumElementsMapping[44] = 1;
  opalParametersAdressMapping[45] = (void*)&rtP.SineWaveC_Amp;
  opalParametersSizeMapping[45] = sizeof(rtP.SineWaveC_Amp);
  opalParametersNumElementsMapping[45] = 1;
  opalParametersAdressMapping[46] = (void*)&rtP.SineWaveC_Bias;
  opalParametersSizeMapping[46] = sizeof(rtP.SineWaveC_Bias);
  opalParametersNumElementsMapping[46] = 1;
  opalParametersAdressMapping[47] = (void*)&rtP.SineWaveC_Hsin;
  opalParametersSizeMapping[47] = sizeof(rtP.SineWaveC_Hsin);
  opalParametersNumElementsMapping[47] = 1;
  opalParametersAdressMapping[48] = (void*)&rtP.SineWaveC_HCos;
  opalParametersSizeMapping[48] = sizeof(rtP.SineWaveC_HCos);
  opalParametersNumElementsMapping[48] = 1;
  opalParametersAdressMapping[49] = (void*)&rtP.SineWaveC_PSin;
  opalParametersSizeMapping[49] = sizeof(rtP.SineWaveC_PSin);
  opalParametersNumElementsMapping[49] = 1;
  opalParametersAdressMapping[50] = (void*)&rtP.SineWaveC_PCos;
  opalParametersSizeMapping[50] = sizeof(rtP.SineWaveC_PCos);
  opalParametersNumElementsMapping[50] = 1;
  opalParametersAdressMapping[51] = (void*)&rtP.StateSpace_DS_param;
  opalParametersSizeMapping[51] = sizeof(rtP.StateSpace_DS_param[0]);
  opalParametersNumElementsMapping[51] = 36;
  opalParametersAdressMapping[52] = (void*)&rtP.donotdeletethisgain_Gain;
  opalParametersSizeMapping[52] = sizeof(rtP.donotdeletethisgain_Gain);
  opalParametersNumElementsMapping[52] = 1;
  opalParametersAdressMapping[53] = (void*)&rtP.donotdeletethisgain_Gain_d;
  opalParametersSizeMapping[53] = sizeof(rtP.donotdeletethisgain_Gain_d);
  opalParametersNumElementsMapping[53] = 1;
  opalParametersAdressMapping[54] = (void*)&rtP.donotdeletethisgain_Gain_b;
  opalParametersSizeMapping[54] = sizeof(rtP.donotdeletethisgain_Gain_b);
  opalParametersNumElementsMapping[54] = 1;
  opalParametersAdressMapping[55] = (void*)&rtP.Kv1_Gain;
  opalParametersSizeMapping[55] = sizeof(rtP.Kv1_Gain);
  opalParametersNumElementsMapping[55] = 1;
  opalParametersAdressMapping[56] = (void*)&rtP.SineWaveA_Amp_j;
  opalParametersSizeMapping[56] = sizeof(rtP.SineWaveA_Amp_j);
  opalParametersNumElementsMapping[56] = 1;
  opalParametersAdressMapping[57] = (void*)&rtP.SineWaveA_Bias_k;
  opalParametersSizeMapping[57] = sizeof(rtP.SineWaveA_Bias_k);
  opalParametersNumElementsMapping[57] = 1;
  opalParametersAdressMapping[58] = (void*)&rtP.SineWaveA_Hsin_l;
  opalParametersSizeMapping[58] = sizeof(rtP.SineWaveA_Hsin_l);
  opalParametersNumElementsMapping[58] = 1;
  opalParametersAdressMapping[59] = (void*)&rtP.SineWaveA_HCos_c;
  opalParametersSizeMapping[59] = sizeof(rtP.SineWaveA_HCos_c);
  opalParametersNumElementsMapping[59] = 1;
  opalParametersAdressMapping[60] = (void*)&rtP.SineWaveA_PSin_f;
  opalParametersSizeMapping[60] = sizeof(rtP.SineWaveA_PSin_f);
  opalParametersNumElementsMapping[60] = 1;
  opalParametersAdressMapping[61] = (void*)&rtP.SineWaveA_PCos_a;
  opalParametersSizeMapping[61] = sizeof(rtP.SineWaveA_PCos_a);
  opalParametersNumElementsMapping[61] = 1;
  opalParametersAdressMapping[62] = (void*)&rtP.SineWaveB_Amp_a;
  opalParametersSizeMapping[62] = sizeof(rtP.SineWaveB_Amp_a);
  opalParametersNumElementsMapping[62] = 1;
  opalParametersAdressMapping[63] = (void*)&rtP.SineWaveB_Bias_e;
  opalParametersSizeMapping[63] = sizeof(rtP.SineWaveB_Bias_e);
  opalParametersNumElementsMapping[63] = 1;
  opalParametersAdressMapping[64] = (void*)&rtP.SineWaveB_Hsin_g;
  opalParametersSizeMapping[64] = sizeof(rtP.SineWaveB_Hsin_g);
  opalParametersNumElementsMapping[64] = 1;
  opalParametersAdressMapping[65] = (void*)&rtP.SineWaveB_HCos_l;
  opalParametersSizeMapping[65] = sizeof(rtP.SineWaveB_HCos_l);
  opalParametersNumElementsMapping[65] = 1;
  opalParametersAdressMapping[66] = (void*)&rtP.SineWaveB_PSin_i;
  opalParametersSizeMapping[66] = sizeof(rtP.SineWaveB_PSin_i);
  opalParametersNumElementsMapping[66] = 1;
  opalParametersAdressMapping[67] = (void*)&rtP.SineWaveB_PCos_f;
  opalParametersSizeMapping[67] = sizeof(rtP.SineWaveB_PCos_f);
  opalParametersNumElementsMapping[67] = 1;
  opalParametersAdressMapping[68] = (void*)&rtP.SineWaveC_Amp_c;
  opalParametersSizeMapping[68] = sizeof(rtP.SineWaveC_Amp_c);
  opalParametersNumElementsMapping[68] = 1;
  opalParametersAdressMapping[69] = (void*)&rtP.SineWaveC_Bias_e;
  opalParametersSizeMapping[69] = sizeof(rtP.SineWaveC_Bias_e);
  opalParametersNumElementsMapping[69] = 1;
  opalParametersAdressMapping[70] = (void*)&rtP.SineWaveC_Hsin_e;
  opalParametersSizeMapping[70] = sizeof(rtP.SineWaveC_Hsin_e);
  opalParametersNumElementsMapping[70] = 1;
  opalParametersAdressMapping[71] = (void*)&rtP.SineWaveC_HCos_j;
  opalParametersSizeMapping[71] = sizeof(rtP.SineWaveC_HCos_j);
  opalParametersNumElementsMapping[71] = 1;
  opalParametersAdressMapping[72] = (void*)&rtP.SineWaveC_PSin_b;
  opalParametersSizeMapping[72] = sizeof(rtP.SineWaveC_PSin_b);
  opalParametersNumElementsMapping[72] = 1;
  opalParametersAdressMapping[73] = (void*)&rtP.SineWaveC_PCos_f;
  opalParametersSizeMapping[73] = sizeof(rtP.SineWaveC_PCos_f);
  opalParametersNumElementsMapping[73] = 1;
  opalParametersAdressMapping[74] = (void*)&rtP.StateSpace_AS_param;
  opalParametersSizeMapping[74] = sizeof(rtP.StateSpace_AS_param[0]);
  opalParametersNumElementsMapping[74] = 36;
  opalParametersAdressMapping[75] = (void*)&rtP.StateSpace_BS_param;
  opalParametersSizeMapping[75] = sizeof(rtP.StateSpace_BS_param[0]);
  opalParametersNumElementsMapping[75] = 18;
  opalParametersAdressMapping[76] = (void*)&rtP.StateSpace_CS_param;
  opalParametersSizeMapping[76] = sizeof(rtP.StateSpace_CS_param[0]);
  opalParametersNumElementsMapping[76] = 36;
  opalParametersAdressMapping[77] = (void*)&rtP.StateSpace_DS_param_c;
  opalParametersSizeMapping[77] = sizeof(rtP.StateSpace_DS_param_c[0]);
  opalParametersNumElementsMapping[77] = 18;
  opalParametersAdressMapping[78] = (void*)&rtP.StateSpace_X0_param;
  opalParametersSizeMapping[78] = sizeof(rtP.StateSpace_X0_param[0]);
  opalParametersNumElementsMapping[78] = 6;
  opalParametersAdressMapping[79] = (void*)&rtP.donotdeletethisgain_Gain_n;
  opalParametersSizeMapping[79] = sizeof(rtP.donotdeletethisgain_Gain_n);
  opalParametersNumElementsMapping[79] = 1;
  opalParametersAdressMapping[80] = (void*)&rtP.donotdeletethisgain_Gain_a;
  opalParametersSizeMapping[80] = sizeof(rtP.donotdeletethisgain_Gain_a);
  opalParametersNumElementsMapping[80] = 1;
  opalParametersAdressMapping[81] = (void*)&rtP.donotdeletethisgain_Gain_dr;
  opalParametersSizeMapping[81] = sizeof(rtP.donotdeletethisgain_Gain_dr);
  opalParametersNumElementsMapping[81] = 1;
  opalParametersAdressMapping[82] = (void*)&rtP.Kv1_Gain_d;
  opalParametersSizeMapping[82] = sizeof(rtP.Kv1_Gain_d);
  opalParametersNumElementsMapping[82] = 1;
  opalParametersAdressMapping[83] = (void*)&rtP.donotdeletethisgain_Gain_k;
  opalParametersSizeMapping[83] = sizeof(rtP.donotdeletethisgain_Gain_k);
  opalParametersNumElementsMapping[83] = 1;
  opalParametersAdressMapping[84] = (void*)&rtP.donotdeletethisgain_Gain_g;
  opalParametersSizeMapping[84] = sizeof(rtP.donotdeletethisgain_Gain_g);
  opalParametersNumElementsMapping[84] = 1;
  opalParametersAdressMapping[85] = (void*)&rtP.donotdeletethisgain_Gain_e;
  opalParametersSizeMapping[85] = sizeof(rtP.donotdeletethisgain_Gain_e);
  opalParametersNumElementsMapping[85] = 1;
  opalParametersAdressMapping[86] = (void*)&rtP.Kv_Gain;
  opalParametersSizeMapping[86] = sizeof(rtP.Kv_Gain);
  opalParametersNumElementsMapping[86] = 1;
  opalParametersAdressMapping[87] = (void*)&rtP.SFunction_P1_Size;
  opalParametersSizeMapping[87] = sizeof(rtP.SFunction_P1_Size[0]);
  opalParametersNumElementsMapping[87] = 2;
  opalParametersAdressMapping[88] = (void*)&rtP.SFunction_P1;
  opalParametersSizeMapping[88] = sizeof(rtP.SFunction_P1);
  opalParametersNumElementsMapping[88] = 1;
  opalParametersAdressMapping[89] = (void*)&rtP.donotdeletethisgain_Gain_f;
  opalParametersSizeMapping[89] = sizeof(rtP.donotdeletethisgain_Gain_f);
  opalParametersNumElementsMapping[89] = 1;
  opalParametersAdressMapping[90] = (void*)&rtP.donotdeletethisgain_Gain_j;
  opalParametersSizeMapping[90] = sizeof(rtP.donotdeletethisgain_Gain_j);
  opalParametersNumElementsMapping[90] = 1;
  opalParametersAdressMapping[91] = (void*)&rtP.donotdeletethisgain_Gain_i;
  opalParametersSizeMapping[91] = sizeof(rtP.donotdeletethisgain_Gain_i);
  opalParametersNumElementsMapping[91] = 1;
  opalParametersAdressMapping[92] = (void*)&rtP.Kv_Gain_h;
  opalParametersSizeMapping[92] = sizeof(rtP.Kv_Gain_h);
  opalParametersNumElementsMapping[92] = 1;
  opalParametersAdressMapping[93] = (void*)&rtP.dataready25kHz_Amp;
  opalParametersSizeMapping[93] = sizeof(rtP.dataready25kHz_Amp);
  opalParametersNumElementsMapping[93] = 1;
  opalParametersAdressMapping[94] = (void*)&rtP.dataready25kHz_Period;
  opalParametersSizeMapping[94] = sizeof(rtP.dataready25kHz_Period);
  opalParametersNumElementsMapping[94] = 1;
  opalParametersAdressMapping[95] = (void*)&rtP.dataready25kHz_Duty;
  opalParametersSizeMapping[95] = sizeof(rtP.dataready25kHz_Duty);
  opalParametersNumElementsMapping[95] = 1;
  opalParametersAdressMapping[96] = (void*)&rtP.dataready25kHz_PhaseDelay;
  opalParametersSizeMapping[96] = sizeof(rtP.dataready25kHz_PhaseDelay);
  opalParametersNumElementsMapping[96] = 1;
  opalParametersAdressMapping[97] = (void*)&rtP.SFunction2_P1_Size;
  opalParametersSizeMapping[97] = sizeof(rtP.SFunction2_P1_Size[0]);
  opalParametersNumElementsMapping[97] = 2;
  opalParametersAdressMapping[98] = (void*)&rtP.SFunction2_P1;
  opalParametersSizeMapping[98] = sizeof(rtP.SFunction2_P1);
  opalParametersNumElementsMapping[98] = 1;
  opalParametersAdressMapping[99] = (void*)&rtP.SFunction2_P2_Size;
  opalParametersSizeMapping[99] = sizeof(rtP.SFunction2_P2_Size[0]);
  opalParametersNumElementsMapping[99] = 2;
  opalParametersAdressMapping[100] = (void*)&rtP.SFunction2_P2;
  opalParametersSizeMapping[100] = sizeof(rtP.SFunction2_P2);
  opalParametersNumElementsMapping[100] = 1;
  opalParametersAdressMapping[101] = (void*)&rtP.SFunction2_P3_Size;
  opalParametersSizeMapping[101] = sizeof(rtP.SFunction2_P3_Size[0]);
  opalParametersNumElementsMapping[101] = 2;
  opalParametersAdressMapping[102] = (void*)&rtP.SFunction2_P3;
  opalParametersSizeMapping[102] = sizeof(rtP.SFunction2_P3);
  opalParametersNumElementsMapping[102] = 1;
  opalParametersAdressMapping[103] = (void*)&rtP.SFunction2_P4_Size;
  opalParametersSizeMapping[103] = sizeof(rtP.SFunction2_P4_Size[0]);
  opalParametersNumElementsMapping[103] = 2;
  opalParametersAdressMapping[104] = (void*)&rtP.SFunction2_P4;
  opalParametersSizeMapping[104] = sizeof(rtP.SFunction2_P4);
  opalParametersNumElementsMapping[104] = 1;
  opalParametersAdressMapping[105] = (void*)&rtP.SFunction2_P5_Size;
  opalParametersSizeMapping[105] = sizeof(rtP.SFunction2_P5_Size[0]);
  opalParametersNumElementsMapping[105] = 2;
  opalParametersAdressMapping[106] = (void*)&rtP.SFunction2_P5;
  opalParametersSizeMapping[106] = sizeof(rtP.SFunction2_P5);
  opalParametersNumElementsMapping[106] = 1;
  opalParametersAdressMapping[107] = (void*)&rtP.SFunction2_P6_Size;
  opalParametersSizeMapping[107] = sizeof(rtP.SFunction2_P6_Size[0]);
  opalParametersNumElementsMapping[107] = 2;
  opalParametersAdressMapping[108] = (void*)&rtP.SFunction2_P6;
  opalParametersSizeMapping[108] = sizeof(rtP.SFunction2_P6);
  opalParametersNumElementsMapping[108] = 1;
  opalParametersAdressMapping[109] = (void*)&rtP.SFunction2_P7_Size;
  opalParametersSizeMapping[109] = sizeof(rtP.SFunction2_P7_Size[0]);
  opalParametersNumElementsMapping[109] = 2;
  opalParametersAdressMapping[110] = (void*)&rtP.SFunction2_P7;
  opalParametersSizeMapping[110] = sizeof(rtP.SFunction2_P7);
  opalParametersNumElementsMapping[110] = 1;
  opalParametersAdressMapping[111] = (void*)&rtP.SFunction2_P8_Size;
  opalParametersSizeMapping[111] = sizeof(rtP.SFunction2_P8_Size[0]);
  opalParametersNumElementsMapping[111] = 2;
  opalParametersAdressMapping[112] = (void*)&rtP.SFunction2_P8;
  opalParametersSizeMapping[112] = sizeof(rtP.SFunction2_P8);
  opalParametersNumElementsMapping[112] = 1;
  opalParametersAdressMapping[113] = (void*)&rtP.SFunction2_P9_Size;
  opalParametersSizeMapping[113] = sizeof(rtP.SFunction2_P9_Size[0]);
  opalParametersNumElementsMapping[113] = 2;
  opalParametersAdressMapping[114] = (void*)&rtP.SFunction2_P9;
  opalParametersSizeMapping[114] = sizeof(rtP.SFunction2_P9[0]);
  opalParametersNumElementsMapping[114] = 7;
  opalParametersAdressMapping[115] = (void*)&rtP.SFunction2_P10_Size;
  opalParametersSizeMapping[115] = sizeof(rtP.SFunction2_P10_Size[0]);
  opalParametersNumElementsMapping[115] = 2;
  opalParametersAdressMapping[116] = (void*)&rtP.SFunction2_P10;
  opalParametersSizeMapping[116] = sizeof(rtP.SFunction2_P10[0]);
  opalParametersNumElementsMapping[116] = 7;
  opalParametersAdressMapping[117] = (void*)&rtP.SFunction2_P11_Size;
  opalParametersSizeMapping[117] = sizeof(rtP.SFunction2_P11_Size[0]);
  opalParametersNumElementsMapping[117] = 2;
  opalParametersAdressMapping[118] = (void*)&rtP.SFunction2_P11;
  opalParametersSizeMapping[118] = sizeof(rtP.SFunction2_P11[0]);
  opalParametersNumElementsMapping[118] = 7;
  opalParametersAdressMapping[119] = (void*)&rtP.SFunction2_P12_Size;
  opalParametersSizeMapping[119] = sizeof(rtP.SFunction2_P12_Size[0]);
  opalParametersNumElementsMapping[119] = 2;
  opalParametersAdressMapping[120] = (void*)&rtP.SFunction2_P12;
  opalParametersSizeMapping[120] = sizeof(rtP.SFunction2_P12[0]);
  opalParametersNumElementsMapping[120] = 7;
  opalParametersAdressMapping[121] = (void*)&rtP.SFunction2_P13_Size;
  opalParametersSizeMapping[121] = sizeof(rtP.SFunction2_P13_Size[0]);
  opalParametersNumElementsMapping[121] = 2;
  opalParametersAdressMapping[122] = (void*)&rtP.SFunction2_P13;
  opalParametersSizeMapping[122] = sizeof(rtP.SFunction2_P13[0]);
  opalParametersNumElementsMapping[122] = 7;
  opalParametersAdressMapping[123] = (void*)&rtP.OpIPSocketCtrl_P1_Size;
  opalParametersSizeMapping[123] = sizeof(rtP.OpIPSocketCtrl_P1_Size[0]);
  opalParametersNumElementsMapping[123] = 2;
  opalParametersAdressMapping[124] = (void*)&rtP.OpIPSocketCtrl_P1;
  opalParametersSizeMapping[124] = sizeof(rtP.OpIPSocketCtrl_P1);
  opalParametersNumElementsMapping[124] = 1;
  opalParametersAdressMapping[125] = (void*)&rtP.OpIPSocketCtrl_P2_Size;
  opalParametersSizeMapping[125] = sizeof(rtP.OpIPSocketCtrl_P2_Size[0]);
  opalParametersNumElementsMapping[125] = 2;
  opalParametersAdressMapping[126] = (void*)&rtP.OpIPSocketCtrl_P2;
  opalParametersSizeMapping[126] = sizeof(rtP.OpIPSocketCtrl_P2);
  opalParametersNumElementsMapping[126] = 1;
  opalParametersAdressMapping[127] = (void*)&rtP.OpIPSocketCtrl_P3_Size;
  opalParametersSizeMapping[127] = sizeof(rtP.OpIPSocketCtrl_P3_Size[0]);
  opalParametersNumElementsMapping[127] = 2;
  opalParametersAdressMapping[128] = (void*)&rtP.OpIPSocketCtrl_P3;
  opalParametersSizeMapping[128] = sizeof(rtP.OpIPSocketCtrl_P3);
  opalParametersNumElementsMapping[128] = 1;
  opalParametersAdressMapping[129] = (void*)&rtP.OpIPSocketCtrl_P4_Size;
  opalParametersSizeMapping[129] = sizeof(rtP.OpIPSocketCtrl_P4_Size[0]);
  opalParametersNumElementsMapping[129] = 2;
  opalParametersAdressMapping[130] = (void*)&rtP.OpIPSocketCtrl_P4;
  opalParametersSizeMapping[130] = sizeof(rtP.OpIPSocketCtrl_P4);
  opalParametersNumElementsMapping[130] = 1;
  opalParametersAdressMapping[131] = (void*)&rtP.OpIPSocketCtrl_P5_Size;
  opalParametersSizeMapping[131] = sizeof(rtP.OpIPSocketCtrl_P5_Size[0]);
  opalParametersNumElementsMapping[131] = 2;
  opalParametersAdressMapping[132] = (void*)&rtP.OpIPSocketCtrl_P5;
  opalParametersSizeMapping[132] = sizeof(rtP.OpIPSocketCtrl_P5);
  opalParametersNumElementsMapping[132] = 1;
  opalParametersAdressMapping[133] = (void*)&rtP.OpIPSocketCtrl_P6_Size;
  opalParametersSizeMapping[133] = sizeof(rtP.OpIPSocketCtrl_P6_Size[0]);
  opalParametersNumElementsMapping[133] = 2;
  opalParametersAdressMapping[134] = (void*)&rtP.OpIPSocketCtrl_P6;
  opalParametersSizeMapping[134] = sizeof(rtP.OpIPSocketCtrl_P6);
  opalParametersNumElementsMapping[134] = 1;
  opalParametersAdressMapping[135] = (void*)&rtP.OpIPSocketCtrl_P7_Size;
  opalParametersSizeMapping[135] = sizeof(rtP.OpIPSocketCtrl_P7_Size[0]);
  opalParametersNumElementsMapping[135] = 2;
  opalParametersAdressMapping[136] = (void*)&rtP.OpIPSocketCtrl_P7;
  opalParametersSizeMapping[136] = sizeof(rtP.OpIPSocketCtrl_P7);
  opalParametersNumElementsMapping[136] = 1;
  opalParametersAdressMapping[137] = (void*)&rtP.OpIPSocketCtrl_P8_Size;
  opalParametersSizeMapping[137] = sizeof(rtP.OpIPSocketCtrl_P8_Size[0]);
  opalParametersNumElementsMapping[137] = 2;
  opalParametersAdressMapping[138] = (void*)&rtP.OpIPSocketCtrl_P8;
  opalParametersSizeMapping[138] = sizeof(rtP.OpIPSocketCtrl_P8);
  opalParametersNumElementsMapping[138] = 1;
  opalParametersAdressMapping[139] = (void*)&rtP.OpIPSocketCtrl_P9_Size;
  opalParametersSizeMapping[139] = sizeof(rtP.OpIPSocketCtrl_P9_Size[0]);
  opalParametersNumElementsMapping[139] = 2;
  opalParametersAdressMapping[140] = (void*)&rtP.OpIPSocketCtrl_P9;
  opalParametersSizeMapping[140] = sizeof(rtP.OpIPSocketCtrl_P9);
  opalParametersNumElementsMapping[140] = 1;
  opalParametersAdressMapping[141] = (void*)&rtP.OpIPSocketCtrl_P10_Size;
  opalParametersSizeMapping[141] = sizeof(rtP.OpIPSocketCtrl_P10_Size[0]);
  opalParametersNumElementsMapping[141] = 2;
  opalParametersAdressMapping[142] = (void*)&rtP.OpIPSocketCtrl_P10;
  opalParametersSizeMapping[142] = sizeof(rtP.OpIPSocketCtrl_P10);
  opalParametersNumElementsMapping[142] = 1;
  opalParametersAdressMapping[143] = (void*)&rtP.OpIPSocketCtrl_P11_Size;
  opalParametersSizeMapping[143] = sizeof(rtP.OpIPSocketCtrl_P11_Size[0]);
  opalParametersNumElementsMapping[143] = 2;
  opalParametersAdressMapping[144] = (void*)&rtP.OpIPSocketCtrl_P11;
  opalParametersSizeMapping[144] = sizeof(rtP.OpIPSocketCtrl_P11);
  opalParametersNumElementsMapping[144] = 1;
  opalParametersAdressMapping[145] = (void*)&rtP.OpIPSocketCtrl_P12_Size;
  opalParametersSizeMapping[145] = sizeof(rtP.OpIPSocketCtrl_P12_Size[0]);
  opalParametersNumElementsMapping[145] = 2;
  opalParametersAdressMapping[146] = (void*)&rtP.OpIPSocketCtrl_P12;
  opalParametersSizeMapping[146] = sizeof(rtP.OpIPSocketCtrl_P12);
  opalParametersNumElementsMapping[146] = 1;
  opalParametersAdressMapping[147] = (void*)&rtP.OpIPSocketCtrl_P13_Size;
  opalParametersSizeMapping[147] = sizeof(rtP.OpIPSocketCtrl_P13_Size[0]);
  opalParametersNumElementsMapping[147] = 2;
  opalParametersAdressMapping[148] = (void*)&rtP.OpIPSocketCtrl_P13;
  opalParametersSizeMapping[148] = sizeof(rtP.OpIPSocketCtrl_P13);
  opalParametersNumElementsMapping[148] = 1;
  opalParametersAdressMapping[149] = (void*)&rtP.OpIPSocketCtrl_P14_Size;
  opalParametersSizeMapping[149] = sizeof(rtP.OpIPSocketCtrl_P14_Size[0]);
  opalParametersNumElementsMapping[149] = 2;
  opalParametersAdressMapping[150] = (void*)&rtP.OpIPSocketCtrl_P14;
  opalParametersSizeMapping[150] = sizeof(rtP.OpIPSocketCtrl_P14[0]);
  opalParametersNumElementsMapping[150] = 13;
  opalParametersAdressMapping[151] = (void*)&rtP.OpIPSocketCtrl_P15_Size;
  opalParametersSizeMapping[151] = sizeof(rtP.OpIPSocketCtrl_P15_Size[0]);
  opalParametersNumElementsMapping[151] = 2;
  opalParametersAdressMapping[152] = (void*)&rtP.OpIPSocketCtrl_P15;
  opalParametersSizeMapping[152] = sizeof(rtP.OpIPSocketCtrl_P15[0]);
  opalParametersNumElementsMapping[152] = 7;
  opalParametersAdressMapping[153] = (void*)&rtP.OpIPSocketCtrl_P16_Size;
  opalParametersSizeMapping[153] = sizeof(rtP.OpIPSocketCtrl_P16_Size[0]);
  opalParametersNumElementsMapping[153] = 2;
  opalParametersAdressMapping[154] = (void*)&rtP.OpIPSocketCtrl_P17_Size;
  opalParametersSizeMapping[154] = sizeof(rtP.OpIPSocketCtrl_P17_Size[0]);
  opalParametersNumElementsMapping[154] = 2;
  opalParametersAdressMapping[155] = (void*)&rtP.OpIPSocketCtrl_P18_Size;
  opalParametersSizeMapping[155] = sizeof(rtP.OpIPSocketCtrl_P18_Size[0]);
  opalParametersNumElementsMapping[155] = 2;
  opalParametersAdressMapping[156] = (void*)&rtP.OpIPSocketCtrl_P19_Size;
  opalParametersSizeMapping[156] = sizeof(rtP.OpIPSocketCtrl_P19_Size[0]);
  opalParametersNumElementsMapping[156] = 2;
  opalParametersAdressMapping[157] = (void*)&rtP.OpIPSocketCtrl_P20_Size;
  opalParametersSizeMapping[157] = sizeof(rtP.OpIPSocketCtrl_P20_Size[0]);
  opalParametersNumElementsMapping[157] = 2;
  opalParametersAdressMapping[158] = (void*)&rtP.OpIPSocketCtrl_P21_Size;
  opalParametersSizeMapping[158] = sizeof(rtP.OpIPSocketCtrl_P21_Size[0]);
  opalParametersNumElementsMapping[158] = 2;
  opalParametersAdressMapping[159] = (void*)&rtP.OpIPSocketCtrl_P22_Size;
  opalParametersSizeMapping[159] = sizeof(rtP.OpIPSocketCtrl_P22_Size[0]);
  opalParametersNumElementsMapping[159] = 2;
  opalParametersAdressMapping[160] = (void*)&rtP.OpIPSocketCtrl_P23_Size;
  opalParametersSizeMapping[160] = sizeof(rtP.OpIPSocketCtrl_P23_Size[0]);
  opalParametersNumElementsMapping[160] = 2;
  opalParametersAdressMapping[161] = (void*)&rtP.OpIPSocketCtrl_P24_Size;
  opalParametersSizeMapping[161] = sizeof(rtP.OpIPSocketCtrl_P24_Size[0]);
  opalParametersNumElementsMapping[161] = 2;
  opalParametersAdressMapping[162] = (void*)&rtP.OpIPSocketCtrl_P25_Size;
  opalParametersSizeMapping[162] = sizeof(rtP.OpIPSocketCtrl_P25_Size[0]);
  opalParametersNumElementsMapping[162] = 2;
  opalParametersAdressMapping[163] = (void*)&rtP.OpIPSocketCtrl_P26_Size;
  opalParametersSizeMapping[163] = sizeof(rtP.OpIPSocketCtrl_P26_Size[0]);
  opalParametersNumElementsMapping[163] = 2;
  opalParametersAdressMapping[164] = (void*)&rtP.OpIPSocketCtrl_P26;
  opalParametersSizeMapping[164] = sizeof(rtP.OpIPSocketCtrl_P26[0]);
  opalParametersNumElementsMapping[164] = 7;
  opalParametersAdressMapping[165] = (void*)&rtP.OpIPSocketCtrl_P27_Size;
  opalParametersSizeMapping[165] = sizeof(rtP.OpIPSocketCtrl_P27_Size[0]);
  opalParametersNumElementsMapping[165] = 2;
  opalParametersAdressMapping[166] = (void*)&rtP.OpIPSocketCtrl_P27;
  opalParametersSizeMapping[166] = sizeof(rtP.OpIPSocketCtrl_P27);
  opalParametersNumElementsMapping[166] = 1;
  opalSignalsAdressMapping[0] = (void*)&rtB.SFunction;
  opalSignalsSizeMapping[0] = sizeof(rtB.SFunction);
  opalSignalsNumElementsMapping[0] = 1;
  opalSignalsAdressMapping[1] = (void*)&rtB.Sum;
  opalSignalsSizeMapping[1] = sizeof(rtB.Sum);
  opalSignalsNumElementsMapping[1] = 1;
  opalSignalsAdressMapping[2] = (void*)&rtB.timeout;
  opalSignalsSizeMapping[2] = sizeof(rtB.timeout);
  opalSignalsNumElementsMapping[2] = 1;
  opalSignalsAdressMapping[3] = (void*)&rtB.SFunction1_o1;
  opalSignalsSizeMapping[3] = sizeof(rtB.SFunction1_o1);
  opalSignalsNumElementsMapping[3] = 1;
  opalSignalsAdressMapping[4] = (void*)&rtB.SFunction1_o2;
  opalSignalsSizeMapping[4] = sizeof(rtB.SFunction1_o2);
  opalSignalsNumElementsMapping[4] = 1;
  opalSignalsAdressMapping[5] = (void*)&rtB.SFunction1_o3;
  opalSignalsSizeMapping[5] = sizeof(rtB.SFunction1_o3[0]);
  opalSignalsNumElementsMapping[5] = 3;
  opalSignalsAdressMapping[6] = (void*)&rtB.SineWaveA;
  opalSignalsSizeMapping[6] = sizeof(rtB.SineWaveA);
  opalSignalsNumElementsMapping[6] = 1;
  opalSignalsAdressMapping[7] = (void*)&rtB.SineWaveB;
  opalSignalsSizeMapping[7] = sizeof(rtB.SineWaveB);
  opalSignalsNumElementsMapping[7] = 1;
  opalSignalsAdressMapping[8] = (void*)&rtB.SineWaveC;
  opalSignalsSizeMapping[8] = sizeof(rtB.SineWaveC);
  opalSignalsNumElementsMapping[8] = 1;
  opalSignalsAdressMapping[9] = (void*)&rtB.StateSpace;
  opalSignalsSizeMapping[9] = sizeof(rtB.StateSpace[0]);
  opalSignalsNumElementsMapping[9] = 6;
  opalSignalsAdressMapping[10] = (void*)&rtB.donotdeletethisgain;
  opalSignalsSizeMapping[10] = sizeof(rtB.donotdeletethisgain);
  opalSignalsNumElementsMapping[10] = 1;
  opalSignalsAdressMapping[11] = (void*)&rtB.donotdeletethisgain_d;
  opalSignalsSizeMapping[11] = sizeof(rtB.donotdeletethisgain_d);
  opalSignalsNumElementsMapping[11] = 1;
  opalSignalsAdressMapping[12] = (void*)&rtB.donotdeletethisgain_c;
  opalSignalsSizeMapping[12] = sizeof(rtB.donotdeletethisgain_c);
  opalSignalsNumElementsMapping[12] = 1;
  opalSignalsAdressMapping[13] = (void*)&rtB.Kv1;
  opalSignalsSizeMapping[13] = sizeof(rtB.Kv1[0]);
  opalSignalsNumElementsMapping[13] = 3;
  opalSignalsAdressMapping[14] = (void*)&rtB.SineWaveA_p;
  opalSignalsSizeMapping[14] = sizeof(rtB.SineWaveA_p);
  opalSignalsNumElementsMapping[14] = 1;
  opalSignalsAdressMapping[15] = (void*)&rtB.SineWaveB_p;
  opalSignalsSizeMapping[15] = sizeof(rtB.SineWaveB_p);
  opalSignalsNumElementsMapping[15] = 1;
  opalSignalsAdressMapping[16] = (void*)&rtB.SineWaveC_i;
  opalSignalsSizeMapping[16] = sizeof(rtB.SineWaveC_i);
  opalSignalsNumElementsMapping[16] = 1;
  opalSignalsAdressMapping[17] = (void*)&rtB.StateSpace_f;
  opalSignalsSizeMapping[17] = sizeof(rtB.StateSpace_f[0]);
  opalSignalsNumElementsMapping[17] = 6;
  opalSignalsAdressMapping[18] = (void*)&rtB.donotdeletethisgain_j;
  opalSignalsSizeMapping[18] = sizeof(rtB.donotdeletethisgain_j);
  opalSignalsNumElementsMapping[18] = 1;
  opalSignalsAdressMapping[19] = (void*)&rtB.donotdeletethisgain_i;
  opalSignalsSizeMapping[19] = sizeof(rtB.donotdeletethisgain_i);
  opalSignalsNumElementsMapping[19] = 1;
  opalSignalsAdressMapping[20] = (void*)&rtB.donotdeletethisgain_p;
  opalSignalsSizeMapping[20] = sizeof(rtB.donotdeletethisgain_p);
  opalSignalsNumElementsMapping[20] = 1;
  opalSignalsAdressMapping[21] = (void*)&rtB.Kv1_p;
  opalSignalsSizeMapping[21] = sizeof(rtB.Kv1_p[0]);
  opalSignalsNumElementsMapping[21] = 3;
  opalSignalsAdressMapping[22] = (void*)&rtB.donotdeletethisgain_k;
  opalSignalsSizeMapping[22] = sizeof(rtB.donotdeletethisgain_k);
  opalSignalsNumElementsMapping[22] = 1;
  opalSignalsAdressMapping[23] = (void*)&rtB.donotdeletethisgain_g;
  opalSignalsSizeMapping[23] = sizeof(rtB.donotdeletethisgain_g);
  opalSignalsNumElementsMapping[23] = 1;
  opalSignalsAdressMapping[24] = (void*)&rtB.donotdeletethisgain_i1;
  opalSignalsSizeMapping[24] = sizeof(rtB.donotdeletethisgain_i1);
  opalSignalsNumElementsMapping[24] = 1;
  opalSignalsAdressMapping[25] = (void*)&rtB.Kv;
  opalSignalsSizeMapping[25] = sizeof(rtB.Kv[0]);
  opalSignalsNumElementsMapping[25] = 3;
  opalSignalsAdressMapping[26] = (void*)&rtB.donotdeletethisgain_di;
  opalSignalsSizeMapping[26] = sizeof(rtB.donotdeletethisgain_di);
  opalSignalsNumElementsMapping[26] = 1;
  opalSignalsAdressMapping[27] = (void*)&rtB.donotdeletethisgain_gn;
  opalSignalsSizeMapping[27] = sizeof(rtB.donotdeletethisgain_gn);
  opalSignalsNumElementsMapping[27] = 1;
  opalSignalsAdressMapping[28] = (void*)&rtB.donotdeletethisgain_g1;
  opalSignalsSizeMapping[28] = sizeof(rtB.donotdeletethisgain_g1);
  opalSignalsNumElementsMapping[28] = 1;
  opalSignalsAdressMapping[29] = (void*)&rtB.Kv_f;
  opalSignalsSizeMapping[29] = sizeof(rtB.Kv_f[0]);
  opalSignalsNumElementsMapping[29] = 3;
  opalSignalsAdressMapping[30] = (void*)&rtB.dataready25kHz;
  opalSignalsSizeMapping[30] = sizeof(rtB.dataready25kHz);
  opalSignalsNumElementsMapping[30] = 1;
  opalSignalsAdressMapping[31] = (void*)&rtB.SFunction2;
  opalSignalsSizeMapping[31] = sizeof(rtB.SFunction2);
  opalSignalsNumElementsMapping[31] = 1;
  opalNumParameters = 167;
  opalNumSignals = 32;
}