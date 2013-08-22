#ifndef __COMMON_LANG_CONSTANTS_H__
#define __COMMON_LANG_CONSTANTS_H__

#include <string>

namespace common_lang_constants
    {
    //punctuation
    const wchar_t TAB = 0x09;
    const wchar_t SPACE = 0x20;
    const wchar_t COMMA = 0x2C;
    const wchar_t LESS_THAN = 60;
    const wchar_t GREATER_THAN = 62;
    const wchar_t POUND = 35;
    const wchar_t AMPERSAND = 38;
    const wchar_t SEMICOLON = 59;
    const wchar_t APOSTROPHE = 0x27;
    const wchar_t DOUBLE_QUOTE = 0x22;
    const wchar_t QUESTION_MARK = 0x3F;
    const wchar_t PERIOD = 0x2E;
    const wchar_t EXLAMATION_MARK = 0x21;
    const wchar_t COLON = 0x3A;
    const wchar_t FORWARD_SLASH = 0x2F;
    const wchar_t BACK_SLASH = 0x5C;
    const wchar_t DOLLAR_SIGN = 0x24;
    const wchar_t PERCENTAGE_SIGN = 0x25;
    const wchar_t HYPHEN = 0x2D;
    const wchar_t RIGHT_PARENTHESIS = 0x29;
    const wchar_t RIGHT_BRACKET = 0x5D;
    //numbers
    const wchar_t NUMBER_0 = 0x30;
    const wchar_t NUMBER_1 = 0x31;
    const wchar_t NUMBER_2 = 0x32;
    const wchar_t NUMBER_3 = 0x33;
    const wchar_t NUMBER_4 = 0x34;
    const wchar_t NUMBER_5 = 0x35;
    const wchar_t NUMBER_6 = 0x36;
    const wchar_t NUMBER_7 = 0x37;
    const wchar_t NUMBER_8 = 0x38;
    const wchar_t NUMBER_9 = 0x39;
    //constants for letters
    const wchar_t UPPER_A = 0x41;
    const wchar_t LOWER_A = 0x61;
    const wchar_t UPPER_B = 0x42;
    const wchar_t LOWER_B = 0x62;
    const wchar_t UPPER_C = 0x43;
    const wchar_t LOWER_C = 0x63;
    const wchar_t UPPER_D = 0x44;
    const wchar_t LOWER_D = 0x64;
    const wchar_t UPPER_E = 0x45;
    const wchar_t LOWER_E = 0x65;
    const wchar_t UPPER_F = 0x46;
    const wchar_t LOWER_F = 0x66;
    const wchar_t UPPER_G = 0x47;
    const wchar_t LOWER_G = 0x67;
    const wchar_t UPPER_H = 0x48;
    const wchar_t LOWER_H = 0x68;
    const wchar_t UPPER_I = 0x49;
    const wchar_t LOWER_I = 0x69;
    const wchar_t UPPER_J = 0x4A;
    const wchar_t LOWER_J = 0x6A;
    const wchar_t UPPER_K = 0x4B;
    const wchar_t LOWER_K = 0x6B;
    const wchar_t UPPER_L = 0x4C;
    const wchar_t LOWER_L = 0x6C;
    const wchar_t UPPER_M = 0x4D;
    const wchar_t LOWER_M = 0x6D;
    const wchar_t UPPER_N = 0x4E;
    const wchar_t LOWER_N = 0x6E;
    const wchar_t UPPER_O = 0x4F;
    const wchar_t LOWER_O = 0x6F;
    const wchar_t UPPER_P = 0x50;
    const wchar_t LOWER_P = 0x70;
    const wchar_t UPPER_Q = 0x51;
    const wchar_t LOWER_Q = 0x71;
    const wchar_t UPPER_R = 0x52;
    const wchar_t LOWER_R = 0x72;
    const wchar_t UPPER_S = 0x53;
    const wchar_t LOWER_S = 0x73;
    const wchar_t UPPER_T = 0x54;
    const wchar_t LOWER_T = 0x74;
    const wchar_t UPPER_U = 0x55;
    const wchar_t LOWER_U = 0x75;
    const wchar_t UPPER_V = 0x56;
    const wchar_t LOWER_V = 0x76;
    const wchar_t UPPER_W = 0x57;
    const wchar_t LOWER_W = 0x77;
    const wchar_t UPPER_X = 0x58;
    const wchar_t LOWER_X = 0x78;
    const wchar_t UPPER_Y = 0x59;
    const wchar_t LOWER_Y = 0x79;
    const wchar_t UPPER_Z = 0x5A;
    const wchar_t LOWER_Z = 0x7A;

    const wchar_t UPPER_A_ACUTE = 0xC1;
    const wchar_t LOWER_A_ACUTE = 0xE1;
    const wchar_t UPPER_E_ACUTE = 0xC9;
    const wchar_t LOWER_E_ACUTE = 0xE9;
    const wchar_t UPPER_I_ACUTE = 0xCD;
    const wchar_t LOWER_I_ACUTE = 0xED;
    const wchar_t UPPER_O_ACUTE = 0xD3;
    const wchar_t LOWER_O_ACUTE = 0xF3;
    const wchar_t UPPER_A_CIRCUMFLEX = 0xC2;
    const wchar_t LOWER_A_CIRCUMFLEX = 0xE2;
    const wchar_t UPPER_E_CIRCUMFLEX = 0xCA;
    const wchar_t LOWER_E_CIRCUMFLEX = 0xEA;
    const wchar_t UPPER_I_CIRCUMFLEX = 0xCE;
    const wchar_t LOWER_I_CIRCUMFLEX = 0xEE;
    const wchar_t UPPER_A_TILDE = 0xC3;
    const wchar_t LOWER_A_TILDE = 0xE3;
    const wchar_t UPPER_O_TILDE = 0xD5;
    const wchar_t LOWER_O_TILDE = 0xF5;
    const wchar_t UPPER_N_TILDE = 0xD1;
    const wchar_t LOWER_N_TILDE = 0xF1;
    const wchar_t UPPER_O_STROKE = 0xD8;
    const wchar_t LOWER_O_STROKE = 0xF8;
    const wchar_t UPPER_C_CEDILLA = 0xC7;
    const wchar_t LOWER_C_CEDILLA = 0xE7;
    const wchar_t LOWER_A_UMLAUTS = 0xC4;
    const wchar_t UPPER_A_UMLAUTS = 0xE4;
    const wchar_t UPPER_O_UMLAUTS = 0xD6;
    const wchar_t LOWER_O_UMLAUTS = 0xF6;
    const wchar_t UPPER_E_UMLAUTS = 0xCB;
    const wchar_t LOWER_E_UMLAUTS = 0xEB;
    const wchar_t UPPER_U_UMLAUTS = 0xDC;
    const wchar_t LOWER_U_UMLAUTS = 0xFC;
    const wchar_t TILDE = 0x7E;
    const wchar_t UPPER_A_GRAVE = 0xC0;
    const wchar_t LOWER_A_GRAVE = 0xE0;
    const wchar_t UPPER_E_GRAVE = 0xC8;
    const wchar_t LOWER_E_GRAVE = 0xE8;
    const wchar_t UPPER_I_GRAVE = 0xCC;
    const wchar_t LOWER_I_GRAVE = 0xEC;
    const wchar_t UPPER_O_GRAVE = 0xD2;
    const wchar_t LOWER_O_GRAVE = 0xF2;
    const wchar_t UPPER_Y_ACUTE = 0xDD;
    const wchar_t LOWER_Y_ACUTE = 0xFD;
    const wchar_t ESZETT = 0xDF; //a.k.a. "sharp s"
    const wchar_t Y_UMLAUT = 0xFF;

    class english_constants
        {
    public:
        static const std::wstring WORD_I;
        static const std::wstring WORD_ID;
        static const std::wstring WORD_IM;
        static const std::wstring WORD_IVE;
        static const std::wstring WORD_MC;
        static const std::wstring WORD_NUMBERS_COMMA;
        static const std::wstring WORD_NUMBERS_PERIOD;
        static const std::wstring WORD_ETC;
        static const std::wstring WORD_IQ;
        static const std::wstring WORD_NT;
        static const std::wstring WORD_UN;
        static const std::wstring WORD_TH;
        static const std::wstring WORD_ST;
        static const std::wstring WORD_LV;
        static const std::wstring WORD_LK;
        static const std::wstring WORD_QU;
        static const std::wstring WORD_RV;
        static const std::wstring WORD_SH;
        static const std::wstring WORD_LY;
        static const std::wstring WORD_ES;
        static const std::wstring WORD_ED;
        static const std::wstring WORD_ER;
        static const std::wstring WORD_OR;
        static const std::wstring WORD_AO;
        static const std::wstring WORD_EU;
        static const std::wstring WORD_OA;
        static const std::wstring WORD_UE;
        static const std::wstring WORD_IO;
        static const std::wstring WORD_EE;
        static const std::wstring WORD_EI;
        static const std::wstring WORD_IC;
        static const std::wstring WORD_EA;
        static const std::wstring WORD_CR;
        static const std::wstring WORD_IU;
        static const std::wstring WORD_EO;
        static const std::wstring WORD_UI;
        static const std::wstring WORD_YI;
        static const std::wstring WORD_OI;
        static const std::wstring WORD_YE;
        static const std::wstring WORD_IA;
        static const std::wstring WORD_UO;
        static const std::wstring WORD_OO;
        static const std::wstring WORD_OE;
        static const std::wstring WORD_IE;
        static const std::wstring WORD_ND;
        static const std::wstring WORD_PR;
        static const std::wstring WORD_UA;
        static const std::wstring WORD_UU;
        static const std::wstring WORD_BB;
        static const std::wstring WORD_CH;
        static const std::wstring WORD_CK;
        static const std::wstring WORD_DD;
        static const std::wstring WORD_DG;
        static const std::wstring WORD_FF;
        static const std::wstring WORD_FR;
        static const std::wstring WORD_GG;
        static const std::wstring WORD_TT;
        static const std::wstring WORD_WN;
        static const std::wstring WORD_SS;
        static const std::wstring WORD_WS;
        static const std::wstring WORD_YC;
        static const std::wstring WORD_GH;
        static const std::wstring WORD_GR;
        static const std::wstring WORD_GU;
        static const std::wstring WORD_KK;
        static const std::wstring WORD_LC;
        static const std::wstring WORD_LF;
        static const std::wstring WORD_LG;
        static const std::wstring WORD_LL;
        static const std::wstring WORD_LM;
        static const std::wstring WORD_LN;
        static const std::wstring WORD_LP;
        static const std::wstring WORD_MB;
        static const std::wstring WORD_LT;
        static const std::wstring WORD_MM;
        static const std::wstring WORD_MP;
        static const std::wstring WORD_NC;
        static const std::wstring WORD_NG;
        static const std::wstring WORD_NK;
        static const std::wstring WORD_NN;
        static const std::wstring WORD_NS;
        static const std::wstring WORD_PP;
        static const std::wstring WORD_PS;
        static const std::wstring WORD_RB;
        static const std::wstring WORD_UT;
        static const std::wstring WORD_IT;
        static const std::wstring WORD_IL;
        static const std::wstring WORD_RR;
        static const std::wstring WORD_MA;
        static const std::wstring WORD_FY;
        static const std::wstring WORD_BO;
        static const std::wstring WORD_MY;
        static const std::wstring WORD_RC;
        static const std::wstring WORD_RD;
        static const std::wstring WORD_RF;
        static const std::wstring WORD_RG;
        static const std::wstring WORD_RS;
        static const std::wstring WORD_RT;
        static const std::wstring WORD_RK;
        static const std::wstring WORD_RL;
        static const std::wstring WORD_RM;
        static const std::wstring WORD_RN;
        static const std::wstring WORD_RP;
        static const std::wstring WORD_RQ;
        static const std::wstring WORD_SK;
        static const std::wstring WORD_SL;
        static const std::wstring WORD_SM;
        static const std::wstring WORD_SN;
        static const std::wstring WORD_SP;
        static const std::wstring WORD_GN;
        static const std::wstring WORD_LS;
        static const std::wstring WORD_MN;
        static const std::wstring WORD_NZ;
        static const std::wstring WORD_RZ;
        static const std::wstring WORD_TZ;
        static const std::wstring WORD_WL;
        static const std::wstring WORD_YL;
        static const std::wstring WORD_ZZ;
        static const std::wstring WORD_BL;
        static const std::wstring WORD_BR;
        static const std::wstring WORD_CL;
        static const std::wstring WORD_DR;
        static const std::wstring WORD_FL;
        static const std::wstring WORD_KN;
        static const std::wstring WORD_PL;
        static const std::wstring WORD_SC;
        static const std::wstring WORD_SQ;
        static const std::wstring WORD_SW;
        static const std::wstring WORD_TR;
        static const std::wstring WORD_WR;
        static const std::wstring WORD_WH;
        static const std::wstring WORD_ATE;
        static const std::wstring WORD_IES;
        static const std::wstring WORD_FOR;
        static const std::wstring WORD_HER;
        static const std::wstring WORD_SOM;
        static const std::wstring WORD_FAC;
        static const std::wstring WORD_DIS;
        static const std::wstring WORD_IND;
        static const std::wstring WORD_TEL;
        static const std::wstring WORD_ARD;
        static const std::wstring WORD_ING;
        static const std::wstring WORD_EUE;
        static const std::wstring WORD_EAU;
        static const std::wstring WORD_EOI;
        static const std::wstring WORD_IAO;
        static const std::wstring WORD_IEU;
        static const std::wstring WORD_OOE;
        static const std::wstring WORD_EOU;
        static const std::wstring WORD_IOU;
        static const std::wstring WORD_OOL;
        static const std::wstring WORD_TCH;
        static const std::wstring WORD_SQU;
        static const std::wstring WORD_ATI;
        static const std::wstring WORD_ECT;
        static const std::wstring WORD_IED;
        static const std::wstring WORD_IZE;
        static const std::wstring WORD_URE;
        static const std::wstring WORD_MIC;
        static const std::wstring WORD_SIS;
        static const std::wstring WORD_TIC;
        static const std::wstring WORD_TER;
        static const std::wstring WORD_TES;
        static const std::wstring WORD_TIN;
        static const std::wstring WORD_TRY;
        static const std::wstring WORD_TUM;
        static const std::wstring WORD_VER;
        static const std::wstring WORD_CHR;
        static const std::wstring WORD_MPT;
        static const std::wstring WORD_NTH;
        static const std::wstring WORD_NCH;
        static const std::wstring WORD_NGU;
        static const std::wstring WORD_NQU;
        static const std::wstring WORD_RST;
        static const std::wstring WORD_RCH;
        static const std::wstring WORD_RGU;
        static const std::wstring WORD_RQU;
        static const std::wstring WORD_RTH;
        static const std::wstring WORD_SCH;
        static const std::wstring WORD_SCR;
        static const std::wstring WORD_LQU;
        static const std::wstring WORD_MPS;
        static const std::wstring WORD_PHR;
        static const std::wstring WORD_SHR;
        static const std::wstring WORD_STR;
        static const std::wstring WORD_SPR;
        static const std::wstring WORD_THR;
        static const std::wstring WORD_DOM;
        static const std::wstring WORD_TISM;
        static const std::wstring WORD_NESS;
        static const std::wstring WORD_MENT;
        static const std::wstring WORD_ROOM;
        static const std::wstring WORD_SHOE;
        static const std::wstring WORD_DRAL;
        static const std::wstring WORD_QUEN;
        static const std::wstring WORD_NIAL;
        static const std::wstring WORD_PTOR;
        static const std::wstring WORD_STOR;
        static const std::wstring WORD_STRY;
        static const std::wstring WORD_THER;
        static const std::wstring WORD_TRIC;
        static const std::wstring WORD_TAKE;
        static const std::wstring WORD_TIME;
        static const std::wstring WORD_TEEN;
        static const std::wstring WORD_MATIC;
        static const std::wstring WORD_SCEND;
        static const std::wstring WORD_STRAL;
        static const std::wstring WORD_CIOUS;
        static const std::wstring WORD_SHION;
        static const std::wstring WORD_CHION;
        static const std::wstring WORD_THING;
        static const std::wstring WORD_NINETY;
        static const std::wstring WORD_SAFETY;
        };
    }

#endif //__COMMON_LANG_CONSTANTS_H__
